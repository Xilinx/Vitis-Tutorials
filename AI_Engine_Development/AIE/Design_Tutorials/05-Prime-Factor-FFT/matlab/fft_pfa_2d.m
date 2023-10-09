%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function [sig_o] = fft_pfa_2d( sig_i, N1, N2 )
    [P_i,P_o,N] = compute_perm_2d(N1,N2);
    % Input permutation
    data = reshape(sig_i(1+P_i),N2,N1);
    % Row transform
    for rr = 1 : N2
      data(rr,:) = fft(data(rr,:));
    end
    % Col transform
    for cc = 1 : N1
      data(:,cc) = fft(data(:,cc));
    end
    % Output permutation
    data_o = reshape(data,1,[]);
    sig_o(1+P_o) = data_o;
end
