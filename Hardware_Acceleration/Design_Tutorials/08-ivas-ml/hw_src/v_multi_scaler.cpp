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

//#define DBG_PRINT
const U8 rgb = 0;
const U8 yuv444 = 1;
const U8 yuv422 = 2;
const U8 yuv420 = 3;

static void v_scaler_top(
	AXIMM srcImgBuf0,
	AXIMM srcImgBuf1,
#if (MAX_NR_PLANES == 3)
	AXIMM srcImgBuf2,
#endif
	U16 HeightIn,
	U16 HeightOut,
	U16 WidthIn,
	U16 WidthOut,
	U16 StrideIn,
	U16 StrideOut,
	U8 InPixelFmt,
	U8 OutPixelFmt,
	U32 PixelRate,
	U32 LineRate,
#if (HSC_SCALE_MODE == HSC_POLYPHASE)
	I16 hfltCoeff[HSC_PHASES][HSC_TAPS][HSC_SAMPLES_PER_CLOCK],
#endif
#if (VSC_SCALE_MODE == VSC_POLYPHASE)
	I16 vfltCoeff[VSC_PHASES][VSC_TAPS],
#endif
	HSC_PHASE_CTRL blkmm_phasesH[HSC_MAX_WIDTH / HSC_SAMPLES_PER_CLOCK],
	AXIMM  dstImgBuf0,
	AXIMM  dstImgBuf1
#if (MAX_NR_PLANES == 3)
	, AXIMM  dstImgBuf2
#endif
);

static void calc_phaseH(U16 HeightIn,
	U16 HeightOut,
	U16 WidthIn,
	U16 WidthOut,
	U32 PixelRate,
	HSC_PHASE_CTRL* blkmm_phasesH,
	ap_uint<1> done_flag)
{
	unsigned int loopWidth = (MAX(WidthIn, WidthOut) + (HSC_SAMPLES_PER_CLOCK - 1)) / HSC_SAMPLES_PER_CLOCK;
	int offset = 0;
	unsigned int xWritePos = 0;
	ap_uint<1> OutputWriteEn;
	ap_uint<1> GetNewPix;
	ap_uint<6> PhaseH;
	ap_uint<3> arrayIdx;

	arrayIdx = 0;
	for (int x = 0; x < loopWidth; x++)
	{
		blkmm_phasesH[x] = 0;
		//nrRdsClck = 0;
		for (int s = 0; s < HSC_SAMPLES_PER_CLOCK; s++)
		{
#pragma HLS pipeline
			/*blkmm_phasesH[x](HSC_PHASE_CTRL_PHASE_MSB + s*HSC_PHASE_CTRL_BITS, HSC_PHASE_CTRL_PHASE_LSB + s*HSC_PHASE_CTRL_BITS) = 0;
			blkmm_phasesH[x](HSC_PHASE_CTRL_INDEX_MSB + s*HSC_PHASE_CTRL_BITS, HSC_PHASE_CTRL_INDEX_LSB + s*HSC_PHASE_CTRL_BITS) = 1;
			blkmm_phasesH[x][                                                  HSC_PHASE_CTRL_ENABLE_LSB + s*HSC_PHASE_CTRL_BITS] = 1;*/

			PhaseH = (offset >> (STEP_PRECISION_SHIFT - HSC_PHASE_SHIFT)) & (HSC_PHASES - 1);
			GetNewPix = 0;
			OutputWriteEn = 0;
			if ((offset >> STEP_PRECISION_SHIFT) != 0)
			{
				// read a new input sample
				GetNewPix = 1;
				offset = offset - (1 << STEP_PRECISION_SHIFT);
				OutputWriteEn = 0;
				arrayIdx++;
				//xReadPos++;
			}
			if (((offset >> STEP_PRECISION_SHIFT) == 0) && (xWritePos < (U32)WidthOut))
			{
				// produce a new output sample
				offset += PixelRate;
				OutputWriteEn = 1;
				xWritePos++;
			}
			//printf("x %5d, offset %5d, phase %5d, arrayIdx %5d, readpos %5d writepos %5d  rden %3d wren %3d\n", (int)x*HSC_SAMPLES_PER_CLOCK+s, offset, (int)PhaseH, (int)arrayIdx, (int)xReadPos, xWritePos, GetNewPix, OutputWriteEn);
			blkmm_phasesH[x](HSC_PHASE_CTRL_PHASE_MSB + s*HSC_PHASE_CTRL_BITS, HSC_PHASE_CTRL_PHASE_LSB + s*HSC_PHASE_CTRL_BITS) = PhaseH;
			blkmm_phasesH[x](HSC_PHASE_CTRL_INDEX_MSB + s*HSC_PHASE_CTRL_BITS, HSC_PHASE_CTRL_INDEX_LSB + s*HSC_PHASE_CTRL_BITS) = arrayIdx;
			blkmm_phasesH[x][HSC_PHASE_CTRL_ENABLE_LSB + s*HSC_PHASE_CTRL_BITS] = OutputWriteEn;
		}

		done_flag = 1;
		if (arrayIdx >= HSC_SAMPLES_PER_CLOCK)
			arrayIdx &= (HSC_SAMPLES_PER_CLOCK - 1);
	}
}

