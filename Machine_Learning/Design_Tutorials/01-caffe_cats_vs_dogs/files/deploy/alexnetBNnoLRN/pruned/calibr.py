# USAGE
# python calibr.py -i  ../regular_rate_0.7/final.prototxt

# by daniele.bagni@xilinx.com

# ##################################################################################################

import warnings
warnings.filterwarnings("ignore", message="numpy.dtype size changed")
warnings.filterwarnings("ignore", message="numpy.ufunc size changed")
import numpy as np

from datetime import datetime
import os
import sys
import argparse

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-f", "--fileinp1", required=True, help="final   prototxt file")
ap.add_argument("-i", "--fileinp2", required=True, help="header  prototxt file")
ap.add_argument("-o", "--fileout",  required=True, help="q_final prototxt file")
args = vars(ap.parse_args())

header     = args["fileinp2"]  # header_calibr.prototxt"
protofile1 = args["fileinp1"]  #   final prototxt file
protofile2 = args["fileout" ]  # q final prototxt file

try:
    f = open(header, "r")
except IOError:
    print("cannot open ", header)
else:
    h_lines = f.readlines()
    h_tot_lines = len(h_lines)
    #print(header, " has ", h_tot_lines, " lines")
f.close()

try:
    f = open(protofile1, "r")
except IOError:
    print("cannot open ", protofile1)
else:
    p_lines = f.readlines()
    p_tot_lines = len(p_lines)
    #print(protofile1, " has ", p_lines, " lines")
f.close()

new_lines = []

for ln1 in range(0, h_tot_lines):
    new_lines.append( h_lines[ln1] )

for ln2 in range(21, p_tot_lines):
    new_lines.append( p_lines[ln2] )

#print(new_lines)

try:
    f = open(protofile2, "w")
except IOError:
    print("cannot open ", protofile2)
else:
    f.writelines(new_lines)
f.close()

