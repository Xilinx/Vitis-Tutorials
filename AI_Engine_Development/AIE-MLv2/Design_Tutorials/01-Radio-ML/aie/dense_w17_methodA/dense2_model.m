%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Faisal El-Shabani
%%
formatSpec = '%f';
fileID = fopen('data/data_i.txt','r');
data_i = fscanf(fileID,formatSpec);
fclose(fileID);
NITER = length(data_i)/128;
data_i = reshape(data_i,128,NITER);

dense2_golden_output = zeros(128,NITER);
fileID = fopen('data/data_o.txt','r');
dense2_golden_output = reshape(fscanf(fileID,formatSpec),size(dense2_golden_output));
fclose(fileID);


fileID = fopen('w17_weights_trained.txt','r');
weights = fscanf(fileID,formatSpec);
fclose(fileID);
weights = reshape(weights,128,128);
fileID = fopen('w17_bias_trained.txt','r');
biases = fscanf(fileID,formatSpec);
fclose(fileID);
%%
dense2_actual_output = zeros(128,NITER);
for j=1:NITER
    dense2_actual_output(:,j) = selu(weights*data_i(:,j)+biases);
end
figure(1);plot(dense2_actual_output(:));hold on;plot(dense2_golden_output(:));hold off;legend('Actual','Golden');grid on;
figure(2);plot(reshape(dense2_actual_output,1,[])-reshape(dense2_golden_output,1,[]));grid on;title('error');
error = max(abs(reshape(dense2_actual_output,1,[])-reshape(dense2_golden_output,1,[])))

function [sig_o] = selu(sig_i)
   idx_p = find(sig_i  > 0);
   idx_n = find(sig_i <= 0);
   sig_o = zeros(size(sig_i));
   sig_o(idx_p) = 1.05070098*sig_i(idx_p);
   sig_o(idx_n) = 1.05070098*1.67326324*(exp(sig_i(idx_n))-1);
   sig_o = varray.bfloat16(sig_o);
end