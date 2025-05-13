%
% Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;

% ------------------------------------------------------------
% Setup Model & Stimulus
% ------------------------------------------------------------

Niter = 1;
Nsamp = 512*512;

% Model:
scale = 6800;                           % Ballpark range from SAR model
sig_i = single(scale*(1-2*rand(Nsamp,Niter)));
sig_i = sort(sig_i);
sig_o = mod(sig_i,1);                   % Reduce to range [0,1]

% ------------------------------------------------------------
% Create stimulus for AI Engine
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

fid = fopen('data/sig_i.txt','w');
data_i = reshape(sig_i,1,[]);
for ii = 1 : 2 : numel(data_i)
  fprintf(fid,'%.9e %.9e\n',data_i(ii),data_i(ii+1)); % Real
end
fclose(fid);

fid = fopen('data/sig_o.txt','w');
data_o = reshape(sig_o,1,[]);
for ii = 1 : 2 : numel(data_o)
  fprintf(fid,'%.9e %.9e\n',data_o(ii),data_o(ii+1)); % Real
end
fclose(fid);

% ------------------------------------------------------------
% Vitis Functional Simulation
% ------------------------------------------------------------

vfs_model = vfs.aieGraph(input_file="fmod_floor_app.cpp",include_paths = {"."},...
                         part="xcvc1902-vsva2197-2MP-e-S");

vfs_i = varray.float(reshape(sig_i,1,[]));
vfs_o = vfs_model.run(vfs_i);
vfs_o = single(reshape(vfs_o,1,numel(vfs_o)));
err_o = unwrap(2*pi*data_o) - unwrap(2*pi*vfs_o);
figure;
subplot(3,1,1); plot(data_i,'b.-'); title('fmod() Workload');
subplot(3,1,2); plot(unwrap(2*pi*data_o),'b.-'); hold on; plot(unwrap(2*pi*vfs_o),'r.--'); hold off; 
title('fmod() Output');
subplot(3,1,3); plot(err_o,'k.-'); ylabel('error'); title('fmod() error');

if (rms(err_o) < 2e-2)                  % These larger errors OK due to MAXVAL clipping
  fprintf(1,'--- PASSED ---\n');
else
  fprintf(1,'*** FAILED ***\n');
end
