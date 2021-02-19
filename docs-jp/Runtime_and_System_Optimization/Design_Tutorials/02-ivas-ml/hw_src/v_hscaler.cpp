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
#include <stdio.h>
#include <assert.h>
#include "v_multi_scaler.h"
#include <algorithm>

#if  (HSC_SCALE_MODE == HSC_BICUBIC)
static void hscale_core_bicubic(HSC_STREAM_MULTIPIX& SrcImg,
	U16 Height,
	U16 WidthIn,
	U16 WidthOut,
	U32 PixelRate,
	U8 ColorMode,
	HSC_PHASE_CTRL phasesH[HSC_MAX_WIDTH / HSC_SAMPLES_PER_CLOCK],
	HSC_STREAM_MULTIPIX& OutImg
);

static void hscale_bicubic(YUV_PIXEL PixArray[HSC_ARRAY_SIZE],
	U16 PhasesH[HSC_SAMPLES_PER_CLOCK],
	U8 ArrayIdx[HSC_SAMPLES_PER_CLOCK],
	YUV_MULTI_PIXEL *OutPix
);
#endif

#if  (HSC_SCALE_MODE == HSC_POLYPHASE)
static void hscale_core_polyphase(HSC_STREAM_MULTIPIX& SrcImg,
	U16 Height,
	U16 WidthIn,
	U16 WidthOut,
	U32 PixelRate,
	U8 ColorMode,
	I16 hfltCoeff[HSC_PHASES][HSC_TAPS][HSC_SAMPLES_PER_CLOCK],
	HSC_PHASE_CTRL phasesH[HSC_MAX_WIDTH / HSC_SAMPLES_PER_CLOCK],
	HSC_STREAM_MULTIPIX& OutImg
);

static void hscale_polyphase(I16 hfltCoeff[HSC_PHASES][HSC_TAPS][HSC_SAMPLES_PER_CLOCK],
	YUV_PIXEL PixArray[HSC_ARRAY_SIZE],
	U16 PhasesH[HSC_SAMPLES_PER_CLOCK],
	U8 ArrayIdx[HSC_SAMPLES_PER_CLOCK],
	YUV_MULTI_PIXEL *OutPix
);
#endif

#if  (HSC_SCALE_MODE == HSC_BILINEAR)
static void hscale_core_bilinear(HSC_STREAM_MULTIPIX& SrcImg,
	U16 Height,
	U16 WidthIn,
	U16 WidthOut,
	U32 PixelRate,
	U8 ColorMode,
	HSC_PHASE_CTRL phasesH[HSC_MAX_WIDTH / HSC_SAMPLES_PER_CLOCK],
	HSC_STREAM_MULTIPIX& OutImg
);

static void hscale_bilinear(YUV_PIXEL PixArray[HSC_ARRAY_SIZE],
	U16 PhasesH[HSC_SAMPLES_PER_CLOCK],
	U8 ArrayIdx[HSC_SAMPLES_PER_CLOCK],
	YUV_MULTI_PIXEL *OutPix
);
#endif

/*********************************************************************************
* Function:    hscale
* Parameters:  Stream of input/output pixels, image resolution, type of scaling etc
* Return:
* Description: Top level function to perform horizontal image resizing
**********************************************************************************/
void v_hscaler(HSC_STREAM_MULTIPIX& stream_in,
	U16 Height,
	U16 WidthIn,
	U16 WidthOut,
	U32 PixelRate,
	U8 ColorMode,
#if (HSC_SCALE_MODE == HSC_POLYPHASE)
	I16 hfltCoeff[HSC_PHASES][HSC_TAPS][HSC_SAMPLES_PER_CLOCK],
#endif
	HSC_PHASE_CTRL PhasesH[HSC_MAX_WIDTH / HSC_SAMPLES_PER_CLOCK],
	HSC_STREAM_MULTIPIX& stream_out
)
{
#if (HSC_SCALE_MODE == HSC_BILINEAR)
	{
		hscale_core_bilinear(stream_in, Height, WidthIn, WidthOut, PixelRate, ColorMode, PhasesH, stream_out);
	}
#elif (HSC_SCALE_MODE == HSC_POLYPHASE)
	{
		hscale_core_polyphase(stream_in, Height, WidthIn, WidthOut, PixelRate, ColorMode, hfltCoeff, PhasesH, stream_out);
	}
#else
	{
		hscale_core_bicubic(stream_in, Height, WidthIn, WidthOut, PixelRate, ColorMode, PhasesH, stream_out);
	}
#endif
}

#if  (HSC_SCALE_MODE == HSC_BILINEAR)

