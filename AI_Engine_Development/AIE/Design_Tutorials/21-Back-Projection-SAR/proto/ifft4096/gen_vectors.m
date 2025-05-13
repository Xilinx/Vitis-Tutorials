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

Niter = 8;
NFFT = 4096;
Nports = 2;

% Model:
sig_i = complex(randn(NFFT,Niter),randn(NFFT,Niter));
sig_o = NFFT*ifft(sig_i,[],1);

% ------------------------------------------------------------
% Create stimulus for AI Engine
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

for pp = 1 : Nports
  fid = fopen(sprintf('data/sig_i_%d.txt',pp-1),'w');
  data_i = reshape(sig_i(pp:Nports:end,:),1,[]);
  for ii = 1 : numel(data_i)
    fprintf(fid,'%.9e %.9e\n',real(data_i(ii)),imag(data_i(ii))); % Complex
  end
  fclose(fid);
end

for pp = 1 : Nports
  fid = fopen(sprintf('data/sig_o_%d.txt',pp-1),'w');
  data_o = reshape(sig_o(pp:Nports:end,:),1,[]);
  for ii = 1 : numel(data_o)
    fprintf(fid,'%.9e %.9e\n',real(data_o(ii)),imag(data_o(ii))); % Complex
  end
  fclose(fid);
end

% ------------------------------------------------------------
% Vitis Functional Simulation
% ------------------------------------------------------------

run(sprintf('%s/%s',getenv("XILINX_VITIS"),'functional_suite/addToMatlabPath.m'));
vfs_model = vfs.aieGraph(input_file="ifft4096_app.cpp",...
                         include_paths = { sprintf('%s/L2/include/aie',getenv('DSPLIB_ROOT')),...
                                           sprintf('%s/L1/include/aie',getenv('DSPLIB_ROOT')),...
                                           sprintf('%s/L1/src/aie',getenv('DSPLIB_ROOT')) },...
                         part="xcvc1902-vsva2197-2MP-e-S");

% Would like to send & return a matrix here, but need to break out ports explicitly for now:
  vfs_i = varray.cfloat(reshape(sig_i,Nports,[]));
if ( Nports == 2 )
  [vfs_0,vfs_1] = vfs_model.run(vfs_i(1,:),vfs_i(2,:));
  vfs_o = reshape(single([transpose(vfs_0);transpose(vfs_1)]),NFFT,Niter);
elseif (Nports == 4)
  [vfs_0,vfs_1,vfs_2,vfs_3] = vfs_model.run(vfs_i(1,:),vfs_i(2,:),vfs_i(3,:),vfs_i(4,:));
  vfs_o = reshape(single([transpose(vfs_0);transpose(vfs_1);transpose(vfs_2);transpose(vfs_3)]),NFFT,Niter);
else
  error('Unsupported');
end
err_o = single(sig_o - vfs_o);

figure;
sig_i = reshape(sig_i,1,[]);
sig_o = reshape(sig_o,1,[]);
vfs_o = reshape(vfs_o,1,[]);
err_o = reshape(err_o,1,[]);
subplot(3,2,1); plot(real(sig_i),'b.-'); ylabel('input real'); title('ifft() Workload');
subplot(3,2,2); plot(imag(sig_i),'b.-'); ylabel('input imag');
subplot(3,2,3); plot(real(sig_o),'b.-'); hold on; plot(real(vfs_o),'r.'); hold off; ylabel('output real');
subplot(3,2,4); plot(imag(sig_o),'b.-'); hold on; plot(imag(vfs_o),'r.'); hold off; ylabel('output imag');
subplot(3,2,5); plot(real(err_o),'k.-'); ylabel('error real');
subplot(3,2,6); plot(imag(err_o),'k.-'); ylabel('error imag');

if (rms(err_o)/rms(sig_o) < 1e-6)
  fprintf(1,'--- PASSED ---\n');
else
  fprintf(1,'*** FAILED ***\n');
end
