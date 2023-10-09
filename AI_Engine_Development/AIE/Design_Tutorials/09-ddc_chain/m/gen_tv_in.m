%  
%  Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
%  SPDX-License-Identifier: MIT
%  

 function x = gen_tv_in(para_in)
 
 
load tv_coe.mat;

%------------------------------------------
% define Carrier Configuration
%------------------------------------------
if ~exist('para_in', 'var')

	%0:5CLTE	1:100M NR
	cfgid =1;
	
	%Other parameters
	N_ite = 100;
	N_len = 512;
	N_ant = 4;
	
else

	cfgid = para_in.cfgid;
	N_ite = para_in.N_ite;
	N_len = para_in.N_len;
	N_ant = para_in.N_ant;

end


%------------------------------------------
% Generate random input data
%------------------------------------------
xbuff = randi([-2^15, 2^15-1], 2*N_len, N_ite*N_ant);


%------------------------------------------
% Generate Header
%------------------------------------------
hdr = zeros(16, 1); % real + imag
hdr(1) = cfgid;
hdr(3+2*[0:4]) = mod(nco.stps, 2^16);
hdr(4+2*[0:4]) = floor( (nco.stps - hdr(3+2*[0:4])) / 2^16);
% Convert Unsigned to Signed
idx = find(hdr>=2^15); hdr(idx)=hdr(idx)-2^16;


%------------------------------------------
% Print Out the Data
%------------------------------------------
dirname = sprintf('../data%d', cfgid);
if ~exist(dirname, 'dir')
    mkdir(dirname);
end

for antid=1:N_ant
    data=[kron(hdr, ones(1, N_ite)); xbuff(:, antid:N_ant:end)];
    data(2,1)=1;
	%--- open the file
	fid = fopen(sprintf('%s/ddc_in%d%d.txt', dirname, floor((antid-1)/8), mod(antid-1, 8)), 'wt');

	%--- pull together data and header
	fprintf(fid, '%d %d\n', data);
	
	%--- close file
	fclose(fid);

end


%------------------------------------------
% Save the data
%------------------------------------------
x = zeros(N_ant, N_ite*N_len);

for antid=1:N_ant
	a = complex(xbuff(1:2:end, antid:N_ant:end), xbuff(2:2:end, antid:N_ant:end));
    %a = reshape(a, 8, numel(a)/8);
    %a_even = a(:, 1:2:end);
    %a_odd  = a(:, 2:2:end);
	%x(antid, 1:2:end)  = reshape(a_even, 1, numel(a_even));
    %x(antid, 2:2:end)  = reshape(a_odd, 1, numel(a_odd));
	
	x(antid, :) = reshape(a, 1, numel(a));
end


% save the input random data to a mat file
save tv_in.mat x cfgid N_ite N_len N_ant;

