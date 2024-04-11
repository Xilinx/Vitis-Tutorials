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
% --> Input data to 'transpose0' is coming from FFT-7 transforms in the 1st dimension
sig_i = repmat(complex([0:Nfft-1]'),1,Ntransform);

% Quantize:
TT = numerictype(1,16,0);
FF = fimath('RoundingMethod','Round','OverflowAction','Saturate');
sig_i = fi(sig_i,TT,FF);

% Generate outputs:
% --> Output data from 'transpose0' is inputs to FFT-9 transforms in the 2nd dimension
perm = compute_addr_3d(N1,N2,N3,2);
sig_o = fi(zeros(size(sig_i)),TT,FF);
for tt = 1 : Ntransform
  data = sig_i(:,tt);
  sig_o(:,tt) = data(1+perm);
end

% ------------------------------------------------------------
% Write I/O Files
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

fid = fopen('data/sig_i.txt','w');
for tt = 1 : Ntransform
  data = reshape(sig_i(:,tt),1,[]);
  for ii = 1 : 2 : numel(data)
    fprintf(fid,'%d %d %d %d \n',...
            real(data(ii+0)).int,imag(data(ii+0)).int,real(data(ii+1)).int,imag(data(ii+1)).int);
  end
end
fclose(fid);

fid = fopen('data/sig_o.txt','w');
for tt = 1 : Ntransform
  data = reshape(sig_o(:,tt),1,[]); % This is the DMA order used by the hardware
  for ii = 1 : 2 : numel(data)
    fprintf(fid,'%d %d %d %d \n',...
            real(data(ii+0)).int,imag(data(ii+0)).int,real(data(ii+1)).int,imag(data(ii+1)).int);
  end
end
fclose(fid);
