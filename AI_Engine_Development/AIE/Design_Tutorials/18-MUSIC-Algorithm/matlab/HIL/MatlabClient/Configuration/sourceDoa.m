% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Bachir Berkane, Fidus Systems
% Configuration parameters for Music Algorithm


function [sysCfg] = sourceDoa(sysCfg, i)

 
% i = 1 ... nSnapshots
t = sysCfg.snapshotTimeSpacing;
if (i~=1)
    for s =1:sysCfg.nSource
        posX = sysCfg.sourcePos(s,1) + sysCfg.sourceVel(s,1) .* t;
        sysCfg.sourcePos(s,1) = max(min(posX,1), -1);

        posY = sysCfg.sourcePos(s,2) +  sysCfg.sourceVel(s,2) .* t;
        sysCfg.sourcePos(s,2) = max(min(posY,1), 0);
    end
end


% Convert from position to direction of arrival
doaDeg = ones(1,sysCfg.nSource) .* NaN;
for s=1: sysCfg.nSource
    tg = sysCfg.sourcePos(s,2)/sysCfg.sourcePos(s,1) ;
    switch ( tg >= 0 )
        case true
            doaDeg(s) = 90 - rad2deg(atan(tg));

        case false
            doaDeg(s) = -rad2deg(atan(tg)) - 90;

    end
end
sysCfg.doaDeg=doaDeg;


%% Check consistency of parameters
% Check min separation/sweep span vs. nbr of sources  
str = 'Error: Cannot generate %d sources within the specified min separation/Sweep-span = %d/%d';
assert (sysCfg.sourceMinSepar * sysCfg.nSource < sysCfg.sourceSpan, str, sysCfg.nSource, sysCfg.sourceMinSepar, sysCfg.sourceSpan );

% Check if spec'd nSource differ from sysCfg.doaDeg and update accordingly
if (sysCfg.nSource ~= numel(sysCfg.doaDeg))
    sysCfg.nSource  = numel(sysCfg.doaDeg);
    strg = '\n<strong>- Warning:</strong> Number of Sources set to %d, the length of the specified DOA vector\n ';
    fprintf(strg, sysCfg.nSource);
end

% Check if spec'd doa vector satisfy min separation
d          = sort(sysCfg.doaDeg);
pre_doaDeg = [d(2:end) 0];
s          = abs(d - pre_doaDeg);
mask       = s(1:end-1) >= sysCfg.sourceMinSepar;
list       = find (mask ==0);  

if (~isempty(list))
    fprintf('\n<strong>- Warning:</strong> Specified DOA vector does not meet the specified min %d \n ', sysCfg.sourceMinSepar);
end


end