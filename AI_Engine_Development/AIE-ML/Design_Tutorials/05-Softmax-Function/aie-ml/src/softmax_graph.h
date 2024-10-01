//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Richard Buz

#pragma once

#include <adf.h>
#include "softmax_kernel.h"
#include "config.h"

using namespace adf;

class softmax_graph : public adf::graph {
private:
  kernel smax_krnl;

public:
  input_plio iplio;
  output_plio oplio;

  softmax_graph()
  {
      std::string iplio_name = "DIN";
      std::string oplio_name = "DOUT";
      std::string iplio_file = "data/smax_input.txt";
      std::string oplio_file = "data/smax_output_aie.txt";
      iplio = input_plio::create(iplio_name, plio_64_bits, iplio_file);
      oplio = output_plio::create(oplio_name, plio_64_bits, oplio_file);

      smax_krnl = kernel::create_object<softmax_kernel>(std::vector<int16>(BUFFSZ), std::vector<int16>(BUFFSZ));

      connect(iplio.out[0], smax_krnl.in[0]);
      connect(smax_krnl.out[0], oplio.in[0]);

      source(smax_krnl) = "src/softmax_kernel.cpp";

      runtime<ratio>(smax_krnl) = 0.9;

      location<parameter>(smax_krnl.param[0]) = location<kernel>(smax_krnl);
      location<parameter>(smax_krnl.param[1]) = location<kernel>(smax_krnl);
  }
};

