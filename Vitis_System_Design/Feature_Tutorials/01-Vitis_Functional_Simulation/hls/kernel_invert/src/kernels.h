//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef _AIE_HLSKERNELS_128B_BASIC_KERNELS_KERNELS_H_
#define _AIE_HLSKERNELS_128B_BASIC_KERNELS_KERNELS_H_

#include "hls_stream.h" 
#include "ap_axi_sdata.h"

void invert(hls::stream< ap_ufixed<128,128> > &in,
            hls::stream< ap_ufixed<128,128> >&out);

void invert87(hls::stream< ap_ufixed<87,87> > &in,
            hls::stream< ap_ufixed<87,87> > &out);

void invert43(hls::stream< ap_ufixed<43,43> > &in,
            hls::stream< ap_ufixed<43,43> > &out);

void invert7_13(hls::stream< ap_ufixed<7,7> > &in7, hls::stream< ap_ufixed<13,13> > &in13,
            hls::stream< ap_ufixed<7,7> > &out7, hls::stream< ap_ufixed<13,13> > &out13 );

#endif // ifndef _AIE_HLSKERNELS_128B_BASIC_KERNELS_KERNELS_H_