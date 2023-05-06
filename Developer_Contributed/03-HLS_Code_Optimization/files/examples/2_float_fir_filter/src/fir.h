/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

Author: Daniele Bagni, Xilinx Inc
*/

#ifndef _H_FIR_H_
#define _H_FIR_H_

#define N	16

#define SAMPLES 1024

const float THRESHOLD = 1.0;

//#define HLS_FIXED_POINT

#ifdef HLS_FIXED_POINT
#include "ap_fixed.h"

typedef ap_fixed<18,2>	coef_t;
typedef ap_fixed<48,12>	out_data_t;
typedef ap_fixed<18,2>	inp_data_t;
typedef ap_fixed<48,12>	acc_t;

#else //FULLY Single-Precision (32-bit) Floating-Point
typedef float coef_t;
typedef float out_data_t;
typedef float inp_data_t;
typedef float acc_t;

#endif

out_data_t fir_filter ( inp_data_t x, coef_t c[N] );

#endif // _H_FIR_H_
