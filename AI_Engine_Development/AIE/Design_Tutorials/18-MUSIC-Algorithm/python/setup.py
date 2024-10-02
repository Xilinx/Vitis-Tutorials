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

source_filename      = 'input_' + sim_type + '_' + nSources + '.txt'
destination_filename = 'sig_i.txt'
command = 'cp -p data/' + source_filename + ' data/' + destination_filename
os.system(command)

block_names = ['qrd', 'svd', 'doa', 'scanner', 'finder']
basename = os.path.basename(os.getcwd())
if (basename not in block_names):
  if (basename in ['io_adapter']):
    command = 'python3 ../../python/split.py'  # to generate sig_i_[01].txt from sig_i.txt
  else:
    # This is for top level from 'aie' folder:
    command = 'python3 ../python/split.py'  # to generate sig_i_[01].txt from sig_i.txt
  os.system(command)

# copy the input file to $MUSIC_PROEJCT_ROOT/data as well
if (basename in block_names):
  command = 'cp -p data/' + source_filename + ' ../../data/' + destination_filename
  os.system(command)
if (basename == 'io_adapter'):
  command = 'cp -p data/sig_i_[01].txt ../../data'
  os.system(command)
