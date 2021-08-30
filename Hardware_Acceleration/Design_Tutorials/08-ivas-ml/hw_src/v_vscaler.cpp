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
#include <stdio.h>

#if (VSC_SCALE_MODE == VSC_BILINEAR)
static void vscale_core_bilinear(VSC_STREAM_MULTIPIX &SrcImg,
                                 U16 HeightIn,
                                 U16 WidthIn,
                                 U16 HeightOut,
                                 U32 LineRate,
                                 VSC_STREAM_MULTIPIX &OutImg);

static void vscale_bilinear(YUV_MULTI_PIXEL PixArray[VSC_TAPS],
                            U8 PhaseV,
                            YUV_MULTI_PIXEL *OutPix);
#endif

#if (VSC_SCALE_MODE == VSC_BICUBIC)
static void vscale_core_bicubic(VSC_STREAM_MULTIPIX &SrcImg,
                                U16 HeightIn,
                                U16 WidthIn,
                                U16 HeightOut,
                                U32 LineRate,
                                VSC_STREAM_MULTIPIX &OutImg);

static void vscale_bicubic(YUV_MULTI_PIXEL PixArray[VSC_TAPS],
                           U8 PhaseV,
                           YUV_MULTI_PIXEL *OutPix);
#endif

#if (VSC_SCALE_MODE == VSC_POLYPHASE)
static void vscale_core_polyphase(VSC_STREAM_MULTIPIX &SrcImg,
                                  U16 HeightIn,
                                  U16 WidthIn,
                                  U16 HeightOut,
                                  U32 LineRate,
                                  I16 vfltCoeff[VSC_PHASES][VSC_TAPS],
                                  VSC_STREAM_MULTIPIX &OutImg);

static void vscale_polyphase(I16 vfltCoeff[VSC_PHASES][VSC_TAPS],
                             YUV_MULTI_PIXEL PixArray[VSC_TAPS],
                             U8 PhaseV,
                             YUV_MULTI_PIXEL *OutPix);

#endif

/*********************************************************************************
* Function:    vscale
* Parameters:  Stream of input/output pixels, image resolution, type of scaling etc
* Return:
* Description: Top level function to perform vertical image resizing
**********************************************************************************/
void v_vscaler(VSC_STREAM_MULTIPIX &stream_in,
               U16 HeightIn,
               U16 WidthIn,
               U16 HeightOut,
               U32 LineRate,
#if (VSC_SCALE_MODE == VSC_POLYPHASE)
               I16 vfltCoeff[VSC_PHASES][VSC_TAPS],
#endif
               VSC_STREAM_MULTIPIX &stream_out)
{

#if (VSC_SCALE_MODE == VSC_BILINEAR)
    {
        vscale_core_bilinear(stream_in, HeightIn, WidthIn, HeightOut, LineRate, stream_out);
    }
#elif (VSC_SCALE_MODE == VSC_BICUBIC)
    {
        vscale_core_bicubic(stream_in, HeightIn, WidthIn, HeightOut, LineRate, stream_out);
    }
#else
    {
        vscale_core_polyphase(stream_in, HeightIn, WidthIn, HeightOut, LineRate, vfltCoeff, stream_out);
    }
#endif
}

/*********************************************************************************
* Function:    vscale_core
* Parameters:  Stream of input/output, image resolution, type of scaling etc
* Return:
* Description: Perform vertical image resizing
* Sub modules - vscale_linear
*               vscale_cubic
*               vscale_poly
**********************************************************************************/

#if (VSC_SCALE_MODE == VSC_POLYPHASE)

