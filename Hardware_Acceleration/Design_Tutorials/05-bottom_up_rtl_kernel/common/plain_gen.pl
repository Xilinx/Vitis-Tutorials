#!/usr/bin/perl
#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

# Usage: ./plain_gen.pl NUM FILENAME
# NUM is the nubmer of 128-bit words of plain data to be generated
# The generated plain data will be written into FILENAME file
$NUM = $ARGV[0];
$FILE = $ARGV[1];
# print("Writing $NUM 128-bit data to file $FILE...\n");
open(OUTF, ">$FILE");
for ($i = 0; $i < 16*$NUM; $i++) {
  $rand_byte = int(rand(256));
  print(OUTF pack("C", $rand_byte));
}
close(OUTF);

