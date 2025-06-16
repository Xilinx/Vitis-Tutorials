//
// Copyright (C) 2023-2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//
// Author Derek Hagen

#include <adf.h>
#include "../kernels/kernels.hpp"

// skip namespace show explicit adf syntax//using namespace adf;

#define STR_LEN 30
/*
 * Adaptive dataflow graph to move data
 */
template <int IN_SIZE, int NUM>
class dm_graph : public adf::graph {

  static_assert(NUM == 4, "Currently this graph only support exactly 4 kernels");

  private:
    adf::kernel  dmove_i[NUM];

  public:
    adf::input_port   in[NUM];
    adf::output_port  out[NUM];

    dm_graph()
    {

      // specify kernels
      dmove_i[0]  = adf::kernel::create_object<my_mul_dm<cint16, IN_SIZE> >();
      dmove_i[1]  = adf::kernel::create_object<my_stream_dm<cint16> >();
      dmove_i[2]  = adf::kernel::create_object<my_stream_32b_dm<cint16> >();
      dmove_i[3]  = adf::kernel::create_object<my_stream_32b_opt_dm<cint16> >();
      adf::source(dmove_i[0]) = "kernels/my_mul_dm.cpp";
      adf::source(dmove_i[1]) = "kernels/my_stream_dm.cpp";
      adf::source(dmove_i[2]) = "kernels/my_stream_32b_dm.cpp";
      adf::source(dmove_i[3]) = "kernels/my_stream_32b_opt_dm.cpp";

      for (int i=0; i<NUM; i++){
        if (i==0) {
          adf::runtime<ratio>(dmove_i[i])   = 0.82;
          adf::connect< > (in[i], dmove_i[i].in[0]);
          adf::connect< > (dmove_i[i].out[0], out[i]);
        }
        else {
          adf::runtime<ratio>(dmove_i[i])   = 0.99;
          adf::connect< adf::stream > dummy_net (in[i], dmove_i[i].in[0]);
          adf::connect< adf::stream > (dmove_i[i].out[0], out[i]);
          fifo_depth(dummy_net) = 8;
        }
      }
    }
};