static void GetMultiScAndCoeff(HSC_HW_STRUCT_REG& HwReg, V_SCALER_TOP_STRUCT& Multi_Sc
#if (HSC_SCALE_MODE == HSC_POLYPHASE)
	, I16 hfltCoeff[HSC_PHASES][HSC_TAPS][HSC_SAMPLES_PER_CLOCK], I16 vfltCoeff[VSC_PHASES][VSC_TAPS]
#endif
)
{
#pragma HLS inline
#define U32_VALUE_FROM_AXIMM_ARRAY(x)	u32TempArray[x]
#define U64_VALUE_FROM_AXIMM_ARRAY(x)	((U64)u32TempArray[x] + ((U64)u32TempArray[x+1]<<32))
#define READ_LENGTH_DESCRIPTOR 	((sizeof(V_SCALER_TOP_STRUCT) + AXIMM_DATA_WIDTH8 - 1) / AXIMM_DATA_WIDTH8)
#define READ_LENGTH_COEFF_H		(HSC_PHASES*HSC_TAPS*16/AXIMM_DATA_WIDTH)
#define READ_LENGTH_COEFF_V		(VSC_PHASES*VSC_TAPS*16/AXIMM_DATA_WIDTH)

	U32 i, j, k, u32TempArray[(AXIMM_DATA_WIDTH8 / 4)*READ_LENGTH_DESCRIPTOR], xOffset, yOffset;
	U64 readOffset, readOffset2;
	ap_uint<AXIMM_DATA_WIDTH> aximmTemp, aximmTemp2;

	readOffset = HwReg.start_addr / AXIMM_DATA_WIDTH8;
	for (i = 0; i < READ_LENGTH_DESCRIPTOR; i++)
	{
		aximmTemp = HwReg.ms_maxi_srcbuf[readOffset + i];
		for (j = 0; j < (AXIMM_DATA_WIDTH8 / 4); j++)
		{
			u32TempArray[(i*AXIMM_DATA_WIDTH8) / 4 + j] = aximmTemp(j * 32 + 31, j * 32);
		}
	}

	Multi_Sc.msc_widthIn = U32_VALUE_FROM_AXIMM_ARRAY(0);
	Multi_Sc.msc_widthOut = U32_VALUE_FROM_AXIMM_ARRAY(1);
	Multi_Sc.msc_heightIn = U32_VALUE_FROM_AXIMM_ARRAY(2);
	Multi_Sc.msc_heightOut = U32_VALUE_FROM_AXIMM_ARRAY(3);
	Multi_Sc.msc_lineRate = U32_VALUE_FROM_AXIMM_ARRAY(4);
	Multi_Sc.msc_pixelRate = U32_VALUE_FROM_AXIMM_ARRAY(5);
	Multi_Sc.msc_inPixelFmt = U32_VALUE_FROM_AXIMM_ARRAY(6);
	Multi_Sc.msc_outPixelFmt = U32_VALUE_FROM_AXIMM_ARRAY(7);
	Multi_Sc.msc_strideIn = U32_VALUE_FROM_AXIMM_ARRAY(8);
	Multi_Sc.msc_strideOut = U32_VALUE_FROM_AXIMM_ARRAY(9);
	Multi_Sc.msc_srcImgBuf0 = U64_VALUE_FROM_AXIMM_ARRAY(10);
	Multi_Sc.msc_srcImgBuf1 = U64_VALUE_FROM_AXIMM_ARRAY(12);
	Multi_Sc.msc_srcImgBuf2 = U64_VALUE_FROM_AXIMM_ARRAY(14);
	Multi_Sc.msc_dstImgBuf0 = U64_VALUE_FROM_AXIMM_ARRAY(16);
	Multi_Sc.msc_dstImgBuf1 = U64_VALUE_FROM_AXIMM_ARRAY(18);
	Multi_Sc.msc_dstImgBuf2 = U64_VALUE_FROM_AXIMM_ARRAY(20);
	Multi_Sc.hfltCoeffOffset = U64_VALUE_FROM_AXIMM_ARRAY(22);
	Multi_Sc.vfltCoeffOffset = U64_VALUE_FROM_AXIMM_ARRAY(24);
	Multi_Sc.msc_nxtaddr = U64_VALUE_FROM_AXIMM_ARRAY(26);

#if (HSC_SCALE_MODE == HSC_POLYPHASE)
	/* hfiltCoff read */
	xOffset = yOffset = 0;
	readOffset = Multi_Sc.hfltCoeffOffset / AXIMM_DATA_WIDTH8;
	for (i = 0; i < READ_LENGTH_COEFF_H; i++)
	{
		aximmTemp = HwReg.ms_maxi_srcbuf[readOffset + i];
		for (j = 0; j < AXIMM_DATA_WIDTH8 / 2; j++)
		{
			for (k = 0; k < HSC_SAMPLES_PER_CLOCK; k++)
			{
				hfltCoeff[yOffset][xOffset][k] = aximmTemp(j * 16 + 15, j * 16);
			}
			yOffset += (xOffset == (HSC_TAPS - 1)) ? 1 : 0;
			xOffset = (xOffset == (HSC_TAPS - 1)) ? 0 : (xOffset + 1);
		}
	}

	/* vfiltCoff read */
	xOffset = yOffset = 0;
	readOffset = Multi_Sc.vfltCoeffOffset / AXIMM_DATA_WIDTH8;
	for (i = 0; i < READ_LENGTH_COEFF_V; i++)
	{
		aximmTemp = HwReg.ms_maxi_srcbuf[readOffset + i];
		for (j = 0; j < AXIMM_DATA_WIDTH8 / 2; j++)
		{
			vfltCoeff[yOffset][xOffset] = aximmTemp(j * 16 + 15, j * 16);
			yOffset += (xOffset == (VSC_TAPS - 1)) ? 1 : 0;
			xOffset = (xOffset == (VSC_TAPS - 1)) ? 0 : (xOffset + 1);
		}
	}
#endif
}


