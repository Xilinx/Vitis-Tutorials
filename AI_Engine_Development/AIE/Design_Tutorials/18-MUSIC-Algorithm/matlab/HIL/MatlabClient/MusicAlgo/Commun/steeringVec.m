% ========================================================================
% Copyright (C) 2023 / 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Authors: Mark   Rollins (AMD):   Original standard model, 
%          Bachir Berkane (Fidus): Restructured code and added ULA spacing        
% ========================================================================  

function [steeringVec] = steeringVec(sysCfg)
nUla     = sysCfg.nUla;     % Number of Ula Elements
dUla     = sysCfg.dUla;     % ULA antenna spacing 
angleRad = sysCfg.angleRad; % angle vector in Radians

steeringVec = single(exp(1i*2*pi*dUla*(0:nUla-1)' * sin(angleRad(:).')));

end