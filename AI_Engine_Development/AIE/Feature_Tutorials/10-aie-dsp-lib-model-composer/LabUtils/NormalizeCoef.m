%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: X11
%

function [IntCoef] = NormalizeCoef(Cbase, Norm)

% First normalisation
K = Norm/sum(Cbase);
C0 = Cbase * K;

for i=1:10
    S = sum(round(C0));
    K = Norm/S;
    C0 = C0 * K;
end

IntCoef = round(C0);

