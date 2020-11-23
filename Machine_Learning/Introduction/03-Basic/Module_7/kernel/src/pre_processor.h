/*
 * Copyright 2019 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _PRE_PROCESSOR_H_
#define _PRE_PROCESSOR_H_

#include "ap_int.h"
#include "common/xf_common.hpp"
#include "common/xf_utility.hpp"
#include "hls_stream.h"
#include "imgproc/xf_crop.hpp"
#include "imgproc/xf_cvt_color.hpp"
#include "imgproc/xf_resize.hpp"

#define AXI_WIDTH 128
#define NPC XF_NPPC2
#define TYPE XF_8UC3
#define MAX_DOWN_SCALE 7
#define MAX_IN_WIDTH 3840
#define MAX_IN_HEIGHT 2160
#define MAX_OUT_WIDTH 3840
#define MAX_OUT_HEIGHT 2160
#define STREAM_DEPTH 2

extern "C" {
void pre_processor(ap_uint<AXI_WIDTH> *image_in, ap_uint<AXI_WIDTH> *image_out,
		ap_uint<AXI_WIDTH> *image_out_full, int width_in, int height_in,
		int width_out, int height_out);
}

#endif
