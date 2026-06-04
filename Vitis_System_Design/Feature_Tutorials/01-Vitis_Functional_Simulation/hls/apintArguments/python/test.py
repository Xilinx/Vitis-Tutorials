#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

import vfs
import varray as va
import numpy as np

myHlsKernel = vfs.hlsKernel(input_files = "../src/pl_func.cpp", 
                             hls_function = "pl_func")

in1 = np.arange(1,11, dtype=np.int32)
in2 = np.arange(11,21, dtype=np.int32)
out1, out2 = myHlsKernel.run(va.array(in1), va.array(in2))

print(out1)
print(out2)

assert(np.all(np.asarray(in1*2) == np.asarray(out1)))
assert(np.all(np.asarray(in2 + 10) == np.asarray(out2)))

print('Test Passed')
