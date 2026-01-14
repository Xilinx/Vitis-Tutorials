%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Faisal El-Shabani

clear all;
close all;
rng(1);

% ------------------------------------------------------------
% System Parameters
% ------------------------------------------------------------

Fs_Gsps = 2;
NCHAN = 4096;
TP_FIR_LEN = 36;

Niter = 4;

Nsamp = Niter * NCHAN;
Ntaps = NCHAN * TP_FIR_LEN;

fprintf(1,'NCHAN:      %g\n',NCHAN);
fprintf(1,'Ntaps:      %g\n',Ntaps);
fprintf(1,'Nsamp:      %g\n',Nsamp);

FB_SSR = 32;
FFT_SSR = 8;
Nfft = 64;
Nfft_all = Nfft*Nfft;

% ------------------------------------------------------------
% Create Channelizer Taps
% ------------------------------------------------------------

TT = numerictype(1,32,31);
WW = numerictype(1,16,15);
FF = fimath('RoundingMethod','Convergent','OverflowAction','Saturate');

sep = 0.15;
sfact = 32;  % Design filter using frequency-domain upsampling with sfact factor
NtapsL = Ntaps / sfact;
tapsL = firgr(NtapsL-1,[0,(0.5+sep)/(NCHAN/sfact),(1-sep)/(NCHAN/sfact),1],[1,1,0,0]);
tapsLf = fft(tapsL);
tapsHf = [tapsLf(1:NtapsL/2),zeros(1,(sfact-1)*NtapsL),tapsLf(1+NtapsL/2:end)];
taps = real(ifft(tapsHf));
taps_fxp = fi(taps,TT,FF);
[Hxx,Fxx] = freqz(double(taps_fxp),1,64*Ntaps,Fs_Gsps);
figure;
idx = find(Fxx < Fs_Gsps);
subplot(2,2,1); plot(Fxx(idx),20*log10(abs(Hxx(idx))),'k-');
xlabel('Frequency (GHz)'); ylabel('Magnitude (dB)'); grid on;
v = axis; axis([v(1) v(2),-100,0]);
title(sprintf('Fs: %g Gsps  # Channels: %g',Fs_Gsps,NCHAN));
idx = find(Fxx < Fs_Gsps/NCHAN);
subplot(2,2,3); plot(1e3*Fxx(idx),20*log10(abs(Hxx(idx))),'k-');
xlabel('Frequency (MHz)'); ylabel('Magnitude (dB)'); grid on;
title(sprintf('Ntaps: %g',Ntaps));
v = axis; axis([v(1) v(2),-100,0]);
subplot(2,2,[2,4]); plot(taps_fxp,'b-');
title(sprintf('# Taps/Phase: %g',TP_FIR_LEN));
grid on;

% ------------------------------------------------------------
% Store taps for design
% ------------------------------------------------------------

taps_hw = reshape(taps_fxp,NCHAN,TP_FIR_LEN);

% Assign 16 channels to each instance:
fid = fopen('channelizer_init_taps.h','w');
fprintf(fid,'//\n');
fprintf(fid,'// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.\n');
fprintf(fid,'// SPDX-License-Identifier: MIT\n');
fprintf(fid,'//\n');
fprintf(fid,'// Author: Faisal El-Shabani\n');
fprintf(fid,'#pragma once\n');
fprintf(fid,'#define TAPS_INIT_0 { ');
range = [1:NCHAN];
taps_u = reshape(taps_hw(range,:),1,[]);
for ii = 1 : numel(taps_u)
    if (ii == numel(taps_u)) fprintf(fid,'%d }\n',taps_u.int(ii));
    else                                       fprintf(fid,'%d, ',  taps_u.int(ii));
    end
end
fclose(fid);

% ------------------------------------------------------------
% I/O Samples
% ------------------------------------------------------------

backoff_dB = -15;
sig_i = sqrt(0.5)*10^(backoff_dB/20)*complex(randn(1,Nsamp),randn(1,Nsamp));
sig_i = fi(sig_i,WW,FF);
sig_i = reshape(sig_i,NCHAN,[]);

% Filter the channels along the rows:
taps_use = double(reshape(taps_hw,NCHAN,TP_FIR_LEN));
filterbank_o = zeros(size(sig_i));
for cc = 1 : NCHAN
  filterbank_o(cc,:) = filter(taps_use(cc,:),1,double(sig_i(cc,:)));
end
filterbank_o = fi(filterbank_o,TT,FF);

% ------------------------------------------------------------
% Generate twiddles for extra rotation:
% ------------------------------------------------------------
twid = complex(zeros(Nfft,Nfft),zeros(Nfft,Nfft));
for rr = 0 : Nfft-1
    for cc = 0 : Nfft-1
        twid(1+rr,1+cc) = double(fi(exp(+1i*2*pi*rr*cc/Nfft_all),WW,FF));
    end
end
twid = fi(twid,WW,FF);

% ------------------------------------------------------------
% Compute first set of transforms (front) along rows:
% ------------------------------------------------------------

