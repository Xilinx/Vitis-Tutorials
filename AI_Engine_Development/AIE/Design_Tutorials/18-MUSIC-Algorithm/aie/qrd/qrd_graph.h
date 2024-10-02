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
#include "qrd_norm.h"
#include "qrd_qr.h"

using namespace adf;

// ------------------------------------------------------------
// Block Level Graph
// ------------------------------------------------------------

class qrd_graph : public graph {
public:
  static constexpr unsigned NUM_QRD_KERNELS = COL * (COL + 1) / 2;
  adf::kernel      qrd_kernel[NUM_QRD_KERNELS];

  input_port  sig_i;
  output_port sig_o;

  // Constructor:
  qrd_graph( void )
  {
    unsigned index = 0;
    qrd_kernel[index] = adf::kernel::create_object<QRD_Norm<COL_NORM_0,           TOTAL_NUM_SEGMENTS, 0, TOTAL_NUM_SEGMENTS>>();
    adf::runtime<ratio>(qrd_kernel[index]) = 0.9;
    adf::source(qrd_kernel[index++]) = "qrd_norm.cpp";
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_0, COL_QR_1, TOTAL_NUM_SEGMENTS, 0, TOTAL_NUM_SEGMENTS>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_0, COL_QR_2, TOTAL_NUM_SEGMENTS, 0, TOTAL_NUM_SEGMENTS>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_0, COL_QR_3, TOTAL_NUM_SEGMENTS, 0, TOTAL_NUM_SEGMENTS>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_0, COL_QR_4, TOTAL_NUM_SEGMENTS, 0, TOTAL_NUM_SEGMENTS>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_0, COL_QR_5, TOTAL_NUM_SEGMENTS, 0, TOTAL_NUM_SEGMENTS>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_0, COL_QR_6, TOTAL_NUM_SEGMENTS, 0, TOTAL_NUM_SEGMENTS>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_0, COL_QR_7, TOTAL_NUM_SEGMENTS, NUM_SEGMENTS_PER_COLUMN, TOTAL_NUM_SEGMENTS>>();
    for (unsigned i = 1; i < COL; ++i) {
      adf::runtime<ratio>(qrd_kernel[i]) = 0.9;
      adf::source(qrd_kernel[i]) = "qrd_qr.cpp";
    }

    qrd_kernel[index] = adf::kernel::create_object<QRD_Norm<COL_NORM_1,           TOTAL_NUM_SEGMENTS_1, 0, TOTAL_NUM_SEGMENTS_1>>();
    adf::runtime<ratio>(qrd_kernel[index]) = 0.9;
    adf::source(qrd_kernel[index++]) = "qrd_norm.cpp";
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_1, COL_QR_2, TOTAL_NUM_SEGMENTS_1, 0, TOTAL_NUM_SEGMENTS_1>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_1, COL_QR_3, TOTAL_NUM_SEGMENTS_1, 0, TOTAL_NUM_SEGMENTS_1>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_1, COL_QR_4, TOTAL_NUM_SEGMENTS_1, 0, TOTAL_NUM_SEGMENTS_1>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_1, COL_QR_5, TOTAL_NUM_SEGMENTS_1, 0, TOTAL_NUM_SEGMENTS_1>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_1, COL_QR_6, TOTAL_NUM_SEGMENTS_1, 0, TOTAL_NUM_SEGMENTS_1>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_1, COL_QR_7, TOTAL_NUM_SEGMENTS_1, NUM_SEGMENTS_PER_COLUMN, TOTAL_NUM_SEGMENTS_1>>();
    for (unsigned i = COL + 1; i < 2 * COL - 1; ++i) {
      adf::runtime<ratio>(qrd_kernel[i]) = 0.9;
      adf::source(qrd_kernel[i]) = "qrd_qr.cpp";
    }

    qrd_kernel[index] = adf::kernel::create_object<QRD_Norm<COL_NORM_2,           TOTAL_NUM_SEGMENTS_2, 0, TOTAL_NUM_SEGMENTS_2>>();
    adf::runtime<ratio>(qrd_kernel[index]) = 0.9;
    adf::source(qrd_kernel[index++]) = "qrd_norm.cpp";
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_2, COL_QR_3, TOTAL_NUM_SEGMENTS_2, 0, TOTAL_NUM_SEGMENTS_2>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_2, COL_QR_4, TOTAL_NUM_SEGMENTS_2, 0, TOTAL_NUM_SEGMENTS_2>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_2, COL_QR_5, TOTAL_NUM_SEGMENTS_2, 0, TOTAL_NUM_SEGMENTS_2>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_2, COL_QR_6, TOTAL_NUM_SEGMENTS_2, 0, TOTAL_NUM_SEGMENTS_2>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_2, COL_QR_7, TOTAL_NUM_SEGMENTS_2, NUM_SEGMENTS_PER_COLUMN, TOTAL_NUM_SEGMENTS_2>>();
    for (unsigned i = 2 * COL; i < 3 * COL - 3; ++i) {
      adf::runtime<ratio>(qrd_kernel[i]) = 0.9;
      adf::source(qrd_kernel[i]) = "qrd_qr.cpp";
    }

    qrd_kernel[index] = adf::kernel::create_object<QRD_Norm<COL_NORM_3,           TOTAL_NUM_SEGMENTS_3, 0, TOTAL_NUM_SEGMENTS_3>>();
    adf::runtime<ratio>(qrd_kernel[index]) = 0.9;
    adf::source(qrd_kernel[index++]) = "qrd_norm.cpp";
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_3, COL_QR_4, TOTAL_NUM_SEGMENTS_3, 0, TOTAL_NUM_SEGMENTS_3>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_3, COL_QR_5, TOTAL_NUM_SEGMENTS_3, 0, TOTAL_NUM_SEGMENTS_3>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_3, COL_QR_6, TOTAL_NUM_SEGMENTS_3, 0, TOTAL_NUM_SEGMENTS_3>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_3, COL_QR_7, TOTAL_NUM_SEGMENTS_3, NUM_SEGMENTS_PER_COLUMN, TOTAL_NUM_SEGMENTS_3>>();
    for (unsigned i = 3 * COL - 2; i < 4 * COL - 6; ++i) {
      adf::runtime<ratio>(qrd_kernel[i]) = 0.9;
      adf::source(qrd_kernel[i]) = "qrd_qr.cpp";
    }

    qrd_kernel[index] = adf::kernel::create_object<QRD_Norm<COL_NORM_4,           TOTAL_NUM_SEGMENTS_4, 0, TOTAL_NUM_SEGMENTS_4>>();
    adf::runtime<ratio>(qrd_kernel[index]) = 0.9;
    adf::source(qrd_kernel[index++]) = "qrd_norm.cpp";
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_4, COL_QR_5, TOTAL_NUM_SEGMENTS_4, 0, TOTAL_NUM_SEGMENTS_4>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_4, COL_QR_6, TOTAL_NUM_SEGMENTS_4, 0, TOTAL_NUM_SEGMENTS_4>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_4, COL_QR_7, TOTAL_NUM_SEGMENTS_4, NUM_SEGMENTS_PER_COLUMN, TOTAL_NUM_SEGMENTS_4>>();
    for (unsigned i = 4 * COL - 5; i < 5 * COL - 10; ++i) {
      adf::runtime<ratio>(qrd_kernel[i]) = 0.9;
      adf::source(qrd_kernel[i]) = "qrd_qr.cpp";
    }

    qrd_kernel[index] = adf::kernel::create_object<QRD_Norm<COL_NORM_5,           TOTAL_NUM_SEGMENTS_5, 0, TOTAL_NUM_SEGMENTS_5>>();
    adf::runtime<ratio>(qrd_kernel[index]) = 0.9;
    adf::source(qrd_kernel[index++]) = "qrd_norm.cpp";
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_5, COL_QR_6, TOTAL_NUM_SEGMENTS_5, 0, TOTAL_NUM_SEGMENTS_5>>();
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_5, COL_QR_7, TOTAL_NUM_SEGMENTS_5, NUM_SEGMENTS_PER_COLUMN, TOTAL_NUM_SEGMENTS_5>>();
    for (unsigned i = 5 * COL - 9; i < 6 * COL - 15; ++i) {
      adf::runtime<ratio>(qrd_kernel[i]) = 0.9;
      adf::source(qrd_kernel[i]) = "qrd_qr.cpp";
    }

    qrd_kernel[index] = adf::kernel::create_object<QRD_Norm<COL_NORM_6,           TOTAL_NUM_SEGMENTS_6, 0, TOTAL_NUM_SEGMENTS_6>>();
    adf::runtime<ratio>(qrd_kernel[index]) = 0.9;
    adf::source(qrd_kernel[index++]) = "qrd_norm.cpp";
    qrd_kernel[index++] = adf::kernel::create_object<QRD_QR<COL_NORM_6, COL_QR_7, TOTAL_NUM_SEGMENTS_6, NUM_SEGMENTS_PER_COLUMN, TOTAL_NUM_SEGMENTS_6>>();
    for (unsigned i = 6 * COL - 14; i < 7 * COL - 21; ++i) {
      adf::runtime<ratio>(qrd_kernel[i]) = 0.9;
      adf::source(qrd_kernel[i]) = "qrd_qr.cpp";
    }

    qrd_kernel[index] = adf::kernel::create_object<QRD_Norm<COL_NORM_7,           TOTAL_NUM_SEGMENTS_7, 0, 0>>();  // no need to send out Q anymore
    adf::runtime<ratio>(qrd_kernel[index]) = 0.9;
    adf::source(qrd_kernel[index++]) = "qrd_norm.cpp";

    // Connections:
    adf::connect(sig_i, qrd_kernel[0].in[0]);
    for (unsigned i = 0; i < NUM_QRD_KERNELS - 1; ++i)
      adf::connect(qrd_kernel[i].out[0], qrd_kernel[i+1].in[0]);
    adf::connect(qrd_kernel[NUM_QRD_KERNELS-1].out[0], sig_o);

    // Dimensions:
    adf::dimensions(qrd_kernel[0].in[0]) = {ROW * COL};
    for (unsigned i = 0; i < COL - 1; ++i) {
      adf::dimensions(qrd_kernel[i].out[0])  = {ROW * COL + COL * COL};
      adf::dimensions(qrd_kernel[i+1].in[0]) = {ROW * COL + COL * COL};
    }
    for (unsigned i = COL - 1; i < 2 * COL - 2; ++i) {
      adf::dimensions(qrd_kernel[i].out[0])  = {ROW * (COL-1) + COL * COL};
      adf::dimensions(qrd_kernel[i+1].in[0]) = {ROW * (COL-1) + COL * COL};
    }
    for (unsigned i = 2 * COL - 2; i < 3 * COL - 4; ++i) {
      adf::dimensions(qrd_kernel[i].out[0])  = {ROW * (COL-2) + COL * COL};
      adf::dimensions(qrd_kernel[i+1].in[0]) = {ROW * (COL-2) + COL * COL};
    }
    for (unsigned i = 3 * COL - 4; i < 4 * COL - 7; ++i) {
      adf::dimensions(qrd_kernel[i].out[0])  = {ROW * (COL-3) + COL * COL};
      adf::dimensions(qrd_kernel[i+1].in[0]) = {ROW * (COL-3) + COL * COL};
    }
    for (unsigned i = 4 * COL - 7; i < 5 * COL - 11; ++i) {
      adf::dimensions(qrd_kernel[i].out[0])  = {ROW * (COL-4) + COL * COL};
      adf::dimensions(qrd_kernel[i+1].in[0]) = {ROW * (COL-4) + COL * COL};
    }
    for (unsigned i = 5 * COL - 11; i < 6 * COL - 16; ++i) {
      adf::dimensions(qrd_kernel[i].out[0])  = {ROW * (COL-5) + COL * COL};
      adf::dimensions(qrd_kernel[i+1].in[0]) = {ROW * (COL-5) + COL * COL};
    }
    for (unsigned i = 6 * COL - 16; i < 7 * COL - 22; ++i) {
      adf::dimensions(qrd_kernel[i].out[0])  = {ROW * (COL-6) + COL * COL};
      adf::dimensions(qrd_kernel[i+1].in[0]) = {ROW * (COL-6) + COL * COL};
    }
    adf::dimensions(qrd_kernel[7*COL-22].out[0])         = {ROW * (COL-7) + COL * COL};
    adf::dimensions(qrd_kernel[NUM_QRD_KERNELS-1].in[0]) = {ROW * (COL-7) + COL * COL};
    adf::dimensions(qrd_kernel[NUM_QRD_KERNELS-1].out[0]) = {COL * COL};
  }
};


