
%% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
%  SPDX-License-Identifier: MIT
%

% Author: Bachir Berkane, Fidus Systems  

function sendMusicResults(server, pm, doa, h, rxFrameBytes, hilCfg)

 
switch hilCfg.hilOpMode

    case 'mission'
        PmUint8      = typecast(single(pm.'), 'uint8');
        doaEstUint8  = typecast(single(doa.'), 'uint8');
        HeaderUnint8 = typecast(single(h.'), 'uint8');
        txBytes      = [HeaderUnint8 PmUint8  doaEstUint8];
        % send Result to remote client
        fprintf(' -- <strong>Note</strong>: Writing Music Results (PseudoSpectrum, Doa Angles, and Headers = %d Bytes) to the remote TCP client \n', length(txBytes));
        write(server, txBytes);
    case 'debug'
        % send Result to remote client
        %rxFrameBytes(20:23) = typecast(single(13), 'uint8');
        fprintf(' -- <strong>Note</strong>: sending the received frames ( %d Bytes) back to the remote TCP client \n', length(rxFrameBytes));
        write(server, rxFrameBytes);
end




end