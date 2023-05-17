/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#include "cholesky_kernel.hpp"

#define MAXN 512
#define NCU 16

extern "C" void cholesky_kernel(int diagSize, double* matrixA) {

#pragma HLS INTERFACE m_axi offset = slave bundle = gmem0 port = matrixA latency = 64 \
  num_read_outstanding = 16 num_write_outstanding = 16 \
  max_read_burst_length = 64 max_write_burst_length = 64 depth = 256

#pragma HLS INTERFACE s_axilite port = diagSize bundle = control
#pragma HLS INTERFACE s_axilite port = matrixA bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control

   int info;
   cholesky<double, MAXN, NCU>(diagSize, matrixA, diagSize, info);

}
