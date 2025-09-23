%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

%%
myHlsKernel = vfs.hlsKernel(input_files = {"../src/kernels.cpp", "../src/kernels.h"},...
                         part = 'xcvc1902-vsva2197-2MP-e-S',...
                         hls_function = "hls_array_of_stream");

input = randi([-2^28, 2^28-1], [1024, 4], 'int32'); 

input_data = varray.int32(input);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Feeding the input as a cell of arrays.
% Output is a list
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
in_data_cell = {input_data(:,1);input_data(:,2);input_data(:,3);input_data(:,4)};
output_data = myHlsKernel.run(in_data_cell);

% check to see if output is correct
for i=1:4
    assert(all(in_data_cell{i}*i == output_data{i}))
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Feeding the input as individual arrays
% Outputs are individual arrays
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
[output_data1,output_data2,output_data3,output_data4] = myHlsKernel.run(input_data(:,1),input_data(:,2),input_data(:,3),input_data(:,4));

% check to see if output is correct
assert(all(input_data(:,1)*1 == output_data1))
assert(all(input_data(:,2)*2 == output_data2))
assert(all(input_data(:,3)*3 == output_data3))
assert(all(input_data(:,4)*4 == output_data4))

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Feeding the input as a matrix, each column is a stream
% Output is a list
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
output_data = myHlsKernel.run(input_data);

% check to see if output is correct
for i=1:4
    assert(all(input_data(:,i)*i == output_data{i}))
end

disp('Test Passed')
