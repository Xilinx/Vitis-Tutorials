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

Niter = 2;
Nsamp = 1024*16;

% Create LUT input:
NFFT = 2048;
lut_i = zeros(2,NFFT);
tmp = single(linspace(0,1,1+NFFT)); xvec = tmp(1:end-1);
yvec = single(sin(2*pi*xvec));
slope = ([yvec(2:end),0] - yvec) / (xvec(2)-xvec(1));   
offset = yvec - slope .* xvec;                          
slope(end)  = 0;                        % Set output to zero if we fall in the last bin
offset(end) = 0;                        % Set output to zero if we fall in the last bin
lut_i(1,:) = slope;
lut_i(2,:) = offset;
lut_i = reshape(lut_i,1,[]);
lut_i = repmat(transpose(lut_i),[1,Niter]);

% Model:
tmp = single(rand(Nsamp,Niter));
domain_i = sort(tmp,1);
interp_o = single(zeros(Nsamp,Niter));
for ii = 1 : Niter
  interp_o(:,ii) = interp1(xvec,single(sin(2*pi*xvec)),domain_i(:,ii),'linear',0);
end

% ------------------------------------------------------------
% Create stimulus for AI Engine
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

fid = fopen('data/lut_i.txt','w');
data_i = reshape(lut_i,1,[]);
for ii = 1 : 2 : numel(data_i)
  fprintf(fid,'%.9e %.9e\n',data_i(ii),data_i(ii+1)); % Real
end
fclose(fid);

fid = fopen('data/domain_i.txt','w');
data_i = reshape(domain_i,1,[]);
for ii = 1 : 2 : numel(data_i)
  fprintf(fid,'%.9e %.9e\n',data_i(ii),data_i(ii+1)); % Real
end
fclose(fid);

fid = fopen('data/interp_o.txt','w');
data_o = reshape(interp_o,1,[]);
for ii = 1 : 2 : numel(data_o)
  fprintf(fid,'%.9e %.9e\n',data_o(ii),data_o(ii+1)); % Real
end
fclose(fid);

% ------------------------------------------------------------
% Vitis Functional Simulation
% ------------------------------------------------------------

lut_i = reshape(lut_i,1,[]);
domain_i = reshape(domain_i,1,[]);
interp_o = reshape(interp_o,1,[]);

vfs_model = vfs.aieGraph(input_file="interp1_app.cpp",...
                         include_paths = { "." },...
                         part="xcvc1902-vsva2197-2MP-e-S");

vfs_lut_i = varray.float(lut_i);
vfs_domain_i = varray.float(domain_i);
vfs_o = vfs_model.run(vfs_lut_i,vfs_domain_i);
vfs_o = single(reshape(vfs_o,1,numel(vfs_o)));
err_o = single(interp_o - vfs_o);

figure;
subplot(3,1,1); plot(domain_i,'b.-'); title('interp() LUT Input');
subplot(3,1,2); plot(interp_o,'b.-'); hold on; plot(vfs_o,'r.'); hold off; title('interp() Output');
subplot(3,1,3); plot(err_o,'k.-'); ylabel('error'); title('interp() Error'); 
yline(5e-6,'r'); yline(-5e-6,'r');

if (max(abs(err_o)) < 5e-6)
  fprintf(1,'--- PASSED ---\n');
else
  fprintf(1,'*** FAILED ***\n');
end
