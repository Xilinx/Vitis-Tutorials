#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#
#!/usr/bin/env python3




from re import I
import sys
import numpy as np
from math import *


def main(args):
    f1 = args[1]
    f2 = args[2]
    print("\n\nComparison of 2 files")
    print("File 1 : " + f1)
    print("File 2 : " + f2)

    fp1 = open(f1,'r')
    fp2 = open(f2,'r')

    Line = 0
    Error = 0

    while True:
        l1 = fp1.readline()
        l2 = fp2.readline()
        s1 = l1.split()
        s2 = l2.split()
        Line = Line + 1

        if len(s1)==0 or len(s2)==0:
            break

        if len(s1) != len(s2):
            print("Error Line " + str(Line))
            Error = Error +1
        else:
            for i in range(len(s1)):
                if float(s1[i]) != float(s2[i]):
                    print("Error Line " + str(Line))
                    Error = Error +1
                    if Error > 10:
                        print("Stops Comparison here. Too many Errors!!!")
                        break
                        fp1.close()
                        fp2.close()

    if Error == 0:
        print("No Error. test PASSED\n\n")
    else:
        print("Test FAILED!!\n\n")





if __name__ == '__main__':
    main(sys.argv)
