%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function [P_i,P_o,N] = compute_perm_2d( N1, N2 )
    assert(gcd(N1,N2)==1,'Factors must be relatively prime');
    N = N1 * N2;
    % Compute Chinese Remainder Theorem (CRT) index mapping:
    c = find(mod([0:N1-1]*N2,N1)==1)-1; % This is multiplicative inverse of N2 mod N1
    d = find(mod([0:N2-1]*N1,N2)==1)-1; % This is multiplicative inverse of N1 mod N2
    K3 = c * N2;
    K4 = d * N1;
    I_i = mod(N2*[0:N1-1] + N1*[0:N2-1]',N);
    I_o = mod(K3*[0:N1-1] + K4*[0:N2-1]',N);
    P_i = reshape(I_i,1,[]);
    P_o = reshape(I_o,1,[]);
end
