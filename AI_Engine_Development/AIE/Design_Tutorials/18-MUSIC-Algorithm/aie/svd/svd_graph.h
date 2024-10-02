//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Peifang Zhou, Bachir Berkane, Mark Rollins
//

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

#include "music_parameters.h"
#include "svd.h"

using namespace adf;

// ------------------------------------------------------------
// Block Level Graph
// ------------------------------------------------------------

class svd_graph : public graph {
public:
  static constexpr unsigned NUM_SVD_KERNELS = (7 + 6 + 5 + 4 + 3 + 2 + 1) + (9 + 1);  // 9 + 1: iteration #4
  adf::kernel svd_kernel[NUM_SVD_KERNELS];

  input_port  sig_i;
  output_port sig_o;

  // Constructor:
  svd_graph( void )
  {
    unsigned index = 0;
    svd_kernel[index++] = adf::kernel::create_object<SVD<0, 0,        COL_0, COL_1, COL_0, COL_2, COL_0, COL_3>>();  // no input of V, output V from V[0],        P = 0,     Q = 1,2,3
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_0, COL_4, COL_0, COL_5, COL_0, COL_6>>();  //    input of V, output V from V[0],        P = 0,     Q = 4,5,6
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_0, COL_7, COL_1, COL_2, COL_1, COL_3>>();  //    input of V, output V from V[0],        P = 0,1,   Q = 7,2,3
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_1, COL_4, COL_1, COL_5, COL_1, COL_6>>();  //    input of V, output V from V[0],        P = 1,     Q = 4,5,6
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_1, COL_7, COL_2, COL_3, COL_2, COL_4>>();  //    input of V, output V from V[0],        P = 1,2,   Q = 7,3,4
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_2, COL_5, COL_2, COL_6, COL_2, COL_7>>();  //    input of V, output V from V[0],        P = 2,     Q = 5,6,7
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_3, COL_4, COL_3, COL_5, COL_3, COL_6>>();  //    input of V, output V from V[0],        P = 3,     Q = 4,5,6
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_3, COL_7, COL_4, COL_5, COL_4, COL_6>>();  //    input of V, output V from V[0],        P = 3,4,   Q = 7,5,6
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_4, COL_7, COL_5, COL_6, COL_5, COL_7>>();  //    input of V, output V from V[0],        P = 4,5,   Q = 7,6,7

    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_6, COL_7, COL_0, COL_1, COL_0, COL_2>>();  //    input of V, output V from V[0],        P = 6,0,   Q = 7,1,2
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_0, COL_3, COL_0, COL_4, COL_0, COL_5>>();  //    input of V, output V from V[0],        P = 0,     Q = 3,4,5
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_0, COL_6, COL_0, COL_7, COL_1, COL_2>>();  //    input of V, output V from V[0],        P = 0,1,   Q = 6,7,2
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_1, COL_3, COL_1, COL_4, COL_1, COL_5>>();  //    input of V, output V from V[0],        P = 1,     Q = 3,4,5
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_1, COL_6, COL_1, COL_7, COL_2, COL_3>>();  //    input of V, output V from V[0],        P = 1,2,   Q = 6,7,3
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_2, COL_4, COL_2, COL_5, COL_2, COL_6>>();  //    input of V, output V from V[0],        P = 2,     Q = 4,5,6
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_2, COL_7, COL_3, COL_4, COL_3, COL_5>>();  //    input of V, output V from V[0],        P = 2,3,   Q = 7,4,5
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_3, COL_6, COL_3, COL_7, COL_4, COL_5>>();  //    input of V, output V from V[0],        P = 3,4,   Q = 6,7,5
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_4, COL_6, COL_4, COL_7, COL_5, COL_6>>();  //    input of V, output V from V[0],        P = 4,5,   Q = 6,7,6
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_5, COL_7, COL_6, COL_7, COL_0, COL_1>>();  //    input of V, output V from V[0],        P = 5,6,0, Q = 7,7,1

    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_0, COL_2, COL_0, COL_3, COL_0, COL_4>>();  //    input of V, output V from V[0],        P = 0,     Q = 2,3,4
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_0, COL_5, COL_0, COL_6, COL_0, COL_7>>();  //    input of V, output V from V[0],        P = 0,     Q = 5,6,7
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_1, COL_2, COL_1, COL_3, COL_1, COL_4>>();  //    input of V, output V from V[0],        P = 1,     Q = 2,3,4
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_1, COL_5, COL_1, COL_6, COL_1, COL_7>>();  //    input of V, output V from V[0],        P = 1,     Q = 5,6,7
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_2, COL_3, COL_2, COL_4, COL_2, COL_5>>();  //    input of V, output V from V[0],        P = 2,     Q = 3,4,5
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_2, COL_6, COL_2, COL_7, COL_3, COL_4>>();  //    input of V, output V from V[0],        P = 2,3,   Q = 6,7,4
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_3, COL_5, COL_3, COL_6, COL_3, COL_7>>();  //    input of V, output V from V[0],        P = 3,     Q = 5,6,7
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_4, COL_5, COL_4, COL_6, COL_4, COL_7>>();  //    input of V, output V from V[0],        P = 4,     Q = 5,6,7
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_5, COL_6, COL_5, COL_7, COL_6, COL_7>>();  //    input of V, output V from V[0],        P = 5,6,   Q = 6,7,7

    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_0, COL_1, COL_0, COL_2, COL_0, COL_3>>();  //    input of V, output V from V[0],        P = 0,     Q = 1,2,3
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_0, COL_4, COL_0, COL_5, COL_0, COL_6>>();  //    input of V, output V from V[0],        P = 0,     Q = 4,5,6
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_0, COL_7, COL_1, COL_2, COL_1, COL_3>>();  //    input of V, output V from V[0],        P = 0,1,   Q = 7,2,3
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_1, COL_4, COL_1, COL_5, COL_1, COL_6>>();  //    input of V, output V from V[0],        P = 1,     Q = 4,5,6
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_1, COL_7, COL_2, COL_3, COL_2, COL_4>>();  //    input of V, output V from V[0],        P = 1,2,   Q = 7,3,4
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_2, COL_5, COL_2, COL_6, COL_2, COL_7>>();  //    input of V, output V from V[0],        P = 2,     Q = 5,6,7
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_3, COL_4, COL_3, COL_5, COL_3, COL_6>>();  //    input of V, output V from V[0],        P = 3,     Q = 4,5,6
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_3, COL_7, COL_4, COL_5, COL_4, COL_6>>();  //    input of V, output V from V[0],        P = 3,4,   Q = 7,5,6
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 0,        COL_4, COL_7, COL_5, COL_6, COL_5, COL_7>>();  //    input of V, output V from V[0],        P = 4,5,   Q = 7,6,7
    svd_kernel[index++] = adf::kernel::create_object<SVD<1, 1,        COL_6, COL_7, COL_NULL, COL_NULL, COL_NULL, COL_NULL>>();  // input of V, output V from V[nSources], P = 6, Q = 7

    for (unsigned i = 0; i < NUM_SVD_KERNELS; ++i) {
      adf::runtime<ratio>(svd_kernel[i]) = 0.9;
      adf::source(svd_kernel[i]) = "svd.cpp";
    }

    adf::connect(sig_i, svd_kernel[0].in[0]);
    for (unsigned i = 0; i < NUM_SVD_KERNELS - 1; ++i) {
      adf::connect(svd_kernel[i].out[0], svd_kernel[i+1].in[0]);
    }
    adf::connect(svd_kernel[NUM_SVD_KERNELS-1].out[0], sig_o);

    dimensions(svd_kernel[0].in[0]) = {N * N};
    for (unsigned i = 0; i < NUM_SVD_KERNELS - 1; ++i) {
      adf::dimensions(svd_kernel[i].out[0])  = {2 * N * N};  // W and V matrices
      adf::dimensions(svd_kernel[i+1].in[0]) = {2 * N * N};  // W and V matrices
    }
    dimensions(svd_kernel[NUM_SVD_KERNELS-1].out[0]) = {N * N};
  }
};


