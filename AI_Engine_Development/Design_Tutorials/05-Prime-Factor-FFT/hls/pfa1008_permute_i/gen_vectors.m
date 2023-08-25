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

% Create incoming streams 'ss0' and 'ss1' delivering samples in polyphase order:
% --> arrange for 1st dimension FFT (along N1 axis)
data_i = reshape(sig_i,2,[]);
ss0_i = data_i(1,:);
ss1_i = data_i(2,:);

% Create expected outputs with applied permutation, with alternating transforms on ss0, ss1:
data_o = reshape(sig_o,N1,[]);
ss0_o = reshape(data_o(:,1:2:end),1,[]);
ss1_o = reshape(data_o(:,2:2:end),1,[]);

% ------------------------------------------------------------
% Write I/O Files
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

fid = fopen('data/ss0_i.txt','w');
for ii = 1 : numel(ss0_i)
  fprintf(fid,'%d %d\n',real(ss0_i(ii)).int,imag(ss0_i(ii)).int);
end
fclose(fid);

fid = fopen('data/ss1_i.txt','w');
for ii = 1 : numel(ss1_i)
  fprintf(fid,'%d %d\n',real(ss1_i(ii)).int,imag(ss1_i(ii)).int);
end
fclose(fid);

fid = fopen('data/ss0_o.txt','w');
for ii = 1 : numel(ss0_o)
  fprintf(fid,'%d %d\n',real(ss0_o(ii)).int,imag(ss0_o(ii)).int);
end
fclose(fid);

fid = fopen('data/ss1_o.txt','w');
for ii = 1 : numel(ss1_o)
  fprintf(fid,'%d %d\n',real(ss1_o(ii)).int,imag(ss1_o(ii)).int);
end
fclose(fid);



