#!/usr/bin/python3
#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Mark Rollins

import argparse
import sys
import os
import re

# ------------------------------------------------------------
# Find Throughput from AIE Simulation
# ------------------------------------------------------------

def check_throughput(args):
    error = 0
    plio = args.plio[0]
    target = float(args.target[0])
    accuracy = float(args.accuracy[0])
    with open("AIESimulator.log") as file:
        for line in file:
            regexp = plio + " .* ([\\d.]+)"
            match = re.search(regexp,line)
            if match:
                actual = float(match.group(1))
                diff = 100*(target-actual)/target
                if (actual < target and diff > accuracy):
                    error = 1
                    print("*** ERROR ***")
                    print("  Target: " + str(target) + " Actual: " + str(actual) + " % Diff: " + str(diff))
                else:
                    error = 0
    return(error)

# ------------------------------------------------------------
# Entry Point
# ------------------------------------------------------------

if __name__ == "__main__":
    parser = argparse.ArgumentParser(prog=sys.argv[0],description='Check throughput against expected')
    parser.add_argument('plio',nargs='+')
    parser.add_argument('target',nargs='+')
    parser.add_argument('accuracy',nargs='+')
    Args = sys.argv
    Args.pop(0)
    args = parser.parse_args(Args)
    error = check_throughput(args)
    sys.exit(error)
