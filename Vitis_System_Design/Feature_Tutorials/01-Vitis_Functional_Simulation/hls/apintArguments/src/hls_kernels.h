//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef __HLS_KERNELS_H__
#define __HLS_KERNELS_H__

#include "hls_stream.h"  
#include "ap_int.h"

#define BITWIDTH 29
void pl_func(hls::stream<ap_int<BITWIDTH> > &in1 ,
             hls::stream<ap_uint<BITWIDTH> > &in2 ,
             hls::stream<ap_int<BITWIDTH> > &out1,
             hls::stream<ap_uint<BITWIDTH> > &out2);  
#endif
