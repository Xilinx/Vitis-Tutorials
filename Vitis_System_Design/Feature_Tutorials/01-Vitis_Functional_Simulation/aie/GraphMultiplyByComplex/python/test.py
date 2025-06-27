#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

import vfs
import varray as va
import numpy as np

# This graph multiplies the real part by two and the imaginary part by 3.
# input is cint16 and output is cint16.

myGraph = vfs.aieGraph(input_file = '../src/mySub.cpp', part = "xcvc1902-vsva2197-2MP-e-S", include_paths = "../src/")

# Inspect the I/O ports (this is optional)
myGraph.getInputSpec()
myGraph.getOutputSpec()

indr = np.random.randint(low=-2**10, high=2**10-1, size=(256,), dtype=np.int16)
indi = np.random.randint(low=-2**10, high=2**10-1, size=(256,), dtype=np.int16)

ind = indr + 1j*indi

#Inputs and outputs to the run method are varray objects.
out = myGraph.run(va.array(ind, dtype = va.cint16))

# Note you can use np.real directly on a varray object
assert(np.all(np.real(ind)*2 == np.real(out)))
assert(np.all(np.imag(ind)*3 == np.imag(out)))

# Use the same object to run the graph again
ind = np.conjugate(ind)
out = myGraph.run(va.array(ind, dtype = va.cint16))

# Here we are converting the varray object to a numpy array using np.asarray
assert(np.all(np.real(ind)*2 == np.asarray(out).real))
assert(np.all(np.imag(ind)*3 == np.asarray(out).imag))

print("Test passed")