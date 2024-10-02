%% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

% TCP listener .. wake-up upon frame receiption, plot and compare to
% reference
% Author: Bachir Berkane, Fidus Systems

function readPlotDataFcn2(src,  ~)


%% Add Paths
addpath  ./TcpClientListener/ ./MiscScripts/

% get Global variables
[tcpHeaderNbytes, sysCfg, musiCfg, hilCfg] = getGlobalVar();

pmNullNBytes = hilCfg.pmNullNBytes;
doaNBytes = hilCfg.doaNBytes;
%% Read a frame when received and print msg on std output
fprintf('\n -- <strong>NOTE</strong>: Received a frame with %d  Bytes from the TCP server \n',  src.NumBytesAvailable);
rxFrames  = read(src,src.NumBytesAvailable/4,"single");
frameNbrFloats = (pmNullNBytes+doaNBytes+tcpHeaderNbytes)/4;
nSnapshots = sysCfg.nSnapshots;
frames = reshape(rxFrames, [], nSnapshots);

% Guarding against waking up too late with more snapshots in buffer due
% slow plotting of previous batch ! 
if ((length(rxFrames)/frameNbrFloats ==  nSnapshots) || ~isempty(rxFrames) )
    for frm=1:nSnapshots

        if ~isempty(frm) % redundant ... but leaving it! 

            sNbr = frames(2,frm);
            sweepTime = frames(11, frm)-288/sysCfg.nSnapshots;

            fprintf(' -- <strong>NOTE</strong>: Frame serial number = %d \n', sNbr);


            % Extract MuSiC doa Vector and pseudo spectrum from the received frame
            hLen          = tcpHeaderNbytes/4;
            pmBoundary    = hLen+pmNullNBytes/4;
            aieBoundary   = pmBoundary + 64;
            pmNullAie     = frames(1+hLen:pmBoundary, frm);

            % header      = frames(1:hLen);
            if strcmp(hilCfg.hilPmNullTyp, 'cfloat')
                pmNullAie  = pmNullAie(1:2:end);
            end
            % aha! ... if rxFrames is longer than expected we r is trouble 
            % frames(pmBoundary+1:end, frm) --> frames(pmBoundary+1:aieBoundary, frm)
            doaAie       =   frames(pmBoundary+1:aieBoundary, frm); 
            if strcmp(hilCfg.hilDoaTyp, 'cfloat')
                doaAie  = doaAie(1:2:end);
            end


            % -- Save PM and DOA
            if (hilCfg.capRxDat == true)

                dDir = './Debug/';
                doa_fileName    = strcat(dDir, 'AieDoa/aiedoa_', num2str(sNbr), ".txt");
                pm_fileName    = strcat(dDir, 'AiePm/aiepm_', num2str(sNbr), ".txt");

                save2File (doaAie, doa_fileName, 'col')
                save2File (pmNullAie, pm_fileName, 'col')
            end
            % -------------------------------------------------------------------------


            %% Read Reference files for DOA and PmNull
            % Get Folder location

            switch hilCfg.demoData
                case 'generated'
                    rDir = './MusicResults/';
                case 'archived'
                    rDir = strcat('./Snapshots/Archive/', hilCfg.demoDataSet, '/MusicResults/');
            end


            % Read matlab doa-vector of Snapshot #sNbr
            doa_fileName    = strcat(rDir, '/DoaMat/doa_', num2str(sNbr), ".bin");
            if isfile(doa_fileName)
                % Read doa reference file
                fileID   = fopen(doa_fileName,'r');
                doaMatDeg  = fread(fileID, 'single');
                fclose(fileID);
                clear fileID;

                % Get rid of NaNs in doa received vector
                l   = sum(~isnan(doaMatDeg));
                doaMatDeg = doaMatDeg(1:l);
            else
                fprintf(' -- <strong>Error</strong>: doa reference file for snapshot #%d is missing\n', sNbr);

            end

            % Read ref doa-vector of Snapshot #sNbr
            doa_fileName    = strcat(rDir, '/DoaRef/doa_', num2str(sNbr), ".bin");
            if isfile(doa_fileName)
                % Read doa reference file
                fileID   = fopen(doa_fileName,'r');
                doa_ref  = fread(fileID, 'single');
                fclose(fileID);
                clear fileID;

                % Get rid of NaNs in doa received vector
                l   = sum(~isnan(doa_ref));
                doa_ref = sort(doa_ref(1:l));
            else
                fprintf(' -- <strong>Error</strong>: doa reference file for snapshot #%d is missing\n', sNbr);

            end

            % Read matlab-calculated pseudo spectrum of Snapshot #sNbr
            pmNull_fileName    = strcat(rDir, '/PM/pmNull_', num2str(sNbr), ".bin");
            if isfile(pmNull_fileName)
                % Read doa reference file
                fileID      = fopen(pmNull_fileName,'r');
                pmNull_mat  = fread(fileID, 'single');
                fclose(fileID);
                clear fileID;

            else
                fprintf(' -- <strong>Error</strong>: PseudoSpectrum reference file for snapshot #%d is missing\n', sNbr);

            end

            posFileName = strcat(rDir, '/SourcePosRef/pos_', num2str(sNbr), ".txt");
            if isfile(posFileName)
                % Read ref positions
                posRef = readmatrix(posFileName);
            else
                fprintf(' -- <strong>Error</strong>: doa reference file for snapshot #%d is missing\n', sNbr);

            end


            %% print DOA Reference, MATLAB-EStimated DOA, AIE-Estimated DOA
            doaAngles = ['\n          >> Reference DOA        Angles (Degrees) = [', repmat('%.3f, ', 1, numel(doa_ref)-1), '%.3f]\n'];
            fprintf(doaAngles,  doa_ref);

            doaAngles = ['\n          >> MATLAB-Estimated DOA Angles (Degrees) = [', repmat('%.3f, ', 1, numel(doaMatDeg)-1), '%.3f]\n'];
            fprintf(doaAngles,  doaMatDeg);

            doaAieIdx = doaAie(find(doaAie > 3))+1;
            sweepSpan = musiCfg.musicSweepSpan;
            SweepNPts = musiCfg.musicSweepNPts;
            doaAieDeg = single (- sweepSpan/2 + (doaAieIdx -1) * sweepSpan/(SweepNPts-1) );
            doaAngles = ['\n          >> AIE-Estimated DOA    Angles (Degrees) = [', repmat('%.3f, ', 1, numel(doaAieDeg)-1), '%.3f]\n'];
            fprintf(doaAngles,  doaAieDeg);


            %% Plots
            sysCfg = sourceDoa(sysCfg, 1);
            pltSources(pmNullAie, doaAieDeg, doaMatDeg, posRef, musiCfg, sysCfg, hilCfg, sweepTime, sNbr);
            pause(0.5);

        end
    end

end
