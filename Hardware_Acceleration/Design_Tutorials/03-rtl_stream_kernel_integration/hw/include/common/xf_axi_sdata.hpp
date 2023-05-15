/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

/*
 * This file contains the definition of the data types for AXI streaming.
 * ap_axi_s is a signed interpretation of the AXI stream
 * ap_axi_u is an unsigned interpretation of the AXI stream
 */

#ifndef __XF__AXI_SDATA__
#define __XF__AXI_SDATA__

#include "ap_int.h"

namespace xf {
namespace cv {

template <int D, int U, int TI, int TD>
struct ap_axis {
    ap_int<D> data;
    ap_uint<(D + 7) / 8> keep;
    ap_uint<(D + 7) / 8> strb;
    ap_uint<U> user;
    ap_uint<1> last;
    ap_uint<TI> id;
    ap_uint<TD> dest;
};

template <int D, int U, int TI, int TD>
struct ap_axiu {
    ap_uint<D> data;
    ap_uint<(D + 7) / 8> keep;
    ap_uint<(D + 7) / 8> strb;
    ap_uint<U> user;
    ap_uint<1> last;
    ap_uint<TI> id;
    ap_uint<TD> dest;
};

// typedef ap_axis<int D, int U, int TI, int TD> ap_axis_unsigned<int D, int U, int TI, int TD>;
} // namespace xf
} // namespace cv
#endif