static void vscale_core_polyphase(VSC_STREAM_MULTIPIX &SrcImg,
                                  U16 HeightIn,
                                  U16 WidthIn,
                                  U16 HeightOut,
                                  U32 LineRate,
                                  I16 vfltCoeff[VSC_PHASES][VSC_TAPS],
                                  VSC_STREAM_MULTIPIX &OutImg)
{
    U16 InLines  = HeightIn;
    U16 InPixels = WidthIn;
    U16 OutLines = HeightOut;
    U32 Rate     = LineRate;

    bool GetNewLine;
    bool OutputWriteEn;

    U16 PixArrayLoc;
    U16 TotalLines, YLoopSize, XLoopSize;
    YUV_MULTI_PIXEL SrcPix, OutPix;
    YUV_MULTI_PIXEL PixArray[VSC_TAPS];
    MEM_LINE_BUFFER LineBuf;

#if (USE_URAM == 1)
#pragma HLS RESOURCE variable = &LineBuf core = XPM_MEMORY uram
#endif

    assert(OutLines <= VSC_MAX_HEIGHT);

    // Loop over max of input and output resolution, add run-in/out for taps
    TotalLines = (OutLines > InLines) ? OutLines : InLines;
    YLoopSize  = TotalLines + (VSC_TAPS >> 1);
    XLoopSize  = (InPixels + (VSC_SAMPLES_PER_CLOCK - 1)) / VSC_SAMPLES_PER_CLOCK;

    assert(YLoopSize < (VSC_MAX_HEIGHT + VSC_TAPS));
    assert(XLoopSize <= VSC_MAX_WIDTH);

#pragma HLS ARRAY_PARTITION variable = &SrcPix complete dim = 0
#pragma HLS ARRAY_PARTITION variable = &PixArray complete dim = 0
#pragma HLS ARRAY_PARTITION variable = &OutPix complete dim = 0

    GetNewLine  = 1;
    PixArrayLoc = 0;

    int GetLine      = 0;
    U8 PhaseV        = 0;
    U32 offset       = 0;
    int ReadLoc      = 0;
    int WriteLoc     = 0;
    int WriteLocNext = 0;

loop_height:
    for (U16 y = 0; y < YLoopSize; y++) {
        OutputWriteEn = 0;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        // Step and phase calculation
        //
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (y >= (VSC_TAPS >> 1)) {

            PhaseV   = ((offset >> (STEP_PRECISION_SHIFT - VSC_PHASE_SHIFT))) & (VSC_PHASES - 1);
            WriteLoc = WriteLocNext;

            GetNewLine = 0;
            if ((offset >> STEP_PRECISION_SHIFT) != 0) {
                // Take a new sample from input
                ReadLoc    = GetLine;
                GetNewLine = 1;
                GetLine++;
                PixArrayLoc++;
                offset        = offset - (1 << STEP_PRECISION_SHIFT);
                OutputWriteEn = 0;
                WriteLocNext  = WriteLoc;
            }

            if (((offset >> STEP_PRECISION_SHIFT) == 0) && (WriteLoc < (U32)OutLines)) {
                // Produce a new output sample
                offset        = offset + Rate;
                OutputWriteEn = 1;
                WriteLocNext  = WriteLoc + 1;
            }
            //printf("Readloc %d writeloc %d GetNewLine %d  OutputWriteEn %d PhaseV %d offset %d\n", ReadLoc, WriteLoc, GetNewLine, OutputWriteEn, PhaseV, offset);
        }

    loop_width_for_procpix:
        for (U16 x = 0; x < XLoopSize; x++) // the loop runs for the max of (in,out)
        {
#pragma HLS LOOP_FLATTEN OFF
#pragma HLS pipeline II = 1

            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //
            // Line buffer control
            //
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            for (I16 i = 0; i < VSC_TAPS; i++) // get cols of pixels from the line buffer to local pixel array
            {
                MEM_PIXEL_TYPE LineBufVal = LineBuf.getval(i, x);
                for (int l = 0; l < VSC_SAMPLES_PER_CLOCK; l++) {
                    for (int k = 0; k < VSC_NR_COMPONENTS; k++) {
                        int start                                                 = (k + l * VSC_NR_COMPONENTS) * VSC_BITS_PER_COMPONENT;
                        PixArray[VSC_TAPS - 1 - i].val[k + l * VSC_NR_COMPONENTS] = LineBufVal(start + VSC_BITS_PER_COMPONENT - 1, start);
                    }
                }
            }

            if ((GetNewLine == 1) || (y <= (VSC_TAPS >> 1))) // If new pixels to be filled in the line buffer
            {
                for (I16 i = 0; i < (VSC_TAPS - 1); i++) {
                    PixArray[i] = PixArray[i + 1];
                }
                if ((PixArrayLoc + (VSC_TAPS >> 1)) < InLines) // get new pixels from stream
                {
                    MEM_PIXEL_TYPE InPix;
                    SrcImg >> SrcPix;
                    for (int l = 0; l < VSC_SAMPLES_PER_CLOCK; l++) {
                        for (int k = 0; k < VSC_NR_COMPONENTS; k++) {
                            int start                                        = (k + l * VSC_NR_COMPONENTS) * VSC_BITS_PER_COMPONENT;
                            InPix(start + VSC_BITS_PER_COMPONENT - 1, start) = SrcPix.val[k + l * VSC_NR_COMPONENTS];
                        }
                    }
                    LineBuf.insert_bottom(InPix, x);
                    PixArray[VSC_TAPS - 1] = SrcPix;
                } // get new pixels from stream

                for (int i = (VSC_TAPS - 1); i > 0; i--) // for circular buffer implementation
                {
                    MEM_PIXEL_TYPE LineBufVal;
                    YUV_MULTI_PIXEL PixArrayVal = (y > 0) ? PixArray[VSC_TAPS - 1 - i] : PixArray[VSC_TAPS - 1];
                    for (int l = 0; l < VSC_SAMPLES_PER_CLOCK; l++) {
                        for (int k = 0; k < VSC_NR_COMPONENTS; k++) {
                            int start                                             = (k + l * VSC_NR_COMPONENTS) * VSC_BITS_PER_COMPONENT;
                            LineBufVal(start + VSC_BITS_PER_COMPONENT - 1, start) = PixArrayVal.val[k + l * VSC_NR_COMPONENTS];
                        }
                    }
                    LineBuf.val[i][x] = LineBufVal;
                }
            }

            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //
            // Processing
            //
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (OutputWriteEn) {
                vscale_polyphase(vfltCoeff, PixArray, PhaseV, &OutPix);
                OutImg << OutPix;
            }
        }
    }
} // vscale_core_polyphase

