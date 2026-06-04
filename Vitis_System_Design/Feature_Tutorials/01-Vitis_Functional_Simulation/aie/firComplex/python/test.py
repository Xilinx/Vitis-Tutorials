#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

import vfs
import varray as va
import os
import numpy as np
from scipy.signal import lfilter

DSPLIB_ROOT = os.environ["DSPLIB_ROOT"]

# FIR filter coefficients (32-tap symmetric lowpass)
taps = [-13, 569, 95, -427, -289, 504, 601, -495, -1052, 323, 1700, 169,
        -2777, -1562, 5883, 13536, 13536, 5883, -1562, -2777, 169, 1700,
        323, -1052, -495, 601, 504, -289, -427, 95, 569, -13]

num_taps = len(taps)

myfir = vfs.aieGraph(input_file = "../src/AI_Engine_FIR.cpp", 
                     include_paths = ["../src",
                                DSPLIB_ROOT + "/L2/include/aie/",
                                DSPLIB_ROOT + "/L1/include/aie/", 
                                DSPLIB_ROOT + "/L1/src/aie/"]) 

zi = np.zeros(num_taps - 1)

in_16bit_r = np.random.randint(low=-2**12, high=2**12-1, size=(2**10,), dtype=np.int16)
in_16bit_i = np.random.randint(low=-2**12, high=2**12-1, size=(2**10,), dtype=np.int16)

in_c16 = in_16bit_r + 1j*in_16bit_i

out_c16 = myfir.run(va.array(in_c16, dtype = va.cint16))

out_c16_r, zi = lfilter(taps, 1.0, in_c16, zi = zi)

assert(np.all(abs(np.asarray(out_c16)-out_c16_r/2**16)<1.42))

print('Test Passed')
