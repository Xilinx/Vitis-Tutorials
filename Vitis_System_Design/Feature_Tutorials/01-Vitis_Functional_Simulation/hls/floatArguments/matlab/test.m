%
% Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

myfunc = vfs.hlsKernel(input_files="../src/pl_func.cpp", ...
                       hls_function="pl_func");

in1 = single([1.1,2.2,3.3,4.4,5.5]);
in2 = double([10.1,20.2,30.3,40.4,50.5]);
[out1, out2] = myfunc.run(varray.float(in1), varray.double(in2));

disp(out1);
disp(out2);

assert(all(abs(double(out1).' - double(in1)*2) < 1e-6), 'out1 mismatch');
assert(all(abs(double(out2).' - in2*2) < 1e-10), 'out2 mismatch');

fprintf('Test Passed\n');
