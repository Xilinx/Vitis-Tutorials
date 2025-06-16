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
template <int IN_SIZE, int IN_MARGIN>
class fir_graph : public adf::graph {

  private:
    adf::kernel  fir16_i;

  public:
    adf::input_port   in;
    adf::input_port   coeff;
    adf::output_port  out;

    fir_graph()
    {
      // specify PLIO ports

      // specify kernels
      fir16_i     = adf::kernel::create_object<my_fir16<cint16, IN_SIZE, IN_MARGIN> >();
      adf::source(fir16_i)    = "kernels/my_fir16.cpp";

      adf::runtime<ratio>(fir16_i)   = 0.80;
      adf::connect< adf::parameter > (coeff, async(fir16_i.in[0]));
      adf::connect< > (in, fir16_i.in[1]);
      adf::connect< > (fir16_i.out[0], out);
    }
};


