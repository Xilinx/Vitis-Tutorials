%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: X11
%

function [IntCoef] = NormalizeHBCoef(Cbase, Norm)

IntCoef = Cbase;
l = length(Cbase);

p = Cbase(1:2:l);
pint = NormalizeCoef(p,Norm);
IntCoef(1:2:l) = pint;
IntCoef((l+1)/2) = Norm;
