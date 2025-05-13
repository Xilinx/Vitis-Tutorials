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

Niter = 16;
Nsamp = 512;

% Model:
pixel_i  = single(complex(randn(Nsamp,Niter),randn(Nsamp,Niter)));
dist_i   = single(complex(randn(Nsamp,Niter),randn(Nsamp,Niter)));
phcorr_i = single(complex(randn(Nsamp,Niter),randn(Nsamp,Niter)));

pixel_o = pixel_i + dist_i .* phcorr_i;

% ------------------------------------------------------------
% Create stimulus for AI Engine
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

fid = fopen('data/pixel_i.txt','w');
data_i = reshape(pixel_i,1,[]);
for ii = 1 : numel(data_i)
  fprintf(fid,'%.9e %.9e\n',real(data_i(ii)),imag(data_i(ii)));
end
fclose(fid);

fid = fopen('data/dist_i.txt','w');
data_i = reshape(dist_i,1,[]);
for ii = 1 : numel(data_i)
  fprintf(fid,'%.9e %.9e\n',real(data_i(ii)),imag(data_i(ii)));
end
fclose(fid);

fid = fopen('data/phcorr_i.txt','w');
data_i = reshape(phcorr_i,1,[]);
for ii = 1 : numel(data_i)
  fprintf(fid,'%.9e %.9e\n',real(data_i(ii)),imag(data_i(ii)));
end
fclose(fid);

fid = fopen('data/pixel_o.txt','w');
data_o = reshape(pixel_o,1,[]);
for ii = 1 : numel(data_o)
  fprintf(fid,'%.9e %.9e\n',real(data_o(ii)),imag(data_o(ii)));
end
fclose(fid);

% ------------------------------------------------------------
% Vitis Functional Simulation
% ------------------------------------------------------------

run(sprintf('%s/%s',getenv("XILINX_VITIS"),'functional_suite/addToMatlabPath.m'));
vfs_model = vfs.aieGraph(input_file="bp_update_app.cpp",include_paths = {"."},...
                         part="xcvc1902-vsva2197-2MP-e-S");

v_pixel_i  = varray.cfloat(reshape(pixel_i,1,[]));
v_dist_i   = varray.cfloat(reshape(dist_i,1,[]));
v_phcorr_i = varray.cfloat(reshape(phcorr_i,1,[]));
vfs_o = vfs_model.run(v_pixel_i,v_dist_i,v_phcorr_i);
vfs_o = single(reshape(vfs_o,1,numel(vfs_o)));
err_o = single(data_o - vfs_o);

figure;
subplot(2,2,1); plot(real(data_o),'b.-'); hold on; plot(real(vfs_o),'r.--'); hold off; ylabel('real');
title('BP Update Workload');
subplot(2,2,2); plot(imag(data_o),'b.-'); hold on; plot(imag(vfs_o),'r.--'); hold off; ylabel('imag');
subplot(2,2,3); plot(real(err_o),'b.-'); ylabel('real');
subplot(2,2,4); plot(imag(err_o),'b.-'); ylabel('imag');

if (max([abs(real(err_o)),abs(imag(err_o))]) < 1e-6)
  fprintf(1,'--- PASSED ---\n');
else
  fprintf(1,'*** FAILED ***\n');
end
