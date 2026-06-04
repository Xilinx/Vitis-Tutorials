%
% Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

myfunc = vfs.hlsKernel(input_files="../src/pl_func.cpp",... 
                       hls_function="pl_func");

in1 = uint32([1+11j,2+12j,3+13j,4+14j,5+15j]);
in2 = single([10.+11.j,20.+22.j,30.+33.j,40.+44.j, 50.+55.j]);
[out1, out2] = myfunc.run(varray.cuint32(in1), varray.cfloat(in2));

disp(out1);
disp(out2);

assert(isequal(double(out1).', double(in1)), 'out1 mismatch');
assert(isequal(double(out2).', double(in2)), 'out2 mismatch');

fprintf('Test Passed\n');
