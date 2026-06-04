//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#ifndef __XMC_MYSUB_H__
#define __XMC_MYSUB_H__

#include <adf.h>
#include "kernels/kernels.h"

class MySub_base : public adf::graph {
public:
   adf::kernel complex_in_out;

public:
   adf::input_port In1;
   adf::output_port Out1;

   MySub_base() {
      // create kernel complex_in_out
      complex_in_out = adf::kernel::create(int32_in_out);
      adf::source(complex_in_out) = "kernels/kernels.cpp";

      // create kernel constraints complex_in_out
      adf::runtime<ratio>(complex_in_out) = 0.9;

      // create nets to specify connections
      adf::connect< adf::stream > net0 (In1, complex_in_out.in[0]);
      adf::connect< adf::stream > net1 (complex_in_out.out[0], Out1);
   }
};

class MySub : public adf::graph {
public:
   MySub_base mygraph;

public:
   adf::input_plio In1;
   adf::output_plio Out1;

   MySub() {
      In1 = adf::input_plio::create("In1",
            adf::plio_32_bits,
            "./data/input/In1.txt");

      Out1 = adf::output_plio::create("Out1",
            adf::plio_32_bits,
            "Out1.txt");

      adf::connect< > (In1.out[0], mygraph.In1);
      adf::connect< > (mygraph.Out1, Out1.in[0]);
   }
};

#endif // __XMC_MYSUB_H__
