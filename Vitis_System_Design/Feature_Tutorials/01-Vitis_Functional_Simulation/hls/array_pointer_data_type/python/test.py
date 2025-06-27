#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

import vfs
import varray as va
import numpy as np

myKernel = vfs.hlsKernel(input_files = ["../src/pl_func.cpp", "../src/hls_kernels.h"], 
                         part = "xcvc1902-vsva2197-2MP-e-S", hls_function = "pl_func")

# Inspect the I/O ports (this is optional)
myKernel.getInputSpec()
myKernel.getOutputSpec()

random_list1 = np.random.randint(-2**30, 2**30, size=5)
random_list2 = np.random.randint(-2**30, 2**30, size=5)

# Note both inputs expect a sample size of 5
# However we are only going to pass the first 3 elements of the first input
# and the first 2 elements of the second input
# Since not enough samples are passed to the ports, simulation will not begin. 
# However these samples are stored as states in the vfs object.

in1 = va.array(random_list1[:3], va.int32)
in2 = va.array(random_list2[:2], va.int32)

[out1, out2] = myKernel.run(in1, in2)

# The output is empty since the input sizes are not equal to the expected size
# The expected input size is 5 for both inputs
print(out1)
print(out2)

assert(out1.size == 0)
assert(out2.size == 0)

# Now we are going to use the same object to run the kernel again
# This time we are going to pass the last 2 elements of the first input
# and the last 3 elements of the second input
in1 = va.array(random_list1[3:], va.int32)
in2 = va.array(random_list2[2:], va.int32)

[out1, out2] = myKernel.run(in1, in2)

# The output size is 5 for both outputs as all together we have fed 
# 5 elements to both inputs. No data is lost. 
print(out1)
print(out2)

assert(np.all(np.asarray(out1 == [x * 2 for x in random_list1])))
assert(np.all(np.asarray(out2 == [x * 2  for x in random_list2])))

print('Test Passed')