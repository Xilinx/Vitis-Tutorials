%%
% Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT%

% Author: Daniele Bagni, Xilinx Inc


function y = fir_filter(x, c)

global N;

persistent ref_shift_reg
if isempty(ref_shift_reg)
    ref_shift_reg = zeros(1,N);
end
acc = 0;
mult= 0;
y = 0;
for i = N-1 : -1 : 0

     if (i==0)
       ref_shift_reg(0+1)=x;
     else
        ref_shift_reg(i+1) = ref_shift_reg(i-1+1);
     end
     mult = ref_shift_reg(i+1)*c(i+1);
     acc  = acc + mult;
end
y = fix(acc/2^14);

end
