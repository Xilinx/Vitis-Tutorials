%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

addpath('../../matlab');
clear all;
close all;

rng(1);

Niter = 4;
NA = 16;
NAB = 32;
NB = 8;

% These variables must match the parameters in 'gemm_16x32x8_graph.h':
TP_SSR = 1;
TP_CASC_LEN = 4;
NPORT_I = TP_CASC_LEN * TP_SSR;
NPORT_O = TP_SSR;

TT = numerictype(1,32,27);
FF = fimath('RoundingMethod','Convergent','OverflowAction','Saturate');


tmpA = 1-2*rand(NA,NAB,Niter);
tmpB = 1-2*rand(NAB,NB,Niter);
A = fi(tmpA,TT,FF);
B = fi(tmpB,TT,FF);

C = zeros(NA,NB,Niter);
for tt = 1 : Niter
  C(:,:,tt) = double(A(:,:,tt)) * double(B(:,:,tt));
end
C = fi(C,TT,FF);

% ------------------------------------------------------------
% Store I/O Vectors to files
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

COL = 1;
ROW = 0;

for pp = 1 : NPORT_I
  fidA = fopen(sprintf('data/sigA_%d_i.txt',pp-1),'w');
  for tt = 1 : Niter
    range = NAB/NPORT_I*(pp-1) + [1:NAB/NPORT_I];
    dataA = tiling_4x4(A(:,range,tt),COL);
    for ii = 1 : 2 : numel(dataA)
      fprintf(fidA,'%d %d\n',dataA.int(ii),dataA.int(ii+1));
    end
  end
  fclose(fidA);
end

for pp = 1 : NPORT_I
  fidB = fopen(sprintf('data/sigB_%d_i.txt',pp-1),'w');
  for tt = 1 : Niter
    range = NAB/NPORT_I*(pp-1) + [1:NAB/NPORT_I];
    dataB = tiling_4x2(B(range,:,tt),ROW);
    for ii = 1 : 2 : numel(dataB)
      fprintf(fidB,'%d %d\n',dataB.int(ii),dataB.int(ii+1));
    end
  end
  fclose(fidB);
end

for pp = 1 : NPORT_O
  fid = fopen(sprintf('data/sig_%d_o.txt',pp-1),'w');
  for tt = 1 : Niter
    range = NB/NPORT_O*(pp-1)+[1:NB/NPORT_O];
    data = tiling_4x2(C(:,range,tt),COL);
    for ii = 1 : 2 : numel(data)
      fprintf(fid,'%d %d\n',data.int(ii),data.int(ii+1));
    end
  end
  fclose(fid);
end


