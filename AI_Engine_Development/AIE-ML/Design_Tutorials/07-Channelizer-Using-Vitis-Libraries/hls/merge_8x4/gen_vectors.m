%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Faisal El-Shabani

clear all;
close all;

% ------------------------------------------------------------
% Create Permutation Mapping
% ------------------------------------------------------------

NSTREAM_I = 8;
NSTREAM_O = 4;
NSAMP = 512;

% ------------------------------------------------------------
% Create Testbench Stimulus
% ------------------------------------------------------------

% Generate walking sequence pattern for easy testing:
seq_i = [0:NSAMP-1];                     % These samples are a proxy for 'cint32' values (ie. 64-bits each)
tmp = reshape(seq_i,NSTREAM_I,[]);
sig_i = tmp;

% ------------------------------------------------------------
% Write I/O Files
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

% Write streams in polyphase order:
for ss = 1 : NSTREAM_I
  fid = fopen(sprintf('data/stream%d_i.txt',ss-1),'w');
  data = sig_i(ss,:);
  for cc = 1 : size(data,2)
    fprintf(fid,'%d\n',data(cc));
  end
  fclose(fid);
end

% Write streams taken row-wise in polyphase order:
data = reshape(seq_i,NSTREAM_O,[]);
for ss = 1 : NSTREAM_O
  fid = fopen(sprintf('data/stream%d_o.txt',ss-1),'w');
    for ii = 1 : size(data,2)
        fprintf(fid,'%d\n',data(ss,ii));
    end
    fclose(fid);
end
