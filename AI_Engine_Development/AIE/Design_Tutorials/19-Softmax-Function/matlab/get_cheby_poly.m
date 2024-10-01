%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Richard Buz

function p = get_cheby_poly(N)

% Chebyshev nodes
unodes = flipud(cos(((1:N)'-0.5) * pi / N));

% evaluate function at Chebyshev nodes
xa = 0;
xb = 1;
x = 0.5*(xb-xa)*unodes + 0.5*(xa+xb);
y = 1 + x - 2 .^ x;

% evalute Chebyshev polynomials at nodes
cheby_eval = zeros(N);
cheby_eval(:,1) = ones(N,1);
cheby_eval(:,2) = unodes;
for idx = 3:N
    cheby_eval(:,idx) = 2 * unodes .* cheby_eval(:,idx-1) - cheby_eval(:,idx-2);
end

% compute coefficients
cheby_coef = mean(cheby_eval .* repmat(y,1,N))';
cheby_coef(2:end) = 2*cheby_coef(2:end);

% compute T polys
tpoly = zeros(N);
tpoly(1,1) = 1;
tpoly(2,2) = 1;
for idx = 3:N
    tpoly(:,idx) = 2*[0; tpoly(1:end-1,idx-1)] - tpoly(:,idx-2);
end

polyu = tpoly * cheby_coef;

% convert from u to x
upoly = [-(xa+xb)/(xb-xa); 2/(xb-xa)];
u2xpoly = zeros(N);
u2xpoly(1,1) = 1;
u2xpoly(1:2,2) = upoly;
for idx = 3:N
    pprd = conv(u2xpoly(1:idx-1,idx-1),upoly);
    u2xpoly(1:idx,idx) = pprd;
end

poly_coef = u2xpoly * polyu;
p = flipud(poly_coef);
