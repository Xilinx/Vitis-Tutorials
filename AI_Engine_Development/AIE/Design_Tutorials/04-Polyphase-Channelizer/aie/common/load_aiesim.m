%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function [data] = load_aiesim( fname, dtype, is_complex )

   fid = fopen(fname,'r');
   done = 0;
   data = [];
   while ( done == 0 )
     line = fgets(fid);
     if ( line == -1 )
       done = 1;
     elseif ( strcmp(line(1),'T') == 0 )
       switch dtype
        case 'int',
         if ( is_complex == 1 )
            [X] = sscanf(line,'%d %d');
            data = [data,complex(X(1),X(2))];
         else
            [val] = sscanf(line,'%d');
            data = [data,val];
         end
        case 'double',
         if ( is_complex == 1 )
            [X] = sscanf(line,'%lf');
            data = [data,complex(X(1),X(2))];
         else
            [val] = sscanf(line,'%lf');
            data = [data,val];
         end
        otherwise,
         error(sprintf('Unsupported %s',dtype));
       end
     end
   end
end

