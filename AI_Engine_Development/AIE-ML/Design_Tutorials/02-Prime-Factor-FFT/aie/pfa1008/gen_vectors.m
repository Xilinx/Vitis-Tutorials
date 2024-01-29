%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;

rng(1);

% ------------------------------------------------------------
% Generate Model
% ------------------------------------------------------------

Nfft = 1008;
N1 = 7;
N2 = 9;
N3 = 16;
Ntransform = 8*4;
backoff_dB = -15;                       % To achieve (-1,+1) range on AWGN signal

TT = numerictype(1,16,15);
FF = fimath('RoundingMethod','Round','OverflowAction','Saturate');

% Input signal:
taps = firgr('minevn',[0,0.3,0.4,1.0],[1,1,0,0],[1e-4,1e-4]);
fir_i = sqrt(0.5)*10^(0.05*backoff_dB)*complex(randn(1,Nfft*Ntransform),randn(1,Nfft*Ntransform));
fir_o = filter(taps,1,fir_i);
scale = 1/32;
sig_i = fi(reshape(scale*fir_o,Nfft,Ntransform),TT,FF);

% Apply FFT-1008 using PFA architecture:
[P_i,P_o] = compute_perm_3d(N1,N2,N3);
dft7_i  = fi(complex(zeros(N1,N2,N3,Ntransform),zeros(N1,N2,N3,Ntransform)),TT,FF);
dft7_o  = fi(complex(zeros(N1,N2,N3,Ntransform),zeros(N1,N2,N3,Ntransform)),TT,FF);
dft9_o  = fi(complex(zeros(N1,N2,N3,Ntransform),zeros(N1,N2,N3,Ntransform)),TT,FF);
dft16_o = fi(complex(zeros(N1,N2,N3,Ntransform),zeros(N1,N2,N3,Ntransform)),TT,FF);
sig_o   = fi(complex(zeros(Nfft,Ntransform),zeros(Nfft,Ntransform)),TT,FF);
for tt = 1 : Ntransform
  % DFT-7:
  dft7_i(:,:,:,tt) = reshape(sig_i(1+P_i,tt),N1,N2,N3);
  for cc = 1 : N2
    for dd = 1 : N3
      dft7_o(:,cc,dd,tt) = fi(fft(double(dft7_i(:,cc,dd,tt))),TT,FF);
    end
  end
  % DFT-9:
  for rr = 1 : N1
    for dd = 1 : N3
      dft9_o(rr,:,dd,tt) = fi(fft(double(dft7_o(rr,:,dd,tt))),TT,FF);
    end
  end
  % DFT-16:
  for rr = 1 : N1
    for cc = 1 : N2
      dft16_o(rr,cc,:,tt) = fi(fft(double(dft9_o(rr,cc,:,tt))),TT,FF);
    end
  end
  sig_o(1+P_o,tt) = reshape(dft16_o(:,:,:,tt),1,[]);
end

% ------------------------------------------------------------
% Save I/O files for Top Level AIE-Only Simulation
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

% Inputs to DFT-7, 1st half to 1st HW instance, 2nd half to the 2nd HW instance
fid = fopen('data/sig_i_aie.txt','w');
for tt = 1 : Ntransform
  data = dft7_i(:,:,:,tt);
  for ii = 1 : 2 : numel(data)
    fprintf(fid,'%d %d %d %d \n',...
            real(data(ii+0)).int,imag(data(ii+0)).int,real(data(ii+1)).int,imag(data(ii+1)).int);
  end
end
fclose(fid);

% Output from DFT-16
fid = fopen('data/sig_o_aie.txt','w');
for tt = 1 : Ntransform  
  data = permute(dft16_o(:,:,:,tt),[3,2,1]); % This is the DMA order used by the hardware
  data = reshape(data,1,[]);
  for ii = 1 : 2 : numel(data)
    fprintf(fid,'%d %d %d %d \n',...
            real(data(ii+0)).int,imag(data(ii+0)).int,real(data(ii+1)).int,imag(data(ii+1)).int);
  end
end
fclose(fid);

% ------------------------------------------------------------
% Save I/O files for Top Level HW Emulation
% ------------------------------------------------------------

% HW Emulation repeats sending 4 transforms 8 times (we load only the first 4 transforms):

% Input samples to DDR in linear order:
fid_i = fopen('data/sig_i.txt','w');
data = reshape(sig_i(:,1:4),1,[]);
for ii = 1 : numel(data)
  fprintf(fid_i,'%d %d \n',real(data.int(ii)),imag(data.int(ii+0)));
end
fclose(fid_i);

% Output samples to DDR in linear order:
fid_o = fopen('data/sig_o.txt','w');
data = reshape(sig_o(:,1:4),1,[]);
for ii = 1 : numel(data)
  fprintf(fid_o,'%d %d \n',real(data.int(ii+0)),imag(data.int(ii+0)));
end
fclose(fid_o);

