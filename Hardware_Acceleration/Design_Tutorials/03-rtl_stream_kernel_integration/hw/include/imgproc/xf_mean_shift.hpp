/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#ifndef _XF_MEAN_SHIFT_WRAPPER_HPP_
#define _XF_MEAN_SHIFT_WRAPPER_HPP_

#include "hls_stream.h"
#include "ap_int.h"
#include "common/xf_common.hpp"
#include "xf_mean_shift_kernel.hpp"

namespace xf {
namespace cv {
template <int MAXOBJ, int MAXITERS, int OBJ_ROWS, int OBJ_COLS, int SRC_T, int ROWS, int COLS, int NPC>
void MeanShift(xf::cv::Mat<SRC_T, ROWS, COLS, NPC>& _in_mat,
               uint16_t* x1,
               uint16_t* y1,
               uint16_t* obj_height,
               uint16_t* obj_width,
               uint16_t* dx,
               uint16_t* dy,
               uint16_t* status,
               uint8_t frame_status,
               uint8_t no_objects,
               uint8_t no_iters) {
    // local arrays for memcopy
    uint16_t img_height[1], img_width[1], objects[1], frame[1];
    uint16_t tlx[MAXOBJ], tly[MAXOBJ], _obj_height[MAXOBJ], _obj_width[MAXOBJ], dispx[MAXOBJ], dispy[MAXOBJ];
    uint16_t track_status[MAXOBJ];

    for (int i = 0; i < no_objects; i++) {
// clang-format off
        #pragma HLS LOOP_TRIPCOUNT min=1 max=MAXOBJ
        #pragma HLS PIPELINE II=1
        // clang-format on
        tlx[i] = x1[i];
        tly[i] = y1[i];
        _obj_width[i] = obj_width[i];
        _obj_height[i] = obj_height[i];
        dispx[i] = dx[i];
        dispy[i] = dy[i];
        track_status[i] = status[i];
    }

    xFMeanShiftKernel<OBJ_ROWS, OBJ_COLS, SRC_T, ROWS, COLS, MAXOBJ, MAXITERS, NPC>(
        _in_mat, tlx, tly, _obj_height, _obj_width, dispx, dispy, track_status, frame_status, no_objects, no_iters);

    for (int i = 0; i < no_objects; i++) {
// clang-format off
        #pragma HLS LOOP_TRIPCOUNT min=1 max=MAXOBJ
        #pragma HLS PIPELINE II=1
        // clang-format on
        dx[i] = dispx[i];
        dy[i] = dispy[i];
        status[i] = track_status[i];
    }
}
} // namespace cv
} // namespace xf

#endif // _XF_MEAN_SHIFT_WRAPPER_HPP_