/*********************************************************************************
* Function:    hscale_core
* Parameters:  Stream of input/output pixels, image resolution, type of scaling etc
* Return:
* Description: Perform horizontal image resizing
* Sub modules - hscale_linear
*               hscale_cubic
*               hscale_poly
**********************************************************************************/
static void hscale_core_bilinear(HSC_STREAM_MULTIPIX& SrcImg,
	U16 Height,
	U16 InPixels,
	U16 OutPixels,
	U32 Rate,
	U8 RegColorMode,
	HSC_PHASE_CTRL arrPhasesH[HSC_MAX_WIDTH / HSC_SAMPLES_PER_CLOCK],
	HSC_STREAM_MULTIPIX& OutImg
)
{
	ap_uint<HSC_SAMPLES_PER_CLOCK> OutputWriteEn;

	U8   ArrayLoc[HSC_SAMPLES_PER_CLOCK];
	U16  xReadPos, xWritePos;
	U16  y, x;
	U16  x_dsent;
	bool WriteEn; //?
	U8   PackCnt;
	U8   ComputedPixels;

	YUV_MULTI_PIXEL SrcPix, OutPix, OutPixPrv;
	YUV_PIXEL PixArray[HSC_ARRAY_SIZE];
	YUV_MULTI_PIXEL OutMultiPix;

	assert(Height <= HSC_MAX_HEIGHT);
	assert(Height >= MIN_PIXELS);

	U16 TotalPixels = std::max(OutPixels, InPixels);
	assert(TotalPixels <= HSC_MAX_WIDTH);
	assert(TotalPixels >= MIN_PIXELS);

	U16 LoopSize = TotalPixels + HSC_ARRAY_SIZE - HSC_SAMPLES_PER_CLOCK;

	assert((LoopSize%HSC_SAMPLES_PER_CLOCK) == 0);

	int dbgNrRds = 0;
	int dbgNrWrs = 0;

#pragma HLS ARRAY_PARTITION variable=&SrcPix        complete dim=0
#pragma HLS ARRAY_PARTITION variable=&PixArray      complete dim=0
#pragma HLS ARRAY_PARTITION variable=&OutPix        complete dim=0
#pragma HLS ARRAY_PARTITION variable=&ArrayLoc      complete dim=0
#pragma HLS ARRAY_PARTITION variable=&OutMultiPix   complete dim=0
#pragma HLS ARRAY_PARTITION variable=&OutPixPrv     complete dim=0

	loop_height:
			   for (y = 0; y < Height; y++)
			   {
				   int ReadEn = 1;
				   xReadPos = 0;
				   xWritePos = 0;
				   x_dsent = 0;
				   PackCnt = 0;
				   OutputWriteEn = 0;

				   U16 PhaseH[HSC_SAMPLES_PER_CLOCK];

				   U8 nrWrsIdxPrev = 0;
				   U8 nrWrsPrev = 0;
				   U8 nrWrsClck = 0;
				   U8 nrWrsAccu = 0;
			   loop_width:
				   for (x = 0; x <= LoopSize; x += HSC_SAMPLES_PER_CLOCK) // the loop runs for the max of (in,out)
				   {
					   //_Pragma("HLS loop_tripcount max=((MSC_MAX_WIDTH+HSC_ARRAY_SIZE-HSC_SAMPLES_PER_CLOCK)/HSC_SAMPLES_PER_CLOCK)")
#pragma HLS LOOP_FLATTEN OFF
#pragma HLS pipeline II=1

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//
			// Pixel buffer control
			//
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					   if (ReadEn)   // If new pixels to be got, shift the old ones
					   {
						   // Shift the pixels in the array over by HSC_SAMPLES_PER_CLOCK
						   for (int i = 0; i <= (HSC_ARRAY_SIZE - 1 - HSC_SAMPLES_PER_CLOCK); i++)
						   {
							   PixArray[i] = PixArray[i + HSC_SAMPLES_PER_CLOCK]; // always shift out the same number of pixels as samples per clock
						   }
						   // Read new pixels if we didn't exceed the width of frame yet
						   if ((xReadPos + HSC_ARRAY_SIZE - 1) < InPixels)
						   {
							   // Get new pixels from stream and insert
							   dbgNrRds++;
							   SrcImg >> SrcPix;
							   for (int i = 0; i < HSC_SAMPLES_PER_CLOCK; i++)
							   {
								   for (int k = 0; k < HSC_NR_COMPONENTS; k++)
								   {
									   PixArray[HSC_ARRAY_SIZE - 1 - HSC_SAMPLES_PER_CLOCK + 1 + i].val[k] = SrcPix.val[HSC_NR_COMPONENTS*i + k];
								   }
							   }
						   }
						   else
						   {
							   // Right border handling
							   for (int i = 0; i < HSC_SAMPLES_PER_CLOCK; i++)
							   {
								   PixArray[HSC_ARRAY_SIZE - 1 - HSC_SAMPLES_PER_CLOCK + 1 + i] = PixArray[HSC_ARRAY_SIZE - 1 - HSC_SAMPLES_PER_CLOCK];
							   }

						   }
					   }

#ifdef DBG_PRINT
					   if ((y == 0) && (ReadEn))
					   {
						   for (int i = 0; i < (HSC_ARRAY_SIZE); i++)
						   {
							   printf("[%2d] %3d.%3d.%3d ", (int)i, (int)PixArray[i].val[0], (int)PixArray[i].val[1], (int)PixArray[i].val[2]);
						   }
						   printf("\n");
					   }
#endif
					   if (x >= (HSC_ARRAY_SIZE - HSC_SAMPLES_PER_CLOCK))
					   {
					   loop_samples:
						   U16 xbySamples = x - (HSC_ARRAY_SIZE - HSC_SAMPLES_PER_CLOCK);
						   xbySamples = xbySamples / HSC_SAMPLES_PER_CLOCK;
						   // Note 'reg()' below.  Under most circumstances, this is not the preferred way to do this.  In most cases,
						   // HLS supports the ability to set the latency of reads from a memory.  However, in this case, because the
						   // memory is part of the axi_lite adapter, that doesn't work.
						   HSC_PHASE_CTRL phases = reg(arrPhasesH[xbySamples]);
						   for (int s = 0; s < HSC_SAMPLES_PER_CLOCK; s++)
						   {
							   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							   //
							   // Step and phase calculation
							   //
							   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							   PhaseH[s] = phases(HSC_PHASE_CTRL_PHASE_MSB + s*HSC_PHASE_CTRL_BITS, HSC_PHASE_CTRL_PHASE_LSB + s*HSC_PHASE_CTRL_BITS);
							   ArrayLoc[s] = phases(HSC_PHASE_CTRL_INDEX_MSB + s*HSC_PHASE_CTRL_BITS, HSC_PHASE_CTRL_INDEX_LSB + s*HSC_PHASE_CTRL_BITS);
							   OutputWriteEn[s] = phases[HSC_PHASE_CTRL_ENABLE_LSB + s*HSC_PHASE_CTRL_BITS];
						   }
						   ReadEn = (ArrayLoc[HSC_SAMPLES_PER_CLOCK - 1] >= HSC_SAMPLES_PER_CLOCK);

#ifdef DBG_PRINT
						   if (y == 0)
						   {
							   for (int s = 0; s < HSC_SAMPLES_PER_CLOCK; s++)
							   {
								   printf("x %5d, phase %5d, pos %5d, readpos %5d writepos %5d\n", (int)x + s, (int)PhaseH[s], (int)ArrayLoc[s], (int)xReadPos, xWritePos);
							   }
							   printf("rden %d\n", ReadEn);
						   }
#endif
						   hscale_bilinear(PixArray, PhaseH, ArrayLoc, &OutPix);

						   if (ReadEn)
						   {
							   xReadPos += HSC_SAMPLES_PER_CLOCK;
						   }

#if (HSC_SAMPLES_PER_CLOCK==1)
						   {
							   if (OutputWriteEn && (x < LoopSize))
							   {
								   OutImg << OutPix;
								   dbgNrWrs++;
							   }
						   }
#else
						   const U8 BitSetCnt[] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 }; // 2^ MAX_SMPLS_PERCLK
						   nrWrsClck = BitSetCnt[OutputWriteEn];

						   // Below table provides an index to the n-th [0..3] 1 bit for numbers between [0..15]
						   const U8 OneBitIdx[4][16] = {
							   {0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0},
							   {0, 0, 0, 1, 0, 2, 2, 1, 0, 3, 3, 1, 3, 2, 2, 1},
							   {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 3, 0, 3, 3, 2},
							   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3}
						   };

						   // If more than HSC_SAMPLES_PER_CLOCK pixels are available we can write them out
						   // This loop combines any pixels that were still waiting to be written with the pixels that were generated this clock cycle
						   nrWrsAccu = nrWrsPrev + nrWrsClck;
						   if ((nrWrsAccu >= HSC_SAMPLES_PER_CLOCK) && (xWritePos < OutPixels))
						   {
							   for (int s = 0; s < HSC_SAMPLES_PER_CLOCK; s++)
							   {
								   for (int c = 0; c < HSC_NR_COMPONENTS; c++)
								   {
									   OutMultiPix.val[s*HSC_NR_COMPONENTS + c] = (s < nrWrsPrev) ? OutPixPrv.val[s*HSC_NR_COMPONENTS + c] : OutPix.val[OneBitIdx[s - nrWrsPrev][OutputWriteEn] * HSC_NR_COMPONENTS + c]; // OutPixCur.val[(s-nrWrsPrev)*HSC_NR_COMPONENTS+c];
								   }
							   }
							   OutImg << OutMultiPix;
							   dbgNrWrs++;
							   xWritePos += HSC_SAMPLES_PER_CLOCK;
						   }

						   // This loop combines any pixels that are still waiting to be written
						   for (int s = 0; s < HSC_SAMPLES_PER_CLOCK; s++)
						   {
							   for (int c = 0; c < HSC_NR_COMPONENTS; c++)
							   {
								   OutPixPrv.val[s*HSC_NR_COMPONENTS + c] = (nrWrsAccu >= HSC_SAMPLES_PER_CLOCK && s < ((nrWrsAccu) % HSC_SAMPLES_PER_CLOCK)) ? OutPix.val[OneBitIdx[s + HSC_SAMPLES_PER_CLOCK - nrWrsPrev][OutputWriteEn] * HSC_NR_COMPONENTS + c] : ((s < nrWrsPrev) ? OutPixPrv.val[s*HSC_NR_COMPONENTS + c] : OutPix.val[OneBitIdx[s - nrWrsPrev][OutputWriteEn] * HSC_NR_COMPONENTS + c]);
							   }
						   }
						   nrWrsPrev = (nrWrsAccu) % HSC_SAMPLES_PER_CLOCK;

#endif
					   }
				   }
			   }
