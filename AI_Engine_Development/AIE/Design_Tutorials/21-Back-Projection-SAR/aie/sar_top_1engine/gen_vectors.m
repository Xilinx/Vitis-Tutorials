%
% Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;
addpath('../../matlab');

% ------------------------------------------------------------
% Load I/O Vectors
% ------------------------------------------------------------

Nifft = 2048;
Nsize = 512;
Npulse = 586; % Must use even number

range_i    = single(load('data/xyz_range_data.txt'));
coord_i    = single(load('data/antenna_coord_data.txt'));
R0_range_i = single(load('data/R0_range_data.txt'));
tmp        = single(load('data/pulse_data.txt'));
ifft_i     = reshape(complex(tmp(:,1),tmp(:,2)),Nifft,[]);

% Initial image:
image_i = single(complex(zeros(1,Nsize*Nsize)));

% ------------------------------------------------------------
% Matlab Golden Simulation
% ------------------------------------------------------------

[imG_raw] = sar_hw_model(Npulse,range_i,ifft_i,coord_i,R0_range_i);

% ------------------------------------------------------------
% Vitis Functional Simulation
% ------------------------------------------------------------

vfs_model = vfs.aieGraph(input_file="sar_top_1engine_app.cpp",...
                         include_paths = { sprintf('%s/L2/include/aie',getenv('DSPLIB_ROOT')),...
                                           sprintf('%s/L1/include/aie',getenv('DSPLIB_ROOT')),...
                                           sprintf('%s/L1/src/aie',getenv('DSPLIB_ROOT')),...
                                           '.',...
                                           '../sar_engine',...
                                           '../ifft2k_async',...
                                           '../range_gen',...
                                           '../diff3dsq',...
                                           '../sqrt_lib',...
                                           '../dR_comp',...
                                           '../fmod_floor',...
                                           '../expjx',...
                                           '../cos_lib',...
                                           '../sin_lib',...
                                           '../bp_update',...
                                           '../interp1'},...
                         part="xcvc1902-vsva2197-2MP-e-S");

vfs_image_i = varray.cfloat(image_i);
vfs_coord_i = varray.float(coord_i);
vfs_R0_range_i = varray.float(R0_range_i);
tic
  disp(sprintf('\nRunning Synthetic Aperture Radar over %g radar pulses...\n',Npulse));
for pp = 1 : Npulse
  vfs_ifft_i = varray.cfloat(ifft_i(:,pp)); % Next radar pulse
  imA_raw = vfs_model.run(vfs_ifft_i,vfs_image_i,vfs_coord_i,vfs_R0_range_i);
  vfs_image_i = imA_raw;
  disp(sprintf('Done %g of %g radar pulses',pp,Npulse));
end
imA_raw = single(transpose(imA_raw));
toc

% Plot resulting image:
imG = abs(single(reshape(imG_raw,1,[])));
imA = abs(single(reshape(imA_raw,1,[])));
scaleG = 1.0/max(imG); 
scaleA = 1.0/max(imA); 
imG = imG * scaleG;
imA = imA * scaleA;
imE = abs(single(reshape(imG_raw-imA_raw,1,[])));

[ssim_val,imS] = ssim(imA,imG);
psnr_val = 20*log10(1.0/rms(imE));

imG = reshape(imG,Nsize,[]);
imA = reshape(imA,Nsize,[]);
imE = reshape(imE,Nsize,[]);
imS = reshape(imS,Nsize,[]);

h = figure('Position',[0,0,800,600]);
subplot(2,2,1); plot_sar(20*log10(imG),[-70,0],'MATLAB');
subplot(2,2,2); plot_sar(20*log10(imA),[-70,0],'AI Engine');
subplot(2,2,3); plot_sar(20*log10(imE),[-120,-60],sprintf('Pixel Error (PSNR: %4.1f dB)',psnr_val));
subplot(2,2,4); plot_sar(20*log10(imS),[-10,0],sprintf('SSIM Error (SSIM: %5.4f)',ssim_val));

% Save files:
saveas(h,sprintf('sar_top_1_engine_app_%g_pulses.png',Npulse),'png');
fid = fopen(sprintf('data/mat_imG_raw_%g_pulses.txt',Npulse),'w');
fprintf(fid,'%.9e %.9e\n',reshape([real(imG_raw);imag(imG_raw)],1,[]));
fclose(fid);
fid = fopen(sprintf('data/vfs_imA_raw_%g_pulses.txt',Npulse),'w');
fprintf(fid,'%.9e %.9e\n',reshape([real(imA_raw);imag(imA_raw)],1,[]));
fclose(fid);

