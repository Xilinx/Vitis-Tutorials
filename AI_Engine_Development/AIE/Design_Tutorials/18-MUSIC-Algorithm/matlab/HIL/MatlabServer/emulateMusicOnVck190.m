% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT


% Script emulate AIE execution of MUSIC and TCP server operations
% Author: Bachir Berkane, Fidus Systems


%% Misc
clc
addpath Music  Music/GSSR Music/Commun Music/DOA  
addpath Music/NoiseSS ../MatlabClient/Configuration/
clear sysCfg hilCfg
nValid  = 3;
sysCfg  = systemCfg();
hilCfg  = hilCfg();
DoaTyp    = hilCfg.hilDoaTyp;
PmNullTyp = hilCfg.hilPmNullTyp;

strError = '\n -- <strong>Error</strong>: ';
strNote  = '\n -- <strong>Note</strong>: ';

%% chcek if server is up and running
if exist('server','var')


    %% Get Snapshots and Headers from TCP/IP Server Link
    [snapshots, h, nFrames, rxBytes, errCode] = ...
                                  getSnapshotsFrmTcpLink (sysCfg, server);
    rxFrames=reshape(rxBytes, [], nFrames);


    %% --------- Run Music and send results to remote Client --------------
    switch (errCode==0)
        case true
            % Assumed all snapshots have the same system header h(4:10,i),
            % for all i.
            sysCfg = musicCfg();
            sysCfg.nSource        = h(4,1);
            sysCfg.nUla           = h(5,1);
            sysCfg.dUla           = h(6,1);
            sysCfg.mSamp          = h(7,1);
            sysCfg.noiseSsCutmeth = h(8,1);
            sysCfg.noiseFloor     = h(9,1);
            sysCfg.nullThreshold  = h(10,1);

            pmNull     = single(zeros (sysCfg.musicSweepNPts, nFrames));
            doaEstIdx  = 0 .* ones (sysCfg.nUla, nFrames);
            pauseTime  = 0.0;
            for s =1:nFrames
                % Run Music -----------------------------------------------
                noteMsg = 'running Music Algorithm on the Rcvd Snapshot #%d';
                fprintf(strcat(strNote,noteMsg), s);
                data               = music (snapshots(:,:,s), sysCfg);
                l                  = numel(data.doaEstIdx);
                pmNull(:, s)       = squeeze(data.Pm_null(1:end-2));
                doaEstIdx(1:l,s)   = data.doaEstIdx;

                pmNull = squeeze( pmNull(:, s));
                doaIdx = squeeze( doaEstIdx(:,s));
                idx    = max(find(doaIdx > 0 ));
                doaIdx = doaIdx (1: idx);

                % ---------------------------------------------------------
                % Emulate pmNull and Doa AIE generation with pmNull and 
                % doa in cfloats.
                % Doa is returned as a 32 values vector with each value
                % represents an interval of 8pts indexes in spectrum
                % Indexing starts from 0
                doaAieSingle = nValid .* ones(1, 32);
                doaPos       = floor(doaIdx/8)+1;
                doaAieSingle(doaPos) = doaIdx-1;

                [m, ~] = size( doaAieSingle.');
                switch DoaTyp
                    case 'sfloat'
                        doaAie = single(doaAieSingle);
                    case 'cfloat'
                        doaAie = zeros(1, 2*m).';
                        doaAie(1:2:end) = doaAieSingle;
                end

                [m, ~] = size( pmNull);
                switch PmNullTyp
                    case 'sfloat'
                        pmNullAie = single(pmNull);
                    case 'cfloat'
                        pmNullAie = single(zeros(1, 2*m).');
                        pmNullAie(1:2:end) = single(pmNull);
                end
                %  --------------------------------------------------------
                % Send results to remote client
                h(11,s)    = single(960.51)-50*rand;    % sweep interval in ns
                header     = squeeze (h(:,s));
    
                frameBytes = squeeze( rxFrames(:, s));
                sendMusicResults(server, pmNullAie, doaAie, header, ...
                                 frameBytes, hilCfg);

                pause(pauseTime);
            end

        case false
            errMsg = " Unable to run Music. Check Error code \n";
            fprintf(strcat(strError, errMsg), errCode);
    end

else

    errMsg = "Server is down \n";
    fprintf(strcat(strError, errMsg));
end
% -------------------------------------------------------------------------

