%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Mark Rollins & Bachir Berkane

function [data] = computeDoa(data, cfg)

%% Get data and configurations
noiseSS   = data.noiseSS;  % Noise SubSpace matrix
p         = data.sigSsDim; % Signal Matrix # of columns.
sweepSpan = cfg.musicSweepSpan;  % Sweep span in degrees
nPts      = cfg.musicSweepNPts;  % Sweep Number of points
sweepPts  = linspace(-sweepSpan/2, sweepSpan/2, nPts);

%% Initialize pseudo spectrum (Pm_peak) and its inverse (Pm_null)
[m, n]    = size(sweepPts);
Pm_null   = single(zeros(m, n+2));    % added 2 bins to track history
Pm_peak   = single(zeros(m, n+2));
%doaEst   = zeros(data.sigSsDim, 1);
steerMtx  = single(zeros(cfg.nUla,nPts));

%% Compute both Pm and its inverse
for p = 1 : nPts % Sweep and Compute Space Spectrum
    % compute spectrum power @ angle p * (sweepSpan/nPts)
    cfg.angleRad    =  single(sweepPts(p)* pi/180); % Angle in rad
    signal          =  single(steeringVec(cfg));    % Signal Steered @ angle
    tmp             =  single(noiseSS.' * signal);  % Null if Signal _|_ Noise
    Pm_null (p)     =  single((tmp'*tmp));       % Norm2 to save compuationat cost
    Pm_peak (p)     =  single(1.0/Pm_null(p));   % Peak (inf, in theory) if Signal _|_ Noise
    steerMtx(:, p)  =  single(signal);
end

% maxPm_null = max(Pm_null());  data.nullThreshold = maxPm_null/10;
data.nullThreshold = cfg.nullThreshold; 

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
            doaEstIdx(i) = p+1; i=i+1;
        end 
    end
end

%% Check # of estimated sources is consistent with the programmed value,
 
if exist('doaEstIdx','var')
    if (length(doaEstIdx) ~= data.sigSsDim || length(doaEstIdx) ~= cfg.nSource)
        fprintf ('\n <strong>- Warning:</strong>  Algorithm Did NOT DETECT all Specified Sources   \n');
        % error ('<strong>- ERROR ERROR:</strong>  See Error message above.');
    end

else
    doaEstIdx = NaN;
end

%% DOAs (angles) in degrees
%doaEstDeg = single (- sweepSpan/2 + (doaEstIdx-1) * sweepSpan/(nPts-1) );

if isnan(doaEstIdx)
     doaEstIdx = [];
end
doaEstDeg = sweepPts(doaEstIdx);
%% Log
data.sweepPts  = single(sweepPts);
data.steerMtx  = single(steerMtx);
data.Pm_null   = Pm_null;
data.Pm_peak   = Pm_peak;
data.doaEstDeg = doaEstDeg;
data.doaEstIdx = doaEstIdx;

end
 
   