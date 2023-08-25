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
Nfft = 16;
Ntransform = 8*63;
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
data = reshape(twiddle(:, 1: 4).',8,[]); TWID0a = reshape(data(:,1:2:end),1,[]);  TWID0b = reshape(data(:,2:2:end),1,[]);  
data = reshape(twiddle(:, 5: 8).',8,[]); TWID1a = reshape(data(:,1:2:end),1,[]);  TWID1b = reshape(data(:,2:2:end),1,[]);  
data = reshape(twiddle(:, 9:12).',8,[]); TWID2a = reshape(data(:,1:2:end),1,[]);  TWID2b = reshape(data(:,2:2:end),1,[]);  
data = reshape(twiddle(:,13:16).',8,[]); TWID3a = reshape(data(:,1:2:end),1,[]);  TWID3b = reshape(data(:,2:2:end),1,[]);  

% Quantize:
TWID0a = fi(TWID0a,TT,'RoundingMethod','Convergent','OverflowAction','Saturate');
TWID0b = fi(TWID0b,TT,'RoundingMethod','Convergent','OverflowAction','Saturate');
TWID1a = fi(TWID1a,TT,'RoundingMethod','Convergent','OverflowAction','Saturate');
TWID1b = fi(TWID1b,TT,'RoundingMethod','Convergent','OverflowAction','Saturate');
TWID2a = fi(TWID2a,TT,'RoundingMethod','Convergent','OverflowAction','Saturate');
TWID2b = fi(TWID2b,TT,'RoundingMethod','Convergent','OverflowAction','Saturate');
TWID3a = fi(TWID3a,TT,'RoundingMethod','Convergent','OverflowAction','Saturate');
TWID3b = fi(TWID3b,TT,'RoundingMethod','Convergent','OverflowAction','Saturate');

fid = fopen('dft16_twiddle.h','w');
fprintf(fid,'//\n');
fprintf(fid,'// Copyright (fid,C) 2023, Advanced Micro Devices, Inc. All rights reserved.\n');
fprintf(fid,'// SPDX-License-Identifier: MIT\n');
fprintf(fid,'//\n');
fprintf(fid,'// Author: Mark Rollins\n');
fprintf(fid,'\n');
fprintf(fid,'#pragma once\n');
write_twiddle( fid, TWID0a, 'DFT16_TWID0a' );
write_twiddle( fid, TWID0b, 'DFT16_TWID0b' );
write_twiddle( fid, TWID1a, 'DFT16_TWID1a' );
write_twiddle( fid, TWID1b, 'DFT16_TWID1b' );
write_twiddle( fid, TWID2a, 'DFT16_TWID2a' );
write_twiddle( fid, TWID2b, 'DFT16_TWID2b' );
write_twiddle( fid, TWID3a, 'DFT16_TWID3a' );
write_twiddle( fid, TWID3b, 'DFT16_TWID3b' );
fclose(fid);

% ------------------------------------------------------------
% Save I/O files for simulation
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

% Samples alternate four from ss0 then four from ss1:
fid0_i = fopen('data/sig_i_0.txt','w');
fid1_i = fopen('data/sig_i_1.txt','w');
tmp = reshape(sig_i,4,[]);
data_ss0 = reshape(tmp(:,1:2:end),1,[]);
data_ss1 = reshape(tmp(:,2:2:end),1,[]);
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

% Samples alternate four from ss0 then four from ss1:
fid0_o = fopen('data/sig_o_0.txt','w');
fid1_o = fopen('data/sig_o_1.txt','w');
tmp = reshape(sig_o,4,[]);
data_ss0 = reshape(tmp(:,1:2:end),1,[]);
data_ss1 = reshape(tmp(:,2:2:end),1,[]);
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


function write_twiddle( fid, TWID, name )
   fprintf(fid,'#define %s {\\\n',name);
   for ii = 1 : numel(TWID)
     if (ii == numel(TWID)) fprintf(fid,'{%d,%d} \\\n',real(TWID.int(ii)),imag(TWID.int(ii)));
     else                   fprintf(fid,'{%d,%d},\\\n',real(TWID.int(ii)),imag(TWID.int(ii)));
     end
   end
   fprintf(fid,'}\n');
end
