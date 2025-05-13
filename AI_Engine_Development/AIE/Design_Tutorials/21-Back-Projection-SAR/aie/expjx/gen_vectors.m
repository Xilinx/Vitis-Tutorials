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

Niter = 256;
Nsamp = 1024;

% Model:
tmp = single(rand(Nsamp,Niter));
sig_i = sort(tmp,1);
sig_o = complex(cos(2*pi*sig_i),sin(2*pi*sig_i));

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
for ii = 1 : numel(data_o)
  fprintf(fid,'%.9e %.9e\n',real(data_o(ii)),imag(data_o(ii))); % Complex
end
fclose(fid);

% ------------------------------------------------------------
% Vitis Functional Simulation
% ------------------------------------------------------------

vfs_model = vfs.aieGraph(input_file="expjx_app.cpp",...
                         include_paths = { sprintf('%s/L2/include/aie',getenv('DSPLIB_ROOT')),...
                                           sprintf('%s/L1/include/aie',getenv('DSPLIB_ROOT')),...
                                           sprintf('%s/L1/src/aie',getenv('DSPLIB_ROOT')),...
                                           '../cos_lib','../sin_lib','.' },...
                         part="xcvc1902-vsva2197-2MP-e-S");

vfs_i = varray.float(reshape(sig_i,1,[]));
vfs_o = vfs_model.run(vfs_i);
vfs_o = single(reshape(vfs_o,1,numel(vfs_o)));
err_o = single(data_o - vfs_o);

figure;
subplot(3,2,[1,2]); plot(data_i,'b.-'); title('expjx() Input');
subplot(3,2,3); plot(real(data_o),'b.-'); hold on; plot(real(vfs_o),'r.'); hold off; title('expjx() Output');
ylabel('real');
subplot(3,2,4); plot(imag(data_o),'b.-'); hold on; plot(imag(vfs_o),'r.'); hold off; title('expjx() Output');
ylabel('imag');
subplot(3,2,5); plot(real(err_o),'k.-'); ylabel('real error'); title('expjx() Error'); 
yline(5e-6,'r'); yline(-5e-6,'r');
subplot(3,2,6); plot(imag(err_o),'k.-'); ylabel('imag error'); title('expjx() Error'); 
yline(5e-6,'r'); yline(-5e-6,'r');

if (max([abs(real(err_o)),abs(imag(err_o))]) < 5e-6)
  fprintf(1,'--- PASSED ---\n');
else
  fprintf(1,'*** FAILED ***\n');
end