static void vscale_polyphase(I16 vfltCoeff[VSC_PHASES][VSC_TAPS],
                             YUV_MULTI_PIXEL PixArray[VSC_TAPS],
                             U8 PhaseV,
                             YUV_MULTI_PIXEL *OutPix)
{
#pragma HLS inline
    U8 Phase = PhaseV;
    for (int s = 0; s < VSC_SAMPLES_PER_CLOCK; s++) // samples
    {
        for (int c = 0; c < VSC_NR_COMPONENTS; c++) {
#pragma HLS expression_balance off
            I32 sum  = (COEFF_PRECISION >> 1); //MP todo 32 bits might not be enough
            I32 norm = 0;

            // Center tap is tap with index (VSC_TAPS>>1)-1, 0 is top most, VSC_TAPS-1 is bottom most pixel
            for (int t = 0; t < VSC_TAPS; t++) {
                sum += PixArray[t].val[VSC_NR_COMPONENTS * s + c] * vfltCoeff[Phase][t];
            }
            norm = sum >> COEFF_PRECISION_SHIFT;
            norm = CLAMP(norm, 0, (1 << VSC_BITS_PER_COMPONENT) - 1);

            OutPix->val[VSC_NR_COMPONENTS * s + c] = (ap_uint<VSC_BITS_PER_COMPONENT>)norm;
        }
    }
}

#endif

#if (VSC_SCALE_MODE == VSC_BILINEAR)

