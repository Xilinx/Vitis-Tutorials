#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11

import sys
import os
import numpy as np
import array as arr

matDim = int(sys.argv[1])
inp_type = int(sys.argv[2])
iterCnt = int(sys.argv[3])
pliowidth = 128
dataSamps = int(pliowidth / 16)

#os.system("rm -rf gemm*")

def printMat(mat,rows,cols,dataSamp,file_name,base_row=0,base_col=0,perl=0):
   dataCtr = 0
   fptr = open(file_name, "a+")
   for row in range(base_row,(base_row + rows)):
      for col in range(base_col,(base_col + cols)):
         if dataCtr < (dataSamp - 1):
            if perl == 1:
               fptr.write(str(mat[row][col]) + " 0 ")
            else:
               fptr.write(str(mat[row][col]) + " ")
            dataCtr = dataCtr + 1
         elif dataCtr == (dataSamp - 1):
            if perl == 1:
               fptr.write(str(mat[row][col]) + " 0\n")
            else:
               fptr.write(str(mat[row][col]) + "\n")
            dataCtr = 0
   fptr.close()

def aiesimInp_gen(dataSamp,inpFile_name,outFile_name,iters):
   inpFptr = open(inpFile_name, "r")
   outFptr = open(outFile_name, "+a")
   dataCtr = 0
   for itr in range(iters):
      for line in inpFptr:
         if dataCtr < (dataSamp - 1):
            outFptr.write(line.strip() + " ")
            dataCtr = dataCtr + 1
         elif dataCtr == (dataSamp - 1):
            outFptr.write(line.strip() + "\n")
            dataCtr = 0
      inpFptr.seek(0)
   inpFptr.close()
   outFptr.close()

# Creating Mat A, B and corresponding C...
## Constant Input...
if inp_type == 0:
   matA = np.full((matDim,matDim), 1, dtype=int)
   matB = np.full((matDim,matDim), 2, dtype=int)

## Increasing matrix A with B as Identity...
elif inp_type == 1:
   matA = np.arange(int(matDim * matDim)).reshape(matDim, -1)
   matB = np.identity(matDim, dtype=int)

## Random Matrix A and B...
else:
   matA = np.random.randint(0, 10, size=(matDim, matDim))
   matB = np.random.randint(0, 10, size=(matDim, matDim))

matC = np.matmul(matA, matB)

# Defining splits and cascs based on graph...
splits = 8
cascs = 4

# Creating IO Directories...
ioDir = "gemm_" + str(matDim) + "x" + str(matDim) + "x" + str(matDim) + "_ioFiles"
os.system("mkdir " + ioDir)

# MatA IO Files Creation...
inpA_python = ioDir + "/a_python.txt"
printMat(matA,matDim,matDim,matDim,inpA_python)
inpA_python_serial = ioDir + "/a_python_serial.txt"
printMat(matA,matDim,matDim,1,inpA_python_serial,0,0,1)
os.chdir(ioDir)
os.system("perl $DSPLIB_VITIS/dsp/L2/tests/aie/common/scripts/matrix_mult_partition_shuffle.pl -f a_python_serial.txt -r " + str(matDim) + " -c " + str(matDim) + " -T_DATA_A int16 -T_DATA_B int16 -p 4")
for casc in range(cascs):
   aiesimInp_gen(int(dataSamps / 2),"tiled_a_python_serial_" + str(casc) + ".txt","a0_casc" + str(casc) + ".txt",iterCnt)
os.chdir("../")

# MatB IO Files Creation...
inpB_python = ioDir + "/b_python.txt"
printMat(matB,matDim,matDim,matDim,inpB_python)
inpB_python_serial = ioDir + "/b_python_serial.txt"
printMat(matB,matDim,matDim,2,inpB_python_serial)
os.chdir(ioDir)
base = 0
for split in range(splits):
   printMat(matB,matDim,int(matDim / splits),1,"b" + str(split) + ".txt",0,base,1)
   base = base + int(matDim / splits)
   os.system("perl $DSPLIB_VITIS/dsp/L2/tests/aie/common/scripts/matrix_mult_partition_shuffle.pl -f b" + str(split) + ".txt -r " + str(matDim) + " -c " + str(int(matDim / splits)) + " -T_DATA_A int16 -T_DATA_B int16 -p 4 --splitRows")
   for casc in range(cascs):
      aiesimInp_gen(int(dataSamps / 2),"tiled_b" + str(split) + "_" + str(casc) + ".txt","b" + str(split) + "_casc" + str(casc) + ".txt",int(iterCnt * splits))

os.chdir("../")

# MatC IO Files Creation...
outC_python = ioDir + "/c_python.txt"
printMat(matC,matDim,matDim,matDim,outC_python)
outC_python_serial = ioDir + "/c_python_serial.txt"
printMat(matC,matDim,matDim,2,outC_python_serial,0,0,1)
os.chdir(ioDir)
os.system("perl $DSPLIB_VITIS/dsp/L2/tests/aie/common/scripts/matrix_mult_partition_shuffle.pl -f c_python_serial.txt -r " + str(matDim) + " -c " + str(matDim) + " -T_DATA_A int16 -T_DATA_B int16 -p 8 --splitRows")
for split in range(splits):
   aiesimInp_gen(dataSamps,"c_python_serial_" + str(split) + ".txt","c" + str(split) + ".txt",iterCnt)

os.chdir("../")

#os.system("chmod 755 -R gemm_*")
