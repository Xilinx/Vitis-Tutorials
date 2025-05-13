%
% Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;

% ------------------------------------------------------------
% Load GOTCHA data
% ------------------------------------------------------------
pass = 1;
pol = 1;
azm = 40+[-2:1:2]; % Integrate over deltaAz = 4 deg centered at 40 deg
Nsize = 512;

% ------------------------------------------------------------
% Dump Vectors
% ------------------------------------------------------------

dump_vectors_a72_host(pass,pol,azm,Nsize);


% ------------------------------------------------------------
% Dump Vectors for A72 Host
% ------------------------------------------------------------

function dump_vectors_a72_host(pass,pol,azm,Nsize)
   data_i = [];
   fid_p = fopen('../aie/sar_top_1engine/data/pulse_data.txt','w');
   fid_r = fopen('../aie/sar_top_1engine/data/xyz_range_data.txt','w');
   fid_0 = fopen('../aie/sar_top_1engine/data/R0_range_data.txt','w');
   fid_A = fopen('../aie/sar_top_1engine/data/antenna_coord_data.txt','w');
   Npulse = 0;
   data_i.AntX = [];
   data_i.AntY = [];
   data_i.AntZ = [];
   for ii = 1 : numel(azm)
     % Load data from GOTCHA database:
     gotcha = load_gotcha_dataset('..',pass,pol,azm(ii));
     Nsamp = size(gotcha.fp,1);
     Np    = size(gotcha.fp,2);
     data_i.Nfft   = 2048; % Implementation uses half size of IFFT compared to Matlab
     data_i.deltaF = mean(diff(gotcha.freq));
     data_i.minF   = min(gotcha.freq)*ones(1,Np);
     data_i.AntX   = cat(2,data_i.AntX,gotcha.x);
     data_i.AntY   = cat(2,data_i.AntY,gotcha.y);
     data_i.AntZ   = cat(2,data_i.AntZ,gotcha.z);
     data_i.R0     = gotcha.r0;
     data_i.phdata = gotcha.fp;
     tmpX = linspace(-50, 50,1+Nsize);  tmpX = tmpX(1:end-1);
     tmpY = linspace(+50,-50,1+Nsize);  tmpY = tmpY(1:end-1);
     data_i.x_mat  = reshape(repmat(tmpX,[Nsize,1]) ,1,[]);
     data_i.y_mat  = reshape(repmat(tmpY,[Nsize,1])',1,[]);
     data_i.z_mat  = zeros(size(data_i.x_mat)); % Paper states they do this too
     % Dump Radar Pulse data:
     for pp = 1 : Np
       % Zero-pad to 4K samples:
       tmp = complex(zeros(1,data_i.Nfft));
       tmp(1:Nsamp) = data_i.phdata(:,pp);
       fprintf(fid_p,'%0.9e %0.9e\n',reshape([real(tmp);imag(tmp)],1,[]));
     end
     fprintf(fid_0,'%0.9e\n',data_i.R0);
     Npulse = Npulse + Np;
   end
   fprintf(fid_A,'%0.9e \n',cat(2,data_i.AntX,data_i.AntY,data_i.AntZ));
   fclose(fid_A);
   fclose(fid_p);
   fclose(fid_0);
   disp(sprintf('Wrote ''antenna_coord_data.txt'' with %g radar pulses',Npulse));
   disp(sprintf('Wrote ''pulse_data.txt'' with %g radar pulses',Npulse));
   disp(sprintf('Wrote ''R0_range_data.txt'' with %g radar pulses',Npulse));

   % Send range data 8 consecutive samples from each dimension at a time:
   fprintf(fid_r,'%0.9e\n',reshape(cat(1,...
                                       reshape(data_i.x_mat,8,[]),...
                                       reshape(data_i.y_mat,8,[]),...
                                       reshape(data_i.z_mat,8,[])),1,[]));
   disp('Wrote ''xyz_range_data.txt'' (x,y,z) coordinates -- common for all pulses');
end

