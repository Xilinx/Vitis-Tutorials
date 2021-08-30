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

#define KERNEL_V_SIZE 3
#define CHROMA_LINES 3
#define LUMA_LINES 2

typedef ap_uint<SAMPLES_PER_CLOCK * VSC_BITS_PER_COMPONENT> Y_MEM_PIXEL_TYPE;
typedef ap_uint<SAMPLES_PER_CLOCK * VSC_BITS_PER_COMPONENT> C_MEM_PIXEL_TYPE;
typedef hls::LineBuffer<LUMA_LINES, (MAX_COLS / SAMPLES_PER_CLOCK), Y_MEM_PIXEL_TYPE> LINE_BUFFER_Y;
typedef hls::LineBuffer<CHROMA_LINES, (MAX_COLS / SAMPLES_PER_CLOCK), C_MEM_PIXEL_TYPE> LINE_BUFFER_C;

void v_vcresampler(VSC_STREAM_MULTIPIX &srcImg,
                   U16 height,
                   U16 width,
                   U8 inColorMode,
                   bool bPassThru,
                   VSC_STREAM_MULTIPIX &outImg)
{
    I16 y, x, k;
    I16 yOffset; //offset between streaming row and processing row
    I16 xOffset = 0;
    I16 loopHeight;
    I16 loopWidth = (width / SAMPLES_PER_CLOCK) + xOffset;
    I16 out_y, out_x;
    I16 ChromaLine;


    YUV_MULTI_PIXEL pix;
    YUV_MULTI_PIXEL outpix;
    Y_MULTI_PIXEL mpix_y;
    C_MULTI_PIXEL mpix_c;
    //local storage for line buffer column (to avoid multiple read clients on BRAM)
    Y_MULTI_PIXEL pixbuf_y[LUMA_LINES + 1];
    C_MULTI_PIXEL pixbuf_c[KERNEL_V_SIZE];
    Y_MEM_PIXEL_TYPE InYPix;
    C_MEM_PIXEL_TYPE InCPix;
    LINE_BUFFER_Y linebuf_y;
    LINE_BUFFER_C linebuf_c;

#if (USE_URAM == 1)
#pragma HLS RESOURCE variable = &linebuf_y core = XPM_MEMORY uram
#pragma HLS RESOURCE variable = &linebuf_c core = XPM_MEMORY uram
#endif

    if (bPassThru) {
        yOffset = 0;
    }
    else if (inColorMode == yuv420) {
        yOffset = 2;
    }
    else {
        yOffset = 1;
    }

    loopHeight = height + yOffset;

    for (y = 0; y < loopHeight; ++y) {

        for (x = 0; x < loopWidth; ++x) {

#pragma HLS LOOP_FLATTEN OFF
#pragma HLS PIPELINE II = 1

            //processing (output) pixel coordinates
            out_y = y - yOffset;
            out_x = x; //x - xOffset;

            if (y < height) {
                srcImg >> pix;
                // create multi-pixel samples of luma only or chroma only
                for (I16 s = 0; s < SAMPLES_PER_CLOCK; ++s) {
                    mpix_y.val[s] = pix.val[s * NUM_VIDEO_COMPONENTS];
                    mpix_c.val[s] = pix.val[s * NUM_VIDEO_COMPONENTS + 1];
                }
            }

            //luma line buffer
#pragma HLS ARRAY_PARTITION variable = &pixbuf_y complete dim = 0
            // get column of pixels from the line buffer to local pixel array
            for (I16 i = 0; i < LUMA_LINES; i++) {
                Y_MEM_PIXEL_TYPE LineBufVal = linebuf_y.getval(i, x);
                for (int l = 0; l < SAMPLES_PER_CLOCK; l++) {
                    int start                           = (l * VSC_BITS_PER_COMPONENT);
                    pixbuf_y[LUMA_LINES - 1 - i].val[l] = LineBufVal(start + VSC_BITS_PER_COMPONENT - 1, start);
                }
            }

            // get new pixels from stream
            for (int l = 0; l < SAMPLES_PER_CLOCK; l++) {
                int start                                         = (l * VSC_BITS_PER_COMPONENT);
                InYPix(start + VSC_BITS_PER_COMPONENT - 1, start) = mpix_y.val[l];
            }
            linebuf_y.insert_bottom(InYPix, x); //pix does not change after final line - bottom edge padding
            pixbuf_y[LUMA_LINES] = mpix_y;

            for (I16 i = LUMA_LINES - 1; i > 0; i--) // for circular buffer implementation
            {
                //on first line, fill line buffer with first pixel value - top edge padding
                Y_MEM_PIXEL_TYPE LineBufVal;
                Y_MULTI_PIXEL PixBufVal = (y > 0) ? pixbuf_y[LUMA_LINES - i] : pixbuf_y[LUMA_LINES];
                for (int l = 0; l < SAMPLES_PER_CLOCK; l++) {
                    int start                                             = (l * VSC_BITS_PER_COMPONENT);
                    LineBufVal(start + VSC_BITS_PER_COMPONENT - 1, start) = PixBufVal.val[l];
                }
                linebuf_y.val[i][x] = LineBufVal;
            }

            // chroma line buffer
#pragma HLS ARRAY_PARTITION variable = &pixbuf_c complete dim = 0
            ChromaLine = ((y & 1) && (inColorMode == yuv420)) ? 0 : 1;
            // get column of pixels from the line buffer to local pixel array
            for (I16 i = 0; i < CHROMA_LINES - 1; i++) {
                C_MULTI_PIXEL CBufVal;
                C_MEM_PIXEL_TYPE LineBufVal = linebuf_c.getval(i, x);
                for (int l = 0; l < SAMPLES_PER_CLOCK; l++) {
                    int start      = (l * VSC_BITS_PER_COMPONENT);
                    CBufVal.val[l] = LineBufVal(start + VSC_BITS_PER_COMPONENT - 1, start);
                }
                if (ChromaLine == 1) {
                    pixbuf_c[CHROMA_LINES - 1 - i - 1] = CBufVal;
                }
                else {
                    pixbuf_c[CHROMA_LINES - 1 - i] = CBufVal;
                }
            }

            if (ChromaLine == 1) {
                if (y < height) {
                    pixbuf_c[CHROMA_LINES - 1] = mpix_c;
                }
                else {
                    pixbuf_c[CHROMA_LINES - 1] = pixbuf_c[CHROMA_LINES - 2];
                }
            }
            else {
                C_MEM_PIXEL_TYPE LineBufCVal = linebuf_c.getval(CHROMA_LINES - 1, x);
                for (int l = 0; l < SAMPLES_PER_CLOCK; l++) {
                    int start          = (l * VSC_BITS_PER_COMPONENT);
                    pixbuf_c[0].val[l] = LineBufCVal(start + VSC_BITS_PER_COMPONENT - 1, start);
                }
            }

            if (ChromaLine == 1) {
                if (y < height) //only insert new pixel if reading in new chroma line from image - bottom edge padding
                {
                    for (int l = 0; l < SAMPLES_PER_CLOCK; l++) {
                        int start                                         = (l * VSC_BITS_PER_COMPONENT);
                        InCPix(start + VSC_BITS_PER_COMPONENT - 1, start) = mpix_c.val[l];
                    }
                    linebuf_c.insert_bottom(InCPix, x); //push read data to line buffer
                }
                for (I16 i = CHROMA_LINES - 2; i > 0; i--) // for circular buffer implementation
                {
                    //on first line, fill line buffer with first pixel value - top edge padding
                    C_MEM_PIXEL_TYPE LineBufVal;
                    C_MULTI_PIXEL PixBufVal = (y > 0) ? pixbuf_c[CHROMA_LINES - i - 1] : pixbuf_c[CHROMA_LINES - 1];
                    for (int l = 0; l < SAMPLES_PER_CLOCK; l++) {
                        int start                                             = (l * VSC_BITS_PER_COMPONENT);
                        LineBufVal(start + VSC_BITS_PER_COMPONENT - 1, start) = PixBufVal.val[l];
                    }
                    linebuf_c.val[i][x] = LineBufVal;
                }
            }

            for (k = 0; k < SAMPLES_PER_CLOCK; ++k) {
                {
                    if (inColorMode == yuv422) {
                        // 422 to 420 fixed coef filtering [ 1/4 1/2 1/4 ]
                        // luma component
                        outpix.val[k * NUM_VIDEO_COMPONENTS] = pixbuf_y[1].val[k];
                        if (out_y & 1) // odd rows (1, 3, 5, etc)
                        {
                            // set rows with no chroma information to zero
                            outpix.val[k * NUM_VIDEO_COMPONENTS + 1] = 0;
                        }
                        else // even rows (0, 2, 4, etc)
                        {
                            // interpolate co-sited pixel
                            outpix.val[k * NUM_VIDEO_COMPONENTS + 1] = (pixbuf_c[0].val[k] + 2 * pixbuf_c[1].val[k] + pixbuf_c[2].val[k] + 2) / 4;
                        }
                    }
                    else {
                        // 420 to 422 fixed coef filtering [ 1/2 1/2 ]
                        // luma component
                        outpix.val[k * NUM_VIDEO_COMPONENTS] = pixbuf_y[0].val[k];
                        if (out_y & 1) // odd rows (1, 3, 5, etc)
                        {
                            //interpolate by averaging nearest neighbors
                            outpix.val[k * NUM_VIDEO_COMPONENTS + 1] = (pixbuf_c[1].val[k] + pixbuf_c[2].val[k] + 1) / 2;
                        }
                        else // even rows (0, 2, 4, etc)
                        {
                            // passthru co-sited pixel
                            outpix.val[k * NUM_VIDEO_COMPONENTS + 1] = pixbuf_c[1].val[k];
                        }
                    }
                }

#if (NUM_VIDEO_COMPONENTS == 3)
                // set unused third video component to zero
                outpix.val[k * NUM_VIDEO_COMPONENTS + 2] = 0;
#endif

            } // for (k

            if ((out_y >= 0) && (out_x >= 0)) {
                outImg << (bPassThru ? pix : outpix);
            }

        } //for(x
    }     //for(y
}
