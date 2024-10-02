#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Peifang Zhou, Bachir Berkane, Mark Rollins
#


import os
import sys

#
# check command line arguments
#
if (len(sys.argv) < 3):
  print('ERROR: missing command line arguments')
  print('Usage:', sys.argv[0], '{x86sim|aiesim} {0|1|2|3|4|5|6|7}')
  quit()

sim_types   = ['x86sim', 'aiesim']
num_sources = ['0', '1', '2', '3', '4', '5', '6', '7']

sim_type = sys.argv[1]
nSources = sys.argv[2]

if (sim_type not in sim_types):
  print('ERROR: invalid simulation type for', sim_type)
  print('Usage:', sys.argv[0], '{x86sim|aiesim} {0|1|2|3|4|5|6|7}')
  quit()
if (nSources not in num_sources):
  print('ERROR: invalid number of signal sources for', nSources)
  print('Usage:', sys.argv[0], '{x86sim|aiesim} {0|1|2|3|4|5|6|7}')
  quit()

rc = -1;
sig_o_file = None
block_names = ['io_adapter', 'qrd', 'svd', 'doa', 'scanner', 'finder']
basename = os.path.basename(os.getcwd())
if (sim_type == 'x86sim'):
  if (basename in block_names):
    sig_o_file = 'x86simulator_output/data/sig_o.txt'
  else:
    sig_o_file = 'x86simulator_output/data/sig_o.txt'
  command = 'diff -w ' + sig_o_file + ' data/output_x86sim_' + nSources + '.txt'
  rc = os.system(command)
else:
  if (basename in block_names):
    sig_o_file = 'aiesimulator_output/data/sig_o.txt'
  else:
    sig_o_file = 'aiesimulator_output/data/sig_o.txt'
  command = 'grep -v T ' + sig_o_file + ' > /tmp/sig_o.txt'
  os.system(command)
  command = 'grep -v T data/output_aiesim_' + nSources + '.txt | diff -w /tmp/sig_o.txt -'
  rc = os.system(command)

if (rc == 0):
  print('PASS')
else:
  print('FAIL')
