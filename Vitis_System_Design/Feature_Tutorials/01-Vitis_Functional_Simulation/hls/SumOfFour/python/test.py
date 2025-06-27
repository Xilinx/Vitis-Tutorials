#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

import vfs
import varray as va
import numpy as np

myHlsKernel = vfs.hlsKernel(input_files = "../src/kernels.cpp", part = "xcvc1902-vsva2197-2MP-e-S", hls_function = "sum_of_four")

# Inspect the I/O ports (this is optional)
myHlsKernel.getInputSpec()
myHlsKernel.getOutputSpec()

in_32bit = np.arange(1,9, dtype=np.int32)

out_32bit = myHlsKernel.run(va.array(in_32bit))

print(out_32bit)

print('Test Passed')