#ifdef DBG_PRINT
			   printf("dbgNrRds %d dbgNrWrs %d\n", dbgNrRds, dbgNrWrs);
#endif
}

static void hscale_bilinear(YUV_PIXEL PixArray[HSC_ARRAY_SIZE],
	U16 PhasesH[HSC_SAMPLES_PER_CLOCK],
	U8 ArrayIdx[HSC_SAMPLES_PER_CLOCK],
	YUV_MULTI_PIXEL *OutPix)

{
#pragma HLS inline
	U32 sum, norm;

	for (int s = 0; s < HSC_SAMPLES_PER_CLOCK; s++) // samples
	{
		U16 PhaseH = PhasesH[s];
		U8 idx = ArrayIdx[s];
		U8 idxP1 = ArrayIdx[s] + 1;
		for (int c = 0; c < HSC_NR_COMPONENTS; c++)
		{
			// Instead of using 2 multpliers following expression: sum = (PixArray[0].p[s].val[c]* PhaseInv) + (PixArray[1].p[s].val[c] * Phase);
			// is rewritten using just 1 multiplier
			sum = (PixArray[idx].val[c] * (HSC_PHASES)) - (PixArray[idx].val[c] - PixArray[idxP1].val[c]) * PhaseH;
			norm = (sum + (HSC_PHASES >> 1)) >> HSC_PHASE_SHIFT;
			norm = CLAMP(norm, 0, (U32)((1 << HSC_BITS_PER_COMPONENT) - 1));
			OutPix->val[HSC_NR_COMPONENTS*s + c] = norm;
		}
	}
}

