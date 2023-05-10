%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: X11
%

%% Half-Band Filter 1

Shift1 = 15;
K1 = 2^Shift1;
hb = HB1;
hb1 = NormalizeHBCoef(hb.Numerator,K1/2); % Decimation so K1/2
hb1((length(hb1)+1)/2) = K1/2;
hb1_aie = hb1([ 1:2:(length(hb1)-1)/2 (length(hb1)+1)/2]);

disp('HB1');
disp(['  Center Tap: ' num2str(hb1((length(hb1)+1)/2)) ]);
disp(['Phase 1 Norm: ' num2str(sum(hb1(1:2:end)))]);
disp([' Max Phase 1: ' num2str(max(hb1(1:2:end)))]);
disp(' ');


%% Half-Band Filter 2

Shift2 = 15;
K2 =2^Shift2;
hb = HB2;
hb2 = NormalizeHBCoef(hb.Numerator,K2/2);
hb2((length(hb2)+1)/2) = K2/2;
hb2_aie = hb2([ 1:2:(length(hb2)-1)/2 (length(hb2)+1)/2]);

disp('HB2');
disp(['  Center Tap: ' num2str(hb2((length(hb2)+1)/2)) ]);
disp(['Phase 1 Norm: ' num2str(sum(hb2(1:2:end)))]);
disp([' Max Phase 1: ' num2str(max(hb2(1:2:end)))]);
disp(' ');



%% Half-Band Filter 3

Shift3 = 15;
K3 =2^Shift3;
hb = HB3;
hb3 = NormalizeHBCoef(hb.Numerator,K3/2);
hb3((length(hb3)+1)/2) = K3/2;
hb3_aie = hb3([ 1:2:(length(hb3)-1)/2 (length(hb3)+1)/2]);

disp('HB3');
disp(['  Center Tap: ' num2str(hb3((length(hb3)+1)/2)) ]);
disp(['Phase 1 Norm: ' num2str(sum(hb3(1:2:end)))]);
disp([' Max Phase 1: ' num2str(max(hb3(1:2:end)))]);
disp(' ');

%% Channel Filter

ShiftCF = 15;
Kcf =2^ShiftCF;
cf = ChannelFilter;
cfi = NormalizeCoef(cf.Numerator,Kcf);
cfi_aie = cfi(1:(length(cfi)+1)/2);

disp('CF');
disp(['Channel Filter Norm: ' num2str(sum(cfi))]);
disp(['           Max Coef: ' num2str(max(cfi))]);
disp(' ');


% %% Store in Filter File
% fp = fopen('FilterParam.txt','w');
% 
% fprintf(fp,'Half-Band Filter 1\n');
% fprintf(fp,'Number of Coefficients: %d\n',length(hb1));
% fprintf(fp,'Normalization shift: %d\n',round(log2(K1))+1);
% fprintf(fp,'%5d',hb1(1));
% fprintf(fp,',%6d',hb1(2:end));
% fprintf(fp,'\n\n\n');
% 
% 
% fprintf(fp,'Half-Band Filter 2\n');
% fprintf(fp,'Number of Coefficients: %d\n',length(hb2));
% fprintf(fp,'Normalization shift: %d\n',round(log2(K2))+1);
% fprintf(fp,'%5d',hb2(1));
% fprintf(fp,',%6d',hb2(2:end));
% fprintf(fp,'\n\n\n');
% 
% 
% fprintf(fp,'Half-Band Filter 3\n');
% fprintf(fp,'Number of Coefficients: %d\n',length(hb3));
% fprintf(fp,'Normalization shift: %d\n',round(log2(K3))+1);
% fprintf(fp,'%5d',hb3(1));
% fprintf(fp,',%6d',hb3(2:end));
% fprintf(fp,'\n\n\n');
% 
% 
% fprintf(fp,'Channel Filter\n');
% fprintf(fp,'Number of Coefficients: %d\n',length(cfi));
% fprintf(fp,'Normalization shift: %d\n',round(log2(Kcf)));
% fprintf(fp,'%5d',cfi(1));
% fprintf(fp,',%6d',cfi(2:end));
% fprintf(fp,'\n\n\n');
% 
% 
% fclose(fp);

% %% Create Data file
% 
% N = 16384;
% x = round((rand(N*2,1)-.5)*50000);  % +- 25000
% fp = fopen("input32.txt","w");
% fprintf(fp,"%+6d %+6d\n",x);
% fclose(fp);
