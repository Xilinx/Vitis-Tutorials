#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#


import numpy as np
from math import *
import random

def GenerateTestVectorGUI(dtval,pliow,NPhases_s,NStreams_s,NSamples_s,NFrames_s,SeqType_s,Basename_s):
    dt = dtval.get()
    pliowidth = pliow.get()
    NPhases = int(NPhases_s.get())
    NStreams = int(NStreams_s.get())
    LFrame = int(NSamples_s.get())
    NFrames = int(NFrames_s.get())
    SequenceType = SeqType_s.get()
    Basename = Basename_s.get()

    GenerateTestVector(dt,pliowidth,NPhases,NStreams,LFrame,NFrames,SequenceType,Basename)

def GenerateTestVector(dtval,pliow,NPhases,NStreams,LFrame,NFrames,SequenceType,Basename):
    print('Data type : ',dtval)
    print('PLIO width : ',pliow)
    print('Number of Phases : ',NPhases)
    print('Number of Streams : ',NStreams)
    print('Frame Length : ',LFrame)
    print('Number of Frames : ',NFrames)
    print('Type of Sequence : ',SequenceType)
    print('Base filename : ',Basename)



    #parameters that should be in the GUI
    # SequenceType ='Linear'   # 'SinCos' 'Linear'  'Random' 'Dirac'
    # Basename = 'PhaseIn'

    NSamples =  NPhases*NStreams*LFrame*NFrames;
    NSamples1 =  NPhases*NStreams*LFrame*(NFrames+1); # A little longer to allow for delay in streams


    NBitsData = 32;
    if( dtval == 'int16'):
        NBitsData = 16


    HasImag = 0
    if (dtval == 'cint16'):
        HasImag = 1

    if(SequenceType != 'SinCos' and SequenceType != 'Linear' and SequenceType != 'Random' and SequenceType != 'Dirac'):
        print ('Unknown Sequence Type')
        return

    # Create the overall signal that will be distributed over all streams
    # it is already separated in phases
    S = np.zeros((NPhases,int(NSamples1/NPhases),1+HasImag))
    for i in range(int(NSamples1/NPhases)):
        for p in range (NPhases):
            k = i*NPhases+p
            if (SequenceType == 'SinCos'):
                vr = int(5000*cos(6.28*5/(NPhases*NStreams*LFrame)*k))
                vi = int(5000*sin(6.28*5/(NPhases*NStreams*LFrame)*k))
            elif (SequenceType == 'Linear'):
                vr = k
                vi = -k
            elif (SequenceType == 'Random'):
                vr = random.randint(-5000,5000)
                vi = random.randint(-5000,5000)
            elif (SequenceType == 'Dirac'):
                vr = 0
                vi = 0
                if(k%151 == 1):
                    vr = 1
                elif(k%151 == 40):
                    vi = 1
                elif(k%151 == 81):
                    vr = 2
                elif(k%151 == 115):
                    vi = -2
                # if(k%311 == 50):
                #     vr = 1

            # S[p,i,0] =
            # if(HasImag==1):
            #     S[p,i,1] = int(5000*sin(6.28*5/(NPhases*NStreams*LFrame)*k))
            S[p,i,0] = vr
            if (HasImag == 1 ):
                S[p,i,1] = vi


    PLIOwidth = pliow
    NSamplesPerLine = int(PLIOwidth/NBitsData)  # Data are read in blocks of 128 bits (4 data in cint16)


    # Create an Input test Vector in TestInputS.txt

    FileNames = [];


    # Easiest case: 1 stream per AI Engine
    if (NStreams == 1):
        #Creates list of filenames
        for Phi in range(NPhases):
            FileNames.append(Basename+'_'+str(Phi)+'.txt')

        #Open all files
        fds = [open(path, 'w') for path in FileNames]

        #Fill all files with the right data
        for p in range(NPhases):
            fd = fds[p]
            for s in range(int(NSamples1/NPhases/NSamplesPerLine)):
                for d in range(NSamplesPerLine):
                    index = s*NSamplesPerLine + d
                    fd.write(str(int(S[p,index,0]))+'  ')
                    if(HasImag):
                        fd.write(str(int(S[p,index,1]))+'  ')
                fd.write('\n')

        for fd in fds:
            fd.close()


    if (NStreams == 2):
        #Creates list of filenames
        for Phi in range(NPhases):
            for Stream in range(NStreams):
                FileNames.append(Basename+'_'+str(Phi)+'_'+str(Stream)+'.txt')


        # Hash table to associate data to streams
        NSamplesIn128bits = int(128/NBitsData )
        H = np.zeros((int(NSamples1/NPhases/2),2))
        H = H.astype('int32')
        index = np.zeros(2)
        index = index.astype('int32')

        for s in range(int(NSamples1/NPhases)):
            k = int(s/NSamplesIn128bits) # Block order
            i = k%2 # Which streams
            H[index[i],i] = s
            index[i] = index[i]+1

        #Open all files
        fds = [open(path, 'w') for path in FileNames]

        #Fill all files with the right data
        for p in range(NPhases):
            for stream in range(2):
                fd = fds[2*p+stream]
                for s in range(int(NSamples1/NPhases/NSamplesPerLine/NStreams)):
                    for d in range(NSamplesPerLine):
                        index = s*NSamplesPerLine + d
                        fd.write(str(int(S[p,H[index,stream],0]))+'  ')
                        if(HasImag):
                            fd.write(str(int(S[p,H[index,stream],1]))+'  ')
                    fd.write('\n')

        for fd in fds:
            fd.close()

    print("\nDone\n")
