%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function [P_i,P_o,N] = compute_perm_3d( N1, N2, N3 )
    assert(gcd(N1,N2)==1,'Factors must be relatively prime');
    assert(gcd(N1,N3)==1,'Factors must be relatively prime');
    assert(gcd(N2,N3)==1,'Factors must be relatively prime');
    N = N1 * N2 * N3;
    d = find(mod([0:N1-1]*(N2*N3),N1)==1)-1;
    e = find(mod([0:N2-1]*(N1*N3),N2)==1)-1;
    f = find(mod([0:N3-1]*(N1*N2),N3)==1)-1;
    M1 = d*N2*N3;
    M2 = e*N1*N3;
    M3 = f*N1*N2;
    I_i = zeros(N1,N2,N3);
    I_o = zeros(N1,N2,N3);
    for ii = 0 : N1-1
      for jj = 0 : N2-1
        for kk = 0 : N3-1
          I_i(1+ii,1+jj,1+kk) = mod(  N2*N3*ii +   N1*N3*jj +   N1*N2*kk,N);
          I_o(1+ii,1+jj,1+kk) = mod(d*N2*N3*ii + e*N1*N3*jj + f*N1*N2*kk,N);
        end
      end
    end
    P_i = reshape(I_i,1,[]);
    P_o = reshape(I_o,1,[]);
    % Debug for (N1,N2,N3) = (3,4,5)
% $$$     fprintf(1,'P_i = [%2d %2d %2d %2d %2d %2d %2d %2d %2d %2d\n',P_i(1:10));
% $$$     fprintf(1,'       %2d %2d %2d %2d %2d %2d %2d %2d %2d %2d\n',P_i(11:20));
% $$$     fprintf(1,'       %2d %2d %2d %2d %2d %2d %2d %2d %2d %2d\n',P_i(21:30));
% $$$     fprintf(1,'       %2d %2d %2d %2d %2d %2d %2d %2d %2d %2d\n',P_i(31:40));
% $$$     fprintf(1,'       %2d %2d %2d %2d %2d %2d %2d %2d %2d %2d\n',P_i(41:50));
% $$$     fprintf(1,'       %2d %2d %2d %2d %2d %2d %2d %2d %2d %2d]\n',P_i(51:60));
% $$$     fprintf(1,'\n');
% $$$     fprintf(1,'\n');
% $$$     fprintf(1,'\n');
% $$$     fprintf(1,'P_o = [%2d %2d %2d %2d %2d %2d %2d %2d %2d %2d\n',P_o(1:10));
% $$$     fprintf(1,'       %2d %2d %2d %2d %2d %2d %2d %2d %2d %2d\n',P_o(11:20));
% $$$     fprintf(1,'       %2d %2d %2d %2d %2d %2d %2d %2d %2d %2d\n',P_o(21:30));
% $$$     fprintf(1,'       %2d %2d %2d %2d %2d %2d %2d %2d %2d %2d\n',P_o(31:40));
% $$$     fprintf(1,'       %2d %2d %2d %2d %2d %2d %2d %2d %2d %2d\n',P_o(41:50));
% $$$     fprintf(1,'       %2d %2d %2d %2d %2d %2d %2d %2d %2d %2d]\n',P_o(51:60));
% $$$     fprintf(1,'\n');
% $$$     fprintf(1,'\n');
% $$$     fprintf(1,'\n');
end
