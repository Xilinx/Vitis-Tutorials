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
    R = int(args[2])
    C = int(args[3])
    N = int(args[4])
    Rt = int(args[5])
    Ct = int(args[6])
    Rind = int(args[7])
    Cind = int(args[8])
    Nind = int(args[9])



    print("Input File :     " + fin)
    print("Matrix Size:     " + str(R) + "x" + str(C))
    print("Tile   Size:     " + str(Rt) + "x" + str(Ct))
    print("Number of Tiles: " + str(int(R/Rt)) + "x" + str(int(C/Ct)))
    print("Tile  Index:     " + str(Rind) + "x" + str(Cind))
    print("Iterations : max " + str(N) + "     extract " + str(Nind))

    A = np.zeros((R*C*N))

    fpin = open(fin,'r')
    ind=0

    for line in fpin:
        w = line.split()
        for i in range(len(w)):
            A[ind] = int(w[i])
            ind = ind+1
    fpin.close

    off1 = (Nind-1)*R*C + (Rind-1)*Rt*C + (Cind-1)*Ct
    for i in range(Rt):
        off2 = off1 + i*C
        for j in range(Ct):
            k = off2 + j
            if(j<Ct-1):
                print("%8d " % (A[k]),end='')
            else:
                print("%8d " % (A[k]),end='\n')



if __name__ == '__main__':
    main(sys.argv)
