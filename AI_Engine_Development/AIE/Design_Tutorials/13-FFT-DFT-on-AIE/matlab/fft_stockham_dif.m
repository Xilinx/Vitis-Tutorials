%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function [fft_o] = fft_stockham_dif(fft_i)
   N = numel(fft_i);
   Nstage = log2(N);
   buff_i = fft_i;
   buff_t = zeros(size(buff_i));
   for stage = 1 : Nstage
     Nhalf = N / 2^stage;
     twid = exp(-1i*2*pi*[0:Nhalf-1]/(2*Nhalf));
     stride = 2^(stage-1);
     for p = 0 : Nhalf-1
       for q = 0 : stride-1
         % Note: address is not "in-place"
         idx0_i = q + stride*(p+0    );
         idx1_i = q + stride*(p+Nhalf);
         idx0_o = q + stride*(2*p+0);
         idx1_o = q + stride*(2*p+1);
         a = buff_i(1+idx0_i);
         b = buff_i(1+idx1_i);
         buff_t(1+idx0_o) =  a + b;
         buff_t(1+idx1_o) = (a - b) * twid(1+p);
       end
     end
     buff_i = buff_t;
   end
   fft_o = buff_i;
end

