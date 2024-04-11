/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#include "hw.h"

void computeD(int D[N][N], int E[N][N]) {
    int acc;
loop_d1:
    for (int t = 0; t < 4; ++t) {
        acc = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                D[i][j] = 2 * E[i][j] + acc;
            }
        }
        acc += D[0][0];
    }
}

int top(int A[N][N], int B[N][N], int C[N][N], int D[N][N], int E[N][N]) {
    int F[N][N], buffer[N][N];

loop1:
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            F[i][j] = i * A[i][j];
        }
    }
// this loop is not named and will show up as process 2
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
        }
    }

loop3:
    for (int i = 0; i < N; ++i) {
        // pipelining this loop with II=1 is not possible
        // Code Analyzer will give a performance guidance message
        // #pragma HLS PIPELINE II=1 
L31:
        for (int j = 0; j < N; ++j) {
            C[i][j] += B[i][j] * E[i][j];
        }
    }

loop4:
    for (int i = 0; i < N; ++i) {
        buffer[i][0] = A[i][0];
        for (int j = 1; j < N; ++j) {
            buffer[i][j] = buffer[i][j - 1] + 5;
        }
        A[i][0] = buffer[i][N - 1];
    }

    computeD(D, E);

    return 0;
}


