//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Chandra Sekar Balakrishnan

#pragma once

#include <adf.h>
#include "farrow_kernel1.h"
#include "farrow_kernel2.h"

using namespace adf;

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

class farrow_graph : public graph {
public:
  // Declare Kernels
  kernel farrow_kernel1_ins;
  kernel farrow_kernel2_ins;
  kernel farrow_kernel3_ins;
  kernel farrow_kernel4_ins;
  kernel farrow_kernel5_ins;    
  
  std::array<port<input>,1>   sig_i;
  std::array<port<input>,1>   del_i;
  std::array<port<output>,1>  sig_o;

  std::vector<int16> taps1 = std::vector<int16>({
      206,-1264,6606,-14835,14835,-6606,1264,-206
  });

  std::vector<int16> taps2 = std::vector<int16>({
      906,-3543,10352,-7628,-7628,10352,-3543,906   // sym
  });

  std::vector<int16> taps3 = std::vector<int16>({
      -51,316,-1652,20093,-20093,1652,-316,51      // anti-sym
  });

  std::vector<int16> taps4 = std::vector<int16>({
      -226,886,-2588,10099,10099,-2588,886,-226     // sym    // flip(f3_taps.int(end-3:end)),flip(f2_taps.int(end-3:end)),flip(f1_taps.int(end-3:end)),flip(f0_taps.int(end-3:end))
  });

  farrow_graph( void )
  {
    farrow_kernel1_ins = kernel::create_object<farrow_kernel1>(taps1);
    source(farrow_kernel1_ins) = "farrow_kernel1.cpp";
    runtime<ratio>(farrow_kernel1_ins) = 0.9;
    
    farrow_kernel2_ins = kernel::create_object<farrow_kernel1>(taps2);
    source(farrow_kernel2_ins) = "farrow_kernel1.cpp";
    runtime<ratio>(farrow_kernel2_ins) = 0.9;

    farrow_kernel3_ins = kernel::create_object<farrow_kernel1>(taps3);
    source(farrow_kernel3_ins) = "farrow_kernel1.cpp";
    runtime<ratio>(farrow_kernel3_ins) = 0.9;

    farrow_kernel4_ins = kernel::create_object<farrow_kernel1>(taps4);
    source(farrow_kernel4_ins) = "farrow_kernel1.cpp";
    runtime<ratio>(farrow_kernel4_ins) = 0.9;

    farrow_kernel5_ins = kernel::create_object<farrow_kernel2>();
    source(farrow_kernel5_ins) = "farrow_kernel2.cpp";
    runtime<ratio>(farrow_kernel5_ins) = 0.9;

    location<stack>(farrow_kernel1_ins) = location<kernel>(farrow_kernel1_ins);
    location<stack>(farrow_kernel2_ins) = location<kernel>(farrow_kernel2_ins);
    location<stack>(farrow_kernel3_ins) = location<kernel>(farrow_kernel3_ins);
    location<stack>(farrow_kernel4_ins) = location<kernel>(farrow_kernel4_ins);
    location<stack>(farrow_kernel5_ins) = location<kernel>(farrow_kernel5_ins);
    
    // Connect inputs:
    connect<> ( sig_i[0], farrow_kernel1_ins.in[0] );
    connect<> ( sig_i[0], farrow_kernel2_ins.in[0] );
    connect<> ( sig_i[0], farrow_kernel3_ins.in[0] );
    connect<> ( sig_i[0], farrow_kernel4_ins.in[0] );
    connect<> ( del_i[0], farrow_kernel5_ins.in[0] );

    connect<> ( farrow_kernel1_ins.out[0], farrow_kernel5_ins.in[1] ); // y3
    connect<> ( farrow_kernel2_ins.out[0], farrow_kernel5_ins.in[2] ); // y2
    connect<> ( farrow_kernel3_ins.out[0], farrow_kernel5_ins.in[3] ); // y1 
    connect<> ( farrow_kernel4_ins.out[0], farrow_kernel5_ins.in[4] ); // y0

   // Connect outputs:
    connect<>( farrow_kernel5_ins.out[0], sig_o[0] );
  }
};

