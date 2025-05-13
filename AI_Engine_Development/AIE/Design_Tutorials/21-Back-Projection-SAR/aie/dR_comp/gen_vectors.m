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
R0_range = single(load('../sar_top_1engine/data/R0_range_data.txt'));
sqrt_expand = single(sqrt(97.5e6));
scale_theta = 2*single(9.288080384e9)/single(299792458);
scale_domain = 2*single(1.471301625e+06)/single(299792458);
sig_i = 1.0 + 1e-3*(24+12*single(rand(Nsamp,Niter)));  % Generate in range (1.024,1.036) to match sqrt() output
theta_o = zeros(size(sig_i));
domain_o = zeros(size(sig_i));
for ii = 1 : Niter
  sqrt_o = sqrt_expand * sig_i(:,ii) - R0_range(ii);
  theta_o(:,ii) = scale_theta * sqrt_o;
  domain_o(:,ii) = 0.5 + scale_domain * sqrt_o;
end

% ------------------------------------------------------------
% Create stimulus for AI Engine
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

fid = fopen('data/sig_i.txt','w');
data_i = reshape(sig_i,1,[]);
for ii = 1 : 2 : numel(data_i)
  fprintf(fid,'%.9e %.9e\n',data_i(ii),data_i(ii+1));
end
fclose(fid);

fid = fopen('data/domain_o.txt','w');
data_o = reshape(domain_o,1,[]);
for ii = 1 : 2 : numel(data_o)
  fprintf(fid,'%.9e %.9e\n',data_o(ii),data_o(ii+1));
end
fclose(fid);

fid = fopen('data/theta_o.txt','w');
data_o = reshape(theta_o,1,[]);
for ii = 1 : 2 : numel(data_o)
  fprintf(fid,'%.9e %.9e\n',data_o(ii),data_o(ii+1));
end
fclose(fid);

% Create LUT for pulses:
fid = fopen('R0_range.h','w');
fprintf(fid,'//\n');
fprintf(fid,'// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.\n');
fprintf(fid,'// SPDX-License-Identifier: MIT\n');
fprintf(fid,'//\n');
fprintf(fid,'// Author: Mark Rollins\n\n');
fprintf(fid,'const float R0_range[dR_comp<256,1024>::NPULSE] = {\n');
fprintf(fid,'%f, ',R0_range(1:end-1));
fprintf(fid,'%f };\n',R0_range(end));
fclose(fid);


% ------------------------------------------------------------
% Vitis Functional Simulation
% ------------------------------------------------------------

vfs_model = vfs.aieGraph(input_file="dR_comp_app.cpp",include_paths = {"."},...
                         part="xcvc1902-vsva2197-2MP-e-S");

v_sig_i  = varray.float(reshape(sig_i,1,[]));
[v_domain_o,v_theta_o] = vfs_model.run(v_sig_i,varray.float(R0_range));
v_domain_o = single(reshape(v_domain_o,1,numel(v_domain_o)));
v_theta_o = single(reshape(v_theta_o,1,numel(v_theta_o)));
err_domain_o = single(reshape(domain_o,1,[])-v_domain_o);
err_theta_o = single(reshape(theta_o,1,[])-v_theta_o);

figure;
sig_i    = reshape(sig_i,1,[]);
theta_o  = reshape(theta_o,1,[]);
domain_o = reshape(domain_o,1,[]);
subplot(3,2,[1,2]); plot(sig_i,'b.-'); title('dRcomp() Workload');
subplot(3,2,3);     plot(theta_o,'b.-'); hold on; plot(v_theta_o,'r.--'); hold off; title('theta');
subplot(3,2,4);     plot(domain_o,'b.-'); hold on; plot(v_domain_o,'r.--'); hold off; title('domain');
subplot(3,2,5);     plot(err_theta_o,'k.-'); title('Error theta');
subplot(3,2,6);     plot(err_domain_o,'k.-');title('Error domain');

max_err = max([abs(err_theta_o),abs(err_domain_o)]);
if (max_err < 1e-6)
  fprintf(1,'--- PASSED ---\n');
else
  fprintf(1,'*** FAILED ***\n');
end
