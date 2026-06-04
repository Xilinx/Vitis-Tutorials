#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

import vfs
import varray as va
import numpy as np

in_data  = np.random.randint(low=0, high=255, size=(256,), dtype=np.int32)

# The weights below are the same as the one in ../src/weighted_sum.cc
weights = np.array([8,7,6,5,4,3,2,1], dtype=np.int32)
out_data_ref = np.convolve(in_data, weights)[0:256]

# Initialize AIE Graphs objects
myGraph = vfs.aieGraph(input_file='../src/graph.cpp', 
                       part = "xcvc1902-vsva2197-2MP-e-S", 
                       include_paths = ['../src','../'])

out_data_graph = myGraph.run(va.array(in_data))

# Note we compare a numpy array with a varray
assert(np.all(out_data_ref == out_data_graph))

print('Test Passed')