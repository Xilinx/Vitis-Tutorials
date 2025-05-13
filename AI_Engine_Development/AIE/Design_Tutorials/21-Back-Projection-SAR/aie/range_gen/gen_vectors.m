%
% Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;

% ------------------------------------------------------------
% Model
% ------------------------------------------------------------

NPIXEL = 512;
REPEAT = 32;
NSAMP = 1024;
NENGINE = 8;
ID = 4;

assert(NENGINE*REPEAT*NSAMP==NPIXEL*NPIXEL);
assert(ID < NENGINE);

tmpX   = linspace(-50, 50,1+NPIXEL);  tmpX = tmpX(1:end-1);
tmpY   = linspace(+50,-50,1+NPIXEL);  tmpY = tmpY(1:end-1);
xx     = repmat(tmpX,[NPIXEL,1]);
yy     = repmat(tmpY,[NPIXEL,1])';
zz     = zeros(size(xx));
offset = NPIXEL/NENGINE;
range  = [ID*offset:(ID+1)*offset-1];

% Select coordinates for this engine:
xx_u = reshape(xx(:,1+range),1,[]);
yy_u = reshape(yy(:,1+range),1,[]);
zz_u = reshape(zz(:,1+range),1,[]);

% ------------------------------------------------------------
% Create stimulus for AI Engine
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

% Interleave (x,y,z) inputs eight at a time:
fid = fopen('data/sig_o.txt','w');
data_o = reshape(cat(1,reshape(xx_u,8,[]),reshape(yy_u,8,[]),reshape(zz_u,8,[])),1,[]);
for ii = 1 : 2 : numel(data_o)
  fprintf(fid,'%.9e %.9e\n',data_o(ii),data_o(ii+1));
end
fclose(fid);

% ------------------------------------------------------------
% Vitis Functional Simulation
% ------------------------------------------------------------

vfs_model = vfs.aieGraph(input_file="range_gen_app.cpp",include_paths = {"."},...
                         part="xcvc1902-vsva2197-2MP-e-S");

vfs_o = vfs_model.run();
vfs_o = single(reshape(vfs_o,1,numel(vfs_o)));
err_o = data_o - vfs_o;

figure;
subplot(2,1,1); plot(data_o,'b.-'); hold on; plot(vfs_o,'r.--'); hold off; title('RangeGen Workload');
legend({'Matlab','AIE'});
subplot(2,1,2); plot(data_o-vfs_o,'k.-'); title('Error');

if (max(abs(err_o)) == 0)
  fprintf(1,'--- PASSED ---\n');
else
  fprintf(1,'*** FAILED ***\n');
end
