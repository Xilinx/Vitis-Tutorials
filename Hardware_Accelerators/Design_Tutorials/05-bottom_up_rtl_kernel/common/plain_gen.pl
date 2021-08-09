#!/usr/bin/perl
# Copyright 2021 Xilinx, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
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