static void vscale_core_bilinear(VSC_STREAM_MULTIPIX &SrcImg,
                                 //HSC_HW_STRUCT_REG *HwReg,
                                 U16 HeightIn,
                                 U16 WidthIn,
                                 U16 HeightOut,
                                 U32 LineRate,
                                 VSC_STREAM_MULTIPIX &OutImg)
{
    U16 InLines  = HeightIn;
    U16 InPixels = WidthIn;
    U16 OutLines = HeightOut;
    U32 Rate     = LineRate;

    bool GetNewLine;
    bool OutputWriteEn;

    U16 PixArrayLoc;
    U16 TotalLines, YLoopSize, XLoopSize;
    YUV_MULTI_PIXEL SrcPix, OutPix;
    YUV_MULTI_PIXEL PixArray[VSC_TAPS];
    MEM_LINE_BUFFER LineBuf;

#if (USE_URAM == 1)
#pragma HLS RESOURCE variable = &LineBuf core = XPM_MEMORY uram
#endif

    assert(OutLines <= VSC_MAX_HEIGHT);

    // Loop over max of input and output resolution, add run-in/out for taps
    TotalLines = (OutLines > InLines) ? OutLines : InLines;
    YLoopSize  = TotalLines + VSC_TAPS;
    XLoopSize  = (InPixels + (VSC_SAMPLES_PER_CLOCK - 1)) / VSC_SAMPLES_PER_CLOCK;

    assert(YLoopSize < (VSC_MAX_HEIGHT + VSC_TAPS));
    assert(XLoopSize <= VSC_MAX_WIDTH);

#pragma HLS ARRAY_PARTITION variable = &SrcPix complete dim = 0
#pragma HLS ARRAY_PARTITION variable = &PixArray complete dim = 0
#pragma HLS ARRAY_PARTITION variable = &OutPix complete dim = 0

    GetNewLine  = 1;
    PixArrayLoc = 0;

    int GetLine      = 0;
    U8 PhaseV        = 0;
    U32 offset       = 0;
    int ReadLoc      = 0;
    int WriteLoc     = 0;
    int WriteLocNext = 0;

loop_height:
    for (U16 y = 0; y < YLoopSize; y++) {
        OutputWriteEn = 0;

        if (y >= (VSC_TAPS - 1)) {
            PhaseV   = ((offset >> (STEP_PRECISION_SHIFT - VSC_PHASE_SHIFT))) & (VSC_PHASES - 1);
            WriteLoc = WriteLocNext;

            GetNewLine = 0;
            if ((offset >> STEP_PRECISION_SHIFT) != 0) {
                // take a new sample from input, but don't process anything
                ReadLoc    = GetLine;
                GetNewLine = 1;
                GetLine++;
                PixArrayLoc++;
                offset        = offset - (1 << STEP_PRECISION_SHIFT);
                OutputWriteEn = 0;
                WriteLocNext  = WriteLoc;
            }

            if (((offset >> STEP_PRECISION_SHIFT) == 0) && (WriteLoc < (U32)OutLines)) {
                // produce a new output sample
                offset        = offset + Rate;
                OutputWriteEn = 1;
                WriteLocNext  = WriteLoc + 1;
            }
        } // compute per line

    loop_width_for_procpix:
        for (U16 x = 0; x < XLoopSize; x++) // the loop runs for the max of (in,out)
        {
#pragma HLS LOOP_FLATTEN OFF
#pragma HLS pipeline II = 1

        loop_shift_pix:
            for (I16 i = 0; i < VSC_TAPS; i++) // get cols of pixels from the line buffer to local pixel array
            {
                MEM_PIXEL_TYPE LineBufVal = LineBuf.getval(i, x);
                for (int l = 0; l < VSC_SAMPLES_PER_CLOCK; l++) {
                    for (int k = 0; k < VSC_NR_COMPONENTS; k++) {
                        int start                                                 = (k + l * VSC_NR_COMPONENTS) * VSC_BITS_PER_COMPONENT;
                        PixArray[VSC_TAPS - 1 - i].val[k + l * VSC_NR_COMPONENTS] = LineBufVal(start + VSC_BITS_PER_COMPONENT - 1, start);
                    }
                }
            }

            if ((GetNewLine == 1) || (y < VSC_TAPS)) // If new pixels to be filled in the line buffer
            {
                for (I16 i = 0; i < (VSC_TAPS - 1); i++) {
                    PixArray[i] = PixArray[i + 1];
                }
                if ((PixArrayLoc + VSC_TAPS - 1) < InLines) // get new pixels from stream
                {
                    MEM_PIXEL_TYPE InPix;
                    SrcImg >> SrcPix;
                    for (int l = 0; l < VSC_SAMPLES_PER_CLOCK; l++) {
                        for (int k = 0; k < VSC_NR_COMPONENTS; k++) {
                            int start                                        = (k + l * VSC_NR_COMPONENTS) * VSC_BITS_PER_COMPONENT;
                            InPix(start + VSC_BITS_PER_COMPONENT - 1, start) = SrcPix.val[k + l * VSC_NR_COMPONENTS];
                        }
                    }
                    LineBuf.insert_bottom(InPix, x);
                    PixArray[VSC_TAPS - 1] = SrcPix;
                } // get new pixels from stream

                for (I16 i = (VSC_TAPS - 1); i > 0; i--) // for circular buffer implementation
                {
                    MEM_PIXEL_TYPE LineBufVal;
                    YUV_MULTI_PIXEL PixArrayVal = (y > 0) ? PixArray[VSC_TAPS - 1 - i] : PixArray[VSC_TAPS - 1];
                    for (int l = 0; l < VSC_SAMPLES_PER_CLOCK; l++) {
                        for (int k = 0; k < VSC_NR_COMPONENTS; k++) {
                            int start                                             = (k + l * VSC_NR_COMPONENTS) * VSC_BITS_PER_COMPONENT;
                            LineBufVal(start + VSC_BITS_PER_COMPONENT - 1, start) = PixArrayVal.val[k + l * VSC_NR_COMPONENTS];
                        }
                    }
                    LineBuf.val[i][x] = LineBufVal;
                }
            } // GetNewLine

            if (y >= (VSC_TAPS - 1)) {

                vscale_bilinear(PixArray, PhaseV, &OutPix);

                if (OutputWriteEn) // line based write enable - high for a line or low for a line
                {
                    OutImg << OutPix;
                }
            }
        }
    }
} // hscale_core_bilinear

