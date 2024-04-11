%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;
addpath('../../matlab');
% rng(1);

% ------------------------------------------------------------
% Generate Model
% ------------------------------------------------------------

TT = numerictype(1,16,15);
FF = fimath('RoundingMethod','Convergent','OverflowAction','Saturate');

% Exercise full range of phase (ie. 20 bits):
tmp = linspace(0,2,2^20+1);
phase = tmp(1:end-1);
twid_m = fi(exp(1i*pi*phase),TT,FF);
twid_a  = load_aiesim('x86simulator_output/data/twid_o.txt', 'int',1);
err = twid_a - double(twid_m.int);

subplot(4,1,1); plot(real(twid_a),'b.-'); hold on; plot(real(twid_m).int,'r.--'); hold off; ylabel('Real');
subplot(4,1,2); plot(imag(twid_a),'b.-'); hold on; plot(imag(twid_m).int,'r.--'); hold off; ylabel('Imag');
subplot(4,1,3); plot(real(err),'k.-'); ylabel('Err Real');
subplot(4,1,4); plot(imag(err),'m.-'); ylabel('Err Imag');

sincos_lut = twid_a;
save('sincos_lut.mat','sincos_lut','-mat');


