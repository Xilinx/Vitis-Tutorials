##
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
##

#!/usr/bin/env python3

import os
import sys
import numpy as np

OK = 1

if (len(sys.argv) != 4):
    OK = 0

if (OK == 0):
    print ("Usage: CreateLinearData DataType PLIO_Width Length", file = sys.stderr)
    exit(1)

DataType = sys.argv[1];
PLIO_Width = int(sys.argv[2])
Length = int(sys.argv[3])

Vmin = 0
Vmax = 0
Nbits = 0

if ( DataType == 'uint8'):
    Vmax = 255
    Nbits = 8
elif(DataType == 'int8'):
    Vmin = -128
    Vmax = 127
    Nbits=8
elif(DataType == 'uint16'):
    Vmax = 65535
    Nbits=16
elif(DataType == 'int16'):
    Vmin = -32768
    Vmax = 32767
    Nbits = 16
elif(DataType == 'uint32'):
    Vmax = 4294967295
    Nbits=32
elif(DataType == 'int32'):
    Vmin = -2147483648
    Vmax = 2147483647
    Nbits = 32

print("Data Type: ",DataType, file = sys.stderr)
print("    Min: ",int(Vmin), file = sys.stderr)
print("    Max: ",int(Vmax), file = sys.stderr)
print("  Nbits: ",int(Nbits), file = sys.stderr)

NDataPerLine = int(PLIO_Width)/int(Nbits)

print("\nTotal Length: ", str(Length), file = sys.stderr)
print("\nN Data Per Line: ", str(NDataPerLine), file = sys.stderr)


V = 0

for i in range(Length):
    if(i%NDataPerLine != NDataPerLine-1):
        print(str(V),end = "\t")
    else:
        print(str(V))

    V = V+1
    if(V>Vmax):
        V = Vmin

print("")
