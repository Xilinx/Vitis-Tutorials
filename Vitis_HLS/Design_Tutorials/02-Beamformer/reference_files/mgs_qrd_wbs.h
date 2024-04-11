/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include <stdio.h>
#include <math.h>

#define PRAGMA_SUB(x) _Pragma (#x)
#define DO_PRAGMA(x) PRAGMA_SUB(x)
#define UNFACT 32				//Unroll factor for FOR Loops
#define MAX_ROW 32				//Max number of ROWs for the design
#define MAX_COL 32					//Max number of COLs for the design
#define MAX_COL_div_2_plus1 17		//Max number of COLs divided by 2 plus 1

void mgs_qrd(float Qr_i[MAX_ROW*MAX_COL], float Qi_i[MAX_ROW*MAX_COL], int col, int row, float br_i[MAX_ROW], float bi_i[MAX_ROW], float xr_i[MAX_COL],float xi_i[MAX_COL]);

void cdiv(float a, float b, float c, float d, float *R, float *I);

void cmult(float a, float b, float c, float d, float *R, float *I);





