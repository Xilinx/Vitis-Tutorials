%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

clear all;
close all;
addpath('../../matlab');

% ------------------------------------------------------------
% Generate Input Permutation
% ------------------------------------------------------------

N1 = 7;
N2 = 9;
N3 = 16;
[P_i,P_o,N] = compute_perm_3d(N1,N2,N3);

% ------------------------------------------------------------
% Write Output Files
% ------------------------------------------------------------

fid = fopen('pfa1008_permute_i_luts.h','w');
fprintf(fid,'//\n');
fprintf(fid,'// Copyright (fid,C) 2023, Advanced Micro Devices, Inc. All rights reserved.\n');
fprintf(fid,'// SPDX-License-Identifier: MIT\n');
fprintf(fid,'//\n');
fprintf(fid,'// Author: Mark Rollins\n');
fprintf(fid,'\n');
write_perm_lut(fid, P_i);
fclose(fid);

% ------------------------------------------------------------
% Write Permutation Address
% ------------------------------------------------------------

function write_perm_lut( fid, addr )
   fprintf(fid,'#define PERM_I_ADDR {\\\n');
   for ii = 1 : numel(addr)
     if ( ii == numel(addr) ) fprintf(fid,'%4d \\\n',addr(ii));
     else                     fprintf(fid,'%4d,\\\n',addr(ii));
     end
   end
   fprintf(fid,'}\n');
end

