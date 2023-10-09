//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Richard Buz

#pragma once

#include <adf.h>
#include "bilinear_kernel.h"
#include "buffers.h"
#include "config.h"

using namespace adf;

alignas(32) std::vector<std::vector<float>> xbufdm(NCORE, std::vector<float>(4*PXLPERGRP));
alignas(32) std::vector<std::vector<float>> ybufdm(NCORE, std::vector<float>(4*PXLPERGRP));
alignas(32) std::vector<std::vector<float>> zbufdm(NCORE, std::vector<float>(4*PXLPERGRP));

class bilinear_graph : public adf::graph {
private:
  kernel bli_krnl[NCORE];

public:
  std::array<input_plio, NCORE> iplio;
  std::array<output_plio, NCORE> oplio;

  bilinear_graph()
  {
    for (int i = 0; i < NCORE; i++) {
      std::string iplio_name = "DIN_" + std::to_string(i);
      std::string oplio_name = "DOUT_" + std::to_string(i);
      std::string iplio_file = "data/input_" + std::to_string(i+1) + ".txt";
      std::string oplio_file = "data/output_" + std::to_string(i+1) + "_aie.txt";
      iplio[i] = input_plio::create(iplio_name, plio_64_bits, iplio_file, 625.0);
      oplio[i] = output_plio::create(oplio_name, plio_64_bits, oplio_file, 625.0);

      bli_krnl[i] = kernel::create_object<bilinear_kernel>(xbufdm[i], ybufdm[i], zbufdm[i]);

      connect(iplio[i].out[0], bli_krnl[i].in[0]);
      connect(bli_krnl[i].out[0], oplio[i].in[0]);

      source(bli_krnl[i]) = "src/bilinear_kernel.cpp";

      runtime<ratio>(bli_krnl[i]) = 0.9;
    }
  }
};

