% ========================================================================
% Copyright (C) 2023 / 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Bachir Berkane, Fidus Systems
%  .. simplied version from original model by Mark Rollins, AMD

function  [A, cfg]  = systemModel(cfg)
% ------------------------------------------------------------
% System Parameters
% ------------------------------------------------------------
nSource    = cfg.nSource;        % Number of sources 
doaRad     = cfg.doaDeg*pi/180;  % Sources DOA in degrees 
nUla       = cfg.nUla;           % Number of ULA elements
mSamp      = cfg.mSamp;          % Number of snapshots

% Generate random symbols for each source
switch cfg.sigMod
    case 1  % BPSK
        sourceSymbolMtx = round(rand(mSamp, nSource))*2-1;  
    case 2  % QPSK
        constellation   = [1+1i,1-1i,-1+1i,-1-1i];
        tmp             = randi([0,3],mSamp,nSource)+1;
        sourceSymbolMtx = constellation(tmp);
        if nSource==1, sourceSymbolMtx = sourceSymbolMtx.'; end
    otherwise
        disp('sigMod Option not Suported');
end


% Generate white noise
noiseVar = 10^(-cfg.Snr/20);
wn      = sqrt(noiseVar/2)*(randn(nUla,mSamp)+1i*randn(nUla,mSamp)); 
%wn = wgn(mSamp, 1, 0).'; % using the comm tool box. mSamp @ 0 dbW

% Steering matrix : array factor per source angle
cfg.angleRad   = doaRad;
steeringVecMtx = steeringVec(cfg);

% signal + noise
A = sourceSymbolMtx*steeringVecMtx.' + wn.'; 
A = single(A);
end
