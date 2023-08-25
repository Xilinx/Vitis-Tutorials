%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;

% Choose (N1,N2) to be relatively prime:
N1 = 7; 
N2 = 9;
N3 = 16;
N = N1 * N2 * N3;
xx = complex(randn(1,N),randn(1,N));
yy = fft_pfa_3d(xx,N1,N2,N3);
zz = fft(xx);

subplot(2,1,1); plot(real(zz),'b.-'); hold on; plot(real(yy),'ro--'); hold off; ylabel('Real');
s1 = sprintf('Prime Factor FFT: N = %g (',N);
s2 = sprintf('%gx',[N1,N2]);
title(sprintf('%s%s%d)',s1,s2,N3));
subplot(2,1,2); plot(imag(zz),'k.-'); hold on; plot(imag(yy),'mo--'); hold off; ylabel('Imag');

