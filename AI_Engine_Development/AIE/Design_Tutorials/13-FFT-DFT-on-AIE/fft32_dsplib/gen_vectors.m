%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;

% $$$ rng(3);
Nfft = 32;
Ntransform = 512;
backoff_dB = -15;                       % To achieve (-1,+1) range on AWGN signal
Nports = 1;

% Set quantization:
FF = fimath('RoundingMethod','Nearest','OverflowAction','Saturate');
TT = numerictype(1,16,15);

% Downshift (to ensure no clipping occurs in FFT):
dnshift = 3;

% Input signal:
sig_i = sqrt(0.5)*10^(0.05*backoff_dB)*complex(randn(Nfft,Ntransform),randn(Nfft,Ntransform));
scale = 0.5^dnshift;
sig_i = fi(scale*sig_i,TT,FF);

% Output (leave FFT unscaled here):
sig_o = fft(double(sig_i),[],1);
sig_o = fi(sig_o,TT,FF);

if (0)                                  % Test passthrough
  sig_o = sig_i;
end

% Turn into streams:
data_i = reshape(sig_i,1,[]);
data_o = reshape(sig_o,1,[]);

% Save to PLIO files:
[~,~,~] = mkdir('data');
for pp = 1 : Nports
  % Write input files:
  fid_i = fopen(sprintf('data/sig%d_i.txt',pp-1),'w');
  xx_re = real(data_i(pp:Nports:end));
  xx_im = imag(data_i(pp:Nports:end));
  for ii = 1 : 2 : length(xx_re)
    fprintf(fid_i,'%d %d  ',xx_re.int(ii  ),xx_im.int(ii  ));
    fprintf(fid_i,'%d %d\n',xx_re.int(ii+1),xx_im.int(ii+1));
  end
  fclose(fid_i);
  % Write output files:
  fid_o = fopen(sprintf('data/sig%d_o.txt',pp-1),'w');
  xx_re = real(data_o(pp:Nports:end));
  xx_im = imag(data_o(pp:Nports:end));
  for ii = 1 : 2 : length(xx_re)
    fprintf(fid_o,'%d %d  ',xx_re.int(ii  ),xx_im.int(ii  ));
    fprintf(fid_o,'%d %d\n',xx_re.int(ii+1),xx_im.int(ii+1));
  end
  fclose(fid_o);
  disp(sprintf('Done writing PLIO files for port %g',pp-1));
end

