% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Bachir Berkane, Fidus Systems
% Configuration parameters for Music Algorithm

 
function cfg     = hilCfg ()

cfg.demoData     = 'archived'; % 'archived' | 'generated'

cfg.demoDataSet  = 'Demo1';     % Select a set under ./Snaphots/Archive
                                % when demoDataSet configured to 'archived'
                                
cfg.capRxDat     = false;       % True | false ... Catupre TCP Rcvd  data
                                % under Debug Directory

cfg.capVideo     = true;        % True | false ...  
                                % Video under MatlabClient Directory


%% DOT NOT modify these cfg parameters 
cfg.debugRxFrameNbytes = 8256; % Expected nbr of received bytes that wake
                               % up client listener when operating mode is  
                               % setto 'debug'

cfg.hilDoaTyp    = 'cfloat';   % Received Doa type: - Default 'cfloat'
                               %  'sfloat' := Singlefloats; 'cfloat': Cfloats

cfg.hilPmNullTyp = 'cfloat';   % Received Pseudo-Spectrum type: - Default 'cfloat'
                               %  'sfloat' := Singlefloats; 'cfloat': Cfloats

cfg.hilOpMode    = 'mission';  % Mode of operation of HIL: - Default 'mission'
                               %  'mission' := normal mode of operation
                               %  'debug'   := debug mode
                               % in debug mode cfg.hilDoaTyp, .hilPmNullTyp
                               % are ignored.
% =========================================================================
end
