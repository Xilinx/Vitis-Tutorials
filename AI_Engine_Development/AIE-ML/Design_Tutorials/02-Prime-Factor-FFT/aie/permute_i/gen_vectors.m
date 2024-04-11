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

N1 = 7;
N2 = 9; 
N3 = 16;
[P_i] = compute_perm_3d(N1,N2,N3);

% ------------------------------------------------------------
% Create Testbench Stimulus
% ------------------------------------------------------------

Ntransform = 8;
Nfft = N1 * N2 * N3;

% Generate inputs as repeated linear ramp to easily test permutation:
sig_i = repmat([0:Nfft-1]',1,Ntransform);

% Quantize:
TT = numerictype(1,16,0);
FF = fimath('RoundingMethod','Round','OverflowAction','Saturate');
sig_i = fi(sig_i,TT,FF);

% Generate outputs:
sig_o = fi(zeros(size(sig_i)),TT,FF);
for tt = 1 : Ntransform
  data = sig_i(:,tt);
  sig_o(:,tt) = data(1+P_i);
end

% ------------------------------------------------------------
% Write I/O Files
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

fid = fopen('data/sig_i.txt','w');
data = reshape(sig_i,1,[]);
for ii = 1 : 2 : numel(data)
  fprintf(fid,'%d %d %d %d \n',...
          real(data(ii+0)).int,imag(data(ii+0)).int,real(data(ii+1)).int,imag(data(ii+1)).int);
end
fclose(fid);

fid = fopen('data/sig_o.txt','w');
data = reshape(sig_o,1,[]);
for ii = 1 : 2 : numel(data)
  fprintf(fid,'%d %d %d %d \n',...
          real(data(ii+0)).int,imag(data(ii+0)).int,real(data(ii+1)).int,imag(data(ii+1)).int);
end
fclose(fid);
