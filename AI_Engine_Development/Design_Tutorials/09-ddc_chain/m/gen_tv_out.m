%  
%  Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
%  SPDX-License-Identifier: MIT
%  	
	
function gen_tv_out(para_in)

load tv_coe.mat;

if ~exist('para_in', 'var')
    load tv_in.mat;
else
    x = para_in.x;
    cfgid = para_in.cfgid;
    N_ite = para_in.N_ite;
    N_len = para_in.N_len;
    N_ant = para_in.N_ant;
end

% derive data from N_len
len_245p76 =N_len;
len_122p88 =len_245p76/2;
len_61p44  =len_122p88/2;
len_30p72  =len_61p44/2;


%----------------------------
% Loop for the antennas
%----------------------------
for antid=1:N_ant

    % Translate Config ID to Num Channels
    switch cfgid
        case 0
            N_NR0_ch = 0;
            N_NR1_ch = 0;
            N_LTE_ch =5;
            N_ch = 5;
        case 1
            N_NR0_ch = 0;
            N_NR1_ch = 1;
            N_LTE_ch =0;
            N_ch = 1;
    end    
    
    
	%load data source
	data_ch1=x(antid,:);


	LTE_ST_cnt=1;
	has_lte=1;

	if N_NR1_ch==1
		source{1}.name     = 'NR';
		source{1}.data1_245p76 = data_ch1;
		source{1}.data1_245p76 = filter(coe{3}.coef,1,source{1}.data1_245p76);
		source{1}.data1_122p88 = srs(source{1}.data1_245p76(1:2:end) , coe{3}.shft, 15);

		source{1}.demixer1     = source{1}.data1_122p88.*me_dds_v3(nco.stps(1), len_122p88*N_ite, 16, 0);
		source{1}.demixer1     = srs(source{1}.demixer1,nco.shft,15);
		
		source{1}.data1        = filter(coe{5}.coef,1,source{1}.demixer1);
		source{1}.data1        = srs(source{1}.data1 , coe{5}.shft, 15);

		source{1}.aa1=reshape(source{1}.data1 ,len_122p88,N_ite);
		has_lte=0;
	end

	if has_lte==1
		for i=LTE_ST_cnt:N_ch
			source{i}.name     = 'LTE';
			source{i}.data1_245p76 = data_ch1;
			source{i}.data1_245p76 = filter(coe{3}.coef,1,source{i}.data1_245p76);
			source{i}.data1_122p88 = srs(source{i}.data1_245p76(1:2:end) , coe{3}.shft, 15);
			source{i}.demixer1     = source{i}.data1_122p88.*me_dds_v3(nco.stps(i), len_122p88*N_ite, 16, 0);
			source{i}.demixer1     = srs(source{i}.demixer1,nco.shft,15);
			source{i}.data1_122p88 = filter(coe{4}.coef,1,source{i}.demixer1);
			source{i}.data1_61p44  = srs(source{i}.data1_122p88(1:2:end) , coe{4}.shft, 15);
			source{i}.data1_61p44  = filter(coe{2}.coef,1,source{i}.data1_61p44);
			source{i}.data1_30p72  = srs(source{i}.data1_61p44(1:2:end) , coe{2}.shft, 15);
			source{i}.data1_30p72  = filter(coe{1}.coef,1,source{i}.data1_30p72);
			source{i}.data1        = srs(source{i}.data1_30p72 , coe{2}.shft, 15);
			source{i}.aa1=reshape(source{i}.data1 ,len_30p72,N_ite);
		end
	end
	
	%zeros data
	if ((N_NR0_ch*2+N_NR1_ch*4+N_LTE_ch)<5)
		source{N_NR0_ch+N_NR1_ch+N_LTE_ch+1}.aa1 =zeros(len_30p72, N_ite);
		N_ch=N_ch+1;
	end


    % Concatenate the Output Data of Carriers
    dout = source{1}.aa1;
    for chid=2:N_ch
		dout = [dout; source{chid}.aa1];
    end

   	fname = sprintf('../data%d/ddc_golden%d%d.txt', cfgid, floor((antid-1)/8), mod(antid-1, 8));
	fid = fopen(fname, 'wt');
    a = reshape(dout, 1, numel(dout));
    fprintf(fid, '%d %d\n', [real(a); imag(a)]);
 	fclose(fid);
    	
end

