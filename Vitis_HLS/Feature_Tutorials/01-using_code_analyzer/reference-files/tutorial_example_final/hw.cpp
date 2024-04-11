/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#include "hw.h"

void computeD(int D[N][N], int E[N][N]) {
  // removed un-needed loop 4 times and accum which was always 0 when used
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
        #pragma HLS PIPELINE II=1
        D[i][j] = 2 * E[i][j];
    }
  }
}

int top(int A[N][N], int B[N][N], int C[N][N], int D[N][N], int E[N][N]) {
  int buffer[N][N];
#pragma HLS DATAFLOW

loop23: // combined loop2 and loop3
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
        #pragma HLS PIPELINE II=1
        C[i][j] = B[i][j] * E[i][j];
    }
  }
loop4:
  for (int i = 0; i < N; ++i) {
    // the original code was basically adding 15 times
    // the value 5 onto A[i][0] in a convoluted manner
    #pragma HLS PIPELINE II=1
    A[i][0] += 15 * 5;
  }

  computeD(D, E);

  return 0;
}
