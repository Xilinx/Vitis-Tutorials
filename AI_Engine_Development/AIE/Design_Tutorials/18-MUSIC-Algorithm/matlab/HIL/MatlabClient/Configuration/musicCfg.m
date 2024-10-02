
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Bachir Berkane, Fidus Systems
% Configuration parameters for Music Algorithm

 
function cfg         = musicCfg ()
 
%% MUISC Algorithm Parameters

% Fixed parameters ... Theses parmeters are fixed in AIE_Implementation
cfg.gssrModel       = 'row'; % 'row' or 'column' R Matrix generation 
cfg.svdRotModel     = 1;     % Version 1 or Version 2.
cfg.svdNumIter      = 4;     % Nbr of iterations of the Jacobi Algorithm
cfg.musicSweepNPts  = 256;   % Granularity of sweep
cfg.musicSweepSpan  = 180;   % Sweep span in degrees .. maximumm 180 deg 
                             % domain = [-SweepSpan/2 SweepSpan/2] 

cfg.musicSweepSpan  = 180;   % in degrees .. maximumm 180 deg 
                             % sweep span in [-SweepSpan/2 SweepSpan/2] 

% These parameters are passed to board in the HIL frame header. 
% In the AIE implemenation these parameters shall be pas through RTP to
% avoid a re-compile.
cfg.noiseSsCutmeth  = 1;     % 1 := use noise floor; 
                             % 2 := use # of programmable sources

cfg.noiseFloor      = 0.05;  % in ]0, 1[

cfg.nullThreshold   = 0.7;   % Null search threshold 



%% Plot/Log Parameters
cfg.figurePos = [10 10 1400  800];  % Position of Figure

end