#endif

#if  (HSC_SCALE_MODE == HSC_POLYPHASE)
/*********************************************************************************
* Function:    hscale_core
* Parameters:  Stream of input/output pixels, image resolution, type of scaling etc
* Return:
* Description: Perform horizontal image resizing
* Sub modules - hscale_linear
*               hscale_cubic
*               hscale_poly
**********************************************************************************/
static void hscale_core_polyphase(HSC_STREAM_MULTIPIX& SrcImg,
	U16 Height,
	U16 InPixels,
	U16 OutPixels,
	U32 Rate,
	U8 RegColorMode,
	I16 hfltCoeff[HSC_PHASES][HSC_TAPS][HSC_SAMPLES_PER_CLOCK],
	HSC_PHASE_CTRL arrPhasesH[HSC_MAX_WIDTH / HSC_SAMPLES_PER_CLOCK],
	HSC_STREAM_MULTIPIX& OutImg
)
{
	ap_uint<HSC_SAMPLES_PER_CLOCK> OutputWriteEn;

	U8 ArrayLoc[HSC_SAMPLES_PER_CLOCK];
	U16 xReadPos, xWritePos;
	U16 y, x;
	U16 x_dsent;
	U8 PackCnt;
	U8 ComputedPixels;

	YUV_MULTI_PIXEL SrcPix, OutPix, OutPixPrv;
	YUV_PIXEL PixArray[HSC_ARRAY_SIZE];
	YUV_MULTI_PIXEL OutMultiPix;

	assert(Height <= HSC_MAX_HEIGHT);
	assert(Height >= MIN_PIXELS);

	U16 TotalPixels = std::max(OutPixels, InPixels);
	assert(TotalPixels <= HSC_MAX_WIDTH);
	assert(TotalPixels >= MIN_PIXELS);

	U16 LoopSize = TotalPixels + HSC_ARRAY_SIZE - HSC_SAMPLES_PER_CLOCK - (HSC_TAPS >> 1);
	LoopSize = ((LoopSize + (HSC_SAMPLES_PER_CLOCK - 1)) / HSC_SAMPLES_PER_CLOCK) * (HSC_SAMPLES_PER_CLOCK);
	assert((LoopSize%HSC_SAMPLES_PER_CLOCK) == 0);

	int dbgNrRds = 0;
	int dbgNrWrs = 0;

#pragma HLS ARRAY_PARTITION variable=&SrcPix        complete dim=0
#pragma HLS ARRAY_PARTITION variable=&PixArray      complete dim=0
#pragma HLS ARRAY_PARTITION variable=&OutPix        complete dim=0
#pragma HLS ARRAY_PARTITION variable=&OutPixPrv     complete dim=0
#pragma HLS ARRAY_PARTITION variable=&OutMultiPix   complete dim=0
#pragma HLS ARRAY_PARTITION variable=&ArrayLoc      complete dim=0

	loop_height:
			   for (y = 0; y < Height; y++)
			   {
				   U16 PhaseH[HSC_SAMPLES_PER_CLOCK];
				   int ReadEn = 1;
				   xReadPos = 0;
				   xWritePos = 0;
				   x_dsent = 0;
				   PackCnt = 0;
				   OutputWriteEn = 0;

				   U8 nrWrsIdxPrev = 0;
				   U8 nrWrsPrev = 0;
				   U8 nrWrsClck = 0;
				   U8 nrWrsAccu = 0;

			   loop_width:
				   for (x = 0; x <= LoopSize; x += HSC_SAMPLES_PER_CLOCK) // the loop runs for the max of (in,out)
				   {
					   //_Pragma("HLS loop_tripcount max=((MSC_MAX_WIDTH+HSC_ARRAY_SIZE-HSC_SAMPLES_PER_CLOCK - (HSC_TAPS>>1))/HSC_SAMPLES_PER_CLOCK)")
#pragma HLS LOOP_FLATTEN OFF
#pragma HLS pipeline II=1

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//
			// Pixel buffer control
			//
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					   if (ReadEn) // If new pixels to be got, shift the old ones
					   {
						   // Shift the pixels in the array over by HSC_SAMPLES_PER_CLOCK
						   for (int i = 0; i <= (HSC_ARRAY_SIZE - 1 - HSC_SAMPLES_PER_CLOCK); i++)
						   {
							   // Includes left border handling
							   PixArray[i] = (((x / HSC_SAMPLES_PER_CLOCK) <= 1) && i < (HSC_ARRAY_SIZE - 2 * HSC_SAMPLES_PER_CLOCK)) ? PixArray[HSC_ARRAY_SIZE - 1 - HSC_SAMPLES_PER_CLOCK + 1] : PixArray[i + HSC_SAMPLES_PER_CLOCK];
						   }
						   // Read new pixels if we didn't exceed the width of frame yet
						   if ((xReadPos + HSC_ARRAY_SIZE - 1 - (HSC_TAPS >> 1)) < InPixels)
						   {
							   // Get new pixels from stream and insert
							   dbgNrRds++;
							   SrcImg >> SrcPix;
							   for (int i = 0; i < HSC_SAMPLES_PER_CLOCK; i++)
							   {
								   for (int k = 0; k < HSC_NR_COMPONENTS; k++)
								   {
									   PixArray[HSC_ARRAY_SIZE - 1 - HSC_SAMPLES_PER_CLOCK + 1 + i].val[k] = SrcPix.val[HSC_NR_COMPONENTS*i + k];
								   }
							   }
						   }
						   else
						   {
							   // Right border handling
							   for (int i = 0; i < HSC_SAMPLES_PER_CLOCK; i++)
							   {
								   PixArray[HSC_ARRAY_SIZE - 1 - HSC_SAMPLES_PER_CLOCK + 1 + i] = PixArray[HSC_ARRAY_SIZE - 1 - HSC_SAMPLES_PER_CLOCK];
							   }

						   }
					   }

#ifdef DBG_PRINT
					   if ((y == 0) && (ReadEn))
					   {
						   for (int i = 0; i < (HSC_ARRAY_SIZE); i++)
						   {
							   printf("[%2d] %3d.%3d.%3d ", (int)i, (int)PixArray[i].val[0], (int)PixArray[i].val[1], (int)PixArray[i].val[2]);
						   }
						   printf("\n");
					   }
#endif

					   if (x >= (HSC_ARRAY_SIZE - HSC_SAMPLES_PER_CLOCK - (HSC_TAPS >> 1)))
					   {
						   U16 xbySamples = x - (HSC_ARRAY_SIZE - HSC_SAMPLES_PER_CLOCK - (HSC_TAPS >> 1));
						   xbySamples = xbySamples / HSC_SAMPLES_PER_CLOCK;
						   // Note 'reg()' below.  Under most circumstances, this is not the preferred way to do this.  In most cases,
						   // HLS supports the ability to set the latency of reads from a memory.  However, in this case, because the
						   // memory is part of the axi_lite adapter, that doesn't work.
						   HSC_PHASE_CTRL phases = reg(arrPhasesH[xbySamples]);
					   loop_samples:
						   for (int s = 0; s < HSC_SAMPLES_PER_CLOCK; s++)
						   {
							   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							   //
							   // Step and phase calculation
							   //
							   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							   PhaseH[s] = phases(HSC_PHASE_CTRL_PHASE_MSB + s*HSC_PHASE_CTRL_BITS, HSC_PHASE_CTRL_PHASE_LSB + s*HSC_PHASE_CTRL_BITS);
							   ArrayLoc[s] = phases(HSC_PHASE_CTRL_INDEX_MSB + s*HSC_PHASE_CTRL_BITS, HSC_PHASE_CTRL_INDEX_LSB + s*HSC_PHASE_CTRL_BITS);
							   OutputWriteEn[s] = phases[HSC_PHASE_CTRL_ENABLE_LSB + s*HSC_PHASE_CTRL_BITS];
						   }
						   ReadEn = (ArrayLoc[HSC_SAMPLES_PER_CLOCK - 1] >= HSC_SAMPLES_PER_CLOCK);

#ifdef DBG_PRINT
						   if (y == 0)
						   {
							   for (int s = 0; s < HSC_SAMPLES_PER_CLOCK; s++)
							   {
								   printf("x %5d, phase %5d, pos %5d, readpos %5d writepos %5d\n", (int)x + s, (int)PhaseH[s], (int)ArrayLoc[s], (int)xReadPos, xWritePos);
							   }
							   printf("rden %d\n", ReadEn);
						   }
#endif
						   hscale_polyphase(hfltCoeff, PixArray, PhaseH, ArrayLoc, &OutPix);
						   if (ReadEn)
						   {
							   xReadPos += HSC_SAMPLES_PER_CLOCK;
						   }

#if (HSC_SAMPLES_PER_CLOCK==1)
						   {
							   if (OutputWriteEn && (x < LoopSize))
							   {
								   OutImg << OutPix;
								   dbgNrWrs++;
							   }
						   }
#else
						   const U8 BitSetCnt[] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 }; // 2^ MAX_SMPLS_PERCLK
						   nrWrsClck = BitSetCnt[OutputWriteEn];

						   // Below table provides an index to the n-th [0..3] 1 bit for numbers between [0..15]
						   const U8 OneBitIdx[4][16] = {
							   {0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0},
							   {0, 0, 0, 1, 0, 2, 2, 1, 0, 3, 3, 1, 3, 2, 2, 1},
							   {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 3, 0, 3, 3, 2},
							   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3}
						   };

						   // If more than HSC_SAMPLES_PER_CLOCK pixels are available we can write them out
						   // This loop combines any pixels that were still waiting to be written with the pixels that were generated this clock cycle
						   nrWrsAccu = nrWrsPrev + nrWrsClck;
						   if ((nrWrsAccu >= HSC_SAMPLES_PER_CLOCK) && (xWritePos < OutPixels))
						   {
							   for (int s = 0; s < HSC_SAMPLES_PER_CLOCK; s++)
							   {
								   for (int c = 0; c < HSC_NR_COMPONENTS; c++)
								   {
									   OutMultiPix.val[s*HSC_NR_COMPONENTS + c] = (s < nrWrsPrev) ? OutPixPrv.val[s*HSC_NR_COMPONENTS + c] : OutPix.val[OneBitIdx[s - nrWrsPrev][OutputWriteEn] * HSC_NR_COMPONENTS + c]; // OutPixCur.val[(s-nrWrsPrev)*HSC_NR_COMPONENTS+c];
								   }
							   }
							   OutImg << OutMultiPix;
							   dbgNrWrs++;
							   xWritePos += HSC_SAMPLES_PER_CLOCK;
						   }

						   // This loop combines any pixels that are still waiting to be written
						   for (int s = 0; s < HSC_SAMPLES_PER_CLOCK; s++)
						   {
							   for (int c = 0; c < HSC_NR_COMPONENTS; c++)
							   {
								   OutPixPrv.val[s*HSC_NR_COMPONENTS + c] = (nrWrsAccu >= HSC_SAMPLES_PER_CLOCK && s < ((nrWrsAccu) % HSC_SAMPLES_PER_CLOCK)) ? OutPix.val[OneBitIdx[s + HSC_SAMPLES_PER_CLOCK - nrWrsPrev][OutputWriteEn] * HSC_NR_COMPONENTS + c] : ((s < nrWrsPrev) ? OutPixPrv.val[s*HSC_NR_COMPONENTS + c] : OutPix.val[OneBitIdx[s - nrWrsPrev][OutputWriteEn] * HSC_NR_COMPONENTS + c]);
							   }
						   }
						   nrWrsPrev = (nrWrsAccu) % HSC_SAMPLES_PER_CLOCK;

#endif

					   }
				   }
			   }
			   //printf("dbgNrRds %d dbgNrWrs %d\n", dbgNrRds, dbgNrWrs);
}

