%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Faisal El-Shabani

% References
% A continuously variable digital delay element https://ieeexplore.ieee.org/document/15483
clear all;close all;
rng(1);

%% Create signal
Nsamp=1024*4;
TT = numerictype(1,16,14);
FF = fimath('RoundingMethod','Round','OverflowAction','Saturate');
backoff_dB = -15;
sig_i = complex(randn(1,Nsamp),randn(1,Nsamp));
sig_i = sig_i/std(sig_i)*10^(backoff_dB/20);
sig_i = lowpass(sig_i,0.2);
sig_i = fi(sig_i,TT,FF);

% fract_delay = 0.3;      % Pick a value between -0.5 and 0.5
% del_i = fi(-fract_delay*ones(1,Nsamp),TT,FF);
del_i = fi(-(rand(1,Nsamp)-0.5),TT,FF);  % uniform distribution -0.5 and 0.5

%% Farrow structure - Matlab model
f3_taps = fi([-0.012573,0.077148,-0.403198,0.905457,-0.905457,0.403198,-0.077148,0.012573],TT,FF);
f2_taps = fi([0.055298,-0.216248,0.631836,-0.465576,-0.465576,0.631836,-0.216248,0.055298],TT,FF);
f1_taps = fi([0.003143,-0.019287,0.1008,-1.226364,1.226364,-0.1008,0.019287,-0.003143],TT,FF);
f0_taps = fi([-0.013824,0.054062,-0.157959,0.616394,0.616394,-0.157959,0.054062,-0.013824],TT,FF);

y3 = double(fi(filter(double(f3_taps),1,double(sig_i)),TT,FF));
y2 = double(fi(filter(double(f2_taps),1,double(sig_i)),TT,FF));
y1 = double(fi(filter(double(f1_taps),1,double(sig_i)),TT,FF));
y0 = double(fi(filter(double(f0_taps),1,double(sig_i)),TT,FF));
z =  double(fi((double(fi((y3.*double(del_i) + y2),TT,FF)).*double(del_i) + y1),TT,FF)).*double(del_i) + y0;
sig_o = fi(z,TT,FF);

%% Print taps for C model
flip(f3_taps.int) %%(end-3:end))
flip(f2_taps.int) %%(end-3:end))
flip(f1_taps.int) %%(end-3:end))
flip(f0_taps.int) %%(end-3:end))

%% Save I/O files for simulation

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

% Input samples
fid0_i = fopen('data/sig_i.txt','w');
fid1_i = fopen('data/del_i.txt','w');
for ii = 1 : 2 : numel(sig_i)
  fprintf(fid0_i,'%d %d %d %d\n',real(sig_i.int(ii)),imag(sig_i.int(ii)),real(sig_i.int(ii+1)),imag(sig_i.int(ii+1)));
  fprintf(fid1_i,'%d %d\n',double(del_i.int(ii+0)),double(del_i.int(ii+1)));
end
fclose(fid0_i);
fclose(fid1_i);

fid2_i = fopen('data/del_i_optimized.txt','w');
for ii = 1 : 8 : numel(del_i)
  fprintf(fid2_i,'%d %d\n',double(del_i.int(ii+0))+2^16 * double(del_i.int(ii+1)),double(del_i.int(ii+2))+2^16 * double(del_i.int(ii+3)));
  fprintf(fid2_i,'%d %d\n',double(del_i.int(ii+4))+2^16 * double(del_i.int(ii+5)),double(del_i.int(ii+6))+2^16 * double(del_i.int(ii+7)));
  fprintf(fid2_i,'%d %d\n',0,0);
  fprintf(fid2_i,'%d %d\n',0,0);
end
fclose(fid2_i);


% Output samples
fid_o = fopen('data/sig_o.txt','w');
for ii = 1 : 2 : numel(sig_o)
  fprintf(fid_o,'%d %d %d %d\n',...
          real(sig_o.int(ii)),imag(sig_o.int(ii)),real(sig_o.int(ii+1)),imag(sig_o.int(ii+1)));
end
fclose(fid_o);