static void vscale_bilinear(YUV_MULTI_PIXEL PixArray[VSC_TAPS], U8 Phase, YUV_MULTI_PIXEL *OutPix)
{
#pragma HLS inline
    U32 sum, norm;

    for (int s = 0; s < VSC_SAMPLES_PER_CLOCK; s++) // samples
    {
        for (int c = 0; c < VSC_NR_COMPONENTS; c++) {
            // Instead of using 2 multpliers following expression: sum = (PixArray[0].p[s].val[c]* PhaseInv) + (PixArray[1].p[s].val[c] * Phase);
            // is rewritten using just 1 multiplier
            sum = (PixArray[0].val[VSC_NR_COMPONENTS * s + c] * (VSC_PHASES)) - (PixArray[0].val[VSC_NR_COMPONENTS * s + c] - PixArray[1].val[VSC_NR_COMPONENTS * s + c]) * Phase;

            norm = (sum + (VSC_PHASES >> 1)) >> VSC_PHASE_SHIFT;
            norm = MIN(norm, (U32)((1 << VSC_BITS_PER_COMPONENT) - 1));

            OutPix->val[VSC_NR_COMPONENTS * s + c] = (ap_uint<VSC_BITS_PER_COMPONENT>)norm;
        }
    } // samples
} // linear

#endif

#if (VSC_SCALE_MODE == VSC_BICUBIC)

