% Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT

clear all
close all
clc
%%
cpf_atan2_res = load('cpf_atan2_data.txt');
m_atan2 = atan2(cpf_atan2_res(:, 1),cpf_atan2_res(:, 2));

%%
diff_atan2 = abs(m_atan2 - cpf_atan2_res(:, 3));
figure; plot(diff_atan2);
max(diff_atan2)
min(diff_atan2)
sum_abs_diff = sum(diff_atan2)

% %% write file input_data.h
% N = size(cpf_atan2_res, 1);
% fprintf(fid, 'int input_data[2][%d]={ \n',N);
% for i = 1 : N-1
%     fprintf(fid, '{%10d, %10d},\n', cpf_atan2_res(i, 1), cpf_atan2_res(i,2));
% end
% fprintf(fid, '{%10d, %10d}\n', cpf_atan2_res(i, 1), cpf_atan2_res(i,2));
% fprintf(fid, '};\n');
% fclose(fid);

%% write file input_data.txt

N = size(cpf_atan2_res, 1);
fid = fopen('input_data.txt','w');
for i = 1 : N
     fprintf(fid, '%10d %10d\n', cpf_atan2_res(i, 1), cpf_atan2_res(i,2));
end
fclose(fid);

% %% write file ref_results.h
% fid2 = fopen('ref_results.h','w');
% fprintf(fid2, 'float ref_results[%d]={ \n',N);
% for i = 1 : N-1
%     fprintf(fid2, '%20.10f, ', cpf_atan2_res(i,3));
% end
%     fprintf(fid2, '%20.10f', cpf_atan2_res(i,3));
% fprintf(fid2, ' };\n');
% fclose(fid2);

%% write file ref_results.txt
fid2 = fopen('ref_results.txt','w');
%fprintf(fid2, 'float ref_results[%d]={ \n',N);
for i = 1 : N
    fprintf(fid2, '%20.10f\n', cpf_atan2_res(i,3));
end
fclose(fid2);
