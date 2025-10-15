#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

import vfs
import varray as va
import numpy as np


myGraph = vfs.aieGraph(input_file = "../src/DUT.cpp", part = "xcve2802-vsvh1760-2MP-e-S", 
                       include_paths = ["../src"])

# Inspect the I/O ports
myGraph.getInputSpec()
myGraph.getOutputSpec()

# Run the graph, passing an array of 64 samples to the input port. 
# This is the same size as the input buffer length for the kenrel.
# The kernel gets triggered.
in_data = va.array(list(range(64)), dtype = va.bfloat16)

out_data = myGraph.run(in_data)

assert(all(np.asarray(out_data)-np.asarray(in_data) == 0))
assert(out_data.size == 64)

# Now we pass only an areay of 32 samples to the input port.
in_data_1 = va.array(list(range(32)), dtype = va.bfloat16)
out_data_1 = myGraph.run(in_data_1)

# The output buffer is empty, as the kernel has not been triggered.
# Note that the run method does not block and out_data_1 is empty.
assert(out_data_1.size == 0)

# Now we pass another array of 32 samples to the input port.
in_data_2 = va.array(list(range(32, 64)), dtype = va.bfloat16)
out_data = myGraph.run(in_data_2)

# The output buffer is now full, as the kernel has been triggered.
# The kernel operates on both in_data_1 (that was stored internally 
# from the first call to the run method) and in_data_2. No data is lost. 
assert(all(np.asarray(out_data) - np.asarray(np.concatenate((in_data_1, in_data_2))) == 0))
assert(out_data.size == 64)
print('Test Passed')
