% ========================================================================
% Copyright (C) 2023 / 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Bachir Berkane, Fidus Systems


function  saveMusicMetrics(data, cfg)


%%         Log Structure             
% ------------------------------------------------------------------------
% 
% - Amtx
%      
% - QRD
%      - Rmtx
%      - Qmtx
%  - TBC




% Create 'testMusicData' directory if it does not exist 

if ~exist('testMusicData', 'dir')
       mkdir('testMusicData')
end

% add current dir to path
currDir=pwd;
addpath(genpath(currDir));

% Create unique Time-stamp directory (log data per run date/time)
dateString = datestr( datetime('now') );
testDir    = strrep(dateString,"-","");
testDir    = strrep(testDir,":","-");
testDir    = strrep(testDir," ","@");
cd testMusicData;  mkdir(testDir); 
cd(testDir)
writestruct(cfg,'testcfg.xml');

%% Amtx ==================================================================
dir = 'Amtx';
mkdir(dir); cd(dir)
fileName = 'Amtx.txt'; save2File(data.A, fileName, 'mtx');
fileName = 'Arow.txt'; save2File(data.A, fileName, 'row');
fileName = 'Acol.txt'; save2File(data.A, fileName, 'col');
cd .. % >> time-stamp dir

%% Gssr ==================================================================
dir = 'Gssr';
mkdir(dir); cd(dir)

% ->> dir = Gssr/Qmtx
dir = 'Qmtx';
mkdir(dir); cd(dir);
fileName = 'Qmtx.txt'; save2File(data.Q, fileName, 'mtx');
fileName = 'Qrow.txt'; save2File(data.Q, fileName, 'row');
fileName = 'Qcol.txt'; save2File(data.Q, fileName, 'col');
cd .. % >> Gssr

% ->> dir = Gssr/Rmtx : upper traingular matrix of roder nULa x nUla
dir = 'Rmtx';
mkdir(dir); cd(dir)
fileName = 'Rmtx.txt'; save2File(data.R, fileName, 'mtx');
fileName = 'Rrow.txt'; save2File(data.R, fileName, 'row');
fileName = 'Rcol.txt'; save2File(data.R, fileName, 'col');
cd .. % >> Gssr
cd ..  % >> time-stamp dir

%% SVD ==================================================================
dir = 'Svd';
mkdir(dir); cd(dir)

% ->> dir = Svd/S
dir = 'Smtx';  % singular values
mkdir(dir); cd(dir);
fileName = 'Smtx.txt'; save2File(data.S, fileName, 'mtx');
fileName = 'Srow.txt'; save2File(data.S, fileName, 'row');
fileName = 'Scol.txt'; save2File(data.S, fileName, 'col');
cd .. % >> Svd

% ->> dir = Svd/V
dir = 'Vmtx';  % 
mkdir(dir); cd(dir);
fileName = 'Vmtx.txt'; save2File(data.V, fileName, 'mtx');
fileName = 'Vrow.txt'; save2File(data.V, fileName, 'row');
fileName = 'Vcol.txt'; save2File(data.V, fileName, 'col')
cd .. % >> Svd

% ->> dir = Svd/U
dir = 'Umtx';  % 
mkdir(dir); cd(dir);
fileName = 'Umtx.txt'; save2File(data.U, fileName, 'mtx');
fileName = 'Urow.txt'; save2File(data.U, fileName, 'row');
fileName = 'Ucol.txt'; save2File(data.U, fileName, 'col')
cd .. % >> Svd


% ->> dir = Svd/NoiseSS
dir = 'NoiseSSmtx';  % 
mkdir(dir); cd(dir);
fileName = 'NoiseSSmtx.txt'; save2File(data.noiseSS, fileName, 'mtx');
fileName = 'NoiseSSrow.txt'; save2File(data.noiseSS, fileName, 'row');
fileName = 'NoiseSScol.txt'; save2File(data.noiseSS, fileName, 'col')
cd .. % >> Svd


% ->> dir = Svd/s
dir = 'sVec';  % 
mkdir(dir); cd(dir);
fileName = 'srow.txt'; save2File(data.s, fileName, 'row');
fileName = 'scol.txt'; save2File(data.s, fileName, 'col')
cd .. % >> Svd

% ->> dir = Svd/InterData
dir = 'IntermData';  %  
mkdir(dir); cd(dir);
 
for ii=cfg.svdPass2Log+1:cfg.nUla
    dir = strcat('pass', num2str(cfg.svdPass2Log), '_', num2str(ii));  
    mkdir(dir); cd(dir);
    W = data.intermData.(dir).W;
    fileName = 'Wmtx.txt'; save2File(W, fileName, 'mtx');
    fileName = 'Wrow.txt'; save2File(W, fileName, 'row');
    fileName = 'Wcol.txt'; save2File(W, fileName, 'col')

    V = data.intermData.(dir).V;
    fileName = 'Vmtx.txt'; save2File(V, fileName, 'mtx');
    fileName = 'Vrow.txt'; save2File(V, fileName, 'row');
    fileName = 'Vcol.txt'; save2File(V, fileName, 'col')

    Rot = data.intermData.(dir).Rot;
    fileName = 'Rotmtx.txt'; save2File(Rot, fileName, 'mtx');
    fileName = 'Rotrow.txt'; save2File(Rot, fileName, 'row');
    fileName = 'Rotcol.txt'; save2File(Rot, fileName, 'col')

    cd ..
end

cd .. % >>  Svd    
cd ..  % >> time-stamp dir

%% Spectrum ===============================================================
dir = 'Pm';
mkdir(dir); cd(dir)

% ->> dir = Pm/Pm_null
dir = 'Pm_null';  % --- 
mkdir(dir); cd(dir);
fileName = 'Pm_nullmtx.txt'; save2File(data.Pm_null, fileName, 'mtx');
fileName = 'Pm_nullrow.txt'; save2File(data.Pm_null, fileName, 'row');
fileName = 'Pm_nullcol.txt'; save2File(data.Pm_null, fileName, 'col')
cd .. % >> Pm

% ->> dir = Pm/Pm_peak
dir = 'Pm_peak';  % --- 
mkdir(dir); cd(dir);
fileName = 'Pm_peakmtx.txt'; save2File(data.Pm_peak, fileName, 'mtx');
fileName = 'Pm_peakrow.txt'; save2File(data.Pm_peak, fileName, 'row');
fileName = 'Pm_peakcol.txt'; save2File(data.Pm_peak, fileName, 'col')
cd .. % >> Pm

% ->> dir = Pm/steerMtx
dir = 'steerMtx';  % --- 
mkdir(dir); cd(dir);
fileName = 'steerMtx.txt'; save2File(data.steerMtx, fileName, 'mtx');
fileName = 'steerMtxrow.txt'; save2File(data.steerMtx, fileName, 'row');
fileName = 'steerMtxcol.txt'; save2File(data.steerMtx, fileName, 'col')
cd .. % >> Pm

% =========================================================================

fprintf(strcat('\n<strong>- Note: </strong>Sim Data saved under testMusicData', testDir));
fprintf('\n \n');
cd ../../..  % >> Root Directory


end