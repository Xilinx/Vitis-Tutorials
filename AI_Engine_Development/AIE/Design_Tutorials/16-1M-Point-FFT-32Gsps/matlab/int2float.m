%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

function y = int2float(x)

if isreal(x)

    idx = find(x<0);
    x(idx) = x(idx) + 2^32;
    
    s  = floor(x./2^31);
    ex = mod(floor(x./2^23), 2^8)-127;
    fr = mod(x, 2^23);
    
    y = (1-2*s) .* (1+fr./2^23) .* (2.^ex);

else

    y = complex( int2float(real(x)), int2float(imag(x)));

end
