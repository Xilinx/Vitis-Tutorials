%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;
addpath('../../matlab');

% ------------------------------------------------------------
% Create Permutation Mapping
% ------------------------------------------------------------

NSTREAM = 5;
NFFT_1D = 256;
EXTRA = 4;
NFFT = NFFT_1D * NFFT_1D;
NSAMP = NFFT_1D + EXTRA;

% ------------------------------------------------------------
% Create Testbench Stimulus
% ------------------------------------------------------------

% Generate walking sequence pattern for easy testing:
seq_i = [0:NFFT-1];                     % These samples are a proxy for 'cint32' values (ie. 64-bits each)
tmp = reshape(seq_i,NFFT_1D,NFFT_1D);
tmp = cat(2,tmp,zeros(NFFT_1D,EXTRA));
sig_i = cat(1,tmp,zeros(EXTRA,NFFT_1D+EXTRA));

% ------------------------------------------------------------
% Write I/O Files
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

% Write input sequence in linear DDR4 order:
fid = fopen('data/seq_i.txt','w');
for ii = 1 : NFFT
  fprintf(fid,'%d\n',seq_i(ii));
end
fclose(fid);

% Write streams in polyphase order:

for ss = 1 : NSTREAM
  fid = fopen(sprintf('data/stream%d_o.txt',ss-1),'w');
  data = sig_i(ss:NSTREAM:end,:);
  for rr = 1 : size(data,1)
    for cc = 1 : size(data,2)
      fprintf(fid,'%d\n',data(rr,cc));
    end
  end
  fclose(fid);
end
