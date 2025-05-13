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
image_i  = single(complex(randn(Nsamp,Niter),randn(Nsamp,Niter)));
interp_i   = single(complex(randn(Nsamp,Niter),randn(Nsamp,Niter)));
phase_i = single(complex(randn(Nsamp,Niter),randn(Nsamp,Niter)));

image_o = image_i + interp_i .* phase_i;

% ------------------------------------------------------------
% Create stimulus for AI Engine
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

fid = fopen('data/image_i.txt','w');
data_i = reshape(image_i,1,[]);
for ii = 1 : numel(data_i)
  fprintf(fid,'%.9e %.9e\n',real(data_i(ii)),imag(data_i(ii)));
end
fclose(fid);

fid = fopen('data/interp_real_i.txt','w');
data_i = reshape(real(interp_i),1,[]);
for ii = 1 : 2 : numel(data_i)
  fprintf(fid,'%.9e %.9e\n',data_i(ii),data_i(ii+1));
end
fclose(fid);

fid = fopen('data/interp_imag_i.txt','w');
data_i = reshape(imag(interp_i),1,[]);
for ii = 1 : 2 : numel(data_i)
  fprintf(fid,'%.9e %.9e\n',data_i(ii),data_i(ii+1));
end
fclose(fid);

fid = fopen('data/phase_i.txt','w');
data_i = reshape(phase_i,1,[]);
for ii = 1 : numel(data_i)
  fprintf(fid,'%.9e %.9e\n',real(data_i(ii)),imag(data_i(ii)));
end
fclose(fid);

fid = fopen('data/image_o.txt','w');
data_o = reshape(image_o,1,[]);
for ii = 1 : numel(data_o)
  fprintf(fid,'%.9e %.9e\n',real(data_o(ii)),imag(data_o(ii)));
end
fclose(fid);

% ------------------------------------------------------------
% Vitis Functional Simulation
% ------------------------------------------------------------

vfs_model = vfs.aieGraph(input_file="bp_update_app.cpp",include_paths = {"."},...
                         part="xcvc1902-vsva2197-2MP-e-S");

v_phase_i       = varray.cfloat(reshape(phase_i,1,[]));
v_image_i       = varray.cfloat(reshape(image_i,1,[]));
v_interp_real_i = varray.float(reshape(real(interp_i),1,[]));
v_interp_imag_i = varray.float(reshape(imag(interp_i),1,[]));
vfs_o = vfs_model.run(v_image_i,v_phase_i,v_interp_real_i,v_interp_imag_i);
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
