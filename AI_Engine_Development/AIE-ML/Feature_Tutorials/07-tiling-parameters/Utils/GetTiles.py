#!/usr/bin/env python3
# Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

import os
import sys
import numpy as np


# Optimize display only on the columns dimension
OptDisplay = 0
UpperLimit = 50
LeftRight = 20

# Print a single Tile
def printTile(arr, NCols, NRows, NLayers, NImages, Tile):
    PrintVal = 1
    NTiles = len(arr)/NCols/NRows/NLayers/NImages
    if (Tile < NTiles):
        print("Tile:", Tile)
        
        # get the maximum integer representation width to align all columns
        Max = 0
        for im in range(NLayers):
            for layer in range(NLayers):
                for row in range(NRows):
                    for col in range(NCols):
                        val = arr[Tile*NRows*NCols*NLayers*NImages+im*NRows*NCols*NLayers+layer*NCols*NRows+row*NCols+col]
                        if val>Max:
                            Max = val
        if (Max == 0):
            w = 1
        else:
            w = int(np.log(Max)/np.log(10)+.999)

        
        # Display the tile
        if(NImages==1):
            for l in range(NLayers):
                if (NLayers>1):
                    print("")
                    for k in range(l):
                        print(f"\t",end="")
                    print(f"Layer = {l}")

                for i in range(NRows):
                    for k in range(l):
                        print(f"\t",end="")
                    for j in range(NCols):
                        if(OptDisplay==0):
                            PrintVal = 1
                        else:
                            if(NCols<=UpperLimit):
                                PrintVal = 1
                            elif(j<LeftRight or j>=NCols-LeftRight):
                                PrintVal = 1
                            elif(j==int(NCols/2)):
                                PrintVal = 2
                            else:
                                PrintVal = 0
                        if(PrintVal == 1):
                            print(f"{arr[Tile*NRows*NCols*NLayers+l*NCols*NRows+i*NCols+j]:{w}}", end = " ")
                        elif(PrintVal==2):
                            print("   ...   ", end = " ")
                    print("")
                if (l==NLayers-1):
                    print("------------------------------------------------------------")
        else:
            for im in range(NImages):
                for row in range(NRows):
                    for layer in range(NLayers):
                        for col in range(NCols):
                            val = arr[Tile*NRows*NCols*NLayers*NImages+im*NRows*NCols*NLayers+layer*NCols*NRows+row*NCols+col]
                            print(f"{val:{w}}", end = " ")
                        print("    ",end=" ")
                    print("")
                print("")
                
# Select a range of tiles and display them, again and again
def tileSelection(arr,NCols,NRows,NLayers,NImages):
    while (1):
        txt = input("\n\nEnter tile index or tile range or '*' for all tiles [ENTER to end]?  ")
        if(len(txt)==0):
            exit(0)
        t = txt.split()
        if(len(t)==0):
            exit(0)

        if(t[0]=="*"):
            t1 = 0
            t2 = int(len(arr)/NCols/NRows/NLayers/NImages)-1
        else:
            t1 = int(t[0])        
            if(len(t)>1):
                t2 = int(t[1])
            else:
                t2 = t1
    
        for i in range(t1,t2+1):
            printTile(arr,NCols,NRows,NLayers,NImages,i)

    
def Usage():
    BOLD = "\033[1m"
    RESET = "\033[0m"  # Resets to default text formatting
    print (f"Usage: GetTiles filename TileDim NCols [ NRows [ NLayers [ NImages ]]] [ Tile# | Tile_start Tile_end]", 
    file = sys.stderr)
    print (f"       filename: input file containing the matrix (without timestamps)", file = sys.stderr)
    print (f"       TileDim: 1D 2D 3D 4D or 1Do 2Do 3Do or 1DO 2DO 3DO for optimized display for number of columns above a certain threshold", file = sys.stderr)
    print (f"                1Do 2Do 3Do: start ellipsis notation at 50 columns")
    print (f"                1DO 2DO 3DO: start ellipsis notation at 30 columns")
    print (f"       NCols: number of columns of each Tile in the matrix", file = sys.stderr)
    print (f"       NRows: number of rows of each Tile in the matrix", file = sys.stderr)
    print (f"       NLayers: number of layers of each Tile in the matrix", file = sys.stderr)
    print (f"       NImages: number of images of each Tile in the matrix", file = sys.stderr)
    print (f"       Tile#:  display the Tile with the given index  ('*' for all tiles)", file = sys.stderr)
    print (f"       Tile_start Tile_end: display the Tiles in the given range", file = sys.stderr)
    exit(1)

    
if __name__ == '__main__':

    OK = 1
    
    minarg = 4
    maxarg = 9
    if (len(sys.argv) < minarg or len(sys.argv)>maxarg):
        Usage()
        
    filename = sys.argv[1]
    TileDim = sys.argv[2]
    NCols = 1
    NRows = 1
    NLayers = 1
    NImages = 1
    TileStart = -1
    TileEnd = -1
    
    if(TileDim[:2]=="1D"):
        maxarg = 6
    elif (TileDim[:2]=="2D"):
        minarg = 5
        maxarg = 7
    elif (TileDim[:2]=="3D"):
        minarg = 6
        maxarg = 8
    elif (TileDim[:2]=="4D" ):
        minarg = 7
        maxarg = 9
    else:
        Usage()

    if(TileDim=="1Do" or TileDim=="2Do"  or TileDim=="3Do" ):
        OptDisplay = 1
        UpperLimit = 50
        LeftRight = 20

    if(TileDim=="1DO" or TileDim=="2DO"  or TileDim=="3DO" ):
        OptDisplay = 1
        UpperLimit = 30
        LeftRight = 10

    if (len(sys.argv) < minarg or len(sys.argv)>maxarg):
        Usage()

    NCols = int(sys.argv[3])
    if (minarg >= 5):   
        NRows = int(sys.argv[4])
        if (minarg >= 6):   
            NLayers = int(sys.argv[5])
            if (minarg == 7):   
                NImages = int(sys.argv[6])
        
    print(f"\n------------------------------------------------------\n")
    print(f"filename: {filename}")
    print(f"NCols: {NCols}")
    print(f"NRows: {NRows}")
    print(f"NLayers: {NLayers}")
    print(f"NImages: {NImages}")
    print(f"\n------------------------------------------------------\n")

    
    integers = []
    with open(filename, 'r') as file:
        for line in file:
            ent = line.split()
            for e in ent:
                integers.append(int(e))

    arr = np.array(integers)
        

    if (len(sys.argv) == minarg):
        print("Dynamic Tile Selection")
        tileSelection(arr,NCols,NRows,NLayers,NImages)
        exit(0)
    
    if (sys.argv[minarg] == "*"):
        TileStart = 0
        TileEnd = int(len(arr)/NCols/NRows/NLayers/NImages)-1
    else:
        TileStart = int(sys.argv[minarg])
        TileEnd = TileStart
        
        if (len(sys.argv) == maxarg):
            TileEnd = int(sys.argv[maxarg-1])
    
    print("Static Tile Selection",end="\n\n")
    
    for i in range(TileStart,TileEnd+1):
        printTile(arr,NCols,NRows,NLayers,NImages,i)

