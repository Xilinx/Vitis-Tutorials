%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;

% rng(1);

% ------------------------------------------------------------
% Generate Model
% ------------------------------------------------------------

Nports = 2;
Nfft = 9;
Ntransform = 8*7*16;
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

% Output (Note scaling to match AIE design):
sig_o = fft(double(sig_i),[],1);
sig_o = fi(sig_o,TT,FF);

% ------------------------------------------------------------
% Generate Twiddle Matrix for DFT
% ------------------------------------------------------------

twiddle = zeros(Nfft,Nfft);
omega = exp(-1i*2*pi/Nfft);             % Note: FFT direction
for rr = 1 : Nfft
  for cc = 1 : Nfft
    twiddle(rr,cc) = omega^((rr-1)*(cc-1));
  end
end
% Take transpose since we formulate the diagrams this way:
twiddle = transpose(twiddle);

% Tables used by kernels:
% --> zero pad 3 columns on the right since we read [2x4] patches but the right patches are [2x1]:
twiddle = cat(2,twiddle,zeros(Nfft,3));
% --> split tables into left, middle and right patches for each tile:
TWID0 = twiddle(:,1:4);
TWID1 = twiddle(:,5:8);
TWID2 = twiddle(:,9:12);

% Quantize:
TWID0 = fi(TWID0,TT,'RoundingMethod','Convergent','OverflowAction','Saturate');
TWID1 = fi(TWID1,TT,'RoundingMethod','Convergent','OverflowAction','Saturate');
TWID2 = fi(TWID2,TT,'RoundingMethod','Convergent','OverflowAction','Saturate');

fid = fopen('dft9_twiddle.h','w');
fprintf(fid,'//\n');
fprintf(fid,'// Copyright (fid,C) 2023, Advanced Micro Devices, Inc. All rights reserved.\n');
fprintf(fid,'// SPDX-License-Identifier: MIT\n');
fprintf(fid,'//\n');
fprintf(fid,'// Author: Mark Rollins\n');
fprintf(fid,'\n');
fprintf(fid,'#pragma once\n');
fprintf(fid,'#define DFT9_TWID0 {\\\n');
for ii = 1 : Nfft
  for jj = 1 : 4
    if (jj == 4 && ii == Nfft) fprintf(fid,'{%d,%d} \\\n',real(TWID0.int(ii,jj)),imag(TWID0.int(ii,jj)));
    else                       fprintf(fid,'{%d,%d},\\\n',real(TWID0.int(ii,jj)),imag(TWID0.int(ii,jj)));
    end
  end
end
fprintf(fid,'}\n');
fprintf(fid,'#define DFT9_TWID1 {\\\n');
for ii = 1 : Nfft
  for jj = 1 : 4
    if (jj == 4 && ii == Nfft) fprintf(fid,'{%d,%d} \\\n',real(TWID1.int(ii,jj)),imag(TWID1.int(ii,jj)));
    else                       fprintf(fid,'{%d,%d},\\\n',real(TWID1.int(ii,jj)),imag(TWID1.int(ii,jj)));
    end
  end
end
fprintf(fid,'}\n');
fprintf(fid,'#define DFT9_TWID2 {\\\n');
for ii = 1 : Nfft
  for jj = 1 : 4
    if (jj == 4 && ii == Nfft) fprintf(fid,'{%d,%d} \\\n',real(TWID2.int(ii,jj)),imag(TWID2.int(ii,jj)));
    else                       fprintf(fid,'{%d,%d},\\\n',real(TWID2.int(ii,jj)),imag(TWID2.int(ii,jj)));
    end
  end
end
fprintf(fid,'}\n');
fclose(fid);

% ------------------------------------------------------------
% Save I/O files for simulation
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

% Input samples in transform order, alternating ss0 then ss1:
fid0_i = fopen('data/sig_i_0.txt','w');
fid1_i = fopen('data/sig_i_1.txt','w');
data_ss0 = reshape(sig_i(:,1:2:end),1,[]);
data_ss1 = reshape(sig_i(:,2:2:end),1,[]);
for ii = 1 : 2 : numel(data_ss0)
  fprintf(fid0_i,'%d %d %d %d\n',...
          real(data_ss0.int(ii+0)),imag(data_ss0.int(ii+0)),...
          real(data_ss0.int(ii+1)),imag(data_ss0.int(ii+1)));
  fprintf(fid1_i,'%d %d %d %d\n',...
          real(data_ss1).int(ii+0),imag(data_ss1).int(ii+0),...
          real(data_ss1).int(ii+1),imag(data_ss1).int(ii+1));
end
fclose(fid0_i);
fclose(fid1_i);

% Output samples in transform order, alternating ss0 then ss1:
fid0_o = fopen('data/sig_o_0.txt','w');
fid1_o = fopen('data/sig_o_1.txt','w');
data_ss0 = reshape(sig_o(:,1:2:end),1,[]);
data_ss1 = reshape(sig_o(:,2:2:end),1,[]);
for ii = 1 : 2 : numel(data_ss0)
  fprintf(fid0_o,'%d %d %d %d\n',...
          real(data_ss0.int(ii+0)),imag(data_ss0.int(ii+0)),...
          real(data_ss0.int(ii+1)),imag(data_ss0.int(ii+1)));
  fprintf(fid1_o,'%d %d %d %d\n',...
          real(data_ss1).int(ii+0),imag(data_ss1).int(ii+0),...
          real(data_ss1).int(ii+1),imag(data_ss1).int(ii+1));
end
fclose(fid0_o);
fclose(fid1_o);


