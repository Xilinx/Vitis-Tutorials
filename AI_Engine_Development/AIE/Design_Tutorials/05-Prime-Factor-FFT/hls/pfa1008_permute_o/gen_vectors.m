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
[P_i,P_o] = compute_perm_3d(N1,N2,N3);

% ------------------------------------------------------------
% Create Testbench Stimulus
% ------------------------------------------------------------

Ntransform = 8;
Nfft = N1 * N2 * N3;

% Generate inputs as repeated linear ramp to easily test permutation:
pattern_i = repmat([0:Nfft-1]',1,Ntransform);

% Quantize:
TT = numerictype(1,16,0);
FF = fimath('RoundingMethod','Round','OverflowAction','Saturate');
pattern_i = fi(pattern_i,TT,FF);

% Generate DFT-16 pattern required for processing N3 dimension transforms:
sig_i = zeros(Nfft,Ntransform);
for tt = 1 : Ntransform
  data_i = reshape(pattern_i(:,tt),N1,N2,N3);
  data_o = [];
  for rr = 1 : N1
    for cc = 1 : N2
      data_o = [data_o,reshape(data_i(rr,cc,:),[],1)];
    end
  end
  sig_i(:,tt) = reshape(data_o,[],1);
end
sig_i = fi(sig_i,TT,FF);

% Generate outputs after applying output permutation:
sig_o = zeros(Nfft,Ntransform);
for tt = 1 : Ntransform
  sig_o(1+P_o,tt) = pattern_i(:,tt);
end
sig_o = fi(sig_o,TT,FF);

% Inputs coming back from FFT-16 alternating four samples each from ss0 and ss1:
data_i = reshape(sig_i,4,[]);
ss0_i = reshape(data_i(:,1:2:end),1,[]);
ss1_i = reshape(data_i(:,2:2:end),1,[]);

% Outputs delivered to DMA sink in polyphase ordering over ss0 and ss1:
data_o = reshape(sig_o,2,[]);
ss0_o = data_o(1,:);
ss1_o = data_o(2,:);

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



