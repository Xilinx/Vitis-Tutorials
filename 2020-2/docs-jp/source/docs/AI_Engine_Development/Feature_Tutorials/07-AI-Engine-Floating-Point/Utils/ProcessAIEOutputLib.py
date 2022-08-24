#
# Copyright 2020 Xilinx, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
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

def ProcessFloat(filename):

    fdr = open(filename,'r')
    line = '  '
    a = np.zeros((0,2))

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
            a = np.append( a , np.array([[ currentTime-1, float(res[0])]]),axis=0)
            a = np.append( a , np.array([[ currentTime, float(res[1])]]),axis=0)
    fdr.close()

    print(a)
    np.savetxt('out.txt', a, delimiter=' ')

    plt.subplot(2,1,1)
    plt.plot(a[:,0],a[:,1])
    plt.title('With Respect to Timestamps')
    plt.subplot(2,1,2)
    plt.plot(a[:,1])

    plt.show()


def ProcessCFloat(filename):

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
            a = np.append( a , np.array([[ currentTime, float(res[0]), float(res[1])]]),axis=0)
    fdr.close()

    print(a)
    np.savetxt('out.txt', a, delimiter=' ')

    plt.subplot(2,1,1)
    plt.plot(a[:,1])
    plt.title('Real Part')
    plt.subplot(2,1,2)
    plt.plot(a[:,2])
    plt.title('Imag Part')

    plt.show()

def NextLine(fdr):
    while (1):
        line = fdr.readline()
        if line=='':
            return(line)
        res = line.split()
        if(res[0] == 'TLAST'):
            continue
        elif(res[0] == 'T'):
            continue
        else:
            return(line)


def CompareFiles(file1,file2):

    fdr1 = open(file1,'r')
    fdr2 = open(file2,'r')

    epsilon = 0.01

    line1 = ' '
    line2 = ' '

    NErrors = 0

    while(line1!="" or line2!=""):
        line1 = NextLine(fdr1)
        line2 = NextLine(fdr2)

        r1 = line1.split()
        r2 = line2.split()

        if (len(r1)!=len(r2)):
            print("Error: Not the same length --> '%s' != '%s'\n" % (line1,line2))
        for i in range(len(r1)):
            if (abs(float(r1[i])-float(r2[i])) > epsilon):
                print("Error: %f != %f\n" % (float(r1[i]),float(r2[i])))
                NErrors = NErrors+1

    if(NErrors == 0):
        print("PASSED!  The 2 files are Equivalent\n")
        return(0)
    else:
        print("FAILED! There are %d Error(s)\n" % (NErrors))
        return(1)
