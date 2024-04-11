
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

Nports = 1;
Nfft = 256;
Nfft_all = Nfft*Nfft;
Nextra = 4;
Ntransform = 60;
backoff_dB = -15;                       % To achieve (-1,+1) range on AWGN signal
NSTREAM = 5;
PHASE = 4;
ALIGN = 16;

% Fixed-point attributes matching hardware:
TT = numerictype(1,32,31);
FF = fimath('RoundingMethod','Convergent','OverflowAction','Saturate');
WW = numerictype(1,16,15);
XX = fimath('RoundingMethod','Convergent','OverflowAction','Saturate');

% Input signal:
type = 'tone';
switch (type)
  case 'noise',
  % Random noise:
  tmp = sqrt(0.5)*10^(0.05*backoff_dB)*complex(randn(1,Nfft*Ntransform),randn(1,Nfft*Ntransform));
  scale = 1/8;
  sig_i = fi(reshape(scale*tmp,Nfft,Ntransform),TT,FF);
 case 'tone',
  % Tone signal:
  tmp = fft(0.6/Nfft*exp(1i*2*pi*8/256*[0:Nfft-1]));
  tmp = repmat(tmp.',[1,Ntransform]);
  sig_i = fi(tmp,TT,FF);
end

% Generate twiddles for extra rotation as done by the hardware:
load('../sincos_test/sincos_lut.mat');
twid = complex(zeros(Nfft,Ntransform));
rr = PHASE;
for tt = 1 : Ntransform
  phase_ramp = bitshift(uint32(rr*[0:7]),ALIGN,'uint32');;
  twid_step = sincos_lut(1+bitshift(phase_ramp,-12,'uint32'))/32768;
  phase_step = bitshift(uint32(rr*8),ALIGN,'uint32');
  phase = uint32(0);
  val = [];
  % Generate twiddles using hardware algorithm: 
  for ss = 1 : 8 : Nfft
    sincos = sincos_lut(1+bitshift(phase,-12,'uint32'))/32768;
    next_twid = double(fi(twid_step*sincos,WW,FF));
    val = [val,next_twid];
    phase = uint32(phase + phase_step);
  end
  rr = mod(rr + NSTREAM,Nfft+Nextra);
  twid(:,tt) = val;
end

% ------------------------------

% Output:
sig_o = zeros(size(sig_i));
for tt = 1 : Ntransform
  tmp = ifft_stockham_dit(sig_i(:,tt),WW,XX,TT,FF);
  
  sig_o(:,tt) = tmp .* twid(:,tt);
end
sig_o = fi(sig_o,TT,FF);

% Pad with extra samples for easing PL transpose design:
sig_i = [sig_i;zeros(Nextra,Ntransform)];
sig_o = [sig_o;zeros(Nextra,Ntransform)];

% ------------------------------------------------------------
% Save I/O files for simulation
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

% Input samples in polyphase order:
data = reshape(sig_i,Nports,[]);
for pp = 1 : Nports
  fid_i = fopen(sprintf('data/sig_i_%d.txt',pp-1),'w');
  % This is 32-bit data so one sample per line:
  for ii = 1 : numel(data(pp,:));
    fprintf(fid_i,'%d %d \n',real(data.int(pp,ii)),imag(data.int(pp,ii)));
  end
  fclose(fid_i);
end

% Output samples in polyphase order:
data = reshape(sig_o,Nports,[]);
for pp = 1 : Nports
  fid_o = fopen(sprintf('data/sig_o_%d.txt',pp-1),'w');
  % This is 32-bit data so one sample per line:
  for ii = 1 : numel(data(pp,:))
    fprintf(fid_o,'%d %d \n',real(data.int(pp,ii)),imag(data.int(pp,ii)));
  end
  fclose(fid_o);
end



