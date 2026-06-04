%
% Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

myHlsKernel = vfs.hlsKernel(input_files = "../src/pl_func.cpp", ...
                             hls_function = "pl_func");

in1 = int32(1:10);
in2 = int32(11:20);
[out1, out2] = myHlsKernel.run(varray.int32(in1), varray.int32(in2));

disp(out1);
disp(out2);

assert(isequal(double(out1).', double(in1*2)), 'out1 mismatch');
assert(isequal(double(out2).', double(in2+10)), 'out2 mismatch');

fprintf('Test Passed\n');
