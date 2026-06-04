%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;
rng(1);

% Create signal:
NSAMP = 64;
CC = 32;
NITER = 4;

TT = numerictype(1,16,15);
FF = fimath('RoundingMethod','Convergent','OverflowAction','Saturate');

% Assign tone frequencies linearly across 5% of the sampling rate:
FREQ = linspace(0.01,0.05,CC);

% Compute PHASE Increment for implementation:
% aie::sincos(arg) is normalized by 1/pi and 'arg' is provided in Q1.31 format.
% exp(+1i*2*pi*FREQ) --> 2*pi*FREQ --> 2*FREQ
tmp = 2*FREQ;
PHASE = fi(tmp,1,32,31,FF);
fid = fopen('../aie/tdm_mixer_phase_inc.h','w');
fprintf(fid,'//\n');
fprintf(fid,'// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.\n');
fprintf(fid,'// SPDX-License-Identifier: MIT\n');
fprintf(fid,'//\n');
fprintf(fid,'// Author: Mark Rollins\n');
fprintf(fid,'\n');
fprintf(fid,'std::vector<int> phase_inc = {');
fprintf(fid,'%d, ',PHASE.int(1:end-1));
fprintf(fid,' %d};\n',PHASE.int(end));
fclose(fid);

% Create I/O signals:
sig_i = ones(CC,NSAMP,NITER);
sig_o = zeros(size(sig_i));
for cc = 1 : CC
  data = exp(1i*2*pi*FREQ(cc)*[0:NSAMP*NITER-1]);
  sig_o(cc,:,:) = reshape(data,NSAMP,NITER);
end

% Place samples in TDM order:
sig_i = reshape(sig_i,1,[]);
sig_o = reshape(sig_o,1,[]);

% Quantize:
sig_i = fi(sig_i,TT,FF);
sig_o = fi(sig_o,TT,FF);

% ------------------------------------------------------------
% Vitis Functional Simulation
% ------------------------------------------------------------

vfs_model = vfs.aieGraph(input_file="../aie/tdm_mixer_app.cpp",...
                         include_paths = {"../aie"},...
                         part="xcvc1902-vsva2197-2MP-e-S");

vfs_i = varray.cint16(sig_i.int);
vfs_o = vfs_model.run(vfs_i);
vfs_o = reshape(vfs_o,1,numel(vfs_o));

figure;
range = [1:1000];
subplot(4,1,1); plot(real(sig_o).int(range),'b.-'); hold on; plot(real(vfs_o(range)),'r.--'); hold off; title('Real');
subplot(4,1,2); plot(imag(sig_o).int(range),'b.-'); hold on; plot(imag(vfs_o(range)),'r.--'); hold off; title('Imag');
subplot(4,1,3); plot(double(real(sig_o).int(range))-cast(real(vfs_o(range)),'double'),'k.-'); title('Error (Real)');                                                                                     
subplot(4,1,4); plot(double(imag(sig_o).int(range))-cast(imag(vfs_o(range)),'double'),'k.-'); title('Error (Imag)');    

% ------------------------------------------------------------
% Save I/O files for simulation
% ------------------------------------------------------------

[~,~,~] = rmdir('../aie/data','s');
[~,~,~] = mkdir('../aie/data');

% Input samples
fid_i = fopen('../aie/data/sig_i.txt','w');
for ii = 1 : 2 : numel(sig_i)
  fprintf(fid_i,'%d %d %d %d\n',...
          real(sig_i.int(ii)),imag(sig_i.int(ii)),real(sig_i.int(ii+1)),imag(sig_i.int(ii+1)));
end
fclose(fid_i);

% Output samples
fid_o = fopen('../aie/data/sig_o.txt','w');
for ii = 1 : 2 : numel(sig_o)
  fprintf(fid_o,'%d %d %d %d\n',...
          real(sig_o.int(ii)),imag(sig_o.int(ii)),real(sig_o.int(ii+1)),imag(sig_o.int(ii+1)));
end
fclose(fid_o);
