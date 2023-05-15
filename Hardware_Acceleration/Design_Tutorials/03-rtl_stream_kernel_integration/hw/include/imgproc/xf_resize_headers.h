/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#ifndef _XF_RESIZE_HEADERS_
#define _XF_RESIZE_HEADERS_
#include "hls_stream.h"
#include "ap_int.h"
#include "common/xf_common.hpp"
#include "common/xf_utility.hpp"

#define POW16 65536
#define POW32 4294967296 // 2^32

#include "imgproc/xf_resize_nn_bilinear.hpp"
#include "imgproc/xf_resize_down_area.hpp"
#include "imgproc/xf_resize_up_area.hpp"

#endif