static void vscale_core_bicubic(VSC_STREAM_MULTIPIX &SrcImg,
                                //HSC_HW_STRUCT_REG *HwReg,
                                U16 HeightIn,
                                U16 WidthIn,
                                U16 HeightOut,
                                U32 LineRate,
                                VSC_STREAM_MULTIPIX &OutImg)
{
    //U16 InLines  = HwReg->HeightIn;
    //U16 InPixels = HwReg->WidthIn;
    //U16 OutLines = HwReg->HeightOut;
    //U32 Rate     = HwReg->LineRate;

    U16 InLines  = HeightIn;
    U16 InPixels = WidthIn;
    U16 OutLines = HeightOut;
    U32 Rate     = LineRate;

    bool GetNewLine;
    bool OutputWriteEn;

    U16 PixArrayLoc;
    U16 TotalLines, YLoopSize, XLoopSize;
    YUV_MULTI_PIXEL SrcPix, OutPix;
    YUV_MULTI_PIXEL PixArray[VSC_TAPS];
    MEM_LINE_BUFFER LineBuf;

#if (USE_URAM == 1)
#pragma HLS RESOURCE variable = &LineBuf core = XPM_MEMORY uram
#endif

    assert(OutLines <= VSC_MAX_HEIGHT);

    // Loop over max of input and output resolution, add run-in/out for taps
    TotalLines = (OutLines > InLines) ? OutLines : InLines;
    YLoopSize  = TotalLines + VSC_TAPS;
    XLoopSize  = (InPixels + (VSC_SAMPLES_PER_CLOCK - 1)) / VSC_SAMPLES_PER_CLOCK;

    assert(YLoopSize < (VSC_MAX_HEIGHT + VSC_TAPS));
    assert(XLoopSize <= VSC_MAX_WIDTH);

#pragma HLS ARRAY_PARTITION variable = &SrcPix complete dim = 0
#pragma HLS ARRAY_PARTITION variable = &PixArray complete dim = 0
#pragma HLS ARRAY_PARTITION variable = &OutPix complete dim = 0

    GetNewLine  = 1;
    PixArrayLoc = 0;

    int GetLine      = 0;
    U8 PhaseV        = 0;
    U32 offset       = 0;
    int ReadLoc      = 0;
    int WriteLoc     = 0;
    int WriteLocNext = 0;

loop_height:
    for (U16 y = 0; y < YLoopSize; y++) {
        OutputWriteEn = 0;

        if (y >= (VSC_TAPS - 1 - 1)) {
            PhaseV   = ((offset >> (STEP_PRECISION_SHIFT - VSC_PHASE_SHIFT))) & (VSC_PHASES - 1);
            WriteLoc = WriteLocNext;

            GetNewLine = 0;
            if ((offset >> STEP_PRECISION_SHIFT) != 0) {
                // take a new sample from input, but don't process anything
                ReadLoc    = GetLine;
                GetNewLine = 1;
                GetLine++;
                PixArrayLoc++;
                offset        = offset - (1 << STEP_PRECISION_SHIFT);
                OutputWriteEn = 0;
                WriteLocNext  = WriteLoc;
            }

            if (((offset >> STEP_PRECISION_SHIFT) == 0) && (WriteLoc < (U32)OutLines)) {
                // produce a new output sample
                offset        = offset + Rate;
                OutputWriteEn = 1;
                WriteLocNext  = WriteLoc + 1;
            }
        } // compute per line

    loop_width_for_procpix:
        for (U16 x = 0; x < XLoopSize; x++) // the loop runs for the max of (in,out)
        {
#pragma HLS LOOP_FLATTEN OFF
#pragma HLS pipeline II = 1

        loop_shift_pix:
            for (I16 i = 0; i < VSC_TAPS; i++) // get cols of pixels from the line buffer to local pixel array
            {
                MEM_PIXEL_TYPE LineBufVal = LineBuf.getval(i, x);
                for (int l = 0; l < VSC_SAMPLES_PER_CLOCK; l++) {
                    for (int k = 0; k < VSC_NR_COMPONENTS; k++) {
                        int start                                                 = (k + l * VSC_NR_COMPONENTS) * VSC_BITS_PER_COMPONENT;
                        PixArray[VSC_TAPS - 1 - i].val[k + l * VSC_NR_COMPONENTS] = LineBufVal(start + VSC_BITS_PER_COMPONENT - 1, start);
                    }
                }
            }

            if ((GetNewLine == 1) || (y < VSC_TAPS - 1)) // If new pixels to be filled in the line buffer
            {
                for (I16 i = 0; i < (VSC_TAPS - 1); i++) {
                    PixArray[i] = PixArray[i + 1];
                }
                if ((PixArrayLoc + VSC_TAPS - 2) < InLines) // get new pixels from stream
                {
                    MEM_PIXEL_TYPE InPix;
                    SrcImg >> SrcPix;
                    for (int l = 0; l < VSC_SAMPLES_PER_CLOCK; l++) {
                        for (int k = 0; k < VSC_NR_COMPONENTS; k++) {
                            int start                                        = (k + l * VSC_NR_COMPONENTS) * VSC_BITS_PER_COMPONENT;
                            InPix(start + VSC_BITS_PER_COMPONENT - 1, start) = SrcPix.val[k + l * VSC_NR_COMPONENTS];
                        }
                    }
                    LineBuf.insert_bottom(InPix, x);
                    PixArray[VSC_TAPS - 1] = SrcPix;
                } // get new pixels from stream

                for (I16 i = (VSC_TAPS - 1); i > 0; i--) // for circular buffer implementation
                {
                    MEM_PIXEL_TYPE LineBufVal;
                    YUV_MULTI_PIXEL PixArrayVal = (y > 0) ? PixArray[VSC_TAPS - 1 - i] : PixArray[VSC_TAPS - 1];
                    for (int l = 0; l < VSC_SAMPLES_PER_CLOCK; l++) {
                        for (int k = 0; k < VSC_NR_COMPONENTS; k++) {
                            int start                                             = (k + l * VSC_NR_COMPONENTS) * VSC_BITS_PER_COMPONENT;
                            LineBufVal(start + VSC_BITS_PER_COMPONENT - 1, start) = PixArrayVal.val[k + l * VSC_NR_COMPONENTS];
                        }
                    }
                    LineBuf.val[i][x] = LineBufVal;
                }
            } // GetNewLine

            if (y >= (VSC_TAPS - 1) - 1) {
                vscale_bicubic(PixArray, PhaseV, &OutPix);

                if (OutputWriteEn) // line based write enable - high for a line or low for a line
                {
                    OutImg << OutPix;
                }
            }
        }
    }
}

