% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

% IP addr and Port for TCP/IP. 
% Author: Bachir Berkane, Fidus Systems

% Functions called :
%  - windows command ipconfig 

function [ip, port] = getIpAddr()

% Set port manually here; change port nbr if being used 
% Following Link for the official assignments of ports for specific uses 
% https://en.wikipedia.org/wiki/List_of_TCP_and_UDP_port_numbers
port = 8888;

% The following code extracts the IP address for Windows if testing the model with
% another matlab instance. Modify code if more than 1 IPv4 address.
% Otherwise get IP address from VCK190 and set Ip addr manulaly,and comment
% ip addr extraction code. %ip = '192.168.119.139';

ip = '192.168.119.87';


%%  IP addr. extraction code. This code tested only on windows
%   Modify code as instructed below if more than 1 IPv4 address
%   Comment this code when running with VCK190, and set IP manually.


% if ispc
%     [~, ipconfigLog]  = system('ipconfig');
%     expression   = '(IPv4 Address. . . . . . . . . . . : )(\d+.)(\d+.)(\d+.)(\d+.)';
%     ipExp        = regexp(ipconfigLog,expression,'tokens');
%     [~, m]       = size(ipExp);
%     if (m==1)
%         l=1;
%     else
%         l=1; % Change n in l=<n> to select the appropraite IP # if Pc has
%         % more than 1 IPv4 address. Run "system('ipconfig')" in matlab
%         % and set n to the desired IPv4 appearance count; that is if
%         % the desired IPv4 appears second, l=2.
%     end
%     ip = strcat(ipExp{l:l}{2}, ...
%         ipExp{l:l}{3}, ...
%         ipExp{l:l}{4}, ...
%         ipExp{l:l}{5}  );
% end
% 


end

        

