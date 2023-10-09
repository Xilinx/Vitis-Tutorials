#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11


import sys
import os
#import numpy as np
#import array as arr

fftPts = [32,64,128,256,1024]
iterCnt = 16
pliowidth = 128

os.system("rm -rf c*")

for fftPt in fftPts:
   ioDir = "cint16_IOfiles/fft2d_" + str(fftPt) + "x" + str(fftPt * 2)  + "_ioFiles"
   os.system("mkdir -p " + ioDir)
   fptr_row_cint16=open(ioDir+"/input0.txt","a+")
   fptr_col_cint16=open(ioDir+"/input1.txt","a+")
   for j in range(0,iterCnt):
      for i in range(0,(int)((fftPt*2*fftPt)/4)):
         if(i==0):
            fptr_row_cint16.write("1 1 0 0 0 0 0 0"+"\n")
         else:
            fptr_row_cint16.write("0 0 0 0 0 0 0 0"+"\n")
   for i in range(0,iterCnt):
      for j in range(0,(int)((fftPt*2*fftPt)/4)):
        if(j==0):
           fptr_col_cint16.write("1 1 0 0 0 0 0 0"+"\n")
        else:
           fptr_col_cint16.write("0 0 0 0 0 0 0 0"+"\n")
fptr_row_cint16.close()
fptr_col_cint16.close()

for fftPt in fftPts:
  ioDir = "cfloat_IOfiles/fft2d_" + str(fftPt) + "x" + str(fftPt * 2)  + "_ioFiles"
  os.system("mkdir -p " + ioDir)
  fptr_row_cfloat=open(ioDir+"/input0.txt","a+")
  fptr_col_cfloat=open(ioDir+"/input1.txt","a+")
  for j in range(0,iterCnt):
     for i in range(0,(int)((fftPt*2*fftPt)/2)):
        if(i==0):
           fptr_row_cfloat.write("1.5 1.5 0 0"+"\n")
        else:
           fptr_row_cfloat.write("0 0 0 0"+"\n")
  for i in range(0,iterCnt):
     for j in range(0,(int)((fftPt*2*fftPt)/2)):
       if(j==0):
          fptr_col_cfloat.write("1.5 1.5 0 0"+"\n")
       else:
          fptr_col_cfloat.write("0 0 0 0"+"\n")

fptr_row_cfloat.close()
fptr_col_cfloat.close()

os.system("chmod 755 -R *")
