% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

% Reads generated snapshots, formats data and sends to remote TCP/IP server
% Author: Bachir Berkane, Fidus Systems

% Functions called :
%  - tcpclient    (Instrumentation-Ctrl Toolbox object)
%  - Configuration/systemCfg
%  - Configuration/musicCfg
%  - TcpIp/getIpAddr


clear; clc;
restoredefaultpath;
close;

addpath Configuration SystemModel Snapshots/GeneratedSnapshots TcpIp
addpath TcpClientListener  
sysCfg  = systemCfg();
musiCfg = musicCfg();
hilCfg  = hilCfg();

%% Set Your IP Address in getIpAddr.m when interacting with VCK190.
[ip, port]   = getIpAddr;

% Define commun TCP/IP Payload headers according to the protocol defined in
% the TcpIp/protocolDef.txt  
h1          = NaN; %  Pkt Identifer ... NaN := 'ffc00000' 
h4          = sysCfg.nSource;
h5          = sysCfg.nUla;
h6          = sysCfg.dUla;
h7          = sysCfg.mSamp;


h8    = single(musiCfg.noiseSsCutmeth);
h9    = single(musiCfg.noiseFloor);
h10   = single(musiCfg.nullThreshold);

Reserved = 0;   % Header h10 through h16 rae reserved with values set to
                % the value assigned to Reserved
musicSweepNPts = musiCfg.musicSweepNPts;
% TCP/IP related frame variables
tcpHeaderNbytes = 64;
tcpTxNbytes     = tcpHeaderNbytes + sysCfg.nUla*sysCfg.mSamp*2*4;  % nUla*mSamp cfloats

pmNullNBytes    = musiCfg.musicSweepNPts*4;
%% AIE returns cfloats -- This may be solved later in project
%  Configure hilPmNullTyp in hiCfg configuration file
if (hilCfg.hilPmNullTyp == 'cfloat')
    pmNullNBytes = pmNullNBytes*2;
end
hilCfg.pmNullNBytes = pmNullNBytes;

doaNBytes       = 32*4;
%% AIE returns cfloats -- This may be solved later in project
%  Configure hilDoaTyp in hiCfg configuration file 
if (hilCfg.hilDoaTyp == 'cfloat')
    doaNBytes = doaNBytes*2;
end
hilCfg.doaNBytes = doaNBytes;

 
%%  Create tcp client iff it is not already up and running
if exist('client','var')
    tcpConnExit = true;
else
    tcpConnExit = false;
    try
        client = tcpclient(ip,port);
    catch
    end
    % if the server is not set TCP/IP client will be created.
    if ~exist('client','var')
        error = '\n -- <strong>Error</strong>: Cannot create a communication link with the remote server. \n --        Please check IP addr and port, and make sure the server is running. \n';
        fprintf(error);
    end
end


%% Read the generated snapshots

genSnapshotsDir = './Snapshots/GeneratedSnapshots';
archSnaphotsDir = strcat('./Snapshots/Archive/', hilCfg.demoDataSet,'/Snapshots');

switch hilCfg.demoData
    case 'archived'
        sDirPath   = archSnaphotsDir;
        sDirStruct = dir([archSnaphotsDir '/*.bin']);
        nSnapshots = size(sDirStruct,1);
    case 'generated'
        sDirPath   = genSnapshotsDir;
        sDirStruct = dir([genSnapshotsDir '/*.bin']);
        nSnapshots = sysCfg.nSnapshots;
end
sysCfg.nSnapshots = nSnapshots;

tcpRxNbytes  = nSnapshots*(tcpHeaderNbytes+pmNullNBytes+doaNBytes);

snapshotBlueGreenConcat = single(zeros(2*sysCfg.mSamp*sysCfg.nUla, nSnapshots));

for s=1: nSnapshots

    %% Call ReadDataFCn Function every time TcpRxBytes are received
    % Client is ready and reads every time TcpRxNbytes are received and
    % plots Music Detection Results

    
 
    % Read Snapshot #s
    fileName     = strcat(sDirPath, '/snapshot_', num2str(s), ".bin");
    fileNamExist = false;
    if isfile(fileName)
        fileNamExist = true;
    end


    if (fileNamExist == true) && exist('client','var')
        fileNamExist = false;
        % Read generated snapshot # s
        fileID           = fopen(fileName,'r');
        % snapshotIQ(:,s)  = fread(fileID, 'single');
        snapshotIQ   = fread(fileID, 'single');
        fclose(fileID);
        clear fileNameExist fileID;

        % Format data into 2 streams to ease Line app implmentation on VC190
        tmp           = reshape(snapshotIQ,8, []);
        toBlueStream  = reshape (tmp(:,1:2:end), [], 1);
        toGreenStream = reshape (tmp(:,2:2:end), [], 1);
        snapshotBlueGreenConcat(:,s) = [toBlueStream; toGreenStream];


        % Define and build payload header
        h2   = s;   %  Snapshot serial number .. monotone from 1 to number of snapshots
        if (nSnapshots==1)
            h3 = 5;  % unique snapshot
        elseif (s==1)
            h3 = 1;  % 1st snapshot
        elseif (s==nSnapshots)
            h3 = 4;  % last snapshot
        else
            h3 = 2;  % middle snapshot
        end
        header = single([h1 h2 h3 h4 h5 h6 h7 h8 h9 h10 Reserved .* ones(1, 6)]);

        % Form packet to send to TCP/IP server
        to_tcpServer = [header snapshotBlueGreenConcat(:,s).'];

        %  Send to TCP/IP server using the Instrumentation-Ctrl toolbox
        %  object write function
        fprintf("\n -- <strong>NOTE</strong>: Writing Snapshot #%d (%d bytes) to the remote TCP/IP Server \n", s, tcpTxNbytes);

        write(client, to_tcpServer);

        % save payload+header into a binary format
        fileName  = strcat("./Snapshots/SentSnapshots/header+snapshot_", num2str(s), ".bin");
        fileID    = fopen(fileName,'w');
        fwrite(fileID, to_tcpServer, 'float32');
        fclose(fileID);

    else
        if ~exist('fileNamExist', 'var')
            warn = strcat ("\n -- <strong>ERROR</strong>: Sanpshot file ", fileName, " does not exits, Run genSnapshots to generate snapshots \n");
            fprintf (warn);
        end

        break
    end
end

if exist('client','var')
    strg = strcat('\n -- <strong>NOTE</strong>: Total bytes written to the remote TCP/IP Server = %d','B\n');
    fprintf(strg, s*tcpTxNbytes);

    strg = strcat('\n -- <strong>NOTE</strong>: TCP/IP Client Enters Listening Mode \n');
    fprintf(strg);
end

% % Start a figure window @ the configured position
if (strcmp(hilCfg.hilOpMode ,'mission') && exist('client','var'))
    f= figure('Name','MUSIC Simulation Plots','NumberTitle','off'); % if f is not an exsting figure it creates it (and steal the focus)
    f.Position = musiCfg.figurePos;
end

setGlobalVar(tcpHeaderNbytes, sysCfg, musiCfg, hilCfg);

if exist('client','var')
    switch hilCfg.hilOpMode

        case 'mission'

            % Configure call back to trigger on event of tcpRxBytes reception
            configureCallback(client,"byte",tcpRxNbytes, @readPlotDataFcn);


        case 'debug'
            % Configure call back to trigger on event of tcpRxBytes reception
            configureCallback(client,"byte",hilCfg.debugRxFrameNbytes, @readDebugDataFcn);
    end


end