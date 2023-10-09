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
                    for line in file:
                        match = re.search('(resume algo).*after folding: (\d+).*folded over (\d+) iterations',line)
                        if match:
                            print("*** [LOOP_II] *** Tile " + tileid + " achieves II = " + match.group(2) + " (folded over " + match.group(3) + " iterations)")
                            break
                        match = re.search('(algo 1a).*no folding: (\d+)',line)
                        if match:
                            print("*** [LOOP_II] *** Tile " + tileid + " achieves II = " + match.group(2) + " (no folding)")
                            break

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
