//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Peifang Zhou, Bachir Berkane, Mark Rollins
//

#pragma once

#include "music_parameters.h"
#include "io_adapter_graph.h"
#include "qrd_graph.h"
#include "svd_graph.h"
#include "doa_graph.h"
#include "scanner_graph.h"
#include "finder_graph.h"

//
// Block Level Graph
//

class music_graph : public adf::graph {
  public:
    std::array<adf::input_port,2> sig_i;
    adf::output_port              sig_o;
    io_adapter_graph              io_adapter;
    qrd_graph                     qrd;
    svd_graph                     svd;
    doa_graph                     doa;
    scanner_graph                 scanner;
    finder_graph                  finder;

    music_graph(void) {
      // Connections:
      adf::connect<>(sig_i[0],         io_adapter.sig_i[0]);
      adf::connect<>(sig_i[1],         io_adapter.sig_i[1]);
      adf::connect<>(io_adapter.sig_o, qrd.sig_i);
      adf::connect<>(qrd.sig_o,        svd.sig_i );
      adf::connect<>(svd.sig_o,        doa.sig_i);
      adf::connect<>(doa.sig_o,        scanner.sig_i);
      adf::connect<>(scanner.sig_o,    finder.sig_i);
      adf::connect<>(finder.sig_o,     sig_o);
    }
};
