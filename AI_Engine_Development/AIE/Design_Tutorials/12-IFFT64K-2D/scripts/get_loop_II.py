#!/tools/gensys/anaconda3/bin/python3
#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

import argparse
import sys
import os
import re

# ------------------------------------------------------------
# Find Loop II from chess logs
# ------------------------------------------------------------

def findLoopII(thisdir):
    # r=root, d=directories, f = files
    for r, d, f in os.walk(thisdir):
        for dir in d:
            match = re.search('(\d+_\d+)',dir)
            if match:
                tileid = match.group(0)
                fname = os.path.join(r,dir) + "/" + match.group(0) + ".log"
                with open(fname) as file:
                    found_hw = 0;
                    for line in file:
                        match = re.search('HW do-loop.*/(\w+).cpp", line (\d+)',line)
                        if match:
                            found_hw = 1;
                            fileid = match.group(1) + ".cpp"
                            lineid = match.group(2)
                        match = re.search('minimum length due to resources: (\d+)',line)
                        if match:
                            minII = match.group(1)
                        match = re.search('\(algo .+\).*cycles: (\d+)',line)
                        if match:
                            beforeII = match.group(1)
                        match = re.search('\(resume algo\).*folding: (\d+)',line)
                        if match:
                            afterII = match.group(1)
                            if ( found_hw == 1 ):
                                print("*** LOOP_II *** Tile: " + tileid + "\tFile: " + fileid + "\tLine: " + lineid + "\tminII: " + minII + "\tbeforeII: " + beforeII + "\tafterII: " + afterII)

# ------------------------------------------------------------
# Entry Point
# ------------------------------------------------------------

if __name__ == "__main__":
    parser = argparse.ArgumentParser(prog=sys.argv[0],description='Dig out II loop performance from chess compile logs')
    parser.add_argument('directory',nargs='+')
    Args = sys.argv
    Args.pop(0)
    args = parser.parse_args(Args)

for d in args.directory:
    findLoopII(d)
