%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Richard Buz

% Performs bilinear interpolation of a single pixel.
% Three equivalent calculation methods are provided.
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