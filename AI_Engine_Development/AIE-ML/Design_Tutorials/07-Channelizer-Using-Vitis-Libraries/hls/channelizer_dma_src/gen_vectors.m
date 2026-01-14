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

NTRANSFORMS = 4;
NCHAN = 4096;                      % Number of channels in channelizer
NSTREAMS=2;                        % Channelizer design Needs to run at cint16@2 Gsps, corresponding to 2 HLS streams 128-bits@312.5

% ------------------------------------------------------------
% Create Testbench Stimulus
% ------------------------------------------------------------

% Generate walking sequence pattern for easy testing:
seq_i = [0:NCHAN*NTRANSFORMS-1];                            % These samples are a proxy for 'cint32' values (ie. 64-bits each)
tmp = reshape(seq_i,NSTREAMS,NCHAN/NSTREAMS,NTRANSFORMS);
sig_i = tmp;

% ------------------------------------------------------------
% Write I/O Files
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

% Write input sequence in linear DDR4 order:
fid = fopen('data/seq_i.txt','w');
for ii = 1 : length(seq_i)
  fprintf(fid,'%d\n',seq_i(ii));
end
fclose(fid);

% Write stream to text file:
for ee = 1 : NSTREAMS
  filename = sprintf('data/stream%d_o.txt',ee-1);
  fid = fopen(filename,'w');
  for tt = 1 : NTRANSFORMS
      data = sig_i(ee,:,tt);
      for rr = 1 : size(data,2)
          fprintf(fid,'%d\n',data(rr));
      end
  end
  fclose(fid);
end