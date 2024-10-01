%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Richard Buz

function y = to_bfloat16(xin)

[r, c] = size(xin);
xfp32 = single(xin(:));
xint32 = typecast(xfp32, 'uint32');
xint16 = uint32(2^16 * round(double(xint32) / 2^16));
y = typecast(xint16, 'single');
y = reshape(y,r,c);