#====================================================================
  Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
  SPDX-License-Identifier: MIT
====================================================================#

# delete all generated files

run(`bash -c 'rm -rf *.dat'`)
run(`bash -c 'rm -rf *.csv'`)
run(`bash -c 'rm -rf *.log'`)
run(`bash -c 'rm -rf ../data/*.dat'`)
run(`bash -c 'rm -rf ../data/*.csv'`)
