/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#pragma once

#define HLS_NO_XIL_FPO_LIB
#include "ap_int.h"
#include "hls_stream.h"

#define AXIMM_DATA_WIDTH		256
#define AXIMM_NUM_OUTSTANDING	4
#define AXIMM_BURST_LENGTH	    16

typedef unsigned char      		U8;
typedef unsigned short     		U16;
typedef unsigned int       		U32;

typedef signed char        		I8;
typedef signed short       		I16;
typedef signed int         		I32;

typedef ap_uint<AXIMM_DATA_WIDTH>*              AXIMM;
typedef hls::stream<ap_uint<AXIMM_DATA_WIDTH> > STREAM_BYTES;
typedef hls::stream<U8>                         STREAM_PIXELS;

void AXIBursts2PixelStream(
		AXIMM axi,
		U16 WidthInBytes,
		U16 Height,
		U16 StrideInBytes,
		STREAM_PIXELS& stream);

void PixelStream2AXIBursts(
		STREAM_PIXELS& stream,
        U16 WidthInBytes,
        U16 Height,
        U16 StrideInBytes,
        AXIMM aximm);

