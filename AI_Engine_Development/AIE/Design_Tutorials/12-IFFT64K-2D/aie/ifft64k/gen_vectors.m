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

Ntransform = 1;
Nextra = 4;
Nports = 5;
Nfft = 256;
Nfft_all = Nfft*Nfft;
backoff_dB = -15;
ALIGN = 16;
NSTREAM = 5;

TT = numerictype(1,32,31);
WW = numerictype(1,16,15);
FF = fimath('RoundingMethod','Convergent','OverflowAction','Saturate');

% ------------------------------------------------------------
% Input signal:
% ------------------------------------------------------------

type = 'tone';
switch (type)
  case 'noise',
  % Random noise:
  tmp = sqrt(0.5)*10^(0.05*backoff_dB)*complex(randn(1,Nfft_all*Ntransform),randn(1,Nfft_all*Ntransform));
  scale = 1/8;
  sig_i = fi(reshape(scale*tmp,Nfft_all,Ntransform),TT,FF);
 case 'tone',
  % Tone signal:
  tmp = fft(0.5^8/(Nfft_all)*exp(1i*2*pi*4/Nfft_all*[0:Nfft_all-1])); % Some backoff
  tmp = repmat(tmp.',[1,Ntransform]);
  sig_i = fi(tmp,TT,FF);
end

% ------------------------------------------------------------
% Generate twiddles for extra rotation as done by the hardware:
% ------------------------------------------------------------

load('../sincos_test/sincos_lut.mat');
twid = complex(zeros(Nfft,Nfft,Ntransform));
for rr = 0 : Nfft-1
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
    twid(1+rr,:,tt) = val;
  end
end

% ------------------------------------------------------------
% Compute first set of transforms (front) along rows:
% ------------------------------------------------------------

data_r = complex(zeros(Nfft,Nfft,Ntransform));
for tt = 1 : Ntransform
  data = squeeze(reshape(sig_i(:,tt),Nfft,Nfft));
  for rr = 1 : Nfft
    tmp = ifft_stockham_dit(data(rr,:),WW,FF,TT,FF);
    data_r(rr,:,tt) = tmp .* twid(rr,:,tt);
  end
end
front_o = fi(data_r,TT,FF);

% ------------------------------------------------------------
% Compute second set of transforms (back) along columns:
% ------------------------------------------------------------

data_b = complex(zeros(Nfft,Nfft,Ntransform));
for tt = 1 : Ntransform
  for cc = 1 : Nfft
    tmp = squeeze(double(front_o(:,cc,tt)));
    data_b(:,cc,tt) = ifft_stockham_dit(tmp,WW,FF,TT,FF);
  end
end
back_o = fi(data_b,TT,FF);

% Compute ideal IFFT output (take data along columns):
sig_o = reshape(permute(back_o,[2,1,3]),1,[]);

disp('Done compute');

% ------------------------------------------------------------
% Save I/O files for simulation
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

% Generate input signal for Hardware Emulation:
% --> assummes col-major order on input:
fid = fopen('data/sig_i.txt','w');
for tt = 1 : Ntransform
  data = squeeze(sig_i(:,tt));
  for ss = 1 : numel(data)
    fprintf(fid,'%d %d\n',real(data(ss)).int,imag(data(ss)).int);
  end
end
fclose(fid);
disp('Done sig_i');

% Generate output signal for Hardware Emulation:
fid = fopen('data/sig_o.txt','w');
for ii = 1 : numel(sig_o)
  fprintf(fid,'%d %d\n',real(sig_o(ii)).int,imag(sig_o(ii)).int);
end
fclose(fid);
disp('Done sig_o');

% Generate input signals for AIE-only simulation:
% --> Inputs to 'front' transforms taken in polyphase order (with zero-padded rows/columns)
for tt = 1 : Ntransform
  data = reshape(sig_i(:,tt),Nfft,Nfft);
  data = [[data,zeros(Nfft,Nextra)];zeros(Nextra,Nextra+Nfft)];
  for pp = 1 : Nports
    fid = fopen(sprintf('data/front_i_%d.txt',pp-1),'a');
    data_u = reshape(transpose(data(pp:Nports:end,:)),1,[]);
    for ii = 1 : numel(data_u)
      fprintf(fid,'%d %d\n',real(data_u(ii)).int,imag(data_u(ii)).int);
    end
    fclose(fid);
  end
end
disp('Done front_i');

% --> Outputs of 'front' transforms taken in polyphase order (with zero-padded rows/columns)
for tt = 1 : Ntransform
  data = squeeze(front_o(:,:,tt));
  data = [[data,zeros(Nfft,Nextra)];zeros(Nextra,Nextra+Nfft)];
  for pp = 1 : Nports
    fid = fopen(sprintf('data/front_o_%d.txt',pp-1),'a');
    data_u = reshape(transpose(data(pp:Nports:end,:)),1,[]);
    for ii = 1 : numel(data_u)
      fprintf(fid,'%d %d\n',real(data_u(ii)).int,imag(data_u(ii)).int);
    end
    fclose(fid);
  end
end
disp('Done front_o');

% --> Inputs to 'back' taken in polyphase order (with zero-padded rows/columns)
for tt = 1 : Ntransform
  data = squeeze(front_o(:,:,tt));
  data = [[data,zeros(Nfft,Nextra)];zeros(Nextra,Nextra+Nfft)];
  for pp = 1 : Nports
    fid = fopen(sprintf('data/back_i_%d.txt',pp-1),'a');
    data_u = reshape(data(:,pp:Nports:end),1,[]);
    for ii = 1 : numel(data_u)
      fprintf(fid,'%d %d\n',real(data_u(ii)).int,imag(data_u(ii)).int);
    end
    fclose(fid);
  end
end
disp('Done back_i');

% --> Outputs of 'back' taken in polyphase order (with zero-padded rows/columns)
for tt = 1 : Ntransform
  data = squeeze(back_o(:,:,tt));
  data = [[data,zeros(Nfft,Nextra)];zeros(Nextra,Nextra+Nfft)];
  for pp = 1 : Nports
    fid = fopen(sprintf('data/back_o_%d.txt',pp-1),'a');
    data_u = reshape(data(:,pp:Nports:end),1,[]);
    for ii = 1 : numel(data_u)
      fprintf(fid,'%d %d\n',real(data_u(ii)).int,imag(data_u(ii)).int);
    end
    fclose(fid);
  end
end
disp('Done back_o');


  

