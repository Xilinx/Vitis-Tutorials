%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;

rng(1);

Nsamp = 16;
Ntrials = 24;
sort_i = zeros(Nsamp,Ntrials);
sort_o = zeros(Nsamp,Ntrials);
for tt = 1 : Ntrials
   sort_i(:,tt) = randn(Nsamp,1);
   sort_o(:,tt) = sort(sort_i(:,tt));
end

% ------------------------------------------------------------
% Generate I/O files:
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

fi = fopen('data/sig_i.txt','w');
fo = fopen('data/sig_o.txt','w');

fprintf(fi,'%16.7e %16.7e\n',reshape(sort_i,1,[]));
fprintf(fo,'%16.7e %16.7e\n',reshape(sort_o,1,[]));

fclose(fi);
fclose(fo);

