%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

%%
myKernel = vfs.hlsKernel(input_files = {"../src/pl_func.cpp", "../src/hls_kernels.h"},...
                         part = 'xcvc1902-vsva2197-2MP-e-S',...
                         hls_function = "pl_func");

% Inspect the I/O ports (this is optional)
myKernel.getInputSpec()
myKernel.getOutputSpec()

random_list1 = randi([-2^30, 2^30-1], [5, 1], 'int32'); 
random_list2 = randi([-2^30, 2^30-1], [5, 1], 'int32'); 

% Note both inputs expect a sample size of 5
% However we are only going to use the first 3 elements of the first input
% and the first 2 elements of the second input
% Since not enough samples are passed to in2 port, simulation will not begin. 
% However these samples are stored as state in vfs object.
in1 = varray.int32(random_list1(1:3));
in2 = varray.int32(random_list2(1:2));

[out1, out2] = myKernel.run(in1, in2);

% The output is empty since the input sizes are not equal to the expected size
% The expected size is 5 for both inputs
disp(out1)
disp(out2)

assert(isempty(out1))
assert(isempty(out2))

% Now we are going to use the same object to run the kernel again
% This time we are going to use the last 2 elements of the first input
% and the last 3 elements of the second input
in1 = varray.int32(random_list1(4:5));
in2 = varray.int32(random_list2(3:5));

[out1, out2] = myKernel.run(in1, in2);

% The output size is 5 for both outputs as all together we have fed 
% 5 elements to both inputs. No data is lost. 
disp(out1)
disp(out2)

assert(all(out1 == random_list1*2))
assert(all(out2 == random_list2*2))

disp('Test Passed')
