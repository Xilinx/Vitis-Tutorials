%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Richard Buz

% Performs bilinear interpolation of a single pixel.
% Four equivalent calculation methods are provided.
function interp_val = blint(ref,method)

if method == 1  % direct calculation, two-step
    f1 = (1 - ref(5)) * ref(1) + ref(5) * ref(3);
    f2 = (1 - ref(5)) * ref(2) + ref(5) * ref(4);
    interp_val = (1 - ref(6)) * f1 + ref(6) * f2;
end
  
if method == 2  % polynomial fit
    coef = [ref(1) ref(3)-ref(1) ref(2)-ref(1) ref(4)-ref(3)-ref(2)+ref(1)];
    vec = [1 ref(5) ref(6) ref(5)*ref(6)]';
    interp_val = coef * vec;
end

if method == 3  % weighted mean
    weights = [(1-ref(5))*(1-ref(6)) (1-ref(5))*ref(6) ref(5)*(1-ref(6)) ref(5)*ref(6)];
    vec = ref([1 2 3 4])';
    interp_val = weights * vec;
end

if method == 4  % AIE implementation
    p11 = ref(1);
    p12 = ref(2);
    p21 = ref(3);
    p22 = ref(4);
    xf = ref(5);
    yf = ref(6);
    pxy1 = (p11 + xf * p21) - xf * p11;
    pxy2 = (p12 + xf * p22) - xf * p12;
    interp_val = (pxy1 + yf * pxy2) - yf * pxy1;
end