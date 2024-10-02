%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins & Bachir Berkane

function [data] = computeDoa(data, sysCfg, musiCfg)

%% Get data and configurations
noiseSS   = data.noiseSS;  % Noise SubSpace matrix
p         = data.sigSsDim; % Signal Matrix # of columns.
sweepSpan = musiCfg.musicSweepSpan;  % Sweep span in degrees
nPts      = musiCfg.musicSweepNPts;  % Sweep Number of points
sweepPts  = linspace(-sweepSpan/2, sweepSpan/2, nPts);

%% Initialize pseudo spectrum (Pm_peak) and its inverse (Pm_null)
[m, n]    = size(sweepPts);
Pm_null   = single(zeros(m, n+2));    % added 2 bins to track history
Pm_peak   = single(zeros(m, n+2));
%doaEst   = zeros(data.sigSsDim, 1);
steerMtx  = single(zeros(sysCfg.nUla,nPts)); 
 
%% Compute both Pm and its inverse
for p = 1 : nPts % Sweep and Compute Space Spectrum
    % compute spectrum power @ angle p * (sweepSpan/nPts)
    sysCfg.angleRad =  single(sweepPts(p)* pi/180); % Angle in rad
    signal           =  single(steeringVec(sysCfg));    % Signal Steered @ angle
    tmp              =  single(noiseSS.' * signal);  % Null if Signal _|_ Noise
    Pm_null (p)      =  single((tmp'*tmp));       % Norm2 to save compute cost
    Pm_peak (p)      =  single(1.0/Pm_null(p));   % Peak (inf, in theory) if Signal _|_ Noise
    steerMtx(:, p)   =  single(signal);
end
maxPm_null = max(Pm_null());

data.nullThreshold = maxPm_null/10;
%% Find nulls below the configurable threshold
i =1;
% ct = 0;  -- Debug only
for p = 1 : nPts % Sweep and search for null below threshold
    % 1st order derivative
    dPm_pt1 = sign(Pm_null(p+1)-Pm_null(p));
    dPm_pt2 = sign(Pm_null(p+2)-Pm_null(p+1));

    % Detect Change in tang direction .. neg == > pos
    if (dPm_pt1 == -1) && (dPm_pt2 == +1)
        
        % record null only if below the programmed threshold
        if Pm_null(p+1) < data.nullThreshold %cfg.nullThreshold
           % ct= ct+1; -- Debug Only
            doaEst(i) = p+1; i=i+1;
        end 
    end
end

 
%% Check # of estimated sources is consistent with the programmed value,

if exist('doaEst','var')
    if (length(doaEst) ~= data.sigSsDim || length(doaEst) ~= sysCfg.nSource)
        fprintf ('\n  -- <strong>- Warning:</strong>  Algorithm Did NOT DETECT all Specified Sources   \n');
        % error ('<strong>- ERROR ERROR:</strong>  See Error message above.');
    end

else
    doaEst = NaN;
end
%% DOAs (angles) in degrees
doaEst = single((doaEst * musiCfg.musicSweepSpan/(nPts-1))-(1+musiCfg.musicSweepSpan/2));

%% Log
data.sweepPts = single(sweepPts);
data.steerMtx = single(steerMtx);
data.Pm_null  = Pm_null;
data.Pm_peak  = Pm_peak;
data.doaEst   = doaEst;

end
 
   