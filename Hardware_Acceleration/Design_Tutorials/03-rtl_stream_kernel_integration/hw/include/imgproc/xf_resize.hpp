/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#ifndef _XF_RESIZE_
#define _XF_RESIZE_

#include "xf_resize_headers.h"

/**
 * Image resizing function.
 */
namespace xf {
namespace cv {

template <int INTERPOLATION_TYPE,
          int TYPE,
          int SRC_ROWS,
          int SRC_COLS,
          int DST_ROWS,
          int DST_COLS,
          int NPC,
          int MAX_DOWN_SCALE>
void resize(xf::cv::Mat<TYPE, SRC_ROWS, SRC_COLS, NPC>& _src, xf::cv::Mat<TYPE, DST_ROWS, DST_COLS, NPC>& _dst) {
// clang-format off
    #pragma HLS INLINE OFF
    // clang-format on

    assert(((INTERPOLATION_TYPE == XF_INTERPOLATION_NN) || (INTERPOLATION_TYPE == XF_INTERPOLATION_BILINEAR) ||
            (INTERPOLATION_TYPE == XF_INTERPOLATION_AREA)) &&
           "Incorrect parameters interpolation type");

    if (INTERPOLATION_TYPE == XF_INTERPOLATION_AREA) {
        assert(((_src.rows <= SRC_ROWS) && (_src.cols <= SRC_COLS)) &&
               "SRC_ROWS and SRC_COLS should be greater than input image");
        assert(((_dst.rows <= DST_ROWS) && (_dst.cols <= DST_COLS)) &&
               "DST_ROWS and DST_COLS should be greater than output image");

        if ((SRC_ROWS < DST_ROWS) || (SRC_COLS < DST_COLS)) {
            xFResizeAreaUpScale<SRC_ROWS, SRC_COLS, XF_CHANNELS(TYPE, NPC), TYPE, NPC, XF_WORDWIDTH(TYPE, NPC),
                                DST_ROWS, DST_COLS, (SRC_COLS >> XF_BITSHIFT(NPC)), (DST_COLS >> XF_BITSHIFT(NPC))>(
                _src, _dst);
        } else if ((SRC_ROWS >= DST_ROWS) || (SRC_COLS >= DST_COLS)) {
            xFResizeAreaDownScale<SRC_ROWS, SRC_COLS, XF_CHANNELS(TYPE, NPC), TYPE, NPC, XF_WORDWIDTH(TYPE, NPC),
                                  DST_ROWS, DST_COLS, (SRC_COLS >> XF_BITSHIFT(NPC)), (DST_COLS >> XF_BITSHIFT(NPC))>(
                _src, _dst);
        }

        return;
    } else {
        resizeNNBilinear<TYPE, SRC_ROWS, SRC_COLS, NPC, DST_ROWS, DST_COLS, INTERPOLATION_TYPE, MAX_DOWN_SCALE>(_src,
                                                                                                                _dst);
    }
}
} // namespace cv
} // namespace xf
#endif
