/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#ifndef FIR24_SYM_H
#define FIR24_SYM_H

#include <adf.h>
using namespace adf;
  void fir24_sym(input_buffer<cint16,adf::extents<64>,adf::margin<24>> & iwin, output_buffer<cint16,adf::extents<64>> & owin,  const int32(&coeffs)[12], int32(&coeffs_readback)[12]);

#endif


