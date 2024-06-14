##
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
##

#!/usr/bin/env python3

import os
import sys
import numpy as np
import csv


OK = 1


NArgs = len(sys.argv)

if (NArgs < 2):
    print ("Usage: DisplayPlot.py Filename(s)", file = sys.stderr)
    exit(1)

#loop on the files
for i in range(1, NArgs):
    Filename = sys.argv[i]

    if (not os.path.isfile(Filename)):
        print("File ", Filename, " does not exist", file = sys.stderr)
        OK = 0
        continue

    print("File ", Filename, " exists", file = sys.stderr)
    
    with open(Filename, 'r') as csvfile:
        # creating a csv reader object
        csvreader = csv.reader(csvfile)
     
        # extracting field names through first row
        fields = next(csvreader)
     
        # extracting each data row one by one
        rows = []
        for row in csvreader:
            rows.append(row)
     
        # get total number of rows
        print("Total no. of rows: %d" % (csvreader.line_num))
         
        # printing the field names
        print('Field names are:' + ', '.join(field for field in fields))
         
        # printing first 5 rows
        print('\nFirst 5 rows are:\n')
        for row in rows[:5]:
            # parsing each column of a row
            for col in row:
                print("%10s" % col, end=" "),
            print('\n')
            
        print(rows)

