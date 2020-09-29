/*
 * Copyright 2020 Xilinx, Inc.
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

/*
 * HLS Video Header File
 */

#ifndef ___HLS__VIDEO__
#define ___HLS__VIDEO__

#ifndef __cplusplus
#error C++ is required to include this header file
#endif

/* for safety*/
#if (defined(ROWS)|| defined(COLS) || defined(SRC_T) || defined(SRC1_T) || defined(SRC2_T) || defined(DST_T))
#error One or more of the following is defined: ROWS, COLS, SRC_T, SRC1_T, SRC2_T, DST_T. Definition conflicts with their usage as template parameters. 
#endif

#include "hls/hls_axi_io.h"
#include "hls_math.h"
#include "hls_stream.h"

#include "utils/x_hls_utils.h"
#include "utils/x_hls_traits.h"
#include "utils/x_hls_defines.h"
#include "hls/hls_video_types.h"
#include "hls/hls_video_mem.h"
#include "hls/hls_video_core.h"
#include "hls/hls_video_imgbase.h"
#include "hls/hls_video_io.h"

#include "hls/hls_video_arithm.h"
#include "hls/hls_video_imgproc.h"
#include "hls/hls_video_histogram.h"
#include "hls/hls_video_fast.h"
#include "hls/hls_video_undistort.h"
#include "hls/hls_video_hough.h"
#include "hls/hls_video_harris.h"
#include "hls/hls_video_haar.h"
#include "hls/hls_video_stereobm.h"

#endif


