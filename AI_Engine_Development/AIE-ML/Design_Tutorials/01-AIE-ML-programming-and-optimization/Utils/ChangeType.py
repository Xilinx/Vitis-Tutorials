#!/usr/bin/env python3
#
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#



from re import I
import sys
import numpy as np
from math import *


def main(args):
    fin = args[1]
    fout = args[2]
    print("Input File : " + fin)
    print("Output File: " + fout)

    Nb = 32
    fpin = open(fin,'r')
    fpout = open(fout,'w')

    for line in fpin:
        w = line.split()
        for i in range(len(w)):
            print(uint2int(int(w[i]),Nb),end=' ',file=fpout)
        print(" ",file=fpout)

    fpout.close()




def uint2int(i,NBits):
    if i>=(1<<(NBits-1)):
        return(i-(1<<(NBits)))
    else:
        return i

if __name__ == '__main__':
    main(sys.argv)
