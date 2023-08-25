%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function [addr_o] = compute_addr_3d(N1,N2,N3,dim)
   N = N1 * N2 * N3;
   addr_i = [0:N-1];
   pattern_i = reshape(addr_i,N1,N2,N3);
   switch (dim)
     % Linear addressing for N1 dimension:
    case 1,
     addr_o = addr_i;

     % Step-by-N2 addressing for N2 dimension:
    case 2,
     addr_o = [];
     for rr = 1 : N1
       for dd = 1 : N3
         addr_o = [addr_o,reshape(pattern_i(rr,:,dd),1,[])];
       end
     end

     % Step-by-N1*N2 addressing for N3 dimension:
    case 3,
     addr_o = [];
     for rr = 1 : N1
       for cc = 1 : N2
         addr_o = [addr_o,reshape(pattern_i(rr,cc,:),1,[])];
       end
     end
   end
end
