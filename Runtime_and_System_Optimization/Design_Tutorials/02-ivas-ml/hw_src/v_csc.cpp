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

#ifndef __SYNTHESIS__
#include <stdio.h>
#endif
#include "v_multi_scaler.h"

#include <assert.h>

#define MAX_COLS VSC_MAX_WIDTH
#define SAMPLES_PER_CLOCK VSC_SAMPLES_PER_CLOCK // 1, 2, 4
#define NUM_VIDEO_COMPONENTS VSC_NR_COMPONENTS

//#define DBG_PRINT
const U8 rgb    = 0;
const U8 yuv444 = 1;
const U8 yuv422 = 2;
const U8 yuv420 = 3;

void v_csc(
    HSC_STREAM_MULTIPIX &srcImg,
    U16 height,
    U16 width,
    U8 colorMode,
    bool bPassThru,
    HSC_STREAM_MULTIPIX &outImg)
{
    U16 y, x, k;

    YUV_MULTI_PIXEL srcpix;
    YUV_MULTI_PIXEL dstpix;

    assert(width <= HSC_MAX_WIDTH);
    assert(height <= HSC_MAX_HEIGHT);

    for (y = 0; y < (height); ++y) {
        for (x = 0; x < (width) / HSC_SAMPLES_PER_CLOCK; ++x) {

#pragma HLS LOOP_FLATTEN OFF
#pragma HLS PIPELINE

            srcImg >> srcpix;

            for (k = 0; k < HSC_SAMPLES_PER_CLOCK; ++k) {
                PIXEL_TYPE r_y, g_u, b_v;

                r_y = srcpix.val[k * HSC_NR_COMPONENTS + 0];
                g_u = srcpix.val[k * HSC_NR_COMPONENTS + 1];
                b_v = srcpix.val[k * HSC_NR_COMPONENTS + 2];

                // yuv to rgb
                int Cr = b_v - (1 << (HSC_BITS_PER_COMPONENT - 1));
                int Cb = g_u - (1 << (HSC_BITS_PER_COMPONENT - 1));
                int r  = (int)r_y + (((int)Cr * 1733) >> 10);
                int g  = (int)r_y - (((int)Cb * 404 + (int)Cr * 595) >> 10);
                int b  = (int)r_y + (((int)Cb * 2081) >> 10);

                // rgb to yuv
                int y = (306 * (int)r_y + 601 * (int)g_u + 117 * (int)b_v) >> 10;
                int u = (1 << (HSC_BITS_PER_COMPONENT - 1)) + ((((int)b_v - (int)y) * 504) >> 10);
                int v = (1 << (HSC_BITS_PER_COMPONENT - 1)) + ((((int)r_y - (int)y) * 898) >> 10);

                dstpix.val[k * HSC_NR_COMPONENTS + 0] = MAX(MIN((colorMode != rgb) ? r : y, (1 << HSC_BITS_PER_COMPONENT) - 1), 0);
                dstpix.val[k * HSC_NR_COMPONENTS + 1] = MAX(MIN((colorMode != rgb) ? g : u, (1 << HSC_BITS_PER_COMPONENT) - 1), 0);
                dstpix.val[k * HSC_NR_COMPONENTS + 2] = MAX(MIN((colorMode != rgb) ? b : v, (1 << HSC_BITS_PER_COMPONENT) - 1), 0);
            }
            outImg << ((bPassThru) ? srcpix : dstpix);
        }
    }
}
