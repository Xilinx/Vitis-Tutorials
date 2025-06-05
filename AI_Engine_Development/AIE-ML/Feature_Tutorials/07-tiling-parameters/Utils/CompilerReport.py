#!/usr/bin/env python3
# Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

import json
import jmespath
import sys


def Usage():
    print("Usage: ExtractBDNumbers.py WorkDirectory option1 [option2 [...]]",file=sys.stderr)
    print(f"\t-bd_memtile: extract Memory Tile BDs",file=sys.stderr)
    print(f"\t-bd_memory: extract Memory Module BDs",file=sys.stderr)
    print(f"\t-bd_interface: extract Interface Tile Channels",file=sys.stderr)
    print(f"\t-bd_all: extract all BDs",file=sys.stderr)
    print(f"\t-tiling: extract tilings and buffer size",file=sys.stderr)

def sort_col_row_chan(a):
    return a['column']*10000+a['row']*100+a['channel'] 
    
def sort_graphType(a):
    return(a['graphType'])
    
def sort_portName(a):
    return(a['portName'])
    
    
def ExtractBD_Memtile(WorkDirectory):
    
    Filename = WorkDirectory+"/reports/compiler_report.json"
    
    with open(Filename) as f:
        data = json.load(f)
    
    a = jmespath.search("mapping.portInstanceMapping.*.dmaInfo",data)
    
    L = len(a)
    
    prevcol = -1
    prevrow = -1
    print(f"\n============= Memory Tile Buffer Descriptors =============")
    if(L==0):
        print(f"\n No Buffer Descriptors")
        return
        
    a.sort(key=sort_col_row_chan)

    for i in range(L):
        if (a[i]['tile'] == 'memory'):
            if(a[i]['column'] != prevcol or a[i]['row'] != prevrow):
                print(f"\nArray Column {a[i]['column']}    Memory Tile Row {a[i]['row']}")
                prevcol = a[i]['column']
                prevrow = a[i]['row']
            dir = 'Output'
            if(a[i]['direction'] == 's2mm'):
                dir = 'Input '
            print(f"\t{dir} Channel {a[i]['channel']}:  Buffer Descriptors: {a[i]['bufferDescriptor']}")

    if(prevcol == -1):
        print(f"\n No Buffer Descriptors")



def ExtractBD_Memory(WorkDirectory):
    
    Filename = WorkDirectory+"/reports/compiler_report.json"
    
    with open(Filename) as f:
        data = json.load(f)
    
    a = jmespath.search("mapping.portInstanceMapping.*.dmaInfo",data)
    
    L = len(a)
    
    prevcol = -1
    prevrow = -1
    print(f"\n============= Memory Module Buffer Descriptors =============")                
    if(L==0):
        print(f"\n No Buffer Descriptors")
        return
        
    a.sort(key=sort_col_row_chan)

    for i in range(L):
        if (a[i]['tile'] == 'aie'):
            if(a[i]['column'] != prevcol or a[i]['row'] != prevrow):
                print(f"\nArray Column {a[i]['column']}    Memory Tile Row {a[i]['row']}")
                prevcol = a[i]['column']
                prevrow = a[i]['row']
            dir = 'Output'
            if(a[i]['direction'] == 's2mm'):
                dir = 'Input '
            print(f"\t{dir} Channel {a[i]['channel']}:  Buffer Descriptors: {a[i]['bufferDescriptor']}")

    if(prevcol == -1):
        print(f"\n No Buffer Descriptors")


def ExtractBD_Interface(WorkDirectory):
    
    Filename = WorkDirectory+"/reports/compiler_report.json"
    
    with open(Filename) as f:
        data = json.load(f)
    
    a = jmespath.search("mapping.portInstanceMapping.*.dmaInfo",data)
    
    L = len(a)
    
    prevcol = -1
    prevrow = -1
    print(f"\n============= Interface Tile Buffer Descriptors =============")                
    if(L==0):
        print(f"\n No Buffer Descriptors")
        return
        
    a.sort(key=sort_col_row_chan)

    for i in range(L):
        if (a[i]['tile'] == 'shim'):
            if(a[i]['column'] != prevcol or a[i]['row'] != prevrow):
                print(f"\nArray Column {a[i]['column']}")
                prevcol = a[i]['column']
                prevrow = a[i]['row']
            dir = 'Output'
            if(a[i]['direction'] == 's2mm'):
                dir = 'Input '
            print(f"\t{dir} Channel {a[i]['channel']}")
            # No BD list on shims

    if(prevcol == -1):
        print(f"\n No Buffer Descriptors")

def ExtractTiling(WorkDirectory):
    Filename = WorkDirectory+"/reports/compiler_report.json"
    
    with open(Filename) as f:
        data = json.load(f)
    
    a = jmespath.search("portInstances.*",data)
    
    L = len(a)
    
    print(f"\n============= Tiling Parameters =============")                
    if(L==0):
        print(f"\n No Tiling Parameters")
        return
        
    a.sort(key=sort_portName)
    a.sort(key=sort_graphType)

    for i in range(L):
        g = a[i]
        print(f"Graph:  {g['graphName']} --> {g['graphType']}")
        print(f"Port: {g['portName']}")
        ap = g.get('accessPatterns', [])
        if ap:
            Nap = len(ap)
            for j in range(Nap):
                if(ap[j]['accessPatternType'] == "tiling"):
                    print(f"Tiling {j}")
                    print(f"\tBuffer  Dimension {ap[j]['tilingBufferDimension']}")
                    print(f"\tTiling  Dimension {ap[j]['tilingDimension']}")
                    print(f"\tTiling  Offset    {ap[j]['tilingOffset']}")
                    tt = ap[j]['tileTraversal']
                    for k in range(len(tt)):
                        if k>0:
                            print(f" -- ",end="")
                        else:
                            print(f"\t",end="")
                        print(f"[dim:{tt[k]['dimension']}  stride:{tt[k]['stride']}  wrap:{tt[k]['wrap']}]",end="")
                    print(f"")
                print(f"")
        buf = g.get('blockSize',[])
        if buf:
            print(f"\tBlock Size:{g['blockSize']} --> Dimensions:{g['dimensions']}\n")
        



if __name__ == '__main__':
    if(len(sys.argv)<3):
        Usage()
        exit(1)
    
    Work = sys.argv[1]
    
    for i in range(2,len(sys.argv)):
        arg = sys.argv[i]
        if(arg == "-bd_memtile"):
            ExtractBD_Memtile(Work)
        elif (arg == "-bd_memory"):
            ExtractBD_Memory(Work)
        elif (arg == "-bd_interface"):
            ExtractBD_Interface(Work)
        elif(arg == "-bd_all"):
            ExtractBD_Memtile(Work)
            ExtractBD_Memory(Work)
            ExtractBD_Interface(Work)
        elif(arg == "-tiling"):
            ExtractTiling(Work)
        else:
            print(f"\n\n Unknown Option: {arg}",file=sys.stderr)
            exit(1)
            
    print(f"\n\n")
    