/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#ifndef __cplusplus
#error C++ is needed to include this header
#endif

#include "hls_stream.h"
#include "common/xf_common.hpp"
#include "common/xf_utility.hpp"

namespace xf {
namespace cv {

template <int MAXDELAY, int SRC_T, int ROWS, int COLS, int NPC>
void delayMat(xf::cv::Mat<SRC_T, ROWS, COLS, NPC>& _src, xf::cv::Mat<SRC_T, ROWS, COLS, NPC>& _dst) {
// clang-format off
    #pragma HLS inline off
    // clang-format on

    // clang-format off
  //  #pragma HLS dataflow
    // clang-format on

    hls::stream<XF_TNAME(SRC_T, NPC)> src;
    hls::stream<XF_TNAME(SRC_T, NPC)> dst;

/********************************************************/

Read_yuyv_Loop:
    for (int i = 0; i < _src.rows; i++) {
// clang-format off
        #pragma HLS LOOP_TRIPCOUNT min=1 max=ROWS
        // clang-format on
        for (int j = 0; j<(_src.cols)>> (XF_BITSHIFT(NPC)); j++) {
// clang-format off
            #pragma HLS LOOP_TRIPCOUNT min=1 max=COLS/NPC
            #pragma HLS PIPELINE
            #pragma HLS loop_flatten off
            // clang-format on
            src.write(_src.read(i * (_src.cols >> (XF_BITSHIFT(NPC))) + j));
        }
    }

// clang-format off
    #pragma HLS stream depth=MAXDELAY variable=src
    // clang-format on

    for (int i = 0; i < _dst.rows; i++) {
// clang-format off
        #pragma HLS LOOP_TRIPCOUNT min=1 max=ROWS
        // clang-format on
        for (int j = 0; j<(_dst.cols)>> (XF_BITSHIFT(NPC)); j++) {
// clang-format off
            #pragma HLS LOOP_TRIPCOUNT min=1 max=COLS/NPC
            #pragma HLS PIPELINE
            #pragma HLS loop_flatten off
            // clang-format on
            _dst.write((i * (_dst.cols >> (XF_BITSHIFT(NPC))) + j), src.read());
        }
    }
}
} // namespace cv
} // namespace xf
