#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Donato Carlo Giorgio

import numpy as np
import math
from ml_dtypes import bfloat16
import sys

## Defining all the needed functions
###############################################################################################################
def make_twiddles(N, datatype, force_r2):
# Outputs a 4D twiddle factor tensor, indexed as tw[real/imag][twiddle_index][entry_index][stage_index]
# Note: the entries are the vectors of twiddle factors to be given to the stage function.
#       For example, a radix-3 has 2 entries, a radix-4 has 3, and so on.
# N is the number of points
# datatype is encoded like this:
#   0 = float32
#   1 = bfloat
#   2 = cint16
#   3 = cint32
# force_r2 is a boolean that forces radix-2 twiddles even if radix-4 is feasible when toggled.

    points = N

    if round(math.log(points,5)) - math.log(points,5) == 0:
        radix = 5
    elif ((round(math.log(points,4)) - math.log(points,4) == 0) and force_r2 == 0):
        radix = 4
    elif round(math.log(points,3)) - math.log(points,3) == 0:
        radix = 3
    elif round(math.log(points,2)) - math.log(points,2) == 0:
        radix = 2
    else:
        print("Supported radixes are only 2, 3, 4, and 5.\nRETURNING 0")
        return 0
    
# Creating the datatype coefficient, 1 is bfloat, 2 is cint16, 3 is cint32

    if datatype <= 1:
        dt_coef = 1
    elif datatype == 2:
        dt_coef = 2**15
    elif datatype == 3:
        dt_coef = 2**31
    else:
        print("Unsupported datatype. Please use\n1 = bfloat\n2 = cint16\n3 = cint32.\nRETURNING 0")
        return 0
    
    stages = round(math.log(points,radix))
    

# Creating a 4D vector for twiddle factors, indexed with:
    # Real and imaginary parts
    # Twiddle factor index
    # Twiddle factor array entry index (e.g. radix-4 has 3 entry per stage)
    # Stage index
    if datatype > 1:
        twiddles = np.zeros((2,points,radix-1,stages), dtype=np.int64)
    elif datatype == 1:
        twiddles = np.zeros((2,points,radix-1,stages), dtype=bfloat16)
    else:
        twiddles = np.zeros((2,points,radix-1,stages), dtype=np.float32)

