#!/usr/bin/env python3
#
#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT
#



import sys
import random
import numpy as np

def generateRandomArray(TileNRows,TileNCols,NTilesPerRow,NTilesPerCol,NBits,Sparsity):
    NRows = TileNRows*NTilesPerRow
    NCols = TileNCols*NTilesPerCol

    if NBits==8:
        Type='int8'
    elif NBits==16:
        Type == 'int16'
    elif NBits==32:
        Type = 'int32'
    else:
        print("Wrong Number of Bites. Only 8, 16, 32 supported")

    A = np.zeros((NRows,NCols))
    A = A.astype(Type)

    NElemTotal = TileNRows*TileNCols
    NElem = int(NElemTotal*Sparsity)
    MinVal = int(-(2**(NBits-1)))
    MaxVal = int((2**(NBits-1)) -1)

    for tr in range(NTilesPerRow):
        for tc in range(NTilesPerCol):
            Ta = np.zeros((TileNRows,TileNCols))
            Ta = Ta.astype(Type)
            for n in range(NElem):
                row = random.randint(0,TileNRows-1)
                col = random.randint(0,TileNCols-1)
                val = random.randint(MinVal,MaxVal)
                Ta[row,col]=val
            A[tr*TileNRows:(tr+1)*TileNRows,tc*TileNCols:(tc+1)*TileNCols] = Ta
    return A

def saveArray(M,widthPLIO,filename,access):
    NBytes = 0
    fmt = '%2d '
    if M.dtype == 'int8':
        NBits = 8
        fmt='%4d '
    elif M.dtype == 'int16':
        NBits = 16
        fmt='%6d '
    if M.dtype == 'int32':
        NBits = 32
        fmt='%8d '

    fp = open(filename,access)
    DCol = int(widthPLIO/NBits)
    (NRows,NCols) = M.shape
    n = 0
    for r in range(NRows):
        for c in range(NCols):
            print(fmt % (M[r,c]),end='',file=fp)
            n = n+1
            if n%DCol == 0:
                print("",file=fp)
    fp.close()


def main(args):
    ans = 1
    NRows = int(args[1])
    NInter = int(args[2])
    NCols = int(args[3])
    NsubRows = int(args[4])
    NsubInter = int(args[5])
    NsubCols = int(args[6])
    NBits = int(args[7])
    NIterations = int(args[8])
    Sparsity = float(args[9])
    Directory = args[10]
    widthPLIO = int(args[11])

    print("NRows    : " + str(NRows))
    print("NInter   : " + str(NInter))
    print("NCols    : " + str(NCols))
    print("NsubRows : " + str(NsubRows))
    print("NsubInter: " + str(NsubInter))
    print("NsubCols : " + str(NsubCols))
    print("NBits    : " + str(NBits))
    print("NIterations    : " + str(NIterations))
    print("Sparsity       : " + str(Sparsity))
    print("Data Directory : " + Directory)
    print("NBits PLIO     : " + str(widthPLIO))

    # Sparse Mode:
    # Size of the sparse matrix
    # 4x16x8 --> 16 rows x 8 colums
    # 8x16x8 --> 16 rows x 8 colums
    # 4x16x16 --> 16 rows x 16 colums
    # 2x8x8 --> 8 rows x 8 colums
    # 4x8x8 --> 8 rows x 8 colums
    # 2x8x16 --> 8 rows x 16 colums


    ATRows = NsubRows
    ATCols = NsubInter
    BTRows = NsubInter
    BTCols = NsubCols

    ANTilesPerRow = int(NRows/ATRows)
    ANTilesPerCols = int(NInter/ATCols)
    BNTilesPerRow = int(NInter/BTRows)
    BNTilesPerCols = int(NCols/BTCols)



    for iter in range(NIterations):
        A = generateRandomArray(ATRows,ATCols,ANTilesPerRow,ANTilesPerCols,NBits,Sparsity)
        B = generateRandomArray(BTRows,BTCols,BNTilesPerRow,BNTilesPerCols,NBits,Sparsity)
        C = np.matmul(A.astype('int32'),B.astype('int32'))


        # Save onto input files nd ref files
        if iter == 0:
            access = "w"
        else:
            access = "a"

        filename = Directory + "/inputA_"+str(widthPLIO)+".txt"
        saveArray(A,widthPLIO,filename,access)

        filename = Directory + "/inputB_"+str(widthPLIO)+".txt"
        saveArray(B,widthPLIO,filename,access)

        filename = Directory + "/outputC_ref_"+str(widthPLIO)+"_32b.txt"
        saveArray(C,widthPLIO,filename,access)

        filename = Directory + "/outputC_ref_"+str(widthPLIO)+"_16b.txt"
        C16 = C>>6;
        C16 = C16.astype('int16')
        saveArray(C16,widthPLIO,filename,access)


if __name__ == '__main__':
    main(sys.argv)
