%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;

rng(1);

% ------------------------------------------------------------
% Matlab System Model
% ------------------------------------------------------------

Nsamp = 1024;
Ntrials = 4;
sort_i = single(zeros(Nsamp,Ntrials));
sort_o = single(zeros(Nsamp,Ntrials));
for tt = 1 : Ntrials
   sort_i(:,tt) = randn(Nsamp,1);
% $$$    sort_i(:,tt) = randi([0,Nsamp-1],Nsamp,1);
   sort_o(:,tt) = sort(sort_i(:,tt));
end

% ------------------------------------------------------------
% Run X86
% ------------------------------------------------------------

if ( exist('./Work/matlab','dir') )
  myGraph = aieGraph('./Work/matlab');
  sort_a = single(zeros(Nsamp,Ntrials));
  for tt = 1 : Ntrials
    sort_a(:,tt) = myGraph.run(sort_i(:,tt));
  end
  data_o = reshape(sort_o,1,[]);
  data_a = reshape(sort_a,1,[]);
  err = data_o - data_a;
  figure;
  subplot(2,1,1); plot(reshape(sort_i,1,[]),'k.-');
  title('Unsorted Inputs');
  subplot(2,1,2); plot(data_o,'b.-'); hold on; plot(data_a,'r.--'); hold off;
  title('Sorted Outputs');
  if ( max(abs(err)) <= single(eps) )
    fprintf(1,'--- PASSED ---\n');
  else
    fprintf(1,'*** FAILED ***\n');
  end
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

