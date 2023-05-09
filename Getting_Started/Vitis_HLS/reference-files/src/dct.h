/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#ifndef __DCT_H__
#define __DCT_H__

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>

#define DW 16
#define N 1024/DW
//#define N 32
#define NUM_TRANS 16

typedef short dct_data_t;

#define DCT_SIZE 8    /* defines the input matrix as 8x8 */
#define CONST_BITS  13
#define DESCALE(x,n)  (((x) + (1 << ((n)-1))) >> n)

extern "C" {
  void dct(short input[N], short output[N]);
}
#endif // __DCT_H__ not defined