# # Filling the twiddle factor tensor

    # Filling every stage
    for stage in range(stages):
                      
        N_stage_tw = radix**(stage+1)       # N_stage_tw is the number of points to process in the stage butterfly.
        
        # Filling every entry of the stage
        for entry in range(1,radix):
            # Filling every value of the entry
            for value in range(N_stage_tw//radix):
                angle = - 2*np.pi * entry * value / (N_stage_tw)
                if dt_coef > 1:
                    twiddles[0][value][entry-1][stage] = np.int64(dt_coef*np.real(np.exp(1j*angle)))
                    twiddles[1][value][entry-1][stage] = np.int64(dt_coef*np.imag(np.exp(1j*angle)))
                    # If needed, adjust the variable for the representable range: 2^dt_coef-1
                    if twiddles[0][value][entry-1][stage] == dt_coef:
                        twiddles[0][value][entry-1][stage] -= 1
                    if twiddles[1][value][entry-1][stage] == dt_coef:
                        twiddles[1][value][entry-1][stage] -= 1
                elif dt_coef == 1:
                    twiddles[0][value][entry-1][stage] = bfloat16(np.real(np.exp(1j*angle)))
                    twiddles[1][value][entry-1][stage] = bfloat16(np.imag(np.exp(1j*angle)))
                else:
                    twiddles[0][value][entry-1][stage] = np.float32(np.real(np.exp(1j*angle)))
                    twiddles[1][value][entry-1][stage] = np.float32(np.imag(np.exp(1j*angle)))

    return twiddles


def print_twiddles(points,datatype,force_r2):

    if round(math.log(points,5)) - math.log(points,5) == 0:
        radix = 5
    elif ((round(math.log(points,4)) - math.log(points,4) == 0) and force_r2 == 0):
        radix = 4
    elif round(math.log(points,3)) - math.log(points,3) == 0:
        radix = 3
    elif round(math.log(points,2)) - math.log(points,2) == 0:
        radix = 2
    else:
        print("Unsupported number of point for the supported radixes.\nSupported radixes are only 2, 3, 4, and 5.")
        return
        
    stages = round(math.log(points,radix))
    twiddles = make_twiddles(points,datatype,force_r2)

    print("// Twiddle vector indexes are stage number and entry")
    print("// Those twiddle factors are for RADIX -",radix,"FFT")

    print("#pragma once\n")

    # INT datatypes
    if datatype > 1:
        for stage in range(stages):
            N_stage_tw = radix**(stage+1)
            for entry in range(1,radix):
                print("#define TWID%01d_%01d {" %(stage,entry-1),end="")
                for value in range(N_stage_tw//radix):
                    if((value != 0) & (value%2 == 0)):
                        print(",\\\n\t\t ", end="")
                    print("{",f"{twiddles[0][value][entry-1][stage]:7d}".rjust(6),end=",")
                    print(f"{twiddles[1][value][entry-1][stage]:7d}".rjust(7),end="}")
                    if((value%2 != 1) & (value < N_stage_tw//radix - 1)):
                        print(", ",end=""),
                    elif(value == N_stage_tw//radix-1):
                        print("}")
    # Float datatypes
    else:
        for stage in range(stages):
                N_stage_tw = radix**(stage+1)
                for entry in range(1,radix):
                    print("#define TWID%01d_%01d {" %(stage,entry-1),end="")
                    for value in range(N_stage_tw//radix):
                        if((value != 0) & (value%2 == 0)):
                            print(",\\\n\t\t ", end="")
                        print("{",twiddles[0][value][entry-1][stage],end=",")
                        print(twiddles[1][value][entry-1][stage],end="}")
                        if((value%2 != 1) & (value < N_stage_tw//radix - 1)):
                            print(", ",end=""),
                        elif(value == N_stage_tw//radix-1):
                            print("}")
    return


def create_twiddles_header(points, datatype, force_r2, header_file_name):

    if round(math.log(points,5)) - math.log(points,5) == 0:
            radix = 5
    elif ((round(math.log(points,4)) - math.log(points,4) == 0) and force_r2 == 0):
            radix = 4
    elif round(math.log(points,3)) - math.log(points,3) == 0:
            radix = 3
    elif round(math.log(points,2)) - math.log(points,2) == 0:
            radix = 2
    else:
        print("Unsupported number of point for the supported radixes.\nSupported radixes are only 2, 3, 4, and 5.")
        return
        
    original_stdout = sys.stdout

    filename = header_file_name

    with open(filename, 'w') as f:
        sys.stdout = f
        print_twiddles(points, datatype, force_r2)
        sys.stdout = original_stdout
        return
    

def tw(n, radix, vec):
    n_stage = n / vec
    points = n_stage / radix
    return np.exp(-2j * np.pi * np.arange(1, radix).reshape(-1,1) * np.arange(0, points) / n_stage)


def print_mixed_tw(n, radixes, vectorizations, datatype):

# Checking for errors in the vectorizations and radixes lengths.

    if(len(vectorizations) != len(radixes)):
        print("ERROR: radixes and vectorizations vectors don't have the same length!")
        return

# Setting the coefficient for the desired datatype.
    if datatype <= 1:
        dt_coef = 1
    elif datatype == 2:
        dt_coef = 2**15
    elif datatype == 3:
        dt_coef = 2**31
    else:
        print("ERROR: Unsupported datatype. Please use\n1 = bfloat\n2 = cint16\n3 = cint32.")
        return
    
    stages = len(vectorizations)

    print("// Twiddle vector indexes are stage number and entry")
    print("#pragma once\n")

    # INT datatypes
    if datatype > 1:

        for stage in range(stages):

            radix = radixes[stage]
            vectorization = vectorizations[stage]
            twiddles = tw(n, radixes[stage], vectorizations[stage])
            N_stage_tw = (n//radix)//vectorization

            for entry in range(radix-1):
                print("#define TWID%01d_%01d {" %(stage,entry),end="")

                for value in range(N_stage_tw):
                    # Perform saturation for twiddle values equal to dt_coef
                    twiddle_real = np.int64(np.real(twiddles[entry][value] * dt_coef))
                    twiddle_imag = np.int64(np.imag(twiddles[entry][value] * dt_coef))
                    if twiddle_real == dt_coef:
                        twiddle_real -= 1
                    if twiddle_imag == dt_coef:
                        twiddle_imag -= 1

                    if((value != 0) & (value%2 == 0)):
                        print(",\\\n\t\t\t\t ", end="")
                    print("{",f"{twiddle_real:7d}".rjust(6),end=",")
                    print(f"{twiddle_imag:7d}".rjust(7),end="}")
                    if((value%2 != 1) & (value < N_stage_tw - 1)):
                        print(", ",end=""),
                    elif(value == N_stage_tw-1):
                        print("}")
    # Float datatypes
    else:
        for stage in range(stages):

            radix = radixes[stage]
            vectorization = vectorizations[stage]
            twiddles = tw(n, radixes[stage], vectorizations[stage])
            N_stage_tw = (n//radix)//vectorization

            for entry in range(radix-1):
                print("#define TWID%01d_%01d {" %(stage,entry),end="")

                for value in range(N_stage_tw):
                    
                    # Adjusting the datatype
                    if datatype == 0:
                        twiddle_real = np.float32(np.real(twiddles[entry][value] * dt_coef))
                        twiddle_imag = np.float32(np.imag(twiddles[entry][value] * dt_coef))
                    elif datatype == 1:
                        twiddle_real = bfloat16(np.real(twiddles[entry][value] * dt_coef))
                        twiddle_imag = bfloat16(np.imag(twiddles[entry][value] * dt_coef))

                    if((value != 0) & (value%2 == 0)):
                        print(",\\\n\t\t", end="")
                    print("{",twiddle_real,end=",")
                    print(twiddle_imag,end="}")
                    if((value%2 != 1) & (value < N_stage_tw - 1)):
                        print(", ",end=""),
                    elif(value == N_stage_tw-1):
                        print("}")
    return


def create_mixed_twiddles_header(n, radixes, vectorizations, datatype, header_file_name):

# Checking for errors in the vectorizations and radixes lengths.

    if(len(vectorizations) != len(radixes)):
        print("ERROR: radixes and vectorizations vectors don't have the same length!")
        return

# Setting the coefficient for the desired datatype.
    if datatype <= 1:
        dt_coef = 1
    elif datatype == 2:
        dt_coef = 2**15
    elif datatype == 3:
        dt_coef = 2**31
    else:
        print("ERROR: Unsupported datatype. Please use\n1 = bfloat\n2 = cint16\n3 = cint32.")
        return
    
    original_stdout = sys.stdout

    filename = header_file_name

    with open(filename, 'w') as f:
        sys.stdout = f

        print_mixed_tw(n,radixes,vectorizations,datatype)

        sys.stdout = original_stdout
        return

def create_variables_file(points, mixed_radix, radixes, vectorizations, force_r2):

    original_stdout = sys.stdout
    filename = "Twiddle_declaration.txt"

    with open(filename, 'w') as f:
        sys.stdout = f

        if mixed_radix == 1:
                stages = len(vectorizations)
                for stage in range(stages):
                    radix = radixes[stage]
                    for entry in range(radix-1):
                        N_stage_tw = (points//radix)//vectorizations[stage]
                        print("alignas(aie::vector_decl_align) static constexpr TT_TWID\ttw%01d_%01d[%d]\t= TWID%01d_%01d;"%(stage,entry,N_stage_tw,stage,entry))               
        elif mixed_radix == 0:
            if round(math.log(points,5)) - math.log(points,5) == 0:
                radix = 5
            elif ((round(math.log(points,4)) - math.log(points,4) == 0) and force_r2 == 0):
                radix = 4
            elif round(math.log(points,3)) - math.log(points,3) == 0:
                radix = 3
            elif round(math.log(points,2)) - math.log(points,2) == 0:
                radix = 2
            else:
                print("Unsupported number of point for the supported radixes.\nSupported radixes are only 2, 3, 4, and 5.")            
            stages = round(math.log(points,radix))
            for stage in range(stages):
                for entry in range(radix-1):
                    print("alignas(aie::vector_decl_align) static constexpr TT_TWID\ttw%01d_%01d[%d]\t= TWID%01d_%01d;"%(stage,entry,radix**stage,stage,entry))

        sys.stdout = original_stdout
        return
###############################################################################################################


datatype = 2
points = 1024

radixes = [4, 4, 4, 4, 4] 
vectorizations = [256, 64, 16, 4, 1]

mixed_radix = 0
force_r2 = 0
file = 1
variable_dec_file = 1
filename = "fft1k_single_twiddles.h"

if file == 1:
    if mixed_radix == 1:
        create_mixed_twiddles_header(points, radixes, vectorizations, datatype, filename)
    elif mixed_radix == 0:
        create_twiddles_header(points, datatype, force_r2, filename)
if variable_dec_file == 1:
    create_variables_file(points, mixed_radix, radixes, vectorizations, force_r2)