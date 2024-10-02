% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

% Author B. Berkane, Fidus Systems 
% Snapshot generation 

% Clean and adda paths
clear; clc;
restoredefaultpath
addpath Configuration SystemModel Snapshots
addpath Snapshots/GeneratedSnapshots Snapshots/SentSnapshots/
addpath MusicAlgo MusicAlgo/GSSR MusicAlgo/Commun MusicAlgo/NoiseSS  
addpath MusicAlgo/DOA  MiscScripts/

%% Get parameters
sysCfg     = systemCfg();
musiCfg    = musicCfg();
nSnapshots = sysCfg.nSnapshots;
doaEst     = NaN .* ones (sysCfg.nUla, nSnapshots);


%% Clean previous data
sDir = './Snapshots/GeneratedSnapshots';
if ~isfolder(sDir), mkdir(sDir); end
msg = 'Deleting previous snapshots';
cleanFiles (sDir, 'bin', msg)

sDir = './MusicResults/DoaMat';
if ~isfolder(sDir), mkdir(sDir); end
msg = 'Deleting previous DoaMat results';
cleanFiles (sDir, 'bin', msg);
 
sDir = './MusicResults/DoaRef';
if ~isfolder(sDir), mkdir(sDir); end
msg = 'Deleting previous DoaRef';
cleanFiles (sDir, 'bin', msg);

sDir = './MusicResults/PM';
if ~isfolder(sDir), mkdir(sDir); end
msg = 'Deleting previous Pm results';
cleanFiles (sDir, 'bin', msg);

sDir = './MusicResults/SourcePosRef';
if ~isfolder(sDir), mkdir(sDir); end
msg = 'Deleting previous SourcePosRef';
cleanFiles (sDir, 'bin', msg);

%% Generate Snapshots
for s=1:nSnapshots
    % Get the configuration parameters : Model and Music algorithm
    nUla    = sysCfg.nUla;
    mSamp   = sysCfg.mSamp;

    % Run system model to generate a snapshot matrix, then save  into file
    if s==1
        sysCfg = systemCfg();
    end
    sysCfg = sourceDoa(sysCfg, s);
    
    % Limitation of the implemenation on AIE 
    e = sum((sysCfg.doaDeg > 84.5) + (sysCfg.doaDeg <-84.5));
    
    error =0;
    if e > 0,  error =1; end


    switch error
        case 0

            [A, sysCfg]  = systemModel(sysCfg);
            fprintf("\n -- <strong>NOTE</strong>: Saving the Generated Snapshot #%d under ./Snapshots/GeneratedSnapshots  \n", s);
            doaAngles = ['          Reference DOA Angles (Degrees)        =  [', repmat('%g, ', 1, numel(sysCfg.doaDeg)-1), '%g]'];
            fprintf(doaAngles, sort(sysCfg.doaDeg));

            doaFileName = strcat("./MusicResults/DoaRef/doa_", num2str(s), ".bin");
            doaFileID   = fopen(doaFileName,'w');
            fwrite(doaFileID, sysCfg.doaDeg, 'float32');
            fclose(doaFileID);

            snapshotIQ = zeros(1, 2*nUla*mSamp);
            i = real(reshape(A, mSamp*nUla, []));
            q = imag(reshape(A, mSamp*nUla, []));
            snapshotIQ(1:2:end) = i;
            snapshotIQ(2:2:end) = q;

            fileName  = strcat("./Snapshots/GeneratedSnapshots/snapshot_", num2str(s), ".bin");
            fileID    = fopen(fileName,'w');
            fwrite(fileID, snapshotIQ, 'float32');
            fclose(fileID);

            % Run Music and save results
            data = music (A,  sysCfg, musiCfg);
            l    = numel(data.doaEst);
            pmNull        =  squeeze(data.Pm_null(1:end-2));
            doaEst(1:l,s) =  data.doaEst;

            pmFileName  = strcat("./MusicResults/PM/pmNull_", num2str(s), ".bin");
            pmFileID    = fopen(pmFileName,'w');
            fwrite(pmFileID, pmNull, 'float32');
            fclose(pmFileID);

            doaFileName = strcat("./MusicResults/DoaMat/doa_", num2str(s), ".bin");
            doaFileID   = fopen(doaFileName,'w');
            fwrite(doaFileID, doaEst(1:l,s), 'float32');
            fclose(doaFileID);

            sourcePosFileName = strcat("./MusicResults/SourcePosRef/pos_", num2str(s), ".txt");
            writematrix(sysCfg.sourcePos, sourcePosFileName);



        case 1
            fprintf('\nSnapshot #%d is not generated as 1 or more sources is located out of the allowed coverage zone of [-84.35, 84.35] degree \n ', s);
    end
end





