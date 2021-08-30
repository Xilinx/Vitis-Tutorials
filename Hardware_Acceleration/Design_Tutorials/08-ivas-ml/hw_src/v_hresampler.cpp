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

#define SAMPLES_PER_CLOCK HSC_SAMPLES_PER_CLOCK // 1, 2, 4

//#define DBG_PRINT

const U8 rgb    = 0;
const U8 yuv444 = 1;
const U8 yuv422 = 2;
const U8 yuv420 = 3;

#define KERNEL_H_SIZE 4

void v_hcresampler(HSC_STREAM_MULTIPIX &srcImg,
                   U16 height,
                   U16 width,
                   U8 inColorMode,
                   bool bPassThru,
                   HSC_STREAM_MULTIPIX &outImg)
{
    I16 y, x, k;
    I16 CRpix, first_pix, last_pix, chroma_out_pix, odd_col;
    I16 center_tap;
    I16 shift;
    I16 yOffset = 0;
    I16 xOffset; //offset between streaming pixel and processing pixel
    I16 loopHeight = height + yOffset;
    I16 loopWidth;
    I16 out_y, out_x;

    YUV_MULTI_PIXEL inpix;
    YUV_MULTI_PIXEL outpix;
    Y_MULTI_PIXEL mpix_y;
    C_MULTI_PIXEL mpix_cb, mpix_cr;

    const U8 PIXBUF_C_DEPTH = (((((((KERNEL_H_SIZE / 2) + 1) + HSC_SAMPLES_PER_CLOCK - 1) + (HSC_SAMPLES_PER_CLOCK - 1)) / HSC_SAMPLES_PER_CLOCK) * HSC_SAMPLES_PER_CLOCK) + (KERNEL_H_SIZE / 2) - 1);
    const U8 PIXBUF_Y_DEPTH = ((PIXBUF_C_DEPTH - ((KERNEL_H_SIZE / 2) - 1)) * 2);

    Y_PIXEL pixbuf_y[PIXBUF_Y_DEPTH];
    C_PIXEL pixbuf_cb[PIXBUF_C_DEPTH], pixbuf_cr[PIXBUF_C_DEPTH];

    if (bPassThru) {
        xOffset    = 0;
        center_tap = 0;
    }
    else if (inColorMode == yuv422) {
        xOffset    = ((PIXBUF_Y_DEPTH / HSC_SAMPLES_PER_CLOCK) - 1);
        center_tap = 0;
    }
    else {
        xOffset    = ((PIXBUF_C_DEPTH - (KERNEL_H_SIZE / 2)) / HSC_SAMPLES_PER_CLOCK);
        center_tap = (PIXBUF_Y_DEPTH - ((xOffset + 1) * HSC_SAMPLES_PER_CLOCK));
    }

    loopWidth = (width / HSC_SAMPLES_PER_CLOCK) + xOffset;

    for (y = 0; y < loopHeight; ++y) {

        for (x = 0; x < loopWidth; ++x) {

#pragma HLS LOOP_FLATTEN OFF
#pragma HLS PIPELINE II = 1

            //processing (output) pixel coordinates
            out_y = y;
            out_x = x - xOffset;

            if (x < (width / HSC_SAMPLES_PER_CLOCK)) {
                srcImg >> inpix;

                // create multi-pixel samples of luma only or chroma only
                for (I16 s = 0; s < HSC_SAMPLES_PER_CLOCK; ++s) {
                    mpix_y.val[s] = inpix.val[s * 3];
                    if (inColorMode == yuv444) {
                        mpix_cb.val[s] = inpix.val[s * 3 + 1];
                        mpix_cr.val[s] = inpix.val[s * 3 + 2];
                    }
                    else {
                        if (((x * HSC_SAMPLES_PER_CLOCK) + s) & 1) {
                            mpix_cr.val[s / 2] = inpix.val[s * 3 + 1];
                        }
                        else {
                            mpix_cb.val[s / 2] = inpix.val[s * 3 + 1];
                        }
                    }
                }
            }
            //luma pixel buffer
#pragma HLS ARRAY_PARTITION variable = &pixbuf_y complete dim = 0
            //shift right by HSC_SAMPLES_PER_CLOCK to make space for next HSC_SAMPLES_PER_CLOCK inputs at top
            for (I16 i = 0; i < (PIXBUF_Y_DEPTH - HSC_SAMPLES_PER_CLOCK); i++) {
                pixbuf_y[i] = pixbuf_y[i + HSC_SAMPLES_PER_CLOCK];
            }
            //push read pixels at FIFO top
            for (k = 0; k < HSC_SAMPLES_PER_CLOCK; k++) {
                pixbuf_y[PIXBUF_Y_DEPTH - 1 - k].val[0] = mpix_y.val[HSC_SAMPLES_PER_CLOCK - 1 - k];
            }

            //chroma pixel buffer
#pragma HLS ARRAY_PARTITION variable = &pixbuf_cb complete dim = 0
#pragma HLS ARRAY_PARTITION variable = &pixbuf_cr complete dim = 0

#if (HSC_SAMPLES_PER_CLOCK == 1)
            shift = 1;
#else
            shift = (inColorMode == yuv444) ? 1 : 2;
#endif

            //push read pixels at FIFO top
            for (k = 0; k < HSC_SAMPLES_PER_CLOCK; k++) {

#if (HSC_SAMPLES_PER_CLOCK == 1)
                first_pix = (inColorMode == yuv444) ? (x == 0) : (x == 1);
                CRpix     = (inColorMode == yuv444) ? 1 : (x & 1);
#else
                first_pix      = (inColorMode == yuv444) ? (x == 0) : ((x == 0) && (k == 1));
                CRpix          = (inColorMode == yuv444) ? 1 : (k & 1);
#endif

                if (CRpix == 1) // work with Cb/Cr pairs
                {
                    //shift right to make space for next input at top
                    for (I16 i = 0; i < (PIXBUF_C_DEPTH - 1); i++) {
                        pixbuf_cb[i] = pixbuf_cb[i + 1];
                        pixbuf_cr[i] = pixbuf_cr[i + 1];
                    }
                    if (x < (width / HSC_SAMPLES_PER_CLOCK)) {
                        pixbuf_cb[PIXBUF_C_DEPTH - 1].val[0] = mpix_cb.val[k / shift];
                        pixbuf_cr[PIXBUF_C_DEPTH - 1].val[0] = mpix_cr.val[k / shift];
                    }
                    else //right edge padding
                    {
                        pixbuf_cb[PIXBUF_C_DEPTH - 1].val[0] = mpix_cb.val[(HSC_SAMPLES_PER_CLOCK / shift) - 1];
                        pixbuf_cr[PIXBUF_C_DEPTH - 1].val[0] = mpix_cr.val[(HSC_SAMPLES_PER_CLOCK / shift) - 1];
                    }
                }
                //left edge padding
                if (first_pix == 1) {
                    for (I16 i = (PIXBUF_C_DEPTH - HSC_SAMPLES_PER_CLOCK); i >= 0; --i) {
                        pixbuf_cb[i].val[0] = mpix_cb.val[0];
                        pixbuf_cr[i].val[0] = mpix_cr.val[0];
                    }
                    if ((inColorMode == yuv422) && (SAMPLES_PER_CLOCK != 1)) {
                        for (I16 i = (PIXBUF_C_DEPTH - (SAMPLES_PER_CLOCK / 2)); i > (PIXBUF_C_DEPTH - SAMPLES_PER_CLOCK); --i) {
                            pixbuf_cb[i] = mpix_cb.val[0];
                            pixbuf_cr[i] = mpix_cr.val[0];
                        }
                    }
                }
            }

            for (k = 0; k < HSC_SAMPLES_PER_CLOCK; ++k) {

#if (HSC_SAMPLES_PER_CLOCK == 1)
                odd_col        = (out_x & 1);
                chroma_out_pix = (inColorMode == yuv422) ? 1 : !(out_x & 1);
#else
                odd_col        = (k & 1);
                chroma_out_pix = (inColorMode == yuv422) ? 1 : !(k & 1);
#endif

                long filt_res0, filt_res1;
                if (inColorMode == yuv444) {
                    // 444 to 422 fixed coef filtering [ 1/4 1/2 1/4 ]
                    // luma component
                    outpix.val[k * 3] = pixbuf_y[center_tap + k].val[0];

                    if (chroma_out_pix == 1) {
                        // filter
                        filt_res0 = (pixbuf_cb[0 + ((k / 2) * 2)].val[0] + 2 * pixbuf_cb[1 + ((k / 2) * 2)].val[0] + pixbuf_cb[2 + ((k / 2) * 2)].val[0] + 2) / 4;
                        filt_res1 = (pixbuf_cr[0 + ((k / 2) * 2)].val[0] + 2 * pixbuf_cr[1 + ((k / 2) * 2)].val[0] + pixbuf_cr[2 + ((k / 2) * 2)].val[0] + 2) / 4;
                    }
                    outpix.val[k * 3 + 1] = (odd_col) ? filt_res1 : filt_res0;
                    outpix.val[k * 3 + 2] = 0;
                }
                else {
                    // 422 to 444 fixed coef filtering [ 1/2 1/2 ]
                    // luma component
                    outpix.val[k * 3] = pixbuf_y[center_tap + k].val[0];
                    if (odd_col) // odd cols (1, 3, 5, etc)
                    {
                        //interpolate by averaging nearest neighbors
                        outpix.val[k * 3 + 1] = (pixbuf_cb[2 + (k / 2)].val[0] + pixbuf_cb[1 + (k / 2)].val[0] + 1) / 2;
                        outpix.val[k * 3 + 2] = (pixbuf_cr[2 + (k / 2)].val[0] + pixbuf_cr[1 + (k / 2)].val[0] + 1) / 2;
                    }
                    else // even cols (0, 2, 4, etc)
                    {
                        // passthru co-sited pixel
                        outpix.val[k * 3 + 1] = pixbuf_cb[1 + (k / 2)].val[0];
                        outpix.val[k * 3 + 2] = pixbuf_cr[1 + (k / 2)].val[0];
                    }
                }
            }

            if ((out_y >= 0) && (out_x >= 0)) {
                outImg << (bPassThru ? inpix : outpix);
            }
        }
    }
}
