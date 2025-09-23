#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

import vfs
import varray as va
import numpy as np

myHlsKernel = vfs.hlsKernel(input_files = ["../src/kernels.cpp"], part = "xcvc1902-vsva2197-2MP-e-S", hls_function = "hls_array_of_stream")

input = [np.random.randint(2**-28, 2**28, 1024) for _ in range(4)]

input_data = [va.array(input[i], va.int32) for i in range(4)]

#########################################
# Feeding the input as a list of arrays.
# Output is a list
#########################################

output_data = myHlsKernel.run(input_data)

# check to see if output is correct
for i in range(4):
    assert(np.all(np.asarray(input_data[i]*(i+1) == output_data[i])))

#########################################
# Feeding the input as individual arrays
# Output is a list
#########################################
    
out = myHlsKernel.run(input_data[0], input_data[1], input_data[2], input_data[3]); 

assert(np.all(np.asarray(input_data[0]*1 == out[0])))
assert(np.all(np.asarray(input_data[1]*2 == out[1])))
assert(np.all(np.asarray(input_data[2]*3 == out[2])))
assert(np.all(np.asarray(input_data[3]*4 == out[3])))

#######################################################
# Feeding the input as a matrix,each column is a stream
# Output is a list
#######################################################

input_data = np.transpose(va.array(input, va.int32))
print(input_data.ndim)
print(input_data.shape)

output_data = myHlsKernel.run(input_data)

# check to see if output is correct
for i in range(4):
    assert(np.all(np.asarray(input_data[:,i]*(i+1) == output_data[i])))

print('Test Passed')