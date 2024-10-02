% ========================================================================
% Copyright (C) 2023 / 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Bachir Berkane, Fidus Systems
% Test Configuration for MUSIC algorithm

 

 
function cfg         = testCfg ()
 
%% Signal and ULa Model Parameters
cfg.sigMod           = 1;       % Modulation Scheme; 1:= BPSK, 2:= QPSk  
cfg.Snr              = 20;      % Value in dB @ ULA element
cfg.mSamp            = 128;     % Number of snapshots
cfg.nSource          = 2;       % Number of signal sources
cfg.nUla             = 8;       % Number of array elements
cfg.dUla             = 0.5;     % Antenna Spacing in wavelength 
cfg.sourceMinSepar   = 5;       % in Degrees .. specify minimum separation between sources 
cfg.sourceSpan       = 160;     % sources are placed in [-sourceSpan/2,  sourceSpan/2]; 
                                

% Check min separation/sweep span vs. nbr of sources  
assert (cfg.sourceMinSepar * cfg.nSource < cfg.sourceSpan, 'Cannot generate %d sources within the specified min separation/Sweep-span = %d/%d', cfg.nSource, cfg.sourceMinSepar, cfg.sourceSpan );

% doa of signals in degree with minimum configured separation
ss=cfg.sourceMinSepar; span=cfg.sourceSpan; 
cfg.doaDeg          = (ss * randperm(floor(span/ss), cfg.nSource))-span/2;

%%% >>>  doa is set explicitly below .. length of vector should match the 
%        configured number of sources, AND SET cfg.nSource ACCORDINGLY 
% E.g., 
%       cfg.doaDeg =  [12,  50,  -25];    %  with cfg.nSource = 3;



% Check if spec'd nSource differ from cfg.doaDeg and update accordingly
if (cfg.nSource ~= numel(cfg.doaDeg))
    cfg.nSource  = numel(cfg.doaDeg);
end

% -- check if spec'd doa vector satisfy min separation
d          = sort(cfg.doaDeg);
pre_doaDeg = [d(2:end) 0];
s          = abs(d - pre_doaDeg);
mask       = s >= cfg.sourceMinSepar;
list       = find (mask ==0);

if (~isempty(list))
    fprintf('\n<strong>- Warning:</strong> Specified DOA vector does not meet the specified min %d \n ', cfg.sourceMinSepar);
end


% check consistency between size of cfg.doaDeg and cfg.nSource
[~, n] = size (cfg.doaDeg);
if n ~= cfg.nSource 
    fprintf('\n<strong>- Note:</strong> DOAs set:  [');
    fprintf('%g, ',  cfg.doaDeg(1:end-1));
    fprintf('%g]\n', cfg.doaDeg(end));
    error ('\n <strong>- ERROR:</strong>  Number of configured sources (=%d), differ from size of specified DOA vector (=%d) \n', cfg.nSource, length(cfg.doaDeg))
end

%% Music Algorithm Parameters
cfg.gssrModel       = 'row'; % 'row' or 'column' R Matrix generation 
cfg.svdRotModel     = 2;     % Version 1 or Version 2.
cfg.svdNumIter      = 4;     % Nbr of iterations of the Jacobi Algorithm
cfg.noiseFloor      = 0.05;  % in ]0, 1[
cfg.noiseSsCutmeth  = 1;     %  1 := use noise floor; 2 := use # of programmable sources
cfg.musicSweepNPts  = 256;   % Granularity
cfg.nullThreshold   = 0.7;     % Null search threshold 
cfg.musicSweepSpan  = 180;   % in degrees .. maximumm 180 deg 
                             % sweep span in [-SweepSpan/2 SweepSpan/2] 


%% Log Parameters
cfg.log = 'on';        % 'on' or 'off'


%% Visulaization
cfg.visiblePlot    = 'on';  %    'on' or 'off'

end