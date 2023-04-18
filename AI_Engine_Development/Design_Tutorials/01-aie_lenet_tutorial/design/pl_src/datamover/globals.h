/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
//#ifndef _GLOBALS_H
//#define _GLOBALS_H

#include "utils/x_hls_utils.h"
#include "assert.h"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <math.h>
#include <hls_stream.h>
#include "ap_int.h"
#include <stdint.h>
#include <cstdlib>
#include "ap_axi_sdata.h"

//#include "ap_cint.h"

using namespace hls;

typedef ap_uint<128> qint;
typedef ap_uint<64>  dint;

//-------------
// CONSTANTS
//-------------

const int DW        = 64;            // data width
// For Phase 1 UWI carries the Connection pointer
const int UW        = 8;             // user width in on the AXIS interface
const int KW        = DW/8;           // keep width


//-------------
// STRUCTS
//-------------
/*
template<int D,int U>
  struct extendedAxiWord{
    static const int WIDTH = D;
    static const int USER_WIDTH = U;
    ap_uint<D>       data;
    ap_uint<(D)/8>   keep;
    ap_uint<U>       user;
    ap_uint<1>       last;
  };
*/
//typedef hls::stream<extendedAxiWord<DW,UW> > axi_stream;
typedef hls::stream<ap_axis<DW,0,0,0> > axi_stream;
//-------------
// FUNCTIONS
//-------------

void dma_mm2s(volatile dint *mem_rd, axi_stream &strm_out, int size);
void dma_s2mm(volatile dint *mem_wr, axi_stream &strm_in, int size);
extern "C" void dma_hls(volatile dint *mem_wr,  volatile dint *mem_rd, axi_stream &strm_out,axi_stream &strm_in,
			 int mem_rd_size, int mem_wr_size,int iterCnt);


