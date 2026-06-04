#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

"""
Example: Multiply with Preprocessor Definition

This example demonstrates the use of the Xpreproc parameter to pass
preprocessor definitions to the AIE compiler. The kernel multiplies
input values by a factor that can be configured at compile-time using
the -DMULTIPLY_FACTOR=<value> preprocessor definition.

The kernel defaults to multiplying by 2 if no preprocessor definition
is provided.
"""

import vfs
import numpy as np
import varray as va

# Example 1: Default behavior (multiply by 2)
# No Xpreproc parameter - uses default MULTIPLY_FACTOR=2 from kernel
print("Example 1: Default behavior (MULTIPLY_FACTOR=2)")
myGraph = vfs.aieGraph(
    input_file="../src/mySub.cpp",
    include_paths=["../src/"]
)

in_data = va.array([1, 2, 3, 4, 5], dtype=va.int32)
out_data = myGraph.run(in_data)
ref = np.array([1, 2, 3, 4, 5]) * 2
assert(np.all(np.asarray(out_data) == ref)), "Example 1 failed"
print(f"Output: {out_data} (matches reference)")
print()

# Example 2: Using Xpreproc to set MULTIPLY_FACTOR=3
print("Example 2: Using Xpreproc with MULTIPLY_FACTOR=3")
myGraph2 = vfs.aieGraph(
    input_file="../src/mySub.cpp",
    include_paths=["../src/"],
    Xpreproc="-DMULTIPLY_FACTOR=3"
)

in_data2 = va.array([1, 2, 3, 4, 5], dtype=va.int32)
out_data2 = myGraph2.run(in_data2)
ref2 = np.array([1, 2, 3, 4, 5]) * 3
assert(np.all(np.asarray(out_data2) == ref2)), "Example 2 failed"
print(f"Output: {out_data2} (matches reference)")
print()

# Example 3: Using Xpreproc to set MULTIPLY_FACTOR=5
print("Example 3: Using Xpreproc with MULTIPLY_FACTOR=5")
myGraph3 = vfs.aieGraph(
    input_file="../src/mySub.cpp",
    include_paths=["../src/"],
    Xpreproc="-DMULTIPLY_FACTOR=5"
)

in_data3 = va.array([1, 2, 3, 4, 5], dtype=va.int32)
out_data3 = myGraph3.run(in_data3)
ref3 = np.array([1, 2, 3, 4, 5]) * 5
assert(np.all(np.asarray(out_data3) == ref3)), "Example 3 failed"
print(f"Output: {out_data3} (matches reference)")
print()

# Example 4: Multiple preprocessor definitions
# This example shows how to pass multiple definitions as a list
print("Example 4: Multiple preprocessor definitions")
myGraph4 = vfs.aieGraph(
    input_file="../src/mySub.cpp",
    include_paths=["../src/"],
    Xpreproc=["-DMULTIPLY_FACTOR=10", "-DENABLE_DEBUG"]
)

in_data4 = va.array([1, 2, 3], dtype=va.int32)
out_data4 = myGraph4.run(in_data4)
ref4 = np.array([1, 2, 3]) * 10
assert(np.all(np.asarray(out_data4) == ref4)), "Example 4 failed"
print(f"Output: {out_data4} (matches reference)")
print()

print('Test Passed')
