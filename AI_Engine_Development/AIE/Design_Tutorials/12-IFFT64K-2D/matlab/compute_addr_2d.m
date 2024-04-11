%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function [addr_o] = compute_addr_2d(N1,N2,dim)
   N = N1 * N2;
   addr_i = [0:N-1];
   pattern_i = reshape(addr_i,N1,N2);
   switch (dim)
     % Linear addressing for N1 dimension:
    case 1,
     addr_o = addr_i;

    case 2,
     % Step-by-N2 addressing for N2 dimension:
     addr_o = [];
     for rr = 1 : N1
       addr_o = [addr_o,reshape(pattern_i(rr,:),1,[])];
     end
   end
end