static void hscale_polyphase(I16 hfltCoeff[HSC_PHASES][HSC_TAPS][HSC_SAMPLES_PER_CLOCK],
	YUV_PIXEL PixArray[HSC_ARRAY_SIZE],
	U16 PhasesH[HSC_SAMPLES_PER_CLOCK],
	U8 ArrayIdx[HSC_SAMPLES_PER_CLOCK],
	YUV_MULTI_PIXEL *OutPix)
{
#pragma HLS inline self off
	for (int s = 0; s < HSC_SAMPLES_PER_CLOCK; s++)
	{
		U16 PhaseH = PhasesH[s];
		U8 idx = ArrayIdx[s] + ((HSC_SAMPLES_PER_CLOCK == 1) ? 1 : 0);
		I16 FiltCoeffRead[HSC_TAPS];
#pragma HLS array_partition variable=&FiltCoeffRead complete
		for (int t = 0; t < HSC_TAPS; t++) {
			FiltCoeffRead[t] = hfltCoeff[PhaseH][t][s];
		}
		for (int c = 0; c < HSC_NR_COMPONENTS; c++)
		{
#pragma HLS expression_balance off
			I32 sum = (COEFF_PRECISION >> 1);  //MP todo 32 bits might not be enough
			I32 norm = 0;

			// Center tap is tap with index (VSC_TAPS>>1), 0 is left most, HSC_ARRAY_SIZE-1 is right most pixel
			for (int t = 0; t < HSC_TAPS; t++)
			{
				assert(PhaseH < HSC_PHASES);
				assert(t < HSC_TAPS);
				assert(idx + t < HSC_ARRAY_SIZE);
				sum += PixArray[idx + t].val[c] * FiltCoeffRead[t];
			}
			norm = sum >> COEFF_PRECISION_SHIFT;
			norm = CLAMP(norm, 0, (1 << HSC_BITS_PER_COMPONENT) - 1);

			OutPix->val[HSC_NR_COMPONENTS*s + c] = norm;
		}
	}
}
#endif

