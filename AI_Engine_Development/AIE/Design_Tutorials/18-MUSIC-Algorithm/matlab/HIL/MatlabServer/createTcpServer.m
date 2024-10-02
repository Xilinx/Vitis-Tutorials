% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

% Script creates TCP server 
% Author: Bachir Berkane, Fidus Systems  
 

% Functions called :
%  - tcpserver    (Instrumentation-Ctrl Toolbox object)

%% Set IP, Port Number

function  [server] = createTcpServer()


addpath ../MatlabClient/TcpIp 

[ip, port] = getIpAddr; % Set IP manually in getIpAddr if connection to VCK190
timeout = 60*100;

%% Create a TCP/IP server
try
    server = tcpserver(ip, port);
catch

end

if exist('server','var')
    strg = strcat ("\n -- <strong>NOTE</strong>: TCP/IP Server is up and running on Address ", ip,  " and Port #%d \n");
    fprintf(strg, port);
    server.Timeout = timeout;
else
    strg = strcat ("\n -- <strong>ERROR</strong>: Could not create a TCP/IP Server on Address ", ip, " and Port #%d, Check IP address and the specified port availability \n");
    fprintf(strg, port);
end

end