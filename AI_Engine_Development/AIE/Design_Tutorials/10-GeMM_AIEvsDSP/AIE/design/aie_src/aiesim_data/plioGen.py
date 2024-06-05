#Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: MIT

import sys
import os
import shutil
import numpy as np
import array as arr

gemmSize = int(sys.argv[1])
iterCnt = int(sys.argv[2])

if gemmSize==32:
    i=0
elif gemmSize==64:
    i=1
elif gemmSize==128:
    i=2
elif gemmSize==256:
    i=3
elif gemmSize==512:
    i=4
elif gemmSize==1024:
    i=5

if (iterCnt<1 or iterCnt>16):
    iterCnt = 16

gemm_dir_name = np.array(["gemm_32x32x32_ioFiles", "gemm_64x64x64_ioFiles", "gemm_128x128x128_ioFiles", "gemm_256x256x256_ioFiles", "gemm_512x512x512_ioFiles", "gemm_1024x1024x1024_ioFiles"])

A_val = "1 1 1 1 1 1 1 1"
B_val = "2 2 2 2 2 2 2 2"
ZP_val = "0 0 0 0 0 0 0 0"

matA_file = np.array(["a0_casc0.txt", "a0_casc1.txt", "a0_casc2.txt", "a0_casc3.txt", "a0_casc4.txt", "a0_casc5.txt", "a0_casc6.txt", "a0_casc7.txt"])
matB_file = np.array(["b0_casc0.txt", "b0_casc1.txt", "b0_casc2.txt", "b0_casc3.txt", "b0_casc4.txt", "b0_casc5.txt", "b0_casc6.txt", "b0_casc7.txt","b1_casc0.txt", "b1_casc1.txt", "b1_casc2.txt", "b1_casc3.txt", "b1_casc4.txt", "b1_casc5.txt", "b1_casc6.txt", "b1_casc7.txt", "b2_casc0.txt", "b2_casc1.txt", "b2_casc2.txt", "b2_casc3.txt", "b2_casc4.txt", "b2_casc5.txt", "b2_casc6.txt", "b2_casc7.txt"])

SPLIT = 3
CASC_LN = 8

WRD_LN = 8 

GEMM_SIZE = np.array([32, 64, 128, 256, 512, 1024])

GEMM_SIZE_ZP_A = np.array([32,64,132,264,512,1024])
GEMM_SIZE_ZP_B = np.array([36,72,132,264,528,1056])

DIM_A = np.array([16,32,44,44,16,16])
DIM_B = np.array([12,24,44,44,16,16])

GEMM_SZ_CASC = (GEMM_SIZE/CASC_LN).astype(int)
GEMM_SZ_SPLIT = (GEMM_SIZE_ZP_B/SPLIT).astype(int)

NON_ZP_BLK_A = (GEMM_SIZE/DIM_A).astype(int)
ZP_BLK_A = ((GEMM_SIZE_ZP_A/DIM_A) - NON_ZP_BLK_A).astype(int)

ROWS_ZP_BLK_A = (GEMM_SZ_CASC*(DIM_A*ZP_BLK_A - (GEMM_SIZE_ZP_A - GEMM_SIZE))).astype(int)
ROWS_ZP_BLK_B = ((GEMM_SIZE_ZP_B/SPLIT) - (GEMM_SIZE_ZP_B - GEMM_SIZE)).astype(int)

TOTAL_A_PER_CH = (GEMM_SZ_CASC*(ZP_BLK_A+NON_ZP_BLK_A)*DIM_A/WRD_LN).astype(int)
NON_ZP_A_PER_CH = ((GEMM_SZ_CASC*DIM_A*NON_ZP_BLK_A/WRD_LN) + (ROWS_ZP_BLK_A/WRD_LN)).astype(int)

TOTAL_B_PER_CH = (GEMM_SZ_SPLIT*GEMM_SZ_CASC/WRD_LN).astype(int)
NON_ZP_BLK_B = (ROWS_ZP_BLK_B/DIM_B).astype(int)
NON_ZP_B_PER_CH = (GEMM_SZ_CASC*DIM_B*NON_ZP_BLK_B/WRD_LN).astype(int)

ZP_BLK_B = ((GEMM_SIZE_ZP_B/SPLIT/DIM_B) - (NON_ZP_BLK_B)).astype(int)
TOTAL_SUB_B_PER_CH = (ZP_BLK_B*DIM_B/2).astype(int)

NON_ZP_SUB_PER_CH = ((ZP_BLK_B*DIM_B - (GEMM_SIZE_ZP_B - GEMM_SIZE))/2).astype(int)

#loop iteration count for A and B
MATA_SZ = (((GEMM_SIZE_ZP_A * GEMM_SIZE / CASC_LN) / 8) * iterCnt * ((GEMM_SIZE_ZP_B/SPLIT) / DIM_B)).astype(int)
MATB_SZ = (((GEMM_SIZE * GEMM_SIZE_ZP_B  / (CASC_LN*SPLIT) ) / 8) * iterCnt * (GEMM_SIZE_ZP_A / DIM_A)).astype(int)

#creating directory for different gemm sizes
current_working_directory = os.getcwd()


path = os.path.join(current_working_directory,gemm_dir_name[i])
if os.path.exists(path):
    shutil.rmtree(path)
os.mkdir(path)
for k in range(0,8) :
    a0 = os.path.join(path, matA_file[k])
    with open(a0, 'w+') as file:
        for j in range(0,MATA_SZ[i]) :
            if (j%TOTAL_A_PER_CH[i]) > (NON_ZP_A_PER_CH[i] - 1):
                file.write(ZP_val+"\n")
            else:
                file.write(A_val+"\n")
for m in range(0,24) :
    b0 = os.path.join(path, matB_file[m])
    if(m<16):
        with open(b0, 'w+') as file:
            for n in range(0,MATB_SZ[i]) :
                file.write(B_val+"\n")
    else:
        with open(b0, 'w+') as file:
            for q in range(0,MATB_SZ[i]) :
                if (((q%TOTAL_B_PER_CH[i]) > (NON_ZP_B_PER_CH[i] - 1)) and (q%(TOTAL_SUB_B_PER_CH[i]) >
                    (NON_ZP_SUB_PER_CH[i] - 1))).all():
                    file.write(ZP_val+"\n")
                else:
                    file.write(B_val+"\n")


