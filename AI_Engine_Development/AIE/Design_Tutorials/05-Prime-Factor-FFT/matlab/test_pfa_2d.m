%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;

% Choose (N1,N2) to be relatively prime:
N1 = 63; 
N2 = 16;

N = N1 * N2;
xx = complex(randn(1,N),randn(1,N));
yy = fft_pfa_2d(xx,N1,N2);
zz = fft(xx);

subplot(2,1,1); plot(real(zz),'b.-'); hold on; plot(real(yy),'ro--'); hold off; ylabel('Real');
title(sprintf('Prime Factor FFT: N = %g x %g = %g',N1,N2,N));
subplot(2,1,2); plot(imag(zz),'k.-'); hold on; plot(imag(yy),'mo--'); hold off; ylabel('Imag');

