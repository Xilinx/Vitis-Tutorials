#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#


import matplotlib.pyplot as plt
import numpy as np
from math import *


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

def ProcessSinglePhase(filename):

    fdr = open(filename,'r')
    line = '  '
    a = np.zeros((0,3))

    while line!="":
        line = fdr.readline()
        if line=='':
            continue

        res = line.split()
        if(res[0] == 'TLAST'):
            continue
        elif(res[0] == 'T'):
            currentTime = GetTime_ns(res)
        else:
            a = np.append( a , np.array([[ currentTime-1, int(res[0]), int(res[1])]]),axis=0)
            a = np.append( a , np.array([[ currentTime, int(res[2]), int(res[3])]]),axis=0)
    fdr.close()

    print(a)
    np.savetxt('out.txt', a, delimiter=' ')

    plt.subplot(2,1,1)
    plt.plot(a[:,0],a[:,1])
    plt.title('With Respect to Timestamps')
    plt.subplot(2,1,2)
    plt.plot(a[:,1])

    plt.show()


def ProcessMultiPhases(FileList):
    #Open all files
    fdr = [open(path, 'r') for path in FileList]
    Nf = len(fdr)

    line = ' '
    out = np.zeros((0,2))
    a = np.zeros((2,2*Nf))
    Atot = np.zeros((0,2*Nf))


    while line!="":
        for i in range(Nf):
            line = fdr[i].readline()
            # print('Discard Line : ',line)
            line = fdr[i].readline()
            # print('Process Line : ',line)
            if(line == ""):
                continue

            res = line.split()
            a[0,2*i] = int(res[0])
            a[0,2*i+1] = int(res[1])
            a[1,2*i] = int(res[2])
            a[1,2*i+1] = int(res[3])

        Atot  = np.append( Atot , a,axis=0)

        if(line == ""):
            continue

        for l in range(2):
            for i in range(Nf):
                out = np.append( out , np.array([[int(a[l,2*i]), int(a[l,2*i+1])]]),axis=0)


    out = out.astype('int32')
    Atot = Atot.astype('int32')
    np.savetxt('out.txt',out,delimiter=" ",fmt="%8d")
    np.savetxt('Atot.txt',Atot,delimiter=" ",fmt="%8d")

    plt.subplot(2,1,1)
    plt.plot(out[:,0])
    plt.title('Real part')

    plt.subplot(2,1,2)
    plt.plot(out[:,1])
    plt.title('Imaginary part')

    plt.show()

def ProcessMultiPhasesMultiStreams(FileList):
    #Open all files
    fdr = [open(path, 'r') for path in FileList]
    Nf = len(fdr)
    NPhi = int(Nf/2)
    NLinesOnEachStream = 2 # 128 bits on each stream (each stream packet is 64 bits)

    line = ' '
    out = np.zeros((0,2))
    a = np.zeros((2,2*NPhi))
    Atot = np.zeros((0,2*NPhi))

    ReadLineNumber = 0

    while line!="":
        ReadStream = int(ReadLineNumber/NLinesOnEachStream)%2
        ReadLineNumber = ReadLineNumber + 1

        for i in range(NPhi):
            line = fdr[2*i+ReadStream].readline()
            # print('Discard Line : ',line)
            line = fdr[2*i+ReadStream].readline()
            # print('Process Line : ',line)
            if(line == ""):
                continue

            res = line.split()
            a[0,2*i] = int(res[0])
            a[0,2*i+1] = int(res[1])
            a[1,2*i] = int(res[2])
            a[1,2*i+1] = int(res[3])

        Atot  = np.append( Atot , a,axis=0)

        if(line == ""):
            continue

        for l in range(2):
            for i in range(NPhi):
                out = np.append( out , np.array([[int(a[l,2*i]), int(a[l,2*i+1])]]),axis=0)

    out = out.astype('int32')
    Atot = Atot.astype('int32')
    np.savetxt('out.txt',out,delimiter=" ",fmt="%8d")
    np.savetxt('Atot.txt',Atot,delimiter=" ",fmt="%8d")

    plt.subplot(2,1,1)
    plt.plot(out[:,0])
    plt.title('Real part')

    plt.subplot(2,1,2)
    plt.plot(out[:,1])
    plt.title('Imaginary part')

    plt.show()



def ProcessMultiFiles(FileList):
    # sort Files to have them in the right order
    FileList.sort()
    print(FileList)

    T = FileList[0].split('_')
    L = len(T)

    if(L==1):
        print('No Id for multi-phase or multi-stream ... Process as single phase')
        ProcessSinglePhase(FileList[0])
        return()
    elif(L==2):
        print('Multi-Phase output')
        ProcessMultiPhases(FileList)
    elif(L==3):
        print('Multi-Stream, Multi-Phase output')
        ProcessMultiPhasesMultiStreams(FileList)
    else:
        print('Too many fields in the filenames')


def ComputeThroughput(FileList):
    FileList.sort()

    Throughput = 0
    TotalThroughput = 0;

    for fname in FileList:
        fdr = open(fname,'r')

        NData = 2

        line = fdr.readline()
        res = line.split()
        StartTime = GetTime_ns(res)
        StopTime = StartTime

        while line != "":
            line = fdr.readline() # 2 data
            line = fdr.readline()
            if(line != ""):
                res = line.split()
                StopTime = GetTime_ns(res)
                NData = NData+2 # Each data line contains 2 data (cint16 on a 64 bit PLIO)


        if(StartTime == StopTime):
            print(fname, ' --> No output')
        else:
            Throughput = NData*1000.0/(StopTime-StartTime)
            formatstr = '%s --> %8.2f Msps' % (fname, Throughput)
            print(formatstr)

        TotalThroughput = TotalThroughput + Throughput


    print('\n-----------------------\n\n')
    if(TotalThroughput == 0):
        print('No Output from these files')
    else:
        formatstr = 'Total Throughput --> %10.2f Msps\n\n' % (TotalThroughput)
        print(formatstr)
