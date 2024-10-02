% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

% Configuration parameters for system model
% Author: Bachir Berkane, Fidus Systems


function readDebugDataFcn(src,  ~)

%% Add Paths
addpath  ./TcpClientListener/



%% Read frame when received and print msg on std output
fprintf('\n -- <strong>NOTE</strong>: Received a frame with %d  Bytes from the TCP server \n',  src.NumBytesAvailable);
rxFrame  = read(src,src.NumBytesAvailable/4,"single");
sNbr = rxFrame(2);
fprintf(' -- <strong>NOTE</strong>: Frame serial number = %d \n', sNbr);

sentFrame_fileName    = strcat("./Snapshots/sentSnapshots/header+snapshot_", num2str(sNbr), ".bin");
%% Compare to archived frame
if isfile(sentFrame_fileName)
    % Read doa reference file
    fileID      = fopen(sentFrame_fileName,'r');
    txFrame_mat  = fread(fileID, 'single');
    fclose(fileID);
    clear fileID;


    flag1 = (isnan(rxFrame(1)) == isnan(txFrame_mat(1)) );
    flag2 = isempty( find(rxFrame(2:end).' ~= txFrame_mat(2:end))); 
    okFlag = flag1*flag2;
    strNotOk = ' -- <strong>Error</strong>: The received Frame %d elements are different from the corresponding Transmitted frame\n';
    strOk = ' -- <strong>NOTE</strong>: Received Frame %d = Transmitted frame %d\n';
    if (okFlag==0)
        fprintf(strNotOk, sNbr);
    else
       fprintf(strOk, sNbr, sNbr);
    end
else
    fprintf(' -- <strong>Error</strong>: Sent frame reference file for snapshot #%d is missing\n', sNbr);

end




end

