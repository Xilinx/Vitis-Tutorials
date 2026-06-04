%
% Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

%%
in_data = int32(randi(255,256,1));
weights = int32([8,7,6,5,4,3,2,1]);
tmp = conv(in_data,weights);
out_data_ref = tmp(1:256);

myGraph = vfs.aieGraph(input_file = '../src/graph.cpp',...
                       part = 'xcvc1902-vsva2197-2MP-e-S',...
                       include_paths = "../src/");


out_data_graph = myGraph.run(varray.int32(in_data));

% Note we compare Matlab array with varray
assert(all(out_data_ref == out_data_graph));
disp('Test Passed')
