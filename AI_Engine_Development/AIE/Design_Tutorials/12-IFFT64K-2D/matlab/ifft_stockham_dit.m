%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function [ifft_o] = ifft_stockham_dit(ifft_i,nt_twid,fi_twid,nt_sig,fi_sig)
   N = numel(ifft_i);
   Nstage = log2(N);
   buff_i = ifft_i;
   buff_t = zeros(size(buff_i));
   for stage = Nstage : -1 : 1
     Nhalf = N / 2^stage;
     twid = double(fi(exp(-1i*2*pi*[0:Nhalf-1]/(2*Nhalf)),nt_twid,fi_twid));
     stride = 2^(stage-1);
     for p = 0 : Nhalf-1
       for q = 0 : stride-1
         % Note: address is not "in-place"
         idx0_i = q + stride*(2*p+0);
         idx1_i = q + stride*(2*p+1);
         idx0_o = q + stride*(p+0    );
         idx1_o = q + stride*(p+Nhalf);
         a = buff_i(1+idx0_i);
         b = buff_i(1+idx1_i) * conj(twid(1+p));
         buff_t(1+idx0_o) = a + b;
         buff_t(1+idx1_o) = a - b;
       end
     end
     buff_i = buff_t;
   end
   ifft_o = buff_i;
   ifft_o = double(fi(ifft_o,nt_sig,fi_sig));
end
