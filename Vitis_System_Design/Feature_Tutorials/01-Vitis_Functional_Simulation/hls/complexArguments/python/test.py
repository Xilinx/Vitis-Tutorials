#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

import vfs
import varray as va
import numpy as np

myfunc = vfs.hlsKernel(input_files="../src/pl_func.cpp", 
                       hls_function="pl_func")

in1 = va.array([1+11j,2+12j,3+13j,4+14j,5+15j], 'cuint32')
in2 = va.array([10.+11.j,20.+22.j,30.+33.j,40.+44.j, 50.+55.j], 'cfloat')
out1, out2 = myfunc.run(in1, in2)

print(out1)
print(out2)

assert(np.all(np.asarray(in1) == np.asarray(out1)))
assert(np.all(np.asarray(in2) == np.asarray(out2)))

print('Test Passed')
