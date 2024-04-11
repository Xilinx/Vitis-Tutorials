% 
% © Copyright 2022 Advanced Micro Devices, Inc.
%
% Licensed under the Apache License, Version 2.0 (the "License");
% you may not use this file except in compliance with the License.
% You may obtain a copy of the License at
%
%     http:%www.apache.org/licenses/LICENSE-2.0
%
% Unless required by applicable law or agreed to in writing, software
% distributed under the License is distributed on an "AS IS" BASIS,
% WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
% See the License for the specific language governing permissions and
% limitations under the License.
% 

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
            [X] = sscanf(line,'%d %d %d %d');
            data = [data,complex(X(1),X(2)),complex(X(3),X(4))];
         else
            [val] = sscanf(line,'%d %d');
            data = [data,val];
         end
        case 'double',
         if ( is_complex == 1 )
            [X] = sscanf(line,'%lf %lf %lf %lf');
            data = [data,complex(X(1),X(2)),complex(X(3),X(4))];
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

