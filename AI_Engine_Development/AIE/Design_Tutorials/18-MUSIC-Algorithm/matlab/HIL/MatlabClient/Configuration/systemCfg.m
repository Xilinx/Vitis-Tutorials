% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

% Configuration parameters for system model
% Author: Bachir Berkane, Fidus Systems
 
function [cfg]     = systemCfg ()
 
%% Signal Parameters
cfg.sigMod           = 1;    % Signal Modulation Scheme; 1:= BPSK, 2:= QPSk  
cfg.nSource          = 2;    % Number of signals = nbr of sources
cfg.sourceMinSepar   = 6;    % in Degrees .. specify minimum separation between sources 
cfg.sourceSpan       = 160;  % sources are placed in [-sourceSpan/2,  sourceSpan/2]; 
     
% specify initial position of sources as [x, y];  x range = [-1, 1]
%                                                 y range = [ 0, 1]
% Specify velocity of sources as [vx, vy]; range of vx, vy in [-1, 1]
% Configure  up to 7 srcs. Nbr must match cfg.nSource (above)
% Model will ignore pos<n>, n > nSource
%-------------------------------------------
pos1 = [ 0.538 0.364];  v1 = [0.10, -0.20];
pos2 = [-0.424 0.514];  v2 = [0.20, 0.30];
pos3 = [0.7 0.252];     v3 = [-0.30, 0.40];

pos4 = [-0.762 0.476];  v4 = [0.10, 0.20];

pos5 = [-0.862 0.476];  v5 = [0.22, 0.20];
pos6 = [0.2, 0.3]; v6 = [0.10, 0.20];
pos7 = [0.2, 0.3]; v7 = [0.10, 0.20];
% -------------------------------------------

sourcePos = [pos1.', pos2.', pos3.', pos4.', pos5.', pos6.', pos7.'];
sourceVel = [v1.', v2.', v3.', v4.', v5.', v6.', v7.'];
cfg.sourcePos =  sourcePos(:, 1:cfg.nSource).';
cfg.sourceVel =  sourceVel(:, 1:cfg.nSource).';


% Nbr of snapshots to Generate
cfg.nSnapshots          = 16;
cfg.snapshotTimeSpacing = 0.03; % In seconds
                                
%% ULA-based Receiver Parameters
cfg.nUla             = 8;       % Number of array elements
cfg.dUla             = 0.5;     % Antenna Spacing in wavelength 
cfg.Snr              = 10;      % Value in dB @ ULA element
cfg.mSamp            = 128;     % Number of snapshots





end