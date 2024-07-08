//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Richard Buz

#pragma once

#include <adf.h>
#include "bilinear_kernel.h"
#include "buffers.h"

using namespace adf;

class bilinear_graph : public adf::graph {
private:
  kernel bli_krnl[NCORE];

public:
  std::array<input_plio, NCORE> iplio_A, iplio_B, iplio_C;
  std::array<output_plio, NCORE> oplio;

  bilinear_graph()
  {
    for (int i = 0; i < NCORE; i++) {
      std::string iplio_A_name = "DIN_" + std::to_string(i) + "_A";
      std::string iplio_B_name = "DIN_" + std::to_string(i) + "_B";
      std::string iplio_C_name = "DIN_" + std::to_string(i) + "_C";
      std::string oplio_name = "DOUT_" + std::to_string(i);
      std::string iplio_A_file = "data/input_" + std::to_string(i+1) + "_A.txt";
      std::string iplio_B_file = "data/input_" + std::to_string(i+1) + "_B.txt";
      std::string iplio_C_file = "data/input_" + std::to_string(i+1) + "_C.txt";
      std::string oplio_file = "data/output_" + std::to_string(i+1) + "_aie.txt";
      iplio_A[i] = input_plio::create(iplio_A_name, plio_64_bits, iplio_A_file, 625.0);
      iplio_B[i] = input_plio::create(iplio_B_name, plio_64_bits, iplio_B_file, 625.0);
      iplio_C[i] = input_plio::create(iplio_C_name, plio_64_bits, iplio_C_file, 625.0);
      oplio[i] = output_plio::create(oplio_name, plio_64_bits, oplio_file, 625.0);

      bli_krnl[i] = kernel::create_object<bilinear_kernel>();

      connect(iplio_A[i].out[0], bli_krnl[i].in[0]);
      connect(iplio_B[i].out[0], bli_krnl[i].in[1]);
      connect(iplio_C[i].out[0], bli_krnl[i].in[2]);
      connect(bli_krnl[i].out[0], oplio[i].in[0]);

      source(bli_krnl[i]) = "src/bilinear_kernel.cpp";

      runtime<ratio>(bli_krnl[i]) = 0.9;
    }
  }
};