static void vscale_bicubic(YUV_MULTI_PIXEL PixArray[VSC_TAPS],
                           U8 Phase,
                           YUV_MULTI_PIXEL *OutPix)
{
#pragma HLS inline
    I64 sum;
    I32 norm;
    I32 a, b, c;
    ap_uint<28> d;
    ap_int<52> ax3;
    ap_int<40> bx2;
    ap_int<28> cx;

    for (int s = 0; s < VSC_SAMPLES_PER_CLOCK; s++) // samples
    {
        const int ArrayLocM1 = 0;
        const int ArrayLoc   = 1;
        const int ArrayLocP1 = 2;
        const int ArrayLocP2 = 3;

        for (int i = 0; i < VSC_NR_COMPONENTS; i++) {
            a = ((PixArray[ArrayLoc].val[VSC_NR_COMPONENTS * s + i] * 3) - (PixArray[ArrayLocP1].val[VSC_NR_COMPONENTS * s + i] * 3) - (PixArray[ArrayLocM1].val[VSC_NR_COMPONENTS * s + i] * 1) + (PixArray[ArrayLocP2].val[VSC_NR_COMPONENTS * s + i] * 1)) >> 1;
            b = ((PixArray[ArrayLocP1].val[VSC_NR_COMPONENTS * s + i] * 4) - (PixArray[ArrayLoc].val[VSC_NR_COMPONENTS * s + i] * 5) + (PixArray[ArrayLocM1].val[VSC_NR_COMPONENTS * s + i] * 2) - (PixArray[ArrayLocP2].val[VSC_NR_COMPONENTS * s + i] * 1)) >> 1;
            c = ((PixArray[ArrayLocP1].val[VSC_NR_COMPONENTS * s + i] * 1) - (PixArray[ArrayLocM1].val[VSC_NR_COMPONENTS * s + i] * 1)) >> 1;
            d = PixArray[ArrayLoc].val[VSC_NR_COMPONENTS * s + i] * VSC_PHASES;

            ax3 = (((ap_int<52>)a * Phase * Phase * Phase) + VSC_PHASES) >> (VSC_PHASE_SHIFT + VSC_PHASE_SHIFT);
            bx2 = (((ap_int<40>)b * Phase * Phase) + (VSC_PHASES >> 1)) >> (VSC_PHASE_SHIFT);
            cx  = ((ap_int<28>)c * Phase);

            sum                                    = (I64)((ax3 + bx2 + cx) + (I32)d);
            norm                                   = (I32)(sum + (VSC_PHASES >> 1)) >> VSC_PHASE_SHIFT;
            norm                                   = CLAMP(norm, 0, (I32)((1 << VSC_BITS_PER_COMPONENT) - 1));
            OutPix->val[VSC_NR_COMPONENTS * s + i] = (ap_uint<VSC_BITS_PER_COMPONENT>)norm;
        }
    }
}
#endif
