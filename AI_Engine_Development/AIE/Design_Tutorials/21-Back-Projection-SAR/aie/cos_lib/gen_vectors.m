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

Niter = 512;
Nsamp = 512;

% Model:
tmp = single(rand(Nsamp,Niter));
sig_i = sort(tmp,1);
sig_o = cos(2*pi*sig_i);

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

vfs_model = vfs.aieGraph(input_file="cos_lib_app.cpp",...
                         include_paths = { sprintf('%s/L2/include/aie',getenv('DSPLIB_ROOT')),...
                                           sprintf('%s/L1/include/aie',getenv('DSPLIB_ROOT')),...
                                           sprintf('%s/L1/src/aie',getenv('DSPLIB_ROOT')) },...
                         part="xcvc1902-vsva2197-2MP-e-S");

vfs_i = varray.float(reshape(sig_i,1,[]));
vfs_o = vfs_model.run(vfs_i);
vfs_o = single(reshape(vfs_o,1,numel(vfs_o)));
err_o = single(data_o - vfs_o);

figure;
subplot(3,1,1); plot(data_i,'b.-'); title('cos() Input');
subplot(3,1,2); plot(data_o,'b.-'); hold on; plot(vfs_o,'r.'); hold off; title('cos() Output');
subplot(3,1,3); plot(err_o,'k.-'); ylabel('error'); title('cos() Error'); 
yline(5e-6,'r'); yline(-5e-6,'r');

if (max(abs(err_o)) < 5e-6)
  fprintf(1,'--- PASSED ---\n');
else
  fprintf(1,'*** FAILED ***\n');
end
