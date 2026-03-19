/*
Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
Copyright (C) 2022-2026 Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include <adf.h>
#include "kernels.h"
#include "include.h"

using namespace adf;

class simpleGraph : public adf::graph {
private:
  kernel first;
  kernel second;
public:
  input_plio  in;
  output_plio out;
  simpleGraph(){
    
    in  = input_plio::create(plio_32_bits, "data/input.txt");
    out = output_plio::create(plio_32_bits, "data/output.txt");

    first = kernel::create(simple);
    second = kernel::create(simple);
    adf::connect(in.out[0], first.in[0]);
    connect(first.out[0], second.in[0]);
    connect(second.out[0], out.in[0]);
    dimensions(first.in[0]) = { NUM_SAMPLES };
    dimensions(first.out[0]) = { NUM_SAMPLES };
    dimensions(second.in[0]) = { NUM_SAMPLES };
    dimensions(second.out[0]) = { NUM_SAMPLES };

    source(first) = "kernels.cc";
    source(second) = "kernels.cc";

    runtime<ratio>(first) = 0.1;
    runtime<ratio>(second) = 0.1;

  }
};
