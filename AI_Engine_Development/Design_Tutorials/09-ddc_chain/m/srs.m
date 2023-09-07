%  
%  Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
%  SPDX-License-Identifier: MIT
%  

function  y = srs(x, shift, bitwidth)

%offset=0.5;
%offset=0;
%------------ Matching Symmetric Rounding to Inf -----
a = real(x);
%ya = floor(a/2^shift+offset);
ya = round(a/2^shift);
amax = 2^bitwidth;
idx = find(ya>=amax);
ya(idx)=amax-1;
idx = find(ya<-amax);
ya(idx)=-amax;

a = imag(x);
%yb = floor(a/2^shift+offset);
yb = round(a/2^shift);
amax = 2^bitwidth;
idx = find(yb>=amax);
yb(idx)=amax-1;
idx = find(yb<-amax);
yb(idx)=-amax;

y = complex(ya, yb);
