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

Nfft = 16;
Ntransform = 8*4*7*9;
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

% Quantize:
twiddle = fi(twiddle,TT,'RoundingMethod','Convergent','OverflowAction','Saturate');

% Reorder into eight sections of 4 x 8 (left side then right side), each in row-major order
twiddle = reshape([reshape(twiddle( 1: 4,1: 8).',1,[]),...
                   reshape(twiddle( 5: 8,1: 8).',1,[]),...
                   reshape(twiddle( 9:12,1: 8).',1,[]),...
                   reshape(twiddle(13:16,1: 8).',1,[]),...
                   reshape(twiddle( 1: 4,9:16).',1,[]),...
                   reshape(twiddle( 5: 8,9:16).',1,[]),...
                   reshape(twiddle( 9:12,9:16).',1,[]),...
                   reshape(twiddle(13:16,9:16).',1,[])],1,[]);

fid = fopen('dft16_twiddle.h','w');
fprintf(fid,'//\n');
fprintf(fid,'// Copyright (fid,C) 2023, Advanced Micro Devices, Inc. All rights reserved.\n');
fprintf(fid,'// SPDX-License-Identifier: MIT\n');
fprintf(fid,'//\n');
fprintf(fid,'// Author: Mark Rollins\n');
fprintf(fid,'\n');
fprintf(fid,'#pragma once\n');

fprintf(fid,'#define DFT16_TWID {\\\n');
for ii = 1 : numel(twiddle)
  if (ii == numel(twiddle)) fprintf(fid,'{%d,%d} \\\n',real(twiddle.int(ii)),imag(twiddle.int(ii)));
  else                      fprintf(fid,'{%d,%d},\\\n',real(twiddle.int(ii)),imag(twiddle.int(ii)));
  end
end
fprintf(fid,'}\n');
fclose(fid);

% ------------------------------------------------------------
% Save I/O files for simulation
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

fid_i = fopen('data/sig_i.txt','w');
data = reshape(sig_i,1,[]);
for ii = 1 : 2 : numel(data)
  fprintf(fid_i,'%d %d %d %d\n',...
          real(data.int(ii+0)),imag(data.int(ii+0)),...
          real(data.int(ii+1)),imag(data.int(ii+1)));
end
fclose(fid_i);

fid_o = fopen('data/sig_o.txt','w');
data = reshape(sig_o,1,[]);
for ii = 1 : 2 : numel(data)
  fprintf(fid_o,'%d %d %d %d\n',...
          real(data.int(ii+0)),imag(data.int(ii+0)),...
          real(data.int(ii+1)),imag(data.int(ii+1)));
end
fclose(fid_o);


