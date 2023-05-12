%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

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

SSR = 8;
Nsamp = 128;

% Generate inputs:
backoff_dB = -18;
sig_i = sqrt(0.5)*10^(0.05*backoff_dB)*complex(randn(app.Mchan,Nsamp),randn(app.Mchan,Nsamp));

% Quantize:
sig_i = double(fi(sig_i,1,16,15));

% Format inputs as they come from AIE FILTERBANK kernel:
mix_i = zeros(SSR,2*Nsamp);
mix_i(1,:) = reshape(cat(1,reshape(sig_i( 1,:),4,[]),reshape(sig_i( 2,:),4,[])),1,[]);
mix_i(2,:) = reshape(cat(1,reshape(sig_i( 3,:),4,[]),reshape(sig_i( 4,:),4,[])),1,[]);
mix_i(3,:) = reshape(cat(1,reshape(sig_i( 5,:),4,[]),reshape(sig_i( 6,:),4,[])),1,[]);
mix_i(4,:) = reshape(cat(1,reshape(sig_i( 7,:),4,[]),reshape(sig_i( 8,:),4,[])),1,[]);
mix_i(5,:) = reshape(cat(1,reshape(sig_i( 9,:),4,[]),reshape(sig_i(10,:),4,[])),1,[]);
mix_i(6,:) = reshape(cat(1,reshape(sig_i(11,:),4,[]),reshape(sig_i(12,:),4,[])),1,[]);
mix_i(7,:) = reshape(cat(1,reshape(sig_i(13,:),4,[]),reshape(sig_i(14,:),4,[])),1,[]);
mix_i(8,:) = reshape(cat(1,reshape(sig_i(15,:),4,[]),reshape(sig_i(16,:),4,[])),1,[]);

% Introduce 8 blocks of latency to match AIE filterbank kernel:
sig_i = cat(2,zeros(app.Mchan,8),sig_i(:,1:end-8));

% Generate outputs:
perm = 0;
sig_o = zeros(app.Mchan,Nsamp);
for ss = 1 : Nsamp
  % Load next inputs into state:
  sig_o(:,ss) = sig_i(1+all_idx(:,1+perm),ss);
  perm = mod(perm+1,app.Ncyclic);
end


% ------------------------------------------------------------
% Write I/O Files
% ------------------------------------------------------------

fid = fopen('sig_i.txt','w');
for ss = 1 : 4 : size(mix_i,2)
  for xx = 1 : SSR
    fprintf(fid,'%.9f %.9f %.9f %.9f %.9f %.9f %.9f %.9f\n',...
            real(mix_i(xx,ss+0)),imag(mix_i(xx,ss+0)),real(mix_i(xx,ss+1)),imag(mix_i(xx,ss+1)),...
            real(mix_i(xx,ss+2)),imag(mix_i(xx,ss+2)),real(mix_i(xx,ss+3)),imag(mix_i(xx,ss+3)));
  end
end
fclose(fid);

fid = fopen('sig_o.txt','w');
for ss = 1 : 2 : Nsamp
  for xx = 1 : SSR
    fprintf(fid,'%.9f %.9f %.9f %.9f %.9f %.9f %.9f %.9f\n',...
            real(sig_o(xx,ss+0)),imag(sig_o(xx,ss+0)),real(sig_o(xx+SSR,ss+0)),imag(sig_o(xx+SSR,ss+0)),...
            real(sig_o(xx,ss+1)),imag(sig_o(xx,ss+1)),real(sig_o(xx+SSR,ss+1)),imag(sig_o(xx+SSR,ss+1)));
  end
end
fclose(fid);




