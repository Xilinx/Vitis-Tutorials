%%
% Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT

%% run fir filter

close all;
clear all;
clc

global N;

N=16;

%%
coef = load('./data/fir_coeff.dat');
inp  = load('./data/input.dat');
ref  = load('./data/ref_res.dat');
SAMPLES = numel(ref);

%% 16_14 bit fixed point
% https://stackoverflow.com/questions/55809019/how-to-convert-float-to-fix16-14

i_coef = round(coef* 2^14);
i_inp  = round(inp* 2^14);
i_ref  = round(ref* 2^14);

%% compute data
i_res = zeros(SAMPLES, 1);
for i = 1 : SAMPLES
   i_res(i) = fir_filter(i_inp(i), i_coef);
end
%% show data
diff = abs(i_res - i_ref);
max_diff = max(diff)

[int32(i_res(1:32)) int32(i_ref(1:32))]
