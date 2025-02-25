%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

% Extract matrix using a 4x4 tiling pattern
% major = row (0) or column (1)

function [vec] = tiling_4x4( mat_i, major )
   [R,C] = size(mat_i);
   assert(mod(R,4)==0,'# of rows must be multiple of 4');
   assert(mod(C,4)==0,'# of cols must be multiple of 4');
   nR = R/4;
   nC = C/4;

   % Note: Output tiling always uses row-major extraction, regardless of input matrix style
   vec = [];
   switch (major)
     case 0, % Row-Major
     for rr = 0 : nR-1
       for cc = 0 : nC-1
          data = mat_i(4*rr+[1:4],4*cc+[1:4]);
          vec = [vec,reshape(transpose(data),1,[])];
       end
     end
    case 1, % Column-Major
     for rr = 0 : nR-1
       for cc = 0 : nC-1
         data = mat_i(4*rr+[1:4],4*cc+[1:4]);
         vec = [vec,reshape(transpose(data),1,[])];
       end
     end
   end
end
