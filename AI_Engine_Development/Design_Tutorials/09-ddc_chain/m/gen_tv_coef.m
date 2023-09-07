%  
%  Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
%  SPDX-License-Identifier: MIT
%  

 function gen_tv_coef

% load the coefficients if they exist
coef_ch89t = load('coef_chFir89t.txt');
coef_ch199t = load('coef_chFir199t.txt');
coef_hbf23 = load('coef_hbf23t.txt');
coef_hbf11 = load('coef_hbf11t.txt');
coef_hbf47 = load('coef_hbf47t.txt');

%------------------------------------------
% (1) CH0 FIR89-tap
%------------------------------------------
a = coef_ch89t;
coe{1}.name = 'FIR89';	
coe{1}.nn = 'FIR89';
coe{1}.shft = 15;
coe{1}.ntap = 89;
amid = (length(a)+1)/2;
coe{1}.coef = a;
coe{1}.coefrom = zeros(1, ceil(amid/8)*8); 
coe{1}.coefrom(1:amid) = a(1:amid);

%------------------------------------------
% (2) HBF23-tap
%------------------------------------------
a = coef_hbf23;
coe{2}.name = 'HB23_2D';	
coe{2}.nn = 'HB23_2D';
coe{2}.shft = 15;
coe{2}.ntap = 23;
amid = (length(a)+1)/2;
coe{2}.coef = a;
coe{2}.coefrom = zeros(1, ceil(amid/2/8)*8); 
coe{2}.coefrom(1:amid/2) = a(1:2:amid-1);
coe{2}.coefrom(amid/2+2) = a(amid);
%------------------------------------------
% (3) HBF43-tap
%------------------------------------------
a = coef_hbf47;
coe{3}.name = 'HB47_2D';	
coe{3}.nn = 'HB47_2D';
coe{3}.shft = 15;
coe{3}.ntap = 47;
amid = (length(a)+1)/2;
coe{3}.coef = a;
coe{3}.coefrom = zeros(1, ceil(amid/2/8)*8); 
coe{3}.coefrom(1:amid/2) = a(1:2:amid-1);
%coe{3}.coefrom(amid/2+1) = a(amid);
%------------------------------------------
% (4) HBF11-tap
%------------------------------------------
a = coef_hbf11;
coe{4}.name = 'HB11_2D';	
coe{4}.nn = 'HB11_2D';
coe{4}.shft = 15;
coe{4}.ntap = 11;
amid = (length(a)+1)/2;
coe{4}.coef = a;
coe{4}.coefrom = zeros(1, max(ceil(amid/2/8)*8,16)); 
coe{4}.coefrom(1:amid/2) = a(1:2:amid-1);
coe{4}.coefrom(amid/2+1) = a(amid);


%------------------------------------------
% (5) CH7 FIR199-tap
%------------------------------------------
a=coef_ch199t;
coe{5}.name = 'FIR199';	
coe{5}.nn = 'FIR199';
coe{5}.shft = 15;
coe{5}.ntap = 199;
amid = (length(a)+1)/2;
coe{5}.coef = a;
coe{5}.coefrom = zeros(1, ceil(amid/8)*8); 
coe{5}.coefrom(1:amid) = a(1:amid);

%------------------------------------------
% (6) NCO Phase Step Quantized to 32 bits
%------------------------------------------
nco.stps = randi([-2^31, 2^31-1], 5, 1);
nco.shft = 16;

% fidc = fopen('../src/inc/ddc_chain_kernel_const.h', 'wt');
% fprintf(fidc, '#ifndef __DDC_CHAINE_KERNEL_CONST_H__\n#define __DDC_CHAINE_KERNEL_CONST_H__\n\n');
% 
% fprintf(fidc, '#define DDC_SHIFT %d\n\n', 15);
% fprintf(fidc, '#define NCO_SHIFT %d\n\n', nco.shft);
% % dump filter coefficients and shift information
%  coef_lis=[1,2,4,5];
% for k=coef_lis
%     fprintf(fidc, '#define %s_COEFFICIENTS %d\n', coe{k}.nn, length(coe{k}.coefrom));
%     %fprintf(fidc, '#define %s_SHIFT %d\n', coe{k}.nn, coe{k}.shft);
% 
%     this_coef=coe{k}.coefrom;
%     fprintf(fidc, 'int16_t chess_storage(%%chess_alignof(v16int16)) coeffs_%s[%s_COEFFICIENTS] ={\n\t\t', lower(coe{k}.name),coe{k}.name);
%     fprintf(fidc, '%4d, '    , this_coef(1:end-1));
%     fprintf(fidc, '%4d\n};\n', this_coef(end));
%     
%     fprintf(fidc, '\n');
%     
% end
% 
%     
% %% %------------------------------------
% fprintf(fidc, '\n\n');
% fprintf(fidc, '#endif\n');
% fclose(fidc);
% 
% fidc = fopen('../src/inc/hbf47d_const.h', 'wt');
% fprintf(fidc, '#ifndef __HBF47D_CONST_H__\n#define __HBF47D_CONST_H__\n\n');
% fprintf(fidc, '#define HBF47D_SHIFT %d\n\n', 15);
% for k=3
%     fprintf(fidc, '#define %s_COEFFICIENTS %d\n', coe{k}.nn, length(coe{k}.coefrom));
%     %fprintf(fidc, '#define %s_SHIFT %d\n', coe{k}.nn, coe{k}.shft);
% 
%     this_coef=coe{k}.coefrom;
%     fprintf(fidc, 'int16_t chess_storage(%%chess_alignof(v16int16)) coeffs_%s[%s_COEFFICIENTS] ={\n\t\t', lower(coe{k}.name),coe{k}.name);
%     fprintf(fidc, '%4d, '    , this_coef(1:end-1));
%     fprintf(fidc, '%4d\n};\n', this_coef(end));
%     
%     fprintf(fidc, '\n');
%     
% end
% 
% fprintf(fidc, '\n\n');
% fprintf(fidc, '#endif\n');
% fclose(fidc);
%% %------------------------------------

% save the input random data to a mat file
save tv_coe.mat coe nco;

