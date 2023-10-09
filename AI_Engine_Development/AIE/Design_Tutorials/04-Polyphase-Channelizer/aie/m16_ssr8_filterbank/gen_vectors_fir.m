%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;

rng(1);

TT = numerictype(1,16,15);
FF = fimath('RoundingMethod','Nearest','OverflowAction','Saturate');

% Scope: Generate I/O vectors for HW_EMU of FILTERBANK plus PERMUTE_I/PERMUTE_O HLS blocks

% ------------------------------------------------------------
% System Configuration
% ------------------------------------------------------------

SSR_I = 7;
SSR_O = 8;

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
% Create Polyphase Filter Taps
% ------------------------------------------------------------

% Design prototype filter to operate @ app.fc_Msps sampling rate:
app.Ntaps = 8;
app.bw_trans_oneside = 45;
fp = (1-app.bw_trans_oneside/100) * (0.5*app.fc_Msps);
fs = (1+app.bw_trans_oneside/100) * (0.5*app.fc_Msps);
tmp = firgr(app.Ntaps*app.Mchan-2,[0,fp,fs,500*app.fs_Gsps]/(500*app.fs_Gsps),[1,1,0,0]);
taps = [zeros(1,1),tmp];

% Quantize taps:
shift = 0;
scale = 2.0^shift;
taps = fi(scale*taps,TT,FF);
fprintf(1,'An shift of %g has been applied to channelizer taps\n',shift);
app.taps = reshape(taps,app.Mchan,[]);


% This computes the I/O permutation needed to drive the streams:
all_idx = [];
for ii = 0 : 7
  all_idx = cat(2,all_idx,circshift([0:15]',app.oversamp_ratio*ii));
end

% ------------------------------------------------------------
% Create input signal to Polyphase filter
% ------------------------------------------------------------

Nsamp = 7*128*app.Mchan;       % This gives 512 x 4 samples per stream
backoff_dB = -18;
sig_i = sqrt(0.5)*10^(0.05*backoff_dB)*complex(randn(1,Nsamp),randn(1,Nsamp));

% Quantize:
sig_i = fi(sig_i,TT,FF);

% ------------------------------------------------------------
% Golden Model
% ------------------------------------------------------------

taps_rx = double(reshape(taps,app.Mchan,[]));
Nblock = numel(sig_i) / app.oversamp_ratio;
data_i = reshape(sig_i,app.oversamp_ratio,[]);
data_i = data_i(end:-1:1,:);            % Reverse order so newer samples at lower indices
state = zeros(app.Mchan,app.Ntaps);             % Filter has Ntaps per phase
rx_filt_o = zeros(app.Mchan,Nblock);
for bb = 1 : Nblock
  % Load next inputs into state:
  state(1:app.oversamp_ratio,1) = double(data_i(:,bb));
  % Compute next filter output:
  rx_filt_o(:,bb) = sum(taps_rx .* state,2);
  % Update state using serpentine shift:
  tmp = circshift(reshape(state,1,[]),app.oversamp_ratio);
  tmp(1:app.oversamp_ratio) = 0;
  state = reshape(tmp,app.Mchan,[]);
end

% Introduce 16 block latency to match system:
rx_filt_o = cat(2,zeros(app.Mchan,16),rx_filt_o(:,1:end-16));

% Convert to fixed-point for output:
sig_o = fi(rx_filt_o,TT,FF);

% ------------------------------------------------------------
% Write Files
% ------------------------------------------------------------

[~,~,~] = mkdir('data');

% Need to send these '4 samples at a time' for each stream, interleaving streams after each set of 4:
fid = fopen('data/sig_i.txt','w');
for ss = 1 : 2 : size(data_i,2)
  for xx = 1 : SSR_I
    fprintf(fid,'%d %d\n',real(data_i(xx,      ss+0)).int,imag(data_i(xx,      ss+0)).int);
    fprintf(fid,'%d %d\n',real(data_i(xx+SSR_I,ss+0)).int,imag(data_i(xx+SSR_I,ss+0)).int);
    fprintf(fid,'%d %d\n',real(data_i(xx,      ss+1)).int,imag(data_i(xx,      ss+1)).int);
    fprintf(fid,'%d %d\n',real(data_i(xx+SSR_I,ss+1)).int,imag(data_i(xx+SSR_I,ss+1)).int);
  end
end
fclose(fid);

% Need to send these '4 samples at a time' for each stream, interleaving streams after each set of 4:
fid = fopen('data/sig_o.txt','w');
for ss = 1 : 2 : size(sig_o,2)
  for xx = 1 : SSR_O
    fprintf(fid,'%d %d\n',real(sig_o(xx,      ss+0)).int,imag(sig_o(xx,      ss+0)).int);
    fprintf(fid,'%d %d\n',real(sig_o(xx+SSR_O,ss+0)).int,imag(sig_o(xx+SSR_O,ss+0)).int);
    fprintf(fid,'%d %d\n',real(sig_o(xx,      ss+1)).int,imag(sig_o(xx,      ss+1)).int);
    fprintf(fid,'%d %d\n',real(sig_o(xx+SSR_O,ss+1)).int,imag(sig_o(xx+SSR_O,ss+1)).int);
  end
end
fclose(fid);




