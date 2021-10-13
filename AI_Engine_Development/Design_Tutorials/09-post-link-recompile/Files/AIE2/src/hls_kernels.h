/*
 * (c) Copyright 2021 Xilinx, Inc. All rights reserved.
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
 *
 */


#ifndef HLS_KERNELS_H
#define HLS_KERNELS_H

#include "hls_stream.h"
#include <adf.h>

////// This header file is for adf only. HLS cpp does NOT include this header. //////

void polar_clip(adf::dir::in<hls::stream<ap_axis<32,0,0,0>>&> in_sample, adf::dir::out<hls::stream<ap_axis<32,0,0,0>>&> out_sample);

#endif
