%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Faisal El-Shabani

clear all;
close all;
rng(1);

% ------------------------------------------------------------
% Generate Model
% ------------------------------------------------------------

Nports = 8;
Niter = 8;
Nfft = 64;
Nfft_all = Nfft*Nfft;
Ntransform = Niter;
backoff_dB = -15;

TT = numerictype(1,32,31);
WW = numerictype(1,16,15);
FF = fimath('RoundingMethod','Convergent','OverflowAction','Saturate');

% ------------------------------------------------------------
% Input signal:
% ------------------------------------------------------------

% Random noise:
tmp = sqrt(0.5)*10^(0.05*backoff_dB)*complex(randn(1,Nfft_all*Ntransform),randn(1,Nfft_all*Ntransform));
scale = 0.5^12;
sig_i = fi(reshape(scale*tmp,Nfft_all,Ntransform),TT,FF);

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

data_r = complex(zeros(Nfft,Nfft,Ntransform));
for tt = 1 : Ntransform
  data = squeeze(reshape(sig_i(:,tt),Nfft,Nfft));
  for rr = 1 : Nfft
    tmp = double(fi(Nfft*ifft(double(data(rr,:))),TT,FF));
    data_r(rr,:,tt) = tmp .* twid(rr,:);
  end
end
front_o = fi(data_r,TT,FF);

% ------------------------------------------------------------
% Compute second set of transforms (back) along columns:
% ------------------------------------------------------------

data_b = complex(zeros(Nfft,Nfft,Ntransform));
for tt = 1 : Ntransform
  for cc = 1 : Nfft
    data_b(:,cc,tt) = double(fi(Nfft*ifft(double(front_o(:,cc,tt))),TT,FF));
  end
end
back_o = fi(data_b,TT,FF);

disp('Done compute');

% ------------------------------------------------------------
% Save I/O files for simulation
% ------------------------------------------------------------

[~,~,~] = rmdir('data','s');
[~,~,~] = mkdir('data');

for tt = 1 : Ntransform
  data = reshape(sig_i(:,tt),Nfft,Nfft);
  for pp = 1 : Nports
    fid = fopen(sprintf('data/front_i_%d.txt',pp-1),'a');
    data_u = reshape(data(pp:Nports:end,:),1,[]);
    for ii = 1 : numel(data_u)
      fprintf(fid,'%d %d\n',real(data_u(ii)).int,imag(data_u(ii)).int);
    end
    fclose(fid);
  end
end
disp('Done front_i');

for tt = 1 : Ntransform
  data = squeeze(front_o(:,:,tt));
  for pp = 1 : Nports
    fid = fopen(sprintf('data/front_o_%d.txt',pp-1),'a');
    data_r = data(pp:Nports:end,:);
    for rr = 1 : 2 : size(data_r,1)
        data_u = reshape(data_r(rr:rr+1,:),1,[]);
        for ii = 1 : numel(data_u)
            fprintf(fid,'%d %d\n',real(data_u(ii)).int,imag(data_u(ii)).int);
        end
    end
    fclose(fid);
  end
end
disp('Done front_o');

for tt = 1 : Ntransform
  data = squeeze(front_o(:,:,tt));
  for pp = 1 : Nports
    fid = fopen(sprintf('data/back_i_%d.txt',pp-1),'a');
    data_r = data(:,pp:Nports:end);
    for rr = 1 : 2 : size(data_r,2)
        data_u = reshape(transpose(data_r(:,rr:rr+1)),1,[]);
        for ii = 1 : numel(data_u)
            fprintf(fid,'%d %d\n',real(data_u(ii)).int,imag(data_u(ii)).int);
        end
    end
    fclose(fid);
  end
end
disp('Done back_i');

for tt = 1 : Ntransform
  data = squeeze(back_o(:,:,tt));
  for pp = 1 : Nports
    fid = fopen(sprintf('data/back_o_%d.txt',pp-1),'a');
    data_u = reshape(transpose(data(:,pp:Nports:end)),1,[]);
    for ii = 1 : numel(data_u)
      fprintf(fid,'%d %d\n',real(data_u(ii)).int,imag(data_u(ii)).int);
    end
    fclose(fid);
  end
end
disp('Done back_o');