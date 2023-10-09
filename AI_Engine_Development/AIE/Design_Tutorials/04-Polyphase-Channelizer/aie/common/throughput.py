#!/tools/gensys/anaconda3/bin/python3
#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

import numpy as np
from math import *
import sys
import argparse

def GetTime_ns(Stamp):
    Time_ns = float(Stamp[1])
    if(Stamp[2] == 'ps'):
        Time_ns = Time_ns/1000.0
    elif(Stamp[2] == 'us'):
        Time_ns = Time_ns*1000.0
    elif(Stamp[2] == 'ms'):
        Time_ns = Time_ns*1000000.0
    elif(Stamp[2] == 's'):
        Time_ns = Time_ns*1000000000.0
    return(Time_ns)

def ReadFile(filename):
    # Detect the number of data per PLIO output
    fdr = open(filename,'r')
    ts = fdr.readline()
    d = fdr.readline()
    dw = d.split()
    fdr.close()
    coltime = 0
    coldata = 1
    numdata = len(dw)
    coltlast = numdata + 1
    # Initializes the output array
    # Format: timestamp (in ns) val1 val2 ... valN TLAST (0 or 1)
    a = np.zeros((0,numdata+2))
    fdr = open(filename,'r')
    line = ' '
    lnum = 0;
    while line !="" :
        line = fdr.readline()
        if line=='':
            continue
        res = line.split()
        if(res[0] != 'T'): # It should be a timestamp
            continue
        l = np.zeros((1,numdata+2))
        # Extract the time stamp
        l[0][0] = GetTime_ns(res)
        line = fdr.readline()
        res = line.split()
        # extract the TLAST
        if(res[0]=='TLAST'):
            tlast = 1
            line = fdr.readline()
            res = line.split()
        else:
            tlast = 0
            l[0,coltlast] = tlast
            # Extract all values
            for i in range(numdata):
                l[0,i+1] = float(res[i])
            # Appends to the whole array
            a = np.append( a , l,axis=0)
    fdr.close()
    return(a)

def Throughput(Filename,IsComplex):
    V = ReadFile(Filename)
#    print("\n==============================")
#    print(Filename)
#    print("\n")
    NRows = V.shape[0]
    NCols = V.shape[1]
    NFullFrames = int(np.sum(V[:,NCols-1]))
#    print("Number of Full Frames: " + str(NFullFrames))
    # Basic Throughput computation
    if IsComplex:
        Ratio = 0.5
    else:
        Ratio = 1
    RawThroughputMsps = float(NRows*(NCols-2))/(V[NRows-1,0]-V[0,0])*Ratio*1000.0
    print(" Raw Throughput: %.2f (Msps)" % RawThroughputMsps)
    # If the output is frame based, compute a more precise throughput
    tlast = np.where(V[:,NCols-1] == 1.0)
    if(len(tlast[0])<=1):
        TotalThroughput = RawThroughputMsps
    else:
        tlast = tlast[0]
        EndRow = tlast[len(tlast)-2]+1
        # EndRow is the number of Rows I take into account for the number of datasource
        # The timestamp I am interested in is the timestamp of the next transaction
        TotalThroughputMsps = float(EndRow*(NCols-2))/(V[EndRow,0]-V[0,0])*Ratio*1000.0
        print(" Throughput: %.2f" % TotalThroughputMsps)
#    print("\n")

# Entry point of this file
if __name__ == "__main__":
    parser = argparse.ArgumentParser(prog=sys.argv[0], description='Compute the throughput corresponding to some output of AIE Simulations')
    parser.add_argument('--iscomplex', action='store_true', help='Indicates Complex data in the file')
    parser.add_argument('filename',nargs='+')
    Args = sys.argv
    Args.pop(0)
    args = parser.parse_args(Args)

for f in args.filename:
    Throughput(f,args.iscomplex)

