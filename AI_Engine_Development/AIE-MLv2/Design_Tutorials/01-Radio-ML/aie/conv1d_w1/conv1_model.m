%
% Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%
% Author: Faisal El-Shabani
%%
formatSpec = '%f';
fileID = fopen('data/data_iq.txt','r');
data_iq = fscanf(fileID,formatSpec);
NITER = length(data_iq)/2048;
data_iq = reshape(data_iq,2,1024,NITER);
data_iq = double(varray.bfloat16(data_iq));
fclose(fileID);

I_in = zeros(1024,NITER);
Q_in = zeros(1024,NITER);
for i=1:NITER
    I_in( :,i ) = reshape(data_iq(1,:,i),1,[]);
    Q_in( :,i ) = reshape(data_iq(2,:,i),1,[]);
end

conv1_golden_output = zeros(1024,64,NITER);
fileID = fopen("./data/data_o.txt",'r');
conv1_golden_output = reshape(fscanf(fileID,formatSpec),size(conv1_golden_output));
fclose(fileID);


fileID = fopen('w1_weights_trained.txt','r');
weights = fscanf(fileID,formatSpec);
weights = reshape(weights,64,2,7);
weights = double(varray.bfloat16(weights));
fclose(fileID);
fileID = fopen('w1_bias_trained.txt','r');
biases = fscanf(fileID,formatSpec);
biases = double(varray.bfloat16(biases));
fclose(fileID);
%%
conv1_actual_output = zeros(1024,64,NITER);
tmp = zeros(64,1024);
for j=1:NITER
    for i=1:64
        conv_I = conv(I_in(:,j),flip(squeeze(weights(i,1,:)).'),'same');
        conv_Q = conv(Q_in(:,j),flip(squeeze(weights(i,2,:)).'),'same');

        tmp(i,:) = max(conv_I + conv_Q + biases(i),0);

    end
    conv1_actual_output(:,:,j) = tmp.';
end
conv1_actual_output = double(varray.bfloat16(conv1_actual_output));

figure(1);plot(reshape(conv1_actual_output,1,[]));hold on;plot(reshape(conv1_golden_output,1,[]));hold off;legend('Actual','Golden');grid on;
figure(2);plot(reshape(conv1_actual_output,1,[])-reshape(conv1_golden_output,1,[]));grid on;title('error');
max(abs(reshape(conv1_actual_output,1,[])-reshape(conv1_golden_output,1,[])))