/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#ifndef __XF_PYR_DENSE_OPTICAL_FLOW_WRAPPER__
#define __XF_PYR_DENSE_OPTICAL_FLOW_WRAPPER__

#include "hls_stream.h"
#include "ap_int.h"
#include "common/xf_common.hpp"
#include "video/xf_pyr_dense_optical_flow.hpp"

namespace xf {
namespace cv {

template <int NUM_PYR_LEVELS,
          int NUM_LINES,
          int WINSIZE,
          int FLOW_WIDTH,
          int FLOW_INT,
          int TYPE,
          int ROWS,
          int COLS,
          int NPC,
          bool USE_URAM = false>
void densePyrOpticalFlow(xf::cv::Mat<XF_8UC1, ROWS, COLS, XF_NPPC1>& _current_img,
                         xf::cv::Mat<XF_8UC1, ROWS, COLS, XF_NPPC1>& _next_image,
                         xf::cv::Mat<XF_32UC1, ROWS, COLS, XF_NPPC1>& _streamFlowin,
                         xf::cv::Mat<XF_32UC1, ROWS, COLS, XF_NPPC1>& _streamFlowout,
                         const int level,
                         const unsigned char scale_up_flag,
                         float scale_in,
                         ap_uint<1> init_flag) {
// clang-format off
    #pragma HLS INLINE OFF
    // clang-format on
    xFLKOpticalFlowDenseKernel<ROWS, COLS, NUM_PYR_LEVELS, NUM_LINES, WINSIZE, FLOW_WIDTH, FLOW_INT, USE_URAM>(
        _current_img, _next_image, _streamFlowin, _streamFlowout, _current_img.rows, _current_img.cols,
        _streamFlowin.rows, _streamFlowin.cols, level, scale_up_flag, scale_in, init_flag);
}
} // namespace cv
} // namespace xf
#endif
