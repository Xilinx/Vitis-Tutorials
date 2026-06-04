#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

import vfs
import varray as va
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import lfilter
import os

# Use git clone https://github.com/Xilinx/Vitis_Libraries.git and set DSPLIB_ROOT to the dsp folder

DSPLIB_ROOT = os.environ["DSPLIB_ROOT"]

# Initialize the AIE graph
myaiefir = vfs.aieGraph(
    input_file="../src/AIE_subsystem.cpp",
    part = "xcvc1902-vsva2197-2MP-e-S",
    include_paths=[
        "../src",
        DSPLIB_ROOT + "/L2/include/aie/",
        DSPLIB_ROOT + "/L1/include/aie/",
        DSPLIB_ROOT + "/L1/src/aie/"
    ]
)

b = [180, 89, -80, -391, -720, -834, -478, 505, 2063, 3896, 5535, 6504]

frame_size = 256
num_of_frames = 40
scale = 2**9

# Set filter in Python (assuming you can use scipy or similar)
Hd = np.array(b + b[::-1])
Hd_states = np.zeros(2 * len(b) - 1)

# Set outputs
out_ref = np.zeros(frame_size * num_of_frames, dtype=complex)
out_aie = np.zeros_like(out_ref)

rtp = va.array(b, va.int32)

# Process each frame
for i in range(num_of_frames):
    # Generate random complex input
    input_data = (np.random.randint(-2**6, 2**6, size=(frame_size,)) + 
                  1j * np.random.randint(-2**6, 2**6, size=(frame_size,))).astype(np.complex64)
    
    # Apply the filter
    y, Hd_states = lfilter(Hd, 1, input_data, zi=Hd_states)
    
    out_ref[i * frame_size: (i + 1) * frame_size] = y / scale

    # Run AIE graph processing
    y_aie = myaiefir.run(va.array(input_data, va.cint16), rtp)
    out_aie[i * frame_size: (i + 1) * frame_size] = y_aie

    # Optional, since this is an asynchronous rtp, we can set the 
    # rtp array to an empty array for subsequent runs
    rtp = va.array([], va.int32)


# Error checking
error = out_ref-out_aie
max_error = max(max(np.real(error)),max(np.imag(error)));
if (max_error<1):
    print('Test Passed')
else:
    print('Test Failed')

# Plot outputs
plt.subplot(3, 1, 1)
plt.plot(np.real(out_ref))
plt.title("Python Output")

print(out_ref[:30])

plt.subplot(3, 1, 2)
plt.plot(np.real(out_aie), 'r')
plt.title("AIEngine Output")

print(out_aie[:30])

plt.subplot(3, 1, 3)
plt.plot(np.real(out_ref - out_aie))
plt.title("Difference")

plt.show()
