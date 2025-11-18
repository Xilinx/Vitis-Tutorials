#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

import varray as va
import numpy as np

################################################################
# Form a simple varray from a list or tuple
################################################################
v1 = va.array([1, 2, 3, 4], va.int32)  # Data type is mandatory
v2  = va.array((1,2,3,4), va.int32) # Data type is mandatory


#################################################################
# Form a varray from a numpy array
#################################################################
vn = np.array([1, 2, 3, 4], dtype=np.uint32)
v2 = va.array(vn) # Inherits the data type from the numpy array if possible
v3 = va.array(vn, va.cint16) # Or you can override the numpy data type

#################################################################
# Coovert a varray to a numpy array
#################################################################
vn = np.array(v3)  # or np.asarray(v3)

#################################################################
# Inspect the varray
#################################################################
print(v3.shape)  # Shape of the varray
print(v3.ndim)   # Number of dimensions
print(v3.dtype)  # Data type of the varray
print(v3.size)   # Number of elements

#################################################################
# Get the byte reperesentation of the varray
#################################################################
print(v3.bytes)  # Returns the byte representation of the varray

#################################################################
# Typecast the varray
#################################################################
print(v3)
v4 = v3.view(va.uint32)  # Typecast the varray to uint32
print(v4)
assert(np.all(v3.bytes == v4.bytes))  # The byte representation should be the same

#################################################################
# Some numpy functions are supported on varray
#################################################################
v5 = np.concatenate((v3, v3))  # Concatenate two varrays, output is a varray
m = np.max(v3)  # Maximum element
c = np.isreal(v3)

