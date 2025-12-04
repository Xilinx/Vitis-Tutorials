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

dense3_golden_output = zeros(24,NITER);
fileID = fopen('data/data_o.txt','r');
dense3_golden_output = reshape(fscanf(fileID,formatSpec),size(dense3_golden_output));
fclose(fileID);


fileID = fopen('w18_weights_trained.txt','r');
weights = fscanf(fileID,formatSpec);
weights = reshape(weights,24,128);
weights = double(varray.bfloat16(weights));
fclose(fileID);
fileID = fopen('w18_bias_trained.txt','r');
biases = fscanf(fileID,formatSpec);
biases = double(varray.bfloat16(biases));
fclose(fileID);
%%
dense3_actual_output = zeros(24,NITER);
for j=1:NITER
    dense3_actual_output(:,j) = softmax(weights*data_i(:,j)+biases);
end
dense3_actual_output = double(varray.bfloat16(dense3_actual_output));

figure(1);plot(dense3_actual_output(:));hold on;plot(dense3_golden_output(:));hold off;legend('Actual','Golden');grid on;
figure(2);plot(reshape(dense3_actual_output,1,[])-reshape(dense3_golden_output,1,[]));grid on;title('error');
error = max(abs(reshape(dense3_actual_output,1,[])-reshape(dense3_golden_output,1,[])))

function [sig_o] = softmax(sig_i)
   exp_x = exp(sig_i - max(sig_i));
   sig_o = exp_x / sum(exp_x);
   sig_o = varray.bfloat16(sig_o);
end