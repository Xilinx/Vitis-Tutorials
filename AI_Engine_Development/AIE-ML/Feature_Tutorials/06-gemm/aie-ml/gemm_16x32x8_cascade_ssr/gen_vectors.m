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
TP_SSR = 2;
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

off_c = NAB/TP_CASC_LEN;
off_s = NA/TP_SSR;

% Note:
%    When using AIE-ML with <int32> x <int32> data types, we use 4x4 tiling exclusively:

pp = 1;
for ss = 1 : TP_SSR
  range_s = off_s*(ss-1) + [1:off_s];
  for cc = 1 : TP_CASC_LEN
    fidA = fopen(sprintf('data/sigA_%d_i.txt',pp-1),'w');
    range_c = off_c*(cc-1) + [1:off_c];
    for tt = 1 : Niter
      dataA = tiling_4x4(A(range_s,range_c,tt),COL);
      for ii = 1 : 2 : numel(dataA)
        fprintf(fidA,'%d %d\n',dataA.int(ii),dataA.int(ii+1));
      end
    end
    fclose(fidA);
    pp = pp + 1;
  end
end

pp = 1;
for ss = 1 : TP_SSR
  for cc = 1 : TP_CASC_LEN
    range_c = off_c*(cc-1) + [1:off_c];
    fidB = fopen(sprintf('data/sigB_%d_i.txt',pp-1),'w');
    for tt = 1 : Niter
      dataB = tiling_4x4(B(range_c,:,tt),ROW);
      for ii = 1 : 2 : numel(dataB)
        fprintf(fidB,'%d %d\n',dataB.int(ii),dataB.int(ii+1));
      end
    end
    fclose(fidB);
    pp = pp+1;
  end
end

off_s = NA/TP_SSR;

for pp = 1 : NPORT_O
  fid = fopen(sprintf('data/sig_%d_o.txt',pp-1),'w');
  range_s = off_s*(pp-1)+[1:off_s];
  for tt = 1 : Niter
    data = tiling_4x4(C(range_s,:,tt),COL);
    for ii = 1 : 2 : numel(data)
      fprintf(fid,'%d %d\n',data.int(ii),data.int(ii+1));
    end
  end
  fclose(fid);
end


