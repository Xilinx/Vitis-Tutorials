#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Peifang Zhou, Bachir Berkane, Mark Rollins
#

def split_file(input_file, output_file1, output_file2, lines_per_chunk=4):
    with open(input_file, 'r') as infile:
        lines = infile.readlines()

    chunks = [lines[i:i + lines_per_chunk] for i in range(0, len(lines), lines_per_chunk)]

    with open(output_file1, 'w') as outfile1:
        for chunk in chunks[::2]:
            outfile1.writelines(chunk)

    with open(output_file2, 'w') as outfile2:
        for chunk in chunks[1::2]:
            outfile2.writelines(chunk)

# Example usage:
input_filename   = 'data/sig_i.txt'
output_filename1 = 'data/sig_i_0.txt'
output_filename2 = 'data/sig_i_1.txt'
split_file(input_filename, output_filename1, output_filename2)

