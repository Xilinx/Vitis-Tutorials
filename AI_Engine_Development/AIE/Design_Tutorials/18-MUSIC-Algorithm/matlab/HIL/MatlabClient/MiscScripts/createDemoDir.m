% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

% Create an archived demo directory by copying teh generated snapshota and
% corresponding results into a demo directory with name as indicated in
% the function argument.

% Author B. Berkane, Fidus Systems 


function createDemoDir(demoDir)
clc
% clear

cd Snapshots;
[~,~,~] = mkdir('Archive');
cd Archive;
%cd Snapshots/Archive;

mkdir(demoDir); cd(demoDir);
mkdir('Snapshots'); mkdir('MusicResults');

% copy generated files into Snaphots
copyfile ../../GeneratedSnapshots/snapshot_* Snapshots

% copy Music reference Results
cd ('MusicResults');
mkdir ('DoaMat'); mkdir ('DoaRef');  mkdir ('PM'); mkdir ('SourcePosRef');

copyfile  ../../../../MusicResults/DoaMat/doa_*  DoaMat;
copyfile  ../../../../MusicResults/DoaRef/doa_*  DoaRef;
copyfile  ../../../../MusicResults/PM/pmNull_*   PM;
copyfile  ../../../../MusicResults/SourcePosRef/pos_*   SourcePosRef;

cd ../../../../

end
