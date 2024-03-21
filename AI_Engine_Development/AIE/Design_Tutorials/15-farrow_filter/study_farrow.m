%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Faisal El-Shabani

% References
% A continuously variable digital delay element https://ieeexplore.ieee.org/document/15483
clear all;close all;
rng(1);
%% Transfer function estimate
% Create signal and filter using baseline fractional delay filters
Fs=1e9;
Nsamp=4096;
backoff_dB = -15;
sig_i = complex(randn(1,Nsamp),randn(1,Nsamp));
sig_i = sig_i/std(sig_i)*10^(backoff_dB/20);

fract_delay = [-0.4,-0.2,0.2,0.4];      % Pick a value between -0.5 and 0.5

% Filter signal using Farrow structure
f3_taps = [-0.012573,0.077148,-0.403198,0.905457,-0.905457,0.403198,-0.077148,0.012573];
f2_taps = [0.055298,-0.216248,0.631836,-0.465576,-0.465576,0.631836,-0.216248,0.055298];
f1_taps = [0.003143,-0.019287,0.1008,-1.226364,1.226364,-0.1008,0.019287,-0.003143];
f0_taps = [-0.013824,0.054062,-0.157959,0.616394,0.616394,-0.157959,0.054062,-0.013824];
del_i_model = -fract_delay.'*ones(1,length(sig_i));
y3 = filter(f3_taps,1,sig_i);
y2 = filter(f2_taps,1,sig_i);
y1 = filter(f1_taps,1,sig_i);
y0 = filter(f0_taps,1,sig_i);
z = ((y3.*del_i_model + y2).*del_i_model + y1).*del_i_model + y0;
sig_o = z;
sig_o = circshift(sig_o,-2); % Remove Group delay of filter

[Txy1,W] = tfestimate(sig_i,sig_o(1,:),1024,[],[],Fs,'centered');
[Txy2,~] = tfestimate(sig_i,sig_o(2,:),1024,[],[],Fs,'centered');
[Txy3,~] = tfestimate(sig_i,sig_o(3,:),1024,[],[],Fs,'centered');
[Txy4,~] = tfestimate(sig_i,sig_o(4,:),1024,[],[],Fs,'centered');
figure(1);
plot(W/1e6,20*log10(abs(Txy1)),W/1e6,20*log10(abs(Txy2)),W/1e6,20*log10(abs(Txy3)),W/1e6,20*log10(abs(Txy4)));xlabel('Frequency (MHz)');ylabel('Magnitude (dB)');
title('Farrow Transfer Function of different delay values');grid on;legend('-0.4','-0.2','+0.2','+0.4');

%% Time domain waveforms
Fs_MHz = 1e9/1e6;
Fc_MHz = 50e6/1e6;
t = linspace(0,1,Fs_MHz);
sig_i = cos(2*pi*Fc_MHz * t);

fract_delay = [-0.4,0,0.4];      % Pick a value between -0.5 and 0.5
del_i = -fract_delay.'*ones(1,length(sig_i));

% Filter signal using Farrow structure
f3_taps = [-0.012573,0.077148,-0.403198,0.905457,-0.905457,0.403198,-0.077148,0.012573];
f2_taps = [0.055298,-0.216248,0.631836,-0.465576,-0.465576,0.631836,-0.216248,0.055298];
f1_taps = [0.003143,-0.019287,0.1008,-1.226364,1.226364,-0.1008,0.019287,-0.003143];
f0_taps = [-0.013824,0.054062,-0.157959,0.616394,0.616394,-0.157959,0.054062,-0.013824];
del_i_model = -del_i;
y3 = filter(f3_taps,1,sig_i);
y2 = filter(f2_taps,1,sig_i);
y1 = filter(f1_taps,1,sig_i);
y0 = filter(f0_taps,1,sig_i);
z = ((y3.*del_i_model + y2).*del_i_model + y1).*del_i_model + y0;
sig_o = z;

figure(2);plot(sig_i(10:50),'.-');hold on;plot(sig_o(1,10:50),'.-');plot(sig_o(2,10:50),'.-');plot(sig_o(3,10:50),'.-');hold off;
grid on;xlabel('Samples');ylabel('Amplitude');title('Time domain response of Farrow Filter');legend('Input signal','-0.4 delay','0 delay','+0.4 delay');
