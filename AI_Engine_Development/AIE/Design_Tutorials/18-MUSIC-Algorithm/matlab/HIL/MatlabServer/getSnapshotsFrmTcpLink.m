%% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
%  SPDX-License-Identifier: MIT
%

% Function extract header and snapshots's payload from TCP/IP link
% Author: Bachir Berkane, Fidus Systems  
 

% Functions called :
%  - tcpserver    (Instrumentation-Ctrl Toolbox object)


%% Protocol Errors
%  - Error  1:  Number of Bytes Received = 0  
%  - Error  2:  No TCP Server Up and Running
%  - Error  3:  Received Nbr of Bytes is not Consistent with the Frame Length   
%  - Error  41: Start of Frame Identifier is NOT NaN  
%  - Error  42: Frame # Serial Number is out of Sequence   
%  - Error  43: 1st Frame Position-Identifier is NOT CORRECT
%  - Error  44: Last Frame Position-Identifier is NOT CORRECT   
%  - Error  45: Middle Frame # Position-Identifier is NOT CORRECT  


function [snapshots, h, nFrames, rxBytes, errCode] = getSnapshotsFrmTcpLink (sysCfg, server) 

snapshots=NaN;

% TCP/IP related variables
tcpHeaderNbytes = 64;
frmLen          = tcpHeaderNbytes + sysCfg.nUla*sysCfg.mSamp*2*4; % nUla*mSamp cfloats

%% Read received frames
if exist('server','var')
    nBytesRcd   = server.NumBytesAvailable;
    nFrames     = server.NumBytesAvailable/frmLen;
    fprintf('\n -- <strong>Note</strong>: Received %d Snapshots (%d bytes) from remote client \n', nFrames, nBytesRcd);
    if  (nBytesRcd == 0)
        errCode = 1;
    else
        rxBytes   = read(server, server.NumBytesAvailable);
        if (mod(rxBytes, nFrames) ~= 0)
            errCode = 3;
        else
            errCode = 0;
        end
    end

else
        errCode = 2;
end

h = zeros(16,nFrames);
% Extract hearder and payload  
switch (errCode)
    case 1
        fprintf('\n -- <strong>Error %d</strong>: Number of Bytes Received = 0 \n', errCode);
    case 2
        fprintf('\n -- <strong>Error %d</strong>: No TCP Server Up and Running \n', errCode);
    case 3
        fprintf('\n -- <strong>Error %d</strong>: Received nbr of bytes is not consistent with the frame length \n', errCode);
    case 0 % 
        snapshots = zeros(sysCfg.mSamp, sysCfg.nUla, nFrames);
        for frm=1:nFrames
      
            frame         = uint8(rxBytes(1+(frmLen)*(frm-1):frmLen*frm));
            uint8Header   = uint8(frame(1:tcpHeaderNbytes));
            
            % Extract header ----------------------------------------------
            header = typecast(uint8Header, "single");
            if ~isnan(header(1))
                errCode = 41; 
                fprintf('\n -- <strong>Error %d</strong>: Start Frame #%d Identifier is NOT NaN\n', errCode, frm);
                break
            end
            
            % if ~(header(2) == frm)
            %     errCode = 42;
            %     fprintf('\n -- <strong>Error %d</strong>: frame %d serial number is out of sequence \n', errCode, frm);
            %     break
            % end
            % 
            % if  (frm ==1) &&  (header(3) ~= 5) && (header(3) ~= 1)  
            %     errCode = 43;
            %     fprintf('\n -- <strong>Error %d</strong>: 1st frame position identifier is NOT CORRECT \n', errCode);
            %     break
            % end
            % 
            % if (frm ==nFrames) && (header(3) ~= 4)
            %     errCode = 44;
            %     fprintf('\n -- <strong>Error %d</strong>: last frame position identifier is NOT CORRECT \n', errCode);
            %     break
            % end
            % 
            % if ( (frm < nFrames) && (frm > 1) ) && (header(3) ~= 2)
            %     errCode = 45;
            %     fprintf('\n -- <strong>Error %d</strong>: middle frame #%d position identifier is NOT CORRECT \n', errCode, frm);
            %     break
            % end

            h(:, frm) = header.';       
            %---------------------------
            % nSource        = h(4,frm);
            % nUla           = h(5.frm);
            % dUla           = h(6.frm);
            % mSamp          = h(7,frm);
            % noiseSsCutmeth = h(8,frm);
            % noiseFloor     = h(9,frm);
            % nullThreshold  = h(10,frm);
            % ---------------------------
            % -------------------------------------------------------------
            % Extract sanapshot payload -----------------------------------
            frmBytes      = frame(tcpHeaderNbytes+1:end);
            uint8Payload  = reshape(frmBytes, 4,[]).';
            [m, ~] = size(uint8Payload);        
           
            payload   = single(zeros(1, h(7, frm)*h(5, frm)*2));
            for j=1:m
                % typecast payload to single type
                payload(j) =(typecast(uint8(uint8Payload(j,:)), 'single'));
            end
 
            % -------------------------------------------------------------
            l            = length(payload);
            tmp          = zeros(8, l/8);
            bluePayload  = reshape(payload(1:     l/2), 8, []);
            greenPayload = reshape(payload(1+l/2: end), 8, []);

            tmp(:,1:2:end) = bluePayload; tmp(:,2:2:end) = greenPayload;
            tmp        = reshape(tmp, [],1);
            iSnapshot  = tmp(1:2:end);
            qSnapshot  = tmp(2:2:end);
            
            snapshotCplx = iSnapshot + 1j* qSnapshot;
            snapshots(:,:,frm) = reshape(snapshotCplx, h(7, frm),h(5, frm), []);
            % -------------------------------------------------------------
        end

    otherwise
        fprintf('\n -- <strong>Error %d</strong>: Unknown Error Code \n', errCode);
end


