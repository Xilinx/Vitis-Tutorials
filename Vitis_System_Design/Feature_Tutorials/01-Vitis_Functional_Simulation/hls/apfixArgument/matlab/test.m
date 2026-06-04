%
% Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

myfunc = vfs.hlsKernel(input_files="../src/pl_func.cpp", ...
                       hls_function="pl_func");

in1 = single([1.1,2.2,3.3,4.4,5.5]);
in2 = single([10.1,20.2,30.3,40.4,50.5]);
[out1, out2] = myfunc.run(varray.fi(in1), varray.fi(in2));

disp(out1);
disp(out2);

fprintf('Test Passed\n');
