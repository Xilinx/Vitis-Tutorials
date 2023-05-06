% Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT

%%
I4c = load("input_128b.txt")
I2c = zeros(2048*4, 2);
%%
I2c(1:4:end, 1) = I4c(:, 1);
I2c(1:4:end, 2) = I4c(:, 2);
I2c(2:4:end, 1) = I4c(:, 3);
I2c(2:4:end, 2) = I4c(:, 4);
I2c(3:4:end, 1) = I4c(:, 5);
I2c(3:4:end, 2) = I4c(:, 6);
I2c(4:4:end, 1) = I4c(:, 7);
I2c(4:4:end, 2) = I4c(:, 8);
%%
fid = fopen("input_2col.txt", "w");
for i= 1 : 8192
    fprintf(fid, "%7d %7d\n", I2c(i, 1), I2c(i, 2));
end
fclose(fid)
%%
