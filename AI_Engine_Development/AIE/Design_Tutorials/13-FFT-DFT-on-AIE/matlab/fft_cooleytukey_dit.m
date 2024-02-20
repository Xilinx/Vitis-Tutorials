%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function [fft_o] = fft_cooleytukey_dit(fft_i)
   N = numel(fft_i);
   Nstage = log2(N);
   buff_i = bitrevorder(fft_i);
   for stage = Nstage : -1 : 1
     Nhalf = N / 2^stage;
     twid = exp(-1i*2*pi*[0:Nhalf-1]/(2*Nhalf));
     stride = 2^(stage-1);
     skip = N/stride;
     for s = 0 : stride-1
       q = s * skip;
       for p = 0 : Nhalf-1
         % Note: "in-place" addresses:
         idx0_i = q + p + 0;
         idx0_o = q + p + 0;
         idx1_i = q + p + Nhalf;
         idx1_o = q + p + Nhalf;
         a = buff_i(1+idx0_i);
         b = buff_i(1+idx1_i) * twid(1+p);
         buff_i(1+idx0_o) = a + b;
         buff_i(1+idx1_o) = a - b;
       end
     end
   end
   fft_o = buff_i;
end