#if  (HSC_SCALE_MODE == HSC_BICUBIC)

/*********************************************************************************
* Function:    hscale_core
* Parameters:  Stream of input/output pixels, image resolution, type of scaling etc
* Return:
* Description: Perform horizontal image resizing
**********************************************************************************/
static void hscale_core_bicubic(HSC_STREAM_MULTIPIX& SrcImg,
	U16 Height,
	U16 InPixels,
	U16 OutPixels,
	U32 Rate,
	U8 RegColorMode,
	HSC_PHASE_CTRL arrPhasesH[HSC_MAX_WIDTH / HSC_SAMPLES_PER_CLOCK],
	HSC_STREAM_MULTIPIX& OutImg
)
{
	ap_uint<HSC_SAMPLES_PER_CLOCK> OutputWriteEn;

	U8    ArrayLoc[HSC_SAMPLES_PER_CLOCK];
	U16  xReadPos, xWritePos;
	U16 y, x;
	U16 x_dsent;
	U8  PackCnt;
	U8  ComputedPixels;

	YUV_MULTI_PIXEL SrcPix, OutPix, OutPixPrv;
	YUV_PIXEL PixArray[HSC_ARRAY_SIZE];
	YUV_MULTI_PIXEL OutMultiPix;

	assert(Height <= HSC_MAX_HEIGHT);
	assert(Height >= MIN_PIXELS);

	U16 TotalPixels = std::max(OutPixels, InPixels);
	assert(TotalPixels <= HSC_MAX_WIDTH);
	assert(TotalPixels >= MIN_PIXELS);

	U16 LoopSize = TotalPixels + HSC_ARRAY_SIZE - HSC_SAMPLES_PER_CLOCK - (HSC_TAPS >> 1);

	assert((LoopSize%HSC_SAMPLES_PER_CLOCK) == 0);

	int dbgNrRds = 0;
	int dbgNrWrs = 0;

#pragma HLS ARRAY_PARTITION variable=&SrcPix        complete dim=0
#pragma HLS ARRAY_PARTITION variable=&PixArray      complete dim=0
#pragma HLS ARRAY_PARTITION variable=&OutPix        complete dim=0
#pragma HLS ARRAY_PARTITION variable=&OutPixPrv     complete dim=0
#pragma HLS ARRAY_PARTITION variable=&OutMultiPix   complete dim=0
#pragma HLS ARRAY_PARTITION variable=&ArrayLoc      complete dim=0

	loop_height:
			   for (y = 0; y < Height; y++)
			   {
				   U16 PhaseH[HSC_SAMPLES_PER_CLOCK];
				   int ReadEn = 1;
				   xReadPos = 0;
				   xWritePos = 0;
				   x_dsent = 0;
				   PackCnt = 0;
				   OutputWriteEn = 0;

				   U8 nrWrsIdxPrev = 0;
				   U8 nrWrsPrev = 0;
				   U8 nrWrsClck = 0;
				   U8 nrWrsAccu = 0;

			   loop_width:
				   for (x = 0; x <= LoopSize; x += HSC_SAMPLES_PER_CLOCK) // the loop runs for the max of (in,out)
				   {
					   //_Pragma("HLS loop_tripcount max=((MSC_MAX_WIDTH+HSC_ARRAY_SIZE-HSC_SAMPLES_PER_CLOCK - (HSC_TAPS>>1)))")
#pragma HLS LOOP_FLATTEN OFF
#pragma HLS pipeline II=1

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//
			// Pixel buffer control
			//
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					   if (ReadEn)   // If new pixels to be got, shift the old ones
					   {
						   // Shift the pixels in the array over by HSC_SAMPLES_PER_CLOCK
						   for (int i = 0; i <= (HSC_ARRAY_SIZE - 1 - HSC_SAMPLES_PER_CLOCK); i++)
						   {
							   // Includes left border handling
							   PixArray[i] = (((x / HSC_SAMPLES_PER_CLOCK) <= 1) && i < (HSC_ARRAY_SIZE - 2 * HSC_SAMPLES_PER_CLOCK)) ? PixArray[HSC_ARRAY_SIZE - 1 - HSC_SAMPLES_PER_CLOCK + 1] : PixArray[i + HSC_SAMPLES_PER_CLOCK];
						   }
						   // Read new pixels if we didn't exceed the width of frame yet
						   if ((xReadPos + HSC_ARRAY_SIZE - 1 - (HSC_TAPS >> 1)) < InPixels)
						   {
							   // Get new pixels from stream and insert
							   dbgNrRds++;
							   SrcImg >> SrcPix;
							   for (int i = 0; i < HSC_SAMPLES_PER_CLOCK; i++)
							   {
								   for (int k = 0; k < HSC_NR_COMPONENTS; k++)
								   {
									   PixArray[HSC_ARRAY_SIZE - 1 - HSC_SAMPLES_PER_CLOCK + 1 + i].val[k] = SrcPix.val[HSC_NR_COMPONENTS*i + k];
								   }
							   }
						   }
						   else
						   {
							   // Right border handling
							   for (int i = 0; i < HSC_SAMPLES_PER_CLOCK; i++)
							   {
								   PixArray[HSC_ARRAY_SIZE - 1 - HSC_SAMPLES_PER_CLOCK + 1 + i] = PixArray[HSC_ARRAY_SIZE - 1 - HSC_SAMPLES_PER_CLOCK];
							   }

						   }
					   }

#ifdef DBG_PRINT
					   if ((y == 0) && (ReadEn))
					   {
						   for (int i = 0; i < (HSC_ARRAY_SIZE); i++)
						   {
							   printf("[%2d] %3d.%3d.%3d ", (int)i, (int)PixArray[i].val[0], (int)PixArray[i].val[1], (int)PixArray[i].val[2]);
						   }
						   printf("\n");
					   }
#endif
					   if (x >= (HSC_ARRAY_SIZE - HSC_SAMPLES_PER_CLOCK - (HSC_TAPS >> 1)))
					   {
						   U16 xbySamples = x - (HSC_ARRAY_SIZE - HSC_SAMPLES_PER_CLOCK - (HSC_TAPS >> 1));
						   xbySamples = xbySamples / HSC_SAMPLES_PER_CLOCK;
						   // Note 'reg()' below.  Under most circumstances, this is not the preferred way to do this.  In most cases,
						   // HLS supports the ability to set the latency of reads from a memory.  However, in this case, because the
						   // memory is part of the axi_lite adapter, that doesn't work.
						   HSC_PHASE_CTRL phases = reg(arrPhasesH[xbySamples]);
					   loop_samples:
						   for (int s = 0; s < HSC_SAMPLES_PER_CLOCK; s++)
						   {
							   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							   //
							   // Step and phase calculation
							   //
							   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							   PhaseH[s] = phases(HSC_PHASE_CTRL_PHASE_MSB + s*HSC_PHASE_CTRL_BITS, HSC_PHASE_CTRL_PHASE_LSB + s*HSC_PHASE_CTRL_BITS);
							   ArrayLoc[s] = phases(HSC_PHASE_CTRL_INDEX_MSB + s*HSC_PHASE_CTRL_BITS, HSC_PHASE_CTRL_INDEX_LSB + s*HSC_PHASE_CTRL_BITS);
							   OutputWriteEn[s] = phases[HSC_PHASE_CTRL_ENABLE_LSB + s*HSC_PHASE_CTRL_BITS];
						   }
						   ReadEn = (ArrayLoc[HSC_SAMPLES_PER_CLOCK - 1] >= HSC_SAMPLES_PER_CLOCK);

#ifdef DBG_PRINT
						   if (y == 0)
						   {
							   for (int s = 0; s < HSC_SAMPLES_PER_CLOCK; s++)
							   {
								   printf("x %5d, phase %5d, pos %5d, readpos %5d writepos %5d\n", (int)x + s, (int)PhaseH[s], (int)ArrayLoc[s], (int)xReadPos, xWritePos);
							   }
							   printf("rden %d\n", ReadEn);
						   }
#endif
						   hscale_bicubic(PixArray, PhaseH, ArrayLoc, &OutPix);

						   if (ReadEn)
						   {
							   xReadPos += HSC_SAMPLES_PER_CLOCK;
						   }

#if (HSC_SAMPLES_PER_CLOCK==1)
						   {
							   if (OutputWriteEn && (x < LoopSize))
							   {
								   OutImg << OutPix;
								   dbgNrWrs++;
								   xWritePos += HSC_SAMPLES_PER_CLOCK;
							   }
						   }
#else
						   const U8 BitSetCnt[] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 }; // 2^ MAX_SMPLS_PERCLK
						   nrWrsClck = BitSetCnt[OutputWriteEn];

						   // Below table provides an index to the n-th [0..3] 1 bit for numbers between [0..15]
						   const U8 OneBitIdx[4][16] = {
							   {0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0},
							   {0, 0, 0, 1, 0, 2, 2, 1, 0, 3, 3, 1, 3, 2, 2, 1},
							   {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 3, 0, 3, 3, 2},
							   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3}
						   };

						   // If more than HSC_SAMPLES_PER_CLOCK pixels are available we can write them out
						   // This loop combines any pixels that were still waiting to be written with the pixels that were generated this clock cycle
						   nrWrsAccu = nrWrsPrev + nrWrsClck;
						   if ((nrWrsAccu >= HSC_SAMPLES_PER_CLOCK) && (xWritePos < OutPixels))
						   {
							   for (int s = 0; s < HSC_SAMPLES_PER_CLOCK; s++)
							   {
								   for (int c = 0; c < HSC_NR_COMPONENTS; c++)
								   {
									   OutMultiPix.val[s*HSC_NR_COMPONENTS + c] = (s < nrWrsPrev) ? OutPixPrv.val[s*HSC_NR_COMPONENTS + c] : OutPix.val[OneBitIdx[s - nrWrsPrev][OutputWriteEn] * HSC_NR_COMPONENTS + c]; // OutPixCur.val[(s-nrWrsPrev)*HSC_NR_COMPONENTS+c];
								   }
							   }
							   OutImg << OutMultiPix;
							   dbgNrWrs++;
							   xWritePos += HSC_SAMPLES_PER_CLOCK;
						   }

						   // This loop combines any pixels that are still waiting to be written
						   for (int s = 0; s < HSC_SAMPLES_PER_CLOCK; s++)
						   {
							   for (int c = 0; c < HSC_NR_COMPONENTS; c++)
							   {
								   OutPixPrv.val[s*HSC_NR_COMPONENTS + c] = (nrWrsAccu >= HSC_SAMPLES_PER_CLOCK && s < ((nrWrsAccu) % HSC_SAMPLES_PER_CLOCK)) ? OutPix.val[OneBitIdx[s + HSC_SAMPLES_PER_CLOCK - nrWrsPrev][OutputWriteEn] * HSC_NR_COMPONENTS + c] : ((s < nrWrsPrev) ? OutPixPrv.val[s*HSC_NR_COMPONENTS + c] : OutPix.val[OneBitIdx[s - nrWrsPrev][OutputWriteEn] * HSC_NR_COMPONENTS + c]);
							   }
						   }
						   nrWrsPrev = (nrWrsAccu) % HSC_SAMPLES_PER_CLOCK;

#endif
					   }
				   }
			   }
