%
% Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

%%
myGraph = vfs.aieGraph(input_file = '../src/DUT.cpp',...
                       part = 'xcve2802-vsvh1760-2MP-e-S',...
                       include_paths = "../src/");
% Inspect the I/O ports (this is optional)
myGraph.getInputSpec()
myGraph.getOutputSpec()

in_data = varray.bfloat16((1:64)');

out_data = myGraph.run(in_data);

assert(all(in_data == out_data));
assert(size(out_data,1)==64);

% Now we pass only an areay of 32 samples to the input port.
in_data_1 = varray.bfloat16((1:32)');
out_data_1 = myGraph.run(in_data_1);

% The output buffer is empty, as the kernel has not been triggered.
% Note that the run method does not block and out_data_1 is empty.
assert(size(out_data_1,1) == 0)

% Now we pass another areay of 32 samples to the input port.
in_data_2 = varray.bfloat16((33:64)');
out_data = myGraph.run(in_data_2);

% The output buffer is now full, as the kernel has been triggered.
% The kernel operates on both in_data_1 (that was stored internally 
% from the first call to the run method) and in_data_2. No data is lost. 
assert(all(double(out_data) - double([in_data_1; in_data_2]) == 0))
assert(size(out_data,1) == 64)

disp('Test Passed')
