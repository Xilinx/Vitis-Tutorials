%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;

% ------------------------------------------------------------
% Setup Model & Stimulus
% ------------------------------------------------------------

Niter = 4;
NFFT = 2048;
Nports = 1;

% Model:
sig_i = single(complex(randn(NFFT,Niter),randn(NFFT,Niter)));
tmp = ifft(sig_i,[],1);
for ii = 1 : Niter
  ifft_o(:,ii) = fftshift(tmp(:,ii));
end

% Create (slope/offset) LUT for each transform:
tmp = single(linspace(0,1,1+NFFT)); xvec = transpose(tmp(1:end-1));
real_o = single(zeros(2*NFFT,Niter));
imag_o = single(zeros(2*NFFT,Niter));
for ii = 1 : Niter
  data = ifft_o(:,ii);
  slope = ([data(2:end);0]-data) * NFFT;
  slope(end) = 0;
  offset = data - slope.*xvec;
  offset(end) = 0;
  tmp = reshape([transpose(slope);transpose(offset)],1,[]);
  real_o(:,ii) = real(tmp);
  imag_o(:,ii) = imag(tmp);
end

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
  fid = fopen(sprintf('data/real_o_%d.txt',pp-1),'w');
  data_o = reshape(real_o(pp:Nports:end,:),1,[]);
  for ii = 1 : 2 : numel(data_o)
    fprintf(fid,'%.9e %.9e\n',data_o(ii),data_o(ii+1));  % Real
  end
  fclose(fid);
end

for pp = 1 : Nports
  fid = fopen(sprintf('data/imag_o_%d.txt',pp-1),'w');
  data_o = reshape(imag_o(pp:Nports:end,:),1,[]);
  for ii = 1 : 2 : numel(data_o)
    fprintf(fid,'%.9e %.9e\n',data_o(ii),data_o(ii+1));  % Real
  end
  fclose(fid);
end

% ------------------------------------------------------------
% Vitis Functional Simulation
% ------------------------------------------------------------

vfs_model = vfs.aieGraph(input_file="ifft2k_async_app.cpp",...
                         include='.',...
                         part="xcvc1902-vsva2197-2MP-e-S");

vfs_i = varray.cfloat(reshape(sig_i,Nports,[]));
[vfs_real_o,vfs_imag_o] = vfs_model.run(vfs_i);
vfs_real_o = reshape(single(transpose(vfs_real_o)),2*NFFT,Niter);
vfs_imag_o = reshape(single(transpose(vfs_imag_o)),2*NFFT,Niter);
err_real_o = single(real_o - vfs_real_o);
err_imag_o = single(imag_o - vfs_imag_o);

figure;
sig_i = reshape(sig_i,1,[]);
real_o = reshape(real_o,1,[]);
imag_o = reshape(imag_o,1,[]);
vfs_real_o = reshape(vfs_real_o,1,[]);
vfs_imag_o = reshape(vfs_imag_o,1,[]);
err_real_o = reshape(err_real_o,1,[]);
err_imag_o = reshape(err_imag_o,1,[]);
subplot(3,2,1); plot(real(sig_i),'b.-'); ylabel('input real'); title('ifft() Workload');
subplot(3,2,2); plot(imag(sig_i),'b.-'); ylabel('input imag');
subplot(3,2,3); plot(real_o,'b.-'); hold on; plot(vfs_real_o,'r.'); hold off; ylabel('output real');
subplot(3,2,4); plot(imag_o,'b.-'); hold on; plot(vfs_imag_o,'r.'); hold off; ylabel('output imag');
subplot(3,2,5); plot(err_real_o,'k.-'); ylabel('error real');
subplot(3,2,6); plot(err_imag_o,'k.-'); ylabel('error imag');

err_bit = log2(min([rms(real_o)/rms(err_real_o),rms(imag_o)/rms(err_imag_o)]));
if (err_bit > 22)
  fprintf(1,'--- PASSED ---\n');
else
  fprintf(1,'*** FAILED ***\n');
end
