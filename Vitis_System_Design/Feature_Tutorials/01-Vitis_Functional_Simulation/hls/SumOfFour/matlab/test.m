%
% Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
% SPDX-License-Identifier: MIT
%

%%
myHlsKernel = vfs.hlsKernel(input_files = "../src/kernels.cpp",...
                            part = 'xcvc1902-vsva2197-2MP-e-S',...
                            hls_function = "sum_of_four");

% Inspect the I/O ports (this is optional)
myHlsKernel.getInputSpec()
myHlsKernel.getOutputSpec()

in_32bit = int32(1:9);

out_32bit = myHlsKernel.run(varray.int32(in_32bit));
assert(size(out_32bit,1)==2)
disp('Test Passed')