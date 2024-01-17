/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#include <cstring>
#include <iostream>

#include "hw.h"

int main(int argc, char**argv) {
    int A[N][N], B[N][N], C[N][N], D[N][N], E[N][N];
    int A_ref[N][N], C_ref[N][N], D_ref[N][N];

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            A[i][j] = i;
            B[i][j] = 1;
            C[i][j] = 0;
            D[i][j] = 0;
            E[i][j] = i+j;
            D_ref[i][j] = 2 * E[i][j];
            C_ref[i][j] = B[i][j] * E[i][j];
            A_ref[i][j] = A[i][j] + (j==0?5*15:0);
        }
    }

    top(A, B, C, D, E);
    int errors=0;
    if ( std::memcmp(A,A_ref,N*N*sizeof(A[0][0])) != 0 ) {errors++;}
    if ( std::memcmp(C,C_ref,N*N*sizeof(C[0][0])) != 0 ) {errors++;}
    if ( std::memcmp(D,D_ref,N*N*sizeof(D[0][0])) != 0 ) {errors++;}
    std::cout << "from " __FILE__ ": got "<<errors<<" error(s)."<<std::endl;

    return errors>0;
}
