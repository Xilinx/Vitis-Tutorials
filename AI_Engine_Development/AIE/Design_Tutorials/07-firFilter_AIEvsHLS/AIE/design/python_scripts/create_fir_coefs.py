#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11

#
# Filter Coefficent Generator for Tutorial XD061
# The Python script generates the output that is intended for a C header (.h) file
# It generates a series of #define; some taken directly from teh command line 
# parameters, but also generates the FIR filter coefficeints for a triangular
# window.
#
# Usage:
#     create_fir_coefs.py 
#         [-aie | dsp]            - specify whether this header is for AIE or DSP implementation
#         <number_of_fir_filters> - Number for fIR Filters in the design
#         <number_of_fir_taps>    - Number of taps in each FIR filter
#         <impulse area>          - the total area under the window
#        [<window_size>]          - the size of the AIE ping-pong buffer window (AIE only)
#         
#-------------------------------------------------------------------------------

from __future__ import print_function
import sys
import copy

def create_fir_coefs(n_taps, area):

    coefs = []

    midpoint = (n_taps + 1.0) / 2.0
    for ix in range (0, (n_taps + 1) // 2) :
        tap = area * (ix + 1) / midpoint**2
        coefs.append(int(tap))

    return coefs



if ((len(sys.argv) != 5) and (len(sys.argv) != 6)) :
    print("Syntax is: ", sys.argv[0], "[-aie | -dsp]  <number_of_fir_filters>  <number_of_fir_taps>  <impulse area> [<window_size>]") 
    sys.exit(1)

window_size = 1024
mode   = sys.argv[1]
n_firs = int(sys.argv[2])
n_taps = int(sys.argv[3])
area   = float(sys.argv[4])

coefs = []
coefs = create_fir_coefs(n_taps, area)

if (mode == "-dsp") :
    coefs2 = copy.copy(coefs)
    coefs2.reverse()
    if ((n_taps % 2) == 1) :
        del coefs2[0]
    coefs.extend(coefs2)
elif (mode == "-aie") :
    if (len(sys.argv) == 6) :
        window_size = int(sys.argv[5])
else :
    print("**ERROR: switch must be either '-aie' or '-dsp'")
    sys.exit(2)

print("// FIR Coefficients Taps: ", n_taps, "Impulse Response Area: ", area)
print("//")
print("#define N_FIR_FILTERS    ", n_firs);
print("#define N_FIR_TAPS       ", n_taps);
if (mode == "-aie") :
    print("#define FIR_WINDOW_SIZE  ", window_size);
print("#define FIR_COEF_VALUES  { ", end="")
print(*coefs, sep=", ", end="")
print(" }")

