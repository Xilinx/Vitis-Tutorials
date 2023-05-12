%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Chandra Sekar Balakrishnan

clear all;
close all;

rng(1);

% ------------------------------------------------------------
% System Configuration
% ------------------------------------------------------------

app.fs_Gsps = 10.5;
app.Mchan = 16;
app.OSR = 3;
app.P = 8; 
app.Q = 7;
app.oversamp_ratio = app.Mchan*app.Q/app.P;
app.Ncyclic = lcm(app.Mchan,app.oversamp_ratio)/app.oversamp_ratio;
app.Nshift = app.Mchan / app.Ncyclic;
app.fc_Msps = 1000 * app.fs_Gsps / app.Mchan;

% ------------------------------------------------------------
% Create Permutation Mapping
% ------------------------------------------------------------

% This computes the I/O permutation needed to drive the streams:
all_idx = [];
for ii = 0 : 7
  all_idx = cat(2,all_idx,circshift([0:15]',app.oversamp_ratio*ii));
end

% ------------------------------------------------------------
% Create Testbench Stimulus
% ------------------------------------------------------------

SSR_I = 8;
SSR_O = 8;
Nsamp = 128;

% Generate inputs:
backoff_dB = -18;
sig_i = sqrt(0.5)*10^(0.05*backoff_dB)*complex(randn(app.Mchan,Nsamp),randn(app.Mchan,Nsamp));

% Quantize:
sig_i = double(fi(sig_i,1,16,15));

   % ------------------------------------------------------------
   % Cyclic Shift and RX IFFT
   % ------------------------------------------------------------

   state = 0;
   sig_o = zeros(app.Mchan,Nsamp);
   for bb = 1 : Nsamp
     % Compute shift amount:
     shift = state * app.Nshift;
     % Perform shift for next FFT input:
     sig_o(:,bb) = circshift(sig_i(:,bb),shift);
     % Update state:
     state = mod(state+1,app.Ncyclic);
   end

% ------------------------------------------------------------
% Write I/O Files
% ------------------------------------------------------------

fid = fopen('sig_i.txt','w');
for ss = 1 : 2 : Nsamp
  for xx = 1 : SSR_I
    	fprintf(fid,'%.9f %.9f %.9f %.9f %.9f %.9f %.9f %.9f\n',...
            real(sig_i(xx,ss+0)),imag(sig_i(xx,ss+0)),real(sig_i(xx+SSR_I,ss+0)),imag(sig_i(xx+SSR_I,ss+0)),...
            real(sig_i(xx,ss+1)),imag(sig_i(xx,ss+1)),real(sig_i(xx+SSR_I,ss+1)),imag(sig_i(xx+SSR_I,ss+1)));
  end
end
fclose(fid);

fid = fopen('sig_o.txt','w');
for ss = 1 : 2 : Nsamp
  for xx = 1 : SSR_O
    	fprintf(fid,'%.9f %.9f %.9f %.9f %.9f %.9f %.9f %.9f\n',...
	  real(sig_o(2*xx-1,ss+0)),imag(sig_o(2*xx-1,ss+0)),real(sig_o(2*xx,ss+0)),imag(sig_o(2*xx,ss+0)),...
	  real(sig_o(2*xx-1,ss+1)),imag(sig_o(2*xx-1,ss+1)),real(sig_o(2*xx,ss+1)),imag(sig_o(2*xx,ss+1)));
  end
end
fclose(fid);




