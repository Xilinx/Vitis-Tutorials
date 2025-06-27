#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

import vfs
import varray as va
import numpy as np

myHLSKernel = vfs.hlsKernel(input_files = "../src/kernels.cpp",part = 'xcvc1902-vsva2197-2MP-e-S',hls_function = "invert7_13");
N = 10

in7 = np.random.randint(0, 2**7 - 1, N)
in13 = np.random.randint(0, 2**13 - 1, N)

#fixed point inputs
input7 = va.array(in7,va.fi(0,7,0))
input13 = va.array(in13,va.fi(0,13,0))
output7,output13 = myHLSKernel.run(input7,input13)

# Now we use ~ (bit compliment) in Python to create a refernce output.
# We also mask the bits that are not relevant and then compare the referene to the output.
outref7 = ~(np.uint8(in7))
outref7 = outref7 & 2**7-1 # mask the last bit


outref13 = ~(np.uint16(in13))
outref13 = outref13 & 2**13-1 # mask the last three bits

assert(np.all(outref7 == np.uint8(output7)))
assert(np.all(outref13 == np.uint16(output13)))
print('Test Passed')