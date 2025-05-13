%
% Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;

% ------------------------------------------------------------
% Setup SAR Model & GOTCHA data set
% ------------------------------------------------------------

Npulse_use = 2;   % Needs 512 graph iterations to generate 1 SAR image for each radar pulse
Npulse_max = 128; % Needs to match the hardware design
Nsize = 512;

coord     = single(load('../../aie/sar_top_1engine/data/antenna_coord_data.txt'));
xyz_range = single(load('../../aie/sar_top_1engine/data/xyz_range_data.txt'));

% Model:
tmp = reshape(xyz_range,8,[]);          % These are (x,y,z) interleaved 8 samples for each
x_mat = reshape(tmp(:,1:3:end),1,[]);
y_mat = reshape(tmp(:,2:3:end),1,[]);
z_mat = reshape(tmp(:,3:3:end),1,[]);
sig_o = zeros(Nsize*Nsize,Npulse_use);
for pp = 1 : Npulse_use
  iX = pp;
  iY = pp+  Npulse_max;
  iZ = pp+2*Npulse_max;
  sig_o(:,pp) = ((coord(iX)-x_mat).^2 + (coord(iY)-y_mat).^2 + (coord(iZ)-z_mat).^2);  
end

% ------------------------------------------------------------
% Create stimulus for AI Engine
                                                                                                           % ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

% Interleave (x,y,z) inputs eight at a time (and repeat for all pulses):
fid = fopen('data/sig_i.txt','w');
data_i = reshape(cat(1,reshape(x_mat,8,[]),reshape(y_mat,8,[]),reshape(z_mat,8,[])),1,[]);
data_i = repmat(data_i,[1,Npulse_use]);
for ii = 1 : 2 : numel(data_i)
  fprintf(fid,'%.9e %.9e\n',data_i(ii),data_i(ii+1));
end
fclose(fid);

fid = fopen('data/sig_o.txt','w');
data_o = reshape(sig_o,1,[]);
for ii = 1 : 2 : numel(data_o)
  fprintf(fid,'%.9e %.9e\n',data_o(ii),data_o(ii+1));
end
fclose(fid);

% Create LUT for pulses:
fid = fopen('diff3dsq_pulses.h','w');
fprintf(fid,'//\n');
fprintf(fid,'// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.\n');
fprintf(fid,'// SPDX-License-Identifier: MIT\n');
fprintf(fid,'//\n');
fprintf(fid,'// Author: Mark Rollins\n\n');
fprintf(fid,'#define radar_coord ');
fprintf(fid,'%f, ',coord(             [1:Npulse_max]));
fprintf(fid,'%f, ',coord(  Npulse_max+[1:Npulse_max]));
fprintf(fid,'%f, ',coord(2*Npulse_max+[1:Npulse_max-1]));
fprintf(fid,'%f \n',coord(end));
fclose(fid);

% ------------------------------------------------------------
% Vitis Functional Simulation
% ------------------------------------------------------------

run(sprintf('%s/%s',getenv("XILINX_VITIS"),'functional_suite/addToMatlabPath.m'));
vfs_model = vfs.aieGraph(input_file="diff3dsq_app.cpp",include_paths = {"."},...
                         part="xcvc1902-vsva2197-2MP-e-S");

vfs_i = varray.float(data_i);
vfs_o = vfs_model.run(vfs_i);
vfs_o = single(reshape(vfs_o,1,numel(vfs_o)));
err_o = data_o - vfs_o;

figure;
subplot(2,1,1); plot(data_o,'b.-'); hold on; plot(vfs_o,'r.--'); hold off; title('Squared Distance Workload');
legend({'Matlab','AIE'});
subplot(2,1,2); plot(data_o-vfs_o,'k.-'); title('Error');

if (max(abs(err_o)) == 0)
  fprintf(1,'--- PASSED ---\n');
else
  fprintf(1,'*** FAILED ***\n');
end
