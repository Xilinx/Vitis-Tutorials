/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

Author: Daniele Bagni, Xilinx Inc
*/

#ifndef _H_FIR_H_
#define _H_FIR_H_

#define N	16

#define SAMPLES 1024


//define HLS_FIXED_POINT


#ifdef HLS_FIXED_POINT
#include "ap_fixed.h"

typedef ap_fixed<18,2>	coef_t;
typedef ap_fixed<48,12>	out_data_t;
typedef ap_fixed<18,2>	inp_data_t;
typedef ap_fixed<48,12>	acc_t;

const float THRESHOLD = 1.0;

#else //FULLY ANSI-C (32-bit) fixed point
typedef signed int coef_t;
typedef signed int out_data_t;
typedef signed int inp_data_t;
typedef signed int acc_t;

const long long int THRESHOLD = (4*SAMPLES);
#endif


out_data_t fir_filter ( inp_data_t x, coef_t c[N] );

#endif // _H_FIR_H_
