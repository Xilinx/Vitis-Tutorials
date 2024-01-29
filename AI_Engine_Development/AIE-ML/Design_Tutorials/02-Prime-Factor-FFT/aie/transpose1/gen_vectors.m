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
% --> Input data to 'transpose1' is coming from FFT-9 transforms in the 2nd dimension

% Original linear order:
sig_L = repmat(complex([0:Nfft-1]'),1,Ntransform);

% Now put in FFT-9 order:
permI = compute_addr_3d(N1,N2,N3,2);    % Select 2nd dimension for input
for tt = 1 : Ntransform
  data = sig_L(:,tt);
  sig_i(:,tt) = data(1+permI);
end

% Generate outputs:
% --> Output data from 'transpose1' is inputs to FFT-16 transforms in the 3rd dimension
permO = compute_addr_3d(N1,N2,N3,3);    % Select 3rd dimension for output
for tt = 1 : Ntransform
  data = sig_L(:,tt);
  sig_o(:,tt) = data(1+permO);
end

% Quantize:
TT = numerictype(1,16,0);
FF = fimath('RoundingMethod','Round','OverflowAction','Saturate');
sig_i = fi(sig_i,TT,FF);
sig_o = fi(sig_o,TT,FF);

% ------------------------------------------------------------
% Write I/O Files
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

fid = fopen('data/sig_i.txt','w');
for tt = 1 : Ntransform
  data = reshape(sig_i(:,tt),1,[]); % This is the DMA order used by the hardware
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
  
