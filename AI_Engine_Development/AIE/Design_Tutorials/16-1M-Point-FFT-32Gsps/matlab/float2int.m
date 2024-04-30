%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

function y = float2int(x)

if isreal(x)
    % make sure input is single
    x = single(x);
    
    % convert single precision to decimal numbers
    y = zeros(size(x));
    
    for k=1:numel(x)
        y(k) = hex2dec(num2hex(x(k)));
    end
    
    % handle plus and minus sign
    idx = find(y >= 2^31);
    y(idx) = y(idx)-2^32;
else
    y = complex(float2int(real(x)), float2int(imag(x)));
end
