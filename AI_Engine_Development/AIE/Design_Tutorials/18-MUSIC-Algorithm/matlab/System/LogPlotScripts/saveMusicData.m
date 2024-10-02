% ========================================================================
% Copyright (C) 2023 / 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Bachir Berkane, Fidus Systems

function  saveMusicData(data, cfg)


%%         Log Structure             
% ------------------------------------------------------------------------
% -data
%   -S<n>, n=0, ...,7
%       -  cfg
%           - testCfg.txt
%           - testCfg.xml
%       -  qrd
%           - input_A.txt
%           - output_R.txt
%           - output_Q.txt
%       - svd
%           - input_R.txt
%           - output_S.txt .. Normalized S
%           - output_s.tx  .. diagonal of S
%           - output_S1.txt .. Non Normalized S
%           - output_s1.tx  .. diagonal of S1
%           - output_V.txt
%           - output_U.txt
%           - output_Nss.txt
%
%       - doa
%           - input_Nss.txt      .. Noise SS
%           - input_steerMtx     .. steering Matrix  (for reference)
%           - output_pmNull.txt  .. Psudo Spectrum Nulls
%           - output_doaDeg.txt  .. doa in Deg
%           - output_doaIdx.txt  .. doa in Idx ranges from 1 to 255 

% Create 'data' directory if it does not exist 
dataDir = 'data';
if ~isfolder(dataDir)
       mkdir(dataDir)
end
cd(dataDir)

% add current dir to path
% currDir=pwd;
% addpath(genpath(currDir));



%% S<n>: Create Parent directory S<n> if it does not exist 
parentDir = strcat('S', num2str(cfg.nSource));

if ~isfolder(parentDir)
       mkdir(parentDir)
end
cd(parentDir)


%% S<>.cfg: Create S<n>/cfg dirctory if it does not exist 
%  Write configuration files into cfg folder
cfgDir = 'cfg';
if ~isfolder(cfgDir)
       mkdir(cfgDir)
end
cd(cfgDir)
writestruct(cfg,'testcfg.xml');
copyfile ../../../Configuration/testCfg.m .
cd .. % >> S<n> Parent directory dir 
% ------------------------------------------------------------------------

%% S<>.qrd: Create S<n>/qrd dirctory if it does not exist
fileName = 'music_input_A.txt';  save2File(data.A, fileName,  'col');
qrdDir = 'qrd';
if ~isfolder(qrdDir)
       mkdir(qrdDir)
end
cd(qrdDir)
fileName = 'input_A.txt';  save2File(data.A, fileName,  'col');
fileName = 'output_Q.txt'; save2File(data.Q, fileName,  'col');
fileName = 'output_R.txt'; save2File(data.R, fileName,  'col');
cd .. % >> S<n> Parent directory dir 
% ------------------------------------------------------------------------

%% S<>.svd: Create S<n>/svd dirctory if it does not exist 
svdDir = 'svd';
if ~isfolder(svdDir)
       mkdir(svdDir)
end
cd(svdDir)
fileName = 'input_R.txt';    save2File(data.R,        fileName, 'col');
fileName = 'output_S.txt';   save2File(data.S,    fileName, 'col');
fileName = 'output_scaledSdiag.txt';  save2File(data.scaledSdiag,fileName, 'col');
fileName = 'output_Sdiag.txt';  save2File(data.Sdiag,fileName, 'col');
fileName = 'output_V.txt';   save2File(data.V,       fileName, 'col');
fileName = 'output_U.txt';   save2File(data.U,       fileName, 'col');
fileName = 'output_Nss.txt'; save2File(data.noiseSS, fileName, 'col');
cd .. % >> S<n> Parent directory dir 
% ------------------------------------------------------------------------


%% S<>.doa: Create S<n>/doa dirctory if it does not exist 
fileName = 'music_output_pmNull.txt';  save2File(data.Pm_null,   fileName, 'col');
fileName = 'music_output_doaIdx.txt';  save2File(data.doaEstIdx, fileName, 'col');
doaDir = 'doa';
if ~isfolder(doaDir)
       mkdir(doaDir)
end
cd(doaDir)
fileName = 'input_Nss.txt';      save2File(data.noiseSS,   fileName, 'col');
fileName = 'input_steerMtx.txt'; save2File(data.steerMtx,  fileName, 'col');
fileName = 'output_pmNull.txt';  save2File(data.Pm_null,   fileName, 'col');
fileName = 'output_doaDeg.txt';  save2File(data.doaEstDeg, fileName, 'col');
fileName = 'output_doaIdx.txt';  save2File(data.doaEstIdx, fileName, 'col');
cd .. % >> S<n> Parent directory dir 
% ------------------------------------------------------------------------

fprintf(strcat('\n<strong>- Note: </strong>Sim Data saved under the ./',  dataDir,  ' folder'));
fprintf('\n \n');
cd ../../  % >> Root Directory


end