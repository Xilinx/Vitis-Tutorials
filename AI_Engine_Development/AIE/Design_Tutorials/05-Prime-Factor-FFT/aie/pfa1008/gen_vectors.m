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

Nports = 2;
Nfft = 1008;
N1 = 7;
N2 = 9;
N3 = 16;
Ntransform = 8;
backoff_dB = -15;                       % To achieve (-1,+1) range on AWGN signal

% Downshift (to ensure no clipping occurs in FFT):
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

% -------------------- DFT-7 I/O Files --------------------

fi0 = fopen('data/dft7_i_0.txt','w');
fi1 = fopen('data/dft7_i_1.txt','w');
fo0 = fopen('data/dft7_o_0.txt','w');
fo1 = fopen('data/dft7_o_1.txt','w');
for tt = 1 : Ntransform
  % Get this data in the proper order (alternate transforms on ss0 then ss1):
  data_i = reshape(dft7_i(:,:,:,tt),N1,[]);
  data_o = reshape(dft7_o(:,:,:,tt),N1,[]);
  ss0_i = data_i(:,1:2:end);
  ss1_i = data_i(:,2:2:end);
  ss0_o = data_o(:,1:2:end);
  ss1_o = data_o(:,2:2:end);
  for ii = 1 : 2 : numel(ss0_i)
    fprintf(fi0,'%d %d %d %d\n',real(ss0_i(ii+0)).int,imag(ss0_i(ii+0)).int,real(ss0_i(ii+1)).int,imag(ss0_i(ii+1)).int);
    fprintf(fi1,'%d %d %d %d\n',real(ss1_i(ii+0)).int,imag(ss1_i(ii+0)).int,real(ss1_i(ii+1)).int,imag(ss1_i(ii+1)).int);
    fprintf(fo0,'%d %d %d %d\n',real(ss0_o(ii+0)).int,imag(ss0_o(ii+0)).int,real(ss0_o(ii+1)).int,imag(ss0_o(ii+1)).int);
    fprintf(fo1,'%d %d %d %d\n',real(ss1_o(ii+0)).int,imag(ss1_o(ii+0)).int,real(ss1_o(ii+1)).int,imag(ss1_o(ii+1)).int);
  end
end
fclose(fi0);
fclose(fi1);
fclose(fo0);
fclose(fo1);

% -------------------- DFT-9 I/O Files --------------------

fi0 = fopen('data/dft9_i_0.txt','w');
fi1 = fopen('data/dft9_i_1.txt','w');
fo0 = fopen('data/dft9_o_0.txt','w');
fo1 = fopen('data/dft9_o_1.txt','w');
addr_N2 = compute_addr_3d(N1,N2,N3,2);
for tt = 1 : Ntransform
  % Get this data in the proper order (alternate transforms on ss0 then ss1):
  data_i = reshape(dft7_o(:,:,:,tt),1,[]); data_i = data_i(1+addr_N2); data_i = reshape(data_i,N2,[]);
  data_o = reshape(dft9_o(:,:,:,tt),1,[]); data_o = data_o(1+addr_N2); data_o = reshape(data_o,N2,[]);
  ss0_i = data_i(:,1:2:end);
  ss1_i = data_i(:,2:2:end);
  ss0_o = data_o(:,1:2:end);
  ss1_o = data_o(:,2:2:end);
  for ii = 1 : 2 : numel(ss0_i)
    fprintf(fi0,'%d %d %d %d\n',real(ss0_i(ii+0)).int,imag(ss0_i(ii+0)).int,real(ss0_i(ii+1)).int,imag(ss0_i(ii+1)).int);
    fprintf(fi1,'%d %d %d %d\n',real(ss1_i(ii+0)).int,imag(ss1_i(ii+0)).int,real(ss1_i(ii+1)).int,imag(ss1_i(ii+1)).int);
    fprintf(fo0,'%d %d %d %d\n',real(ss0_o(ii+0)).int,imag(ss0_o(ii+0)).int,real(ss0_o(ii+1)).int,imag(ss0_o(ii+1)).int);
    fprintf(fo1,'%d %d %d %d\n',real(ss1_o(ii+0)).int,imag(ss1_o(ii+0)).int,real(ss1_o(ii+1)).int,imag(ss1_o(ii+1)).int);
  end
end
fclose(fi0);
fclose(fi1);
fclose(fo0);
fclose(fo1);

% -------------------- DFT-16 I/O Files --------------------

fi0 = fopen('data/dft16_i_0.txt','w');
fi1 = fopen('data/dft16_i_1.txt','w');
fo0 = fopen('data/dft16_o_0.txt','w');
fo1 = fopen('data/dft16_o_1.txt','w');
addr_N3 = compute_addr_3d(N1,N2,N3,3);
for tt = 1 : Ntransform
  % Get this data in the proper order (alternate 4 samples on ss0 then ss1):
  data_i = reshape( dft9_o(:,:,:,tt),1,[]); data_i = data_i(1+addr_N3); data_i = reshape(data_i,4,[]);
  data_o = reshape(dft16_o(:,:,:,tt),1,[]); data_o = data_o(1+addr_N3); data_o = reshape(data_o,4,[]);
  ss0_i = data_i(:,1:2:end);
  ss1_i = data_i(:,2:2:end);
  ss0_o = data_o(:,1:2:end);
  ss1_o = data_o(:,2:2:end);
  for ii = 1 : 2 : numel(ss0_i)
    fprintf(fi0,'%d %d %d %d\n',real(ss0_i(ii+0)).int,imag(ss0_i(ii+0)).int,real(ss0_i(ii+1)).int,imag(ss0_i(ii+1)).int);
    fprintf(fi1,'%d %d %d %d\n',real(ss1_i(ii+0)).int,imag(ss1_i(ii+0)).int,real(ss1_i(ii+1)).int,imag(ss1_i(ii+1)).int);
    fprintf(fo0,'%d %d %d %d\n',real(ss0_o(ii+0)).int,imag(ss0_o(ii+0)).int,real(ss0_o(ii+1)).int,imag(ss0_o(ii+1)).int);
    fprintf(fo1,'%d %d %d %d\n',real(ss1_o(ii+0)).int,imag(ss1_o(ii+0)).int,real(ss1_o(ii+1)).int,imag(ss1_o(ii+1)).int);
  end
end
fclose(fi0);
fclose(fi1);
fclose(fo0);
fclose(fo1);

% ------------------------------------------------------------
% Save I/O files for Top Level HW Emulation
% ------------------------------------------------------------

% Input samples to DDR in linear order:
fid_i = fopen('data/sig_i.txt','w');
sig_i = reshape(sig_i,1,[]);
for ii = 1 : numel(sig_i)
  fprintf(fid_i,'%d %d \n',real(sig_i.int(ii)),imag(sig_i.int(ii+0)));
end
fclose(fid_i);

% Output samples to DDR in linear order:
fid_o = fopen('data/sig_o.txt','w');
sig_o = reshape(sig_o,1,[]);
for ii = 1 : numel(sig_o)
  fprintf(fid_o,'%d %d \n',real(sig_o.int(ii+0)),imag(sig_o.int(ii+0)));
end
fclose(fid_o);


