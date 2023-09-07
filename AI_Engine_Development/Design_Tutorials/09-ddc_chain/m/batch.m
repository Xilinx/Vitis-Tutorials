%  
%  Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
%  SPDX-License-Identifier: MIT
%  
	
	%-------------------
	close all;
	clear all;
	%------------------
	fprintf(1, '--------------------------------\n');
	fprintf(1, '  Generate DDC v1.0 Test Vectors  \n');
    fprintf(1, '--------------------------------\n');
	
    %Parameters
	para.N_ite = 100;
	para.N_len = 512;
	para.N_ant = 32;

    % Generate coefficients and random NCO phase
    gen_tv_coef;
    
    for cfgid=0:1
        %0:5CLTE	1:100M NR
        para.cfgid =cfgid;
        fprintf(1, '   CfgID = %d  ...  ', cfgid);
        para.x = gen_tv_in(para);
        gen_tv_out(para);
        fprintf(1, 'Done!\n');
    end
	fprintf(1, '--------------------------------\n\n');
    