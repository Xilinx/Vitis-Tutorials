#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

import vfs
import varray as va
import numpy as np

myfunc = vfs.hlsKernel(input_files="../src/pl_func.cpp", 
                       hls_function="pl_func")

in1 = va.array([1.1,2.2,3.3,4.4,5.5], 'float')
in2 = va.array([10.1,20.2,30.3,40.4,50.5], 'double')
out1, out2 = myfunc.run(in1, in2)

print(out1)
print(out2)

assert(np.all(np.asarray(in1)*2 == np.asarray(out1)))
assert(np.all(np.asarray(in2)*2 == np.asarray(out2)))

print('Test Passed')