#ifdef DBG_PRINT
			   printf("dbgNrRds %d dbgNrWrs %d\n", dbgNrRds, dbgNrWrs);
#endif
}

static void hscale_bicubic(YUV_PIXEL PixArray[HSC_ARRAY_SIZE],
	U16 PhasesH[HSC_SAMPLES_PER_CLOCK],
	U8 ArrayIdx[HSC_SAMPLES_PER_CLOCK],
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

	for (int s = 0; s < HSC_SAMPLES_PER_CLOCK; s++) // samples
	{
		U16 PhaseH = PhasesH[s];
		U8 idxM1 = ArrayIdx[s] + 1;
		U8 idx = ArrayIdx[s] + 2;
		U8 idxP1 = ArrayIdx[s] + 3;
		U8 idxP2 = ArrayIdx[s] + 4;

		for (int i = 0; i < HSC_NR_COMPONENTS; i++)
		{
			a = ((PixArray[idx].val[i] * 3) - (PixArray[idxP1].val[i] * 3) - (PixArray[idxM1].val[i] * 1) + (PixArray[idxP2].val[i] * 1)) >> 1;
			b = ((PixArray[idxP1].val[i] * 4) - (PixArray[idx].val[i] * 5) + (PixArray[idxM1].val[i] * 2) - (PixArray[idxP2].val[i] * 1)) >> 1;
			c = ((PixArray[idxP1].val[i] * 1) - (PixArray[idxM1].val[i] * 1)) >> 1;
			d = PixArray[idx].val[i] * HSC_PHASES;

			ax3 = (((ap_int<52>) a * PhaseH * PhaseH * PhaseH) + HSC_PHASES) >> (HSC_PHASE_SHIFT + HSC_PHASE_SHIFT); // mapped to 5 DSPs
			bx2 = (((ap_int<40>) b * PhaseH * PhaseH) + (HSC_PHASES >> 1)) >> (HSC_PHASE_SHIFT); // mapped to 3 DSPs, reuses PhaseH*PhaseH
			cx = ((ap_int<28>) c * PhaseH);   // mapped to 3 DSPs

			sum = (I64)((ax3 + bx2 + cx) + (I32)d);
			norm = (I32)(sum + (HSC_PHASES >> 1)) >> HSC_PHASE_SHIFT;
			norm = CLAMP(norm, 0, (I32)((1 << HSC_BITS_PER_COMPONENT) - 1));
			OutPix->val[HSC_NR_COMPONENTS*s + i] = norm;
		}
	}
}
#endif