data_r = complex(zeros(Nfft,Nfft,Niter));
for tt = 1 : Niter
  data = reshape(filterbank_o(:,tt),Nfft,Nfft);
  for rr = 1 : Nfft
    tmp = Nfft*ifft(double(data(rr,:)));
    data_r(rr,:,tt) = tmp .* twid(rr,:);
  end
end
front_o = fi(data_r,TT,FF);

% ------------------------------------------------------------
% Compute second set of transforms (back) along columns:
% ------------------------------------------------------------

data_b = complex(zeros(Nfft,Nfft,Niter));
for tt = 1 : Niter
  for cc = 1 : Nfft
    tmp = double(front_o(:,cc,tt));
    data_b(:,cc,tt) = Nfft*ifft(tmp);
  end
end
back_o = fi(data_b,TT,FF);

% Compute ideal output:
sig_o = zeros(Nfft,Nfft,Niter);
for tt = 1 : Niter
    sig_o(:,:,tt) = transpose(back_o(:,:,tt));
end
sig_o = fi(sig_o,TT,FF);

% ------------------------------------------------------------
% Store I/O Files
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

% Generate IO signals for HW_EMU:
fid_i = fopen(sprintf('data/sig_i.txt'),'w');
data_i = reshape(sig_i,1,[]);
for ii = 1 : numel(data_i)
  fprintf(fid_i,'%d %d\n',...
          real(data_i(ii  )).int,imag(data_i(ii  )).int);
end
fclose(fid_i);

fid_o = fopen(sprintf('data/sig_o.txt'),'w');
data_o = reshape(sig_o,1,[]);
for ii = 1 : numel(data_o)
  fprintf(fid_o,'%d %d\n',...
          real(data_o(ii  )).int,imag(data_o(ii  )).int);
end
fclose(fid_o);

% Generate input signals for AIE-only simulation:
for nn = 1 : FB_SSR
  fid_i = fopen(sprintf('data/filterbank_i_%d.txt',nn-1),'w');
  fid_o = fopen(sprintf('data/filterbank_o_%d.txt',nn-1),'w');
  range = nn:FB_SSR:NCHAN;
  data_i = reshape(sig_i       (range,:),1,[]);
  data_o = reshape(filterbank_o(range,:),1,[]);
  for ii = 1 : 2 : numel(data_i)
    fprintf(fid_i,'%d %d %d %d\n',...
            real(data_i(ii  )).int,imag(data_i(ii  )).int,real(data_i(ii+1)).int,imag(data_i(ii+1)).int);
  end
  for ii = 1 : numel(data_o)
    fprintf(fid_o,'%d %d\n',...
            real(data_o(ii  )).int,imag(data_o(ii  )).int);
  end
  fclose(fid_i);
  fclose(fid_o);
end

for tt = 1 : Niter
  data = reshape(filterbank_o(:,tt),Nfft,Nfft);
  for pp = 1 : FFT_SSR
    fid = fopen(sprintf('data/fft_front_i_%d.txt',pp-1),'a');
    data_u = reshape(data(pp:FFT_SSR:end,:),1,[]);
    for ii = 1 : numel(data_u)
      fprintf(fid,'%d %d\n',real(data_u(ii)).int,imag(data_u(ii)).int);
    end
    fclose(fid);
  end
end

for tt = 1 : Niter
  data = squeeze(front_o(:,:,tt));
  for pp = 1 : FFT_SSR
    fid = fopen(sprintf('data/fft_front_o_%d.txt',pp-1),'a');
    data_r = data(pp:FFT_SSR:end,:);
    for rr = 1 : 2 : size(data_r,1)
        data_u = reshape(data_r(rr:rr+1,:),1,[]);
        for ii = 1 : numel(data_u)
            fprintf(fid,'%d %d\n',real(data_u(ii)).int,imag(data_u(ii)).int);
        end
    end
    fclose(fid);
  end
end

for tt = 1 : Niter
  data = squeeze(front_o(:,:,tt));
  for pp = 1 : FFT_SSR
    fid = fopen(sprintf('data/fft_back_i_%d.txt',pp-1),'a');
    data_r = data(:,pp:FFT_SSR:end);
    for rr = 1 : 2 : size(data_r,2)
        data_u = reshape(transpose(data_r(:,rr:rr+1)),1,[]);
        for ii = 1 : numel(data_u)
            fprintf(fid,'%d %d\n',real(data_u(ii)).int,imag(data_u(ii)).int);
        end
    end
    fclose(fid);
  end
end

for tt = 1 : Niter
  data = squeeze(back_o(:,:,tt));
  for pp = 1 : FFT_SSR
    fid = fopen(sprintf('data/fft_back_o_%d.txt',pp-1),'a');
    data_u = reshape(transpose(data(:,pp:FFT_SSR:end)),1,[]);
    for ii = 1 : numel(data_u)
      fprintf(fid,'%d %d\n',real(data_u(ii)).int,imag(data_u(ii)).int);
    end
    fclose(fid);
  end
end