/*********************************************************************************
* Function:    hscale_top
* Parameters:  Stream of input/output pixels, image resolution, type of scaling etc
* Return:
* Description: Top level function to perform horizontal image resizing
* submodules - AXIvideo2MultiPixStream
*              hscale_core
*              MultiPixStream2AXIvideo
**********************************************************************************/
extern "C" {
void v_multi_scaler(U8  num_outs, U64 start_addr, AXIMM ms_maxi_srcbuf,
		AXIMM ms_maxi_dstbuf, U8 ms_status)
{
	__xilinx_ip_top(0);

	const int NUM_OUTSTANDING = AXIMM_NUM_OUTSTANDING;
	const int BURST_LENGTH = AXIMM_BURST_LENGTH;

#pragma HLS INTERFACE s_axilite port=&num_outs bundle=CTRL offset=0x0020
#pragma HLS INTERFACE ap_none port=&num_outs

#pragma HLS INTERFACE s_axilite port=&start_addr bundle=CTRL offset=0x0030
#pragma HLS INTERFACE ap_none port=&start_addr

#pragma HLS INTERFACE m_axi port=&ms_maxi_srcbuf offset=slave bundle=mm_video latency=100 depth=2*128*128 num_write_outstanding=NUM_OUTSTANDING num_read_outstanding=NUM_OUTSTANDING max_write_burst_length=BURST_LENGTH max_read_burst_length=BURST_LENGTH
#pragma HLS INTERFACE s_axilite port=&ms_maxi_srcbuf bundle=CTRL offset=0x0040

#pragma HLS INTERFACE m_axi port=&ms_maxi_dstbuf offset=slave bundle=mm_video latency=100 depth=2*256*256 num_write_outstanding=NUM_OUTSTANDING num_read_outstanding=NUM_OUTSTANDING max_write_burst_length=BURST_LENGTH max_read_burst_length=BURST_LENGTH
#pragma HLS INTERFACE s_axilite port=&ms_maxi_dstbuf bundle=CTRL offset=0x0050

#pragma HLS INTERFACE s_axilite port=&ms_status bundle=CTRL offset=0x0060
#pragma HLS INTERFACE ap_none port=&ms_status

#pragma HLS INTERFACE s_axilite port=return bundle=CTRL
#pragma HLS INTERFACE ap_ctrl_hs port=return

#pragma HLS ALLOCATION instances=v_scaler_top   limit=1 function
#pragma HLS ALLOCATION instances=calc_phaseH    limit=1 function

	HSC_HW_STRUCT_REG HwReg;
	HwReg.ms_maxi_dstbuf = ms_maxi_dstbuf;
	HwReg.ms_maxi_srcbuf = ms_maxi_srcbuf;
	HwReg.ms_status = ms_status;
	HwReg.num_outs = num_outs;
	HwReg.start_addr = start_addr;

	V_SCALER_TOP_STRUCT Multi_Sc;
	I16 hfltCoeff[HSC_PHASES][HSC_TAPS][HSC_SAMPLES_PER_CLOCK];
#pragma HLS ARRAY_PARTITION variable=&hfltCoeff     complete dim=2
#pragma HLS ARRAY_PARTITION variable=&hfltCoeff     complete dim=3
#pragma HLS RESOURCE variable=&hfltCoeff core=RAM_1P_LUTRAM

	I16 vfltCoeff[VSC_PHASES][VSC_TAPS];
#pragma HLS RESOURCE variable=&vfltCoeff core=RAM_1P_LUTRAM
#pragma HLS ARRAY_PARTITION variable=&vfltCoeff complete dim=2

	HSC_PHASE_CTRL blkmm_phasesH[HSC_MAX_WIDTH / HSC_SAMPLES_PER_CLOCK];
	ap_uint<1> done_flag;
	U8 stats = 0;
	U8 dummy = 0;
	ap_uint<1> RdnxtDesc = 1;

#if(dbg==1)
	printf("\n chloop = %d", chloop);
	printf("\nHwReg.start_addr = 0X%x\n", HwReg.start_addr);
#endif

	while (RdnxtDesc != 0)
	{
		GetMultiScAndCoeff(HwReg, Multi_Sc
#if (HSC_SCALE_MODE == HSC_POLYPHASE)
			, hfltCoeff, vfltCoeff
#endif
		);
#if (dbg==1)
		printf("\nMulti_Sc.msc_widthIn 0x%x", Multi_Sc.msc_widthIn);
		printf("\nMulti_Sc.msc_widthOut 0x%x", Multi_Sc.msc_widthOut);
		printf("\nMulti_Sc.msc_heightIn 0x%x", Multi_Sc.msc_heightIn);
		printf("\nMulti_Sc.msc_heightOut 0x%x", Multi_Sc.msc_heightOut);
		printf("\nMulti_Sc.msc_lineRate 0x%x", Multi_Sc.msc_lineRate);
		printf("\nMulti_Sc.msc_pixelRate 0x%x", Multi_Sc.msc_pixelRate);
		printf("\nMulti_Sc.msc_inPixelFmt 0x%x", Multi_Sc.msc_inPixelFmt);
		printf("\nMulti_Sc.msc_outPixelFmt 0x%x", Multi_Sc.msc_outPixelFmt);
		printf("\nMulti_Sc.msc_strideIn 0x%x", Multi_Sc.msc_strideIn);
		printf("\nMulti_Sc.msc_strideOut 0x%x", Multi_Sc.msc_strideOut);
		printf("\nMulti_Sc.msc_srcImgBuf0 0x%x", Multi_Sc.msc_srcImgBuf0);
		printf("\nMulti_Sc.msc_srcImgBuf1 0x%x", Multi_Sc.msc_srcImgBuf1);
		printf("\nMulti_Sc.msc_srcImgBuf2 0x%x", Multi_Sc.msc_srcImgBuf2);
		printf("\nMulti_Sc.msc_dstImgBuf0 0x%x", Multi_Sc.msc_dstImgBuf0);
		printf("\nMulti_Sc.msc_dstImgBuf1 0x%x", Multi_Sc.msc_dstImgBuf1);
		printf("\nMulti_Sc.msc_dstImgBuf2 0x%x", Multi_Sc.msc_dstImgBuf2);
		printf("\nMulti_Sc.msc_blkmm_hfltCoeff 0x%x", Multi_Sc.msc_blkmm_hfltCoeff);
		printf("\nMulti_Sc.msc_blkmm_vfltCoeff 0x%x", Multi_Sc.msc_blkmm_vfltCoeff);
		printf("\nMulti_Sc.msc_nxtaddr 0x%x", Multi_Sc.msc_nxtaddr);
#endif

		calc_phaseH(Multi_Sc.msc_heightIn, Multi_Sc.msc_heightOut, Multi_Sc.msc_widthIn, Multi_Sc.msc_widthOut, Multi_Sc.msc_pixelRate, blkmm_phasesH, done_flag); //do calc_phaseH
#if(dbg==1)
		printf("\n(AXIMM) &HwReg.ms_maxi_srcbuf[Multi_Sc.msc_srcImgBuf0] = 0x%x", &HwReg.ms_maxib[Multi_Sc.msc_srcImgBuf0 / 4]);
#endif

		AXIMM src0, src1, src2, dst0, dst1, dst2;

		src0 = (AXIMM)&HwReg.ms_maxi_srcbuf[Multi_Sc.msc_srcImgBuf0 / AXIMM_DATA_WIDTH8];
		src1 = (AXIMM)&HwReg.ms_maxi_srcbuf[Multi_Sc.msc_srcImgBuf1 / AXIMM_DATA_WIDTH8];
#if (MAX_NR_PLANES == 3)
		src2 = (AXIMM)&HwReg.ms_maxi_srcbuf[Multi_Sc.msc_srcImgBuf2 / AXIMM_DATA_WIDTH8];
#endif

		dst0 = (AXIMM)&HwReg.ms_maxi_dstbuf[Multi_Sc.msc_dstImgBuf0 / AXIMM_DATA_WIDTH8];
		dst1 = (AXIMM)&HwReg.ms_maxi_dstbuf[Multi_Sc.msc_dstImgBuf1 / AXIMM_DATA_WIDTH8];
#if (MAX_NR_PLANES == 3)
		dst2 = (AXIMM)&HwReg.ms_maxi_dstbuf[Multi_Sc.msc_dstImgBuf2 / AXIMM_DATA_WIDTH8];
#endif

		v_scaler_top(src0,
			src1,
#if (MAX_NR_PLANES == 3)
			src2,
#endif
			(U16)Multi_Sc.msc_heightIn,
			(U16)Multi_Sc.msc_heightOut,
			(U16)Multi_Sc.msc_widthIn,
			(U16)Multi_Sc.msc_widthOut,
			(U16)Multi_Sc.msc_strideIn,
			(U16)Multi_Sc.msc_strideOut,
			(U8)Multi_Sc.msc_inPixelFmt,
			(U8)Multi_Sc.msc_outPixelFmt,
			(U32)Multi_Sc.msc_pixelRate,
			(U32)Multi_Sc.msc_lineRate,
#if (HSC_SCALE_MODE == HSC_POLYPHASE)
			hfltCoeff,
#endif
#if (VSC_SCALE_MODE == VSC_POLYPHASE)
			vfltCoeff,
#endif
			blkmm_phasesH,
			dst0,
			dst1
#if (MAX_NR_PLANES == 3)
			, dst2
#endif
		);

		stats = stats + 1;
		HwReg.ms_status = stats;
		HwReg.start_addr = Multi_Sc.msc_nxtaddr;
		if ((HwReg.start_addr == 0) || (stats == HwReg.num_outs))
			RdnxtDesc = 0;
	}
}
}

