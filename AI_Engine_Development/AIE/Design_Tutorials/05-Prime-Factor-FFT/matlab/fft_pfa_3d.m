%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins

function [sig_o] = fft_pfa_3d( sig_i, N1, N2, N3 )
    [P_i,P_o,N] = compute_perm_3d(N1,N2,N3);
    % Input permutation:
    data = reshape(sig_i(1+P_i),N1,N2,N3);
    % Transforms
    for cc = 1 : N2
      for dd = 1 : N3
        data(:,cc,dd) = fft(data(:,cc,dd));
      end
    end
    for rr = 1 : N1
      for dd = 1 : N3
        data(rr,:,dd) = fft(data(rr,:,dd));
      end
    end
    for rr = 1 : N1
      for cc = 1 : N2
        data(rr,cc,:) = fft(data(rr,cc,:));
      end
    end
    % Output permutation:
    data_o = reshape(data,1,[]);
    sig_o(1+P_o) = data_o;
end
