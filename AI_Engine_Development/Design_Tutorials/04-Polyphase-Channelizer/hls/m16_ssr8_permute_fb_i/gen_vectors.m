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

SSR_I = 7;
SSR_O = 8;
Nsamp = 128;

% Generate inputs:
backoff_dB = -18;
sig_i = sqrt(0.5)*10^(0.05*backoff_dB)*complex(randn(app.oversamp_ratio,Nsamp),randn(app.oversamp_ratio,Nsamp));

% Quantize:
sig_i = double(fi(sig_i,1,16,15));

% Generate outputs:
state = zeros(app.Mchan,1);
perm = 0;
sig_o = zeros(app.Mchan,Nsamp);
for ss = 1 : Nsamp
  % Load next inputs into state:
  state(1:app.oversamp_ratio,1) = sig_i(:,ss);
  sig_o(1+all_idx(:,1+perm),ss) = state(:,1);
  perm = mod(perm+1,app.Ncyclic);
  state = circshift(state,app.oversamp_ratio);
end

% Introduce eight blocks of latency to match C++ model:

sig_o = cat(2,zeros(app.Mchan,8),sig_o(:,1:end-8));

% Format into streams for Polyphase FIR AIE kernel:
all_o = zeros(SSR_O,2*Nsamp);
all_o(1,:) = reshape(cat(1,reshape(sig_o( 1,:),4,[]),reshape(sig_o( 2,:),4,[])),1,[]);
all_o(2,:) = reshape(cat(1,reshape(sig_o( 3,:),4,[]),reshape(sig_o( 4,:),4,[])),1,[]);
all_o(3,:) = reshape(cat(1,reshape(sig_o( 5,:),4,[]),reshape(sig_o( 6,:),4,[])),1,[]);
all_o(4,:) = reshape(cat(1,reshape(sig_o( 7,:),4,[]),reshape(sig_o( 8,:),4,[])),1,[]);
all_o(5,:) = reshape(cat(1,reshape(sig_o( 9,:),4,[]),reshape(sig_o(10,:),4,[])),1,[]);
all_o(6,:) = reshape(cat(1,reshape(sig_o(11,:),4,[]),reshape(sig_o(12,:),4,[])),1,[]);
all_o(7,:) = reshape(cat(1,reshape(sig_o(13,:),4,[]),reshape(sig_o(14,:),4,[])),1,[]);
all_o(8,:) = reshape(cat(1,reshape(sig_o(15,:),4,[]),reshape(sig_o(16,:),4,[])),1,[]);

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
for ss = 1 : 4 : size(all_o,2)
  for xx = 1 : SSR_O
    fprintf(fid,'%.9f %.9f %.9f %.9f %.9f %.9f %.9f %.9f\n',...
            real(all_o(xx,ss+0)),imag(all_o(xx,ss+0)),real(all_o(xx,ss+1)),imag(all_o(xx,ss+1)),...
            real(all_o(xx,ss+2)),imag(all_o(xx,ss+2)),real(all_o(xx,ss+3)),imag(all_o(xx,ss+3)));
  end
end
fclose(fid);