static void v_scaler_top(AXIMM srcImgBuf0,
	AXIMM srcImgBuf1,
#if (MAX_NR_PLANES == 3)
	AXIMM srcImgBuf2,
#endif
	U16 HeightIn,
	U16 HeightOut,
	U16 WidthIn,
	U16 WidthOut,
	U16 StrideIn,
	U16 StrideOut,
	U8 InPixelFmt,
	U8 OutPixelFmt,
	U32 PixelRate,
	U32 LineRate,
#if (HSC_SCALE_MODE == HSC_POLYPHASE)
	I16 hfltCoeff[HSC_PHASES][HSC_TAPS][HSC_SAMPLES_PER_CLOCK],
#endif
#if (VSC_SCALE_MODE == VSC_POLYPHASE)
	I16 vfltCoeff[VSC_PHASES][VSC_TAPS],
#endif
	HSC_PHASE_CTRL blkmm_phasesH[HSC_MAX_WIDTH / HSC_SAMPLES_PER_CLOCK],
	AXIMM dstImgBuf0,
	AXIMM dstImgBuf1
#if (MAX_NR_PLANES == 3)
	, AXIMM dstImgBuf2
#endif
)
{
#if(dbg==1)
	printf("\nsrcImgBuf0 = 0x%x\n", srcImgBuf0);
	printf("\nsrcImgBuf1 = 0x%x\n", srcImgBuf1);
	//printf("\nsrcImgBuf2 = 0x%x\n", srcImgBuf2);
	printf("\ndstImgBuf0 = 0x%x\n", dstImgBuf0);
	printf("\ndstImgBuf1 = 0x%x\n", dstImgBuf1);
	//printf("\ndstImgBuf2 = 0x%x\n", dstImgBuf2);
#endif

	U8 ColorModeIn = MEMORY2LIVE[InPixelFmt];
	U8 ColorModeOut = MEMORY2LIVE[OutPixelFmt];
	bool bPassThruVcrUp = (ColorModeIn == yuv420) ? false : true;
	bool bPassThruHcrUp = (ColorModeIn == yuv422 || ColorModeIn == yuv420) ? false : true;
	bool bPassThruHcrDown = (ColorModeOut == yuv422 || ColorModeOut == yuv420) ? false : true;
	bool bPassThruVcrDown = (ColorModeOut == yuv420) ? false : true;
	bool bPassThruCsc = ((ColorModeIn == rgb && ColorModeOut != rgb) || (ColorModeIn != rgb && ColorModeOut == rgb)) ? false : true;

	STREAM_PLANES     srcPlanes;
	STREAM_PLANES     dstPlanes;
	int WidthInBytes;
	int WidthOutBytes;

	const int PLANE_STREAM_DEPTH0 = 2 * PLANE0_STREAM_DEPTH;
	const int PLANE_STREAM_DEPTH1 = 2 * PLANE0_STREAM_DEPTH;
#if (MAX_NR_PLANES==3)
	const int PLANE_STREAM_DEPTH2 = 2 * PLANE0_STREAM_DEPTH;
#endif

#pragma HLS stream depth=PLANE_STREAM_DEPTH0 variable=&srcPlanes.plane0
#pragma HLS resource variable=&srcPlanes.plane0 core=FIFO_BRAM
#pragma HLS stream depth=PLANE_STREAM_DEPTH0 variable=&dstPlanes.plane0
#pragma HLS resource variable=&dstPlanes.plane0 core=FIFO_BRAM

#if (MAX_NR_PLANES==2)
#pragma HLS stream depth=PLANE_STREAM_DEPTH1 variable=&srcPlanes.plane1
#pragma HLS resource variable=&srcPlanes.plane1 core=FIFO_BRAM
#pragma HLS stream depth=PLANE_STREAM_DEPTH1 variable=&dstPlanes.plane1
#pragma HLS resource variable=&dstPlanes.plane1 core=FIFO_BRAM
#endif

#if (MAX_NR_PLANES==3)
#pragma HLS stream depth=PLANE_STREAM_DEPTH1 variable=&srcPlanes.plane1
#pragma HLS resource variable=&srcPlanes.plane1 core=FIFO_BRAM
#pragma HLS stream depth=PLANE_STREAM_DEPTH1 variable=&dstPlanes.plane1
#pragma HLS resource variable=&dstPlanes.plane1 core=FIFO_BRAM

#pragma HLS stream depth=PLANE_STREAM_DEPTH2 variable=&srcPlanes.plane2
#pragma HLS resource variable=&srcPlanes.plane2 core=FIFO_BRAM
#pragma HLS stream depth=PLANE_STREAM_DEPTH2 variable=&dstPlanes.plane2
#pragma HLS resource variable=&dstPlanes.plane2 core=FIFO_BRAM
#endif

	HSC_STREAM_MULTIPIX stream_in;
	HSC_STREAM_MULTIPIX stream_1;
	HSC_STREAM_MULTIPIX stream_2;
	HSC_STREAM_MULTIPIX stream_3;
	HSC_STREAM_MULTIPIX stream_4;
	HSC_STREAM_MULTIPIX stream_4_csc;
	HSC_STREAM_MULTIPIX stream_5;
	HSC_STREAM_MULTIPIX stream_out;


#pragma HLS DATAFLOW

#pragma HLS stream depth=16 variable=&stream_in
#pragma HLS stream depth=16 variable=&stream_1
#pragma HLS stream depth=16 variable=&stream_2
#pragma HLS stream depth=4096 variable=&stream_3
#pragma HLS stream depth=16 variable=&stream_4
#pragma HLS stream depth=16 variable=&stream_4_csc
#pragma HLS stream depth=16 variable=&stream_5
#pragma HLS stream depth=16 variable=&stream_out

#if ( (HAS_Y_UV10_Y_UV10_420==1) || (HAS_Y10==1) )
	if (InPixelFmt == Y_UV10 || InPixelFmt == Y_UV10_420 || InPixelFmt == Y10)
	{
		//4 bytes per 3 pixels
		WidthInBytes = (WidthIn * 4) / 3;
	}
	else
#endif
	{
		WidthInBytes = WidthIn*BYTES_PER_PIXEL[InPixelFmt];
	}

#if ( (HAS_Y_UV10_Y_UV10_420==1) || (HAS_Y10==1) )
	if (OutPixelFmt == Y_UV10 || OutPixelFmt == Y_UV10_420 || OutPixelFmt == Y10)
	{
		//4 bytes per 3 pixels
		WidthOutBytes = (WidthOut * 4) / 3;
	}
	else
#endif
	{
		WidthOutBytes = WidthOut*BYTES_PER_PIXEL[OutPixelFmt];
	}

#if (MAX_NR_PLANES == 3)
	AXIMMvideo2Bytes(srcImgBuf0, srcImgBuf1, srcImgBuf2, srcPlanes, HeightIn, WidthIn, WidthInBytes, StrideIn, InPixelFmt);
#endif

#if ( (MAX_NR_PLANES == 2) || (MAX_NR_PLANES == 1))
	AXIMMvideo2Bytes(srcImgBuf0, srcImgBuf1, srcPlanes, HeightIn, WidthInBytes, StrideIn, InPixelFmt);
#endif

	Bytes2MultiPixStream(srcPlanes, stream_in, HeightIn, WidthIn, WidthInBytes, StrideIn, InPixelFmt);

	v_vcresampler(stream_in, HeightIn, WidthIn, yuv420, bPassThruVcrUp, stream_1);

	v_hcresampler(stream_1, HeightIn, WidthIn, yuv422, bPassThruHcrUp, stream_2);

	v_vscaler(stream_2,
		HeightIn,
		WidthIn,
		HeightOut,
		LineRate,
#if  (VSC_SCALE_MODE == VSC_POLYPHASE)
		vfltCoeff,
#endif
		stream_3
	);

	v_hscaler(stream_3,
		HeightOut,
		WidthIn,
		WidthOut,
		PixelRate,
		ColorModeIn,
#if (HSC_SCALE_MODE == HSC_POLYPHASE)
		hfltCoeff,
#endif
		blkmm_phasesH,
		stream_4
	);

	v_csc(stream_4, HeightOut, WidthOut, ColorModeIn, bPassThruCsc, stream_4_csc);

	v_hcresampler(stream_4_csc, HeightOut, WidthOut, yuv444, bPassThruHcrDown, stream_5);

	v_vcresampler(stream_5, HeightOut, WidthOut, yuv422, bPassThruVcrDown, stream_out);

	MultiPixStream2Bytes(stream_out, dstPlanes, HeightOut, WidthOut, WidthOutBytes, StrideOut, OutPixelFmt);

#if (MAX_NR_PLANES == 3)
	//JH First
#if(dbg == 1)
	printf("Before: Bytes2AXIMMvideo: ###############################################################\n");
	for (int kp = 0; kp < 10; kp++) printf("dstImgBuf0=0x%llx\n", ((*(dstImgBuf0 + kp)).to_int()));
	printf("###############################################################\n");
	for (int kp = 0; kp < 10; kp++) printf("dstImgBuf1=0x%llx\n", ((*(dstImgBuf1 + kp)).to_int()));
	printf("###############################################################\n");
	for (int kp = 0; kp < 10; kp++) printf("dstImgBuf2=0x%llx\n", ((*(dstImgBuf2 + kp)).to_int()));
	printf("###############################################################\n");
#endif
	//JH Last

	Bytes2AXIMMvideo(dstPlanes, dstImgBuf0, dstImgBuf1, dstImgBuf2, HeightOut, WidthOut, WidthOutBytes, StrideOut, OutPixelFmt);
#if(dbg == 1)
	printf("After: Bytes2AXIMMvideo###############################################################\n");
	for (int kp = 0; kp < 10; kp++) printf("dstImgBuf0=0x%llx\n", ((*(dstImgBuf0 + kp)).to_int()));
	printf("###############################################################\n");
	for (int kp = 0; kp < 10; kp++) printf("dstImgBuf1=0x%llx\n", ((*(dstImgBuf1 + kp)).to_int()));
	printf("###############################################################\n");
	for (int kp = 0; kp < 10; kp++) printf("dstImgBuf2=0x%llx\n", ((*(dstImgBuf2 + kp)).to_int()));
	printf("###############################################################\n");
#endif
#endif

#if ( (MAX_NR_PLANES == 2) || (MAX_NR_PLANES == 1))
	Bytes2AXIMMvideo(dstPlanes, dstImgBuf0, dstImgBuf1, HeightOut, WidthOutBytes, StrideOut, OutPixelFmt);
#endif
}
