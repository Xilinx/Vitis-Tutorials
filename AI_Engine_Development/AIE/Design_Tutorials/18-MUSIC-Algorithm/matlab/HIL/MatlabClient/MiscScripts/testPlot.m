% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Bachir Berkane, Fidus Systems

clear; clc;
restoredefaultpath
addpath Configuration TcpClientListener
close;

sysCfg     = systemCfg();
musiCfg    = musicCfg();

f= figure('Name','MUSIC Simulation Plots','NumberTitle','off'); % if f is not an exsting figure it creates it (and steal the focus)
f.Position = musiCfg.figurePos;
y = 0:0.05:1.05; x = -1:0.1:1.1; plot(x,y,'w');
hold on
for sNbr =1:sysCfg.nSnapshots

% Read ref doa-vector of Snapshot #sNbr
doa_fileName    = strcat("./MusicResults/DoaRef/doa_", num2str(sNbr), ".bin");
if isfile(doa_fileName)
    % Read doa reference file
    fileID   = fopen(doa_fileName,'r');
    doa_ref  = fread(fileID, 'single');
    fclose(fileID);
    clear fileID;
else
    fprintf(' -- <strong>Error</strong>: doa reference file for snapshot #%d is missing\n', sNbr);

end

posFileName = strcat("./MusicResults/SourcePosRef/pos_", num2str(sNbr), ".txt");
if isfile(posFileName)
    % Read ref positions
    posRef = readmatrix(posFileName);
else
    fprintf(' -- <strong>Error</strong>: doa reference file for snapshot #%d is missing\n', sNbr);

end

%% print DOA Reference, MATLAB-EStimated DOA, AIE-Estimated DOA
doaAngles = ['\n          >> Reference DOA        Angles (Degrees) = [', repmat('%.3f, ', 1, numel(doa_ref)-1), '%.3f]\n'];
fprintf(doaAngles,  doa_ref);

pltSources(doa_ref, posRef);

 end