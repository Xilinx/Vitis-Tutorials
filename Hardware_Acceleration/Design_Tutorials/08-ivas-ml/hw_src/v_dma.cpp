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
#include <assert.h>
#include "v_multi_scaler.h"

#define MAX_DATA_WIDTH 		HSC_BITS_PER_COMPONENT
#define NR_COMPONENTS		HSC_NR_COMPONENTS
#define SAMPLES_PER_CLOCK	HSC_SAMPLES_PER_CLOCK

#if (MAX_NR_PLANES == 1)
void AXIMMvideo2Bytes(
	AXIMM srcImg,
	AXIMM srcImg2,
	STREAM_PLANES& img,
	const U16 Height,
	const U16 WidthInBytes,
	const U16 StrideInBytes,
	const U8  VideoFormat)
{
	int offset = 0;

loop_AXIMMvideo2Bytes_1plane:
	for (int y = 0; y < Height; y++)
	{
#pragma HLS loop_tripcount max=4320

#pragma HLS loop_flatten off
		for (int x = 0; x < (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8); x++)
		{
#pragma HLS pipeline II=1

			ap_uint<AXIMM_DATA_WIDTH> fb_pix = srcImg[offset + x];

			img.plane0 << fb_pix;
		}
		offset += StrideInBytes / (AXIMM_DATA_WIDTH / 8);
	}
}
#endif

#if (MAX_NR_PLANES == 2)
void AXIMMvideo2Bytes(
	AXIMM srcImg,
	AXIMM srcImg2,
	STREAM_PLANES& img,
	const U16 Height,
	const U16 WidthInBytes,
	const U16 StrideInBytes,
	const U8  VideoFormat)
{
	int offsetY = 0;
	int offsetUV = 0; //(Height)*StrideInBytes/(AXIMM_DATA_WIDTH/8);

loop_AXIMMvideo2Bytes_2planes:
	for (int y = 0; y < Height; y++)
	{
#pragma HLS loop_tripcount max=4320

#pragma HLS loop_flatten off
		for (int x = 0; x < (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8); x++)
		{
#pragma HLS pipeline II=1

			ap_uint<AXIMM_DATA_WIDTH> fb_pix = srcImg[offsetY + x];

			img.plane0 << fb_pix;
		}
		offsetY += StrideInBytes / (AXIMM_DATA_WIDTH / 8);

		if (NR_PLANES(VideoFormat) == 2 && (!(y & 1) || !IS_420(VideoFormat)))
		{
			for (int x = 0; x < (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8); x++)
			{
#pragma HLS pipeline II=1

				ap_uint<AXIMM_DATA_WIDTH> fb_pix = srcImg2[offsetUV + x];

				img.plane1 << fb_pix;
			}
			offsetUV += StrideInBytes / (AXIMM_DATA_WIDTH / 8);
		}
	}
}
#endif

#if (MAX_NR_PLANES == 3)
void AXIMMvideo2Bytes(
	AXIMM srcImg,
	AXIMM srcImg2,
	AXIMM srcImg3,
	STREAM_PLANES& img,
	const U16 Height,
	const U16 WidthIn,
	const U16 WidthInBytes,
	const U16 StrideInBytes,
	const U8  VideoFormat)
{

	if ((VideoFormat == Y_U_V8_420) || (VideoFormat == R_G_B8))
	{
#if(MAX_NR_PLANES == 3)
#if ( (HAS_Y_U_V8_420 == 1) || (HAS_R_G_B8 == 1) )
		ap_uint<AXIMM_DATA_WIDTH> *yBase, *uBase, *vBase;
		yBase = srcImg;
		uBase = srcImg2;
		vBase = srcImg3;
		const int rows = Height;
		const int cols = WidthIn;
		const int videosize = rows*cols;
		ap_uint<AXIMM_DATA_WIDTH> y, u, v; // fix width!

		int ratio = AXIMM_DATA_WIDTH / 8;
		int YnumTsf = (videosize + ratio - 1) / ratio;
		int UVnumTsf = (videosize / 4 + ratio - 1) / ratio;
		int numBst = (YnumTsf + (AXIMM_DATA_WIDTH - 1)) / AXIMM_DATA_WIDTH;
		for (int i = 0; i < numBst; i++)
		{
			//#pragma HLS pipeline II=1
			//#pragma HLS loop_flatten off
			int offset = i*AXIMM_DATA_WIDTH;
			for (int j = 0;j < AXIMM_DATA_WIDTH;j++)
			{
#pragma HLS pipeline II=1
				y = yBase[offset + j];
				if (offset + j < YnumTsf) {
					img.plane0 << y;
				}
			}
			if (VideoFormat == Y_U_V8_420)
			{
				if (i % 4 == 0)
				{
					for (int j = 0;j < AXIMM_DATA_WIDTH;j++)
					{
#pragma HLS pipeline II=1
						u = uBase[offset / 4 + j];
						if (offset / 4 + j < UVnumTsf) {
							img.plane1 << u;
						}
					}

					for (int j = 0;j < AXIMM_DATA_WIDTH;j++)
					{
#pragma HLS pipeline II=1
						v = vBase[offset / 4 + j];
						if (offset / 4 + j < UVnumTsf) {
							img.plane2 << v;
						}
					}
				}
			}
			else // ColorMode==rgb
			{
				for (int j = 0;j < AXIMM_DATA_WIDTH;j++)
				{
#pragma HLS pipeline II=1
					u = uBase[offset + j];
					if (offset + j < YnumTsf) {
						img.plane1 << u;
					}
				}
				for (int j = 0;j < AXIMM_DATA_WIDTH;j++)
				{
#pragma HLS pipeline II=1
					v = vBase[offset + j];
					if (offset + j < YnumTsf) {
						img.plane2 << v;
					}
				}
			}
		}
#endif
#endif
	}
	else
	{
		int offsetY = 0;
		int offsetUV = 0; //(Height)*StrideInBytes/(AXIMM_DATA_WIDTH/8);

	loop_AXIMMvideo2Bytes_3planes:
		for (int y = 0; y < Height; y++)
		{
#pragma HLS loop_tripcount max=4320

#pragma HLS loop_flatten off
			for (int x = 0; x < (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8); x++)
			{
#pragma HLS pipeline II=1

				ap_uint<AXIMM_DATA_WIDTH> fb_pix = srcImg[offsetY + x];

				img.plane0 << fb_pix;
			}
			offsetY += StrideInBytes / (AXIMM_DATA_WIDTH / 8);

			if (NR_PLANES(VideoFormat) == 2 && (!(y & 1) || !IS_420(VideoFormat)))
			{
				for (int x = 0; x < (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8); x++)
				{
#pragma HLS pipeline II=1

					ap_uint<AXIMM_DATA_WIDTH> fb_pix = srcImg2[offsetUV + x];

					img.plane1 << fb_pix;
				}
				offsetUV += StrideInBytes / (AXIMM_DATA_WIDTH / 8);
			}
		}
	}
}
#endif

void Bytes2MultiPixStream(
	STREAM_PLANES& bytes,
	HSC_STREAM_MULTIPIX& img,
	const U16 Height,
	const U16 Width,
	const U16 WidthInBytes,
	const U16 StrideInBytes,
	const U8  VideoFormat)
{
	if ((VideoFormat == Y_U_V8_420) || (VideoFormat == R_G_B8))
	{
#if (MAX_NR_PLANES == 3)
#if ((HAS_Y_U_V8_420 == 1) || (HAS_R_G_B8 == 1) )
		//#pragma HLS inline off
		YUV_MULTI_PIXEL pix;
		const int rows = Height;
		const int cols = Width;
		const int videosize = rows*cols;
		//const int ratio = DMA_SIZE/AXIMM_DATA_WIDTH;
		//const int endindex = videosize/ratio%ratio;
		ap_uint<AXIMM_DATA_WIDTH> Yfifodata, Ufifodata, Vfifodata; // fix width!
		int xmodratio, uv_xmodratio;
		int ratio = AXIMM_DATA_WIDTH / AXIMM_DATA_WIDTH_420;
		ap_uint<AXIMM_DATA_WIDTH_420> y_fb_pix; // fix width!

		int uv_cnt = 0;
		int hor_cnt = 0;
		int ver_cnt = 0;

		if (VideoFormat == Y_U_V8_420)
		{
#if (HAS_Y_U_V8_420 == 1)
#if (HSC_SAMPLES_PER_CLOCK==1)
			ap_uint<AXIMM_DATA_WIDTH_420> u_fb_pix, v_fb_pix;
#else
			ap_uint<AXIMM_DATA_WIDTH_420 / 2> u_fb_pix, v_fb_pix;
#endif

			//for (int x = 0; x < (videosize+ratio-1)/ratio*ratio/HSC_SAMPLES_PER_CLOCK; x++)
			for (int x = 0; x < (videosize / HSC_SAMPLES_PER_CLOCK + ratio - 1) / ratio*ratio; x++)
			{
#pragma HLS pipeline II=1
				//#pragma HLS loop_flatten off

				xmodratio = (x&(ratio - 1));
				uv_xmodratio = (uv_cnt&(ratio * 2 - 1));

				if (xmodratio == 0)
					bytes.plane0 >> Yfifodata;

				if (uv_xmodratio == 0 && (ver_cnt & 1) == 0 && ver_cnt < rows)
				{
					bytes.plane1 >> Ufifodata;
					bytes.plane2 >> Vfifodata;
				}

				y_fb_pix = Yfifodata(AXIMM_DATA_WIDTH_420*(xmodratio + 1) - 1, AXIMM_DATA_WIDTH_420*xmodratio);

#if (HSC_SAMPLES_PER_CLOCK==1)
				if ((x & 1) == 0)
					u_fb_pix = Ufifodata(AXIMM_DATA_WIDTH_420*(uv_xmodratio / 2 + 1) - 1, AXIMM_DATA_WIDTH_420*(uv_xmodratio / 2));
				else
					v_fb_pix = Vfifodata(AXIMM_DATA_WIDTH_420*(uv_xmodratio / 2 + 1) - 1, AXIMM_DATA_WIDTH_420*(uv_xmodratio / 2));
#else
				u_fb_pix = Ufifodata(AXIMM_DATA_WIDTH_420 / 2 * (uv_xmodratio + 1) - 1, AXIMM_DATA_WIDTH_420 / 2 * uv_xmodratio);
				v_fb_pix = Vfifodata(AXIMM_DATA_WIDTH_420 / 2 * (uv_xmodratio + 1) - 1, AXIMM_DATA_WIDTH_420 / 2 * uv_xmodratio);
#endif

				for (int l = 0; l < HSC_SAMPLES_PER_CLOCK; l++)
				{
					pix.val[l * HSC_NR_COMPONENTS] = y_fb_pix(8 * l + HSC_BITS_PER_COMPONENT - 1, 8 * l);
					//pix.val[1 + l * 2] = u_fb_pix(8*(l+1)-1,8*l);
				}
#if (HSC_SAMPLES_PER_CLOCK==1)
				if ((x & 1) == 0)
					pix.val[1] = u_fb_pix;
				else
					pix.val[1] = v_fb_pix;
#elif (HSC_SAMPLES_PER_CLOCK==2)
				pix.val[1] = u_fb_pix;
				pix.val[4] = v_fb_pix;
#else
				pix.val[1] = u_fb_pix(7, 0);
				pix.val[4] = v_fb_pix(7, 0);
				pix.val[7] = u_fb_pix(15, 8);
				pix.val[10] = v_fb_pix(15, 8);
#endif
				//push pix to output stream
				if (x < videosize / HSC_SAMPLES_PER_CLOCK)
					img << pix;

				//update counters
				if ((ver_cnt & 1) == 0)
					uv_cnt++;

				if (hor_cnt == cols / HSC_SAMPLES_PER_CLOCK - 1)
				{
					hor_cnt = 0;
					ver_cnt++;
				}
				else
				{
					hor_cnt = hor_cnt + 1;
				}
			}
#endif
		}

		if (VideoFormat == R_G_B8)
		{
#if (HAS_R_G_B8 == 1)
			ap_uint<AXIMM_DATA_WIDTH_420> u_fb_pix, v_fb_pix;

			for (int x = 0; x < (videosize / HSC_SAMPLES_PER_CLOCK + ratio - 1) / ratio*ratio; x++)
			{
#pragma HLS pipeline II=1
				//#pragma HLS loop_flatten off

				xmodratio = (x&(ratio - 1));

				if (xmodratio == 0)
				{
					bytes.plane0 >> Yfifodata;
					bytes.plane1 >> Ufifodata;
					bytes.plane2 >> Vfifodata;
				}

				y_fb_pix = Yfifodata(AXIMM_DATA_WIDTH_420*(xmodratio + 1) - 1, AXIMM_DATA_WIDTH_420*xmodratio);
				u_fb_pix = Ufifodata(AXIMM_DATA_WIDTH_420*(xmodratio + 1) - 1, AXIMM_DATA_WIDTH_420*xmodratio);
				v_fb_pix = Vfifodata(AXIMM_DATA_WIDTH_420*(xmodratio + 1) - 1, AXIMM_DATA_WIDTH_420*xmodratio);

				for (int l = 0; l < HSC_SAMPLES_PER_CLOCK; l++)
				{
					pix.val[0 + (l * HSC_NR_COMPONENTS)] = y_fb_pix(8 * l + HSC_BITS_PER_COMPONENT - 1, 8 * l);
					pix.val[1 + (l * HSC_NR_COMPONENTS)] = u_fb_pix(8 * l + HSC_BITS_PER_COMPONENT - 1, 8 * l);
					pix.val[2 + (l * HSC_NR_COMPONENTS)] = v_fb_pix(8 * l + HSC_BITS_PER_COMPONENT - 1, 8 * l);
				}

				//push pix to output stream
				if (x < videosize / HSC_SAMPLES_PER_CLOCK)
					img << pix;

			}
#endif
		}
#endif
#endif
	}
	else
	{
		// Optimized implementations for every format
		if (VideoFormat == Y_UV8 || VideoFormat == Y_UV8_420)
		{

#if (HAS_Y_UV8_Y_UV8_420==1)
			// Optimized Y_UV8, Y_UV8_420 implementation
			int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);
			int widthInPix = WidthInBytes;
			int remainPix = widthInPix % (4 * (AXIMM_DATA_WIDTH / 32));
			remainPix = (remainPix == 0) ? (4 * (AXIMM_DATA_WIDTH / 32)) : remainPix;

		loop_Y_UV8_Y_UV8_420:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320

				YUV_MULTI_PIXEL pix;

#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=8

					ap_uint<AXIMM_DATA_WIDTH> rd;
					bytes.plane0 >> rd;
					ap_uint<AXIMM_DATA_WIDTH> rdUv;
					if (!(y & 1) || !IS_420(VideoFormat)) bytes.plane1 >> rdUv;

					for (int i = 0; i < 4 * (AXIMM_DATA_WIDTH / 32) / HSC_SAMPLES_PER_CLOCK; i++)
					{
						for (int k = 0; k < HSC_SAMPLES_PER_CLOCK; k++)
						{
							pix.val[0 + k * 3] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 8 + 7, (HSC_SAMPLES_PER_CLOCK*i + k) * 8) << (MAX_DATA_WIDTH - 8));
							pix.val[1 + k * 3] = (rdUv((HSC_SAMPLES_PER_CLOCK*i + k) * 8 + 7, (HSC_SAMPLES_PER_CLOCK*i + k) * 8) << (MAX_DATA_WIDTH - 8));
						}
						if (x < loopWidth - 1 || i < remainPix / HSC_SAMPLES_PER_CLOCK) img << pix;
					}
				}
			}
#endif
		}
		else if (VideoFormat == Y_UV10 || VideoFormat == Y_UV10_420)
		{
#if (HAS_Y_UV10_Y_UV10_420==1)
			// Optimized Y_UV10, Y_UV10_420 implementation
			int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);
			int widthInPix = Width;
			int remainPix = widthInPix % (3 * (AXIMM_DATA_WIDTH / 32));
			remainPix = (remainPix == 0) ? (3 * (AXIMM_DATA_WIDTH / 32)) : remainPix;

		loop_Y_UV10_Y_UV10_420:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320

				YUV_MULTI_PIXEL pix;

#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=6

					ap_uint<AXIMM_DATA_WIDTH> rd_raw;
					ap_uint<AXIMM_DATA_WIDTH> rd;
					bytes.plane0 >> rd_raw;
					for (int j = 0; j < (AXIMM_DATA_WIDTH / 32); j++)
					{
						rd((30 * j) + 29, 30 * j) = rd_raw((32 * j) + 29, 32 * j);
					}

					ap_uint<AXIMM_DATA_WIDTH> rdUv_raw;
					ap_uint<AXIMM_DATA_WIDTH> rdUv;
					if (!(y & 1) || !IS_420(VideoFormat))
					{
						bytes.plane1 >> rdUv_raw;
						for (int j = 0; j < (AXIMM_DATA_WIDTH / 32); j++)
						{
							rdUv((30 * j) + 29, 30 * j) = rdUv_raw((32 * j) + 29, 32 * j);
						}
					}

					for (int i = 0; i < 3 * (AXIMM_DATA_WIDTH / 32) / HSC_SAMPLES_PER_CLOCK; i++)
					{
						for (int k = 0; k < HSC_SAMPLES_PER_CLOCK; k++)
						{
							pix.val[0 + k * 3] = rd((HSC_SAMPLES_PER_CLOCK*i + k) * 10 + 9, (HSC_SAMPLES_PER_CLOCK*i + k) * 10);
							pix.val[1 + k * 3] = rdUv((HSC_SAMPLES_PER_CLOCK*i + k) * 10 + 9, (HSC_SAMPLES_PER_CLOCK*i + k) * 10);
						}
						if (x < loopWidth - 1 || i < remainPix / HSC_SAMPLES_PER_CLOCK) img << pix;
					}
				}
			}
#endif
		}
		else if (VideoFormat == YUYV8)
		{
#if (HAS_YUYV8==1)
			// Optimized YUYV8 implementation
			int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);
			int widthInPix = WidthInBytes / 2;
			int remainPix = widthInPix % (2 * (AXIMM_DATA_WIDTH / 32));
			remainPix = (remainPix == 0) ? (2 * (AXIMM_DATA_WIDTH / 32)) : remainPix;

		loop_YUYV8:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320

				YUV_MULTI_PIXEL pix;

#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=4

					ap_uint<AXIMM_DATA_WIDTH> rd;
					bytes.plane0 >> rd;

					for (int i = 0; i < 2 * (AXIMM_DATA_WIDTH / 32) / HSC_SAMPLES_PER_CLOCK; i++)
					{
						for (int k = 0; k < HSC_SAMPLES_PER_CLOCK; k++)
						{
							pix.val[0 + k * 3] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 16 + 7, (HSC_SAMPLES_PER_CLOCK*i + k) * 16) << (MAX_DATA_WIDTH - 8));
							pix.val[1 + k * 3] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 16 + 15, (HSC_SAMPLES_PER_CLOCK*i + k) * 16 + 8) << (MAX_DATA_WIDTH - 8));
						}
						if (x < loopWidth - 1 || i < remainPix / HSC_SAMPLES_PER_CLOCK) img << pix;
					}
				}
			}
#endif
		}
		else if (VideoFormat == UYVY8)
		{
#if (HAS_UYVY8==1)
			// Optimized UYVY8 implementation
			int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);
			int widthInPix = WidthInBytes / 2;
			int remainPix = widthInPix % (2 * (AXIMM_DATA_WIDTH / 32));
			remainPix = (remainPix == 0) ? (2 * (AXIMM_DATA_WIDTH / 32)) : remainPix;

		loop_UYVY8:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320

				YUV_MULTI_PIXEL pix;

#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=4

					ap_uint<AXIMM_DATA_WIDTH> rd;
					bytes.plane0 >> rd;

					for (int i = 0; i < 2 * (AXIMM_DATA_WIDTH / 32) / HSC_SAMPLES_PER_CLOCK; i++)
					{
						for (int k = 0; k < HSC_SAMPLES_PER_CLOCK; k++)
						{
							pix.val[1 + k * 3] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 16 + 7, (HSC_SAMPLES_PER_CLOCK*i + k) * 16) << (MAX_DATA_WIDTH - 8));
							pix.val[0 + k * 3] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 16 + 15, (HSC_SAMPLES_PER_CLOCK*i + k) * 16 + 8) << (MAX_DATA_WIDTH - 8));
						}
						if (x < loopWidth - 1 || i < remainPix / HSC_SAMPLES_PER_CLOCK) img << pix;
					}
				}
			}
#endif
		}
		else if (VideoFormat == RGB8 || VideoFormat == YUV8)
		{
#if (HAS_RGB8_YUV8==1)
			// Optimized RGB8, YUV8 implementation
			int loopWidth1 = ((WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8));
			int loopWidth = ((loopWidth1 + 2) / 3);
			int remainTrx = loopWidth1 % 3;
			int widthInPix = Width;
			int remainPix = widthInPix % (4 * (AXIMM_DATA_WIDTH / 32));
			remainPix = (remainPix == 0) ? (4 * (AXIMM_DATA_WIDTH / 32)) : remainPix;

		loop_RGB8_YUV8:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320

				ap_uint<AXIMM_DATA_WIDTH> rd0, rd1, rd2;
				YUV_MULTI_PIXEL pix;

#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=8

					ap_uint<AXIMM_DATA_WIDTH * 3> rd;
					bytes.plane0 >> rd0;
					rd(AXIMM_DATA_WIDTH - 1, 0) = rd0;
					if (x < loopWidth - 1 || remainTrx != 1) bytes.plane0 >> rd1;
					rd(2 * AXIMM_DATA_WIDTH - 1, AXIMM_DATA_WIDTH) = rd1;
					if (x < loopWidth - 1 || remainTrx == 0) bytes.plane0 >> rd2;
					rd(3 * AXIMM_DATA_WIDTH - 1, 2 * AXIMM_DATA_WIDTH) = rd2;

					for (int i = 0; i < 4 * (AXIMM_DATA_WIDTH / 32) / HSC_SAMPLES_PER_CLOCK; i++)
					{
						for (int k = 0; k < HSC_SAMPLES_PER_CLOCK; k++)
						{
							//pix.val[0+k*3] = ( rd((HSC_SAMPLES_PER_CLOCK*i+k)*24+7, (HSC_SAMPLES_PER_CLOCK*i+k)*24) << (MAX_DATA_WIDTH-8) );
							//pix.val[1+k*3] = ( rd((HSC_SAMPLES_PER_CLOCK*i+k)*24+15, (HSC_SAMPLES_PER_CLOCK*i+k)*24+8) << (MAX_DATA_WIDTH-8) );
							//pix.val[2+k*3] = ( rd((HSC_SAMPLES_PER_CLOCK*i+k)*24+23, (HSC_SAMPLES_PER_CLOCK*i+k)*24+16) << (MAX_DATA_WIDTH-8) );
							pix.val[0 + k * 3] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 24 + 7, (HSC_SAMPLES_PER_CLOCK*i + k) * 24));
							pix.val[1 + k * 3] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 24 + 15, (HSC_SAMPLES_PER_CLOCK*i + k) * 24 + 8));
							pix.val[2 + k * 3] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 24 + 23, (HSC_SAMPLES_PER_CLOCK*i + k) * 24 + 16));
							pix.val[0 + k * 3] = pix.val[0 + k * 3] << (MAX_DATA_WIDTH - 8);
							pix.val[1 + k * 3] = pix.val[1 + k * 3] << (MAX_DATA_WIDTH - 8);
							pix.val[2 + k * 3] = pix.val[2 + k * 3] << (MAX_DATA_WIDTH - 8);
						}
						if (x < loopWidth - 1 || i < remainPix / HSC_SAMPLES_PER_CLOCK) img << pix;
					}
				}
			}
#endif
		}
		else if (VideoFormat == BGR8)
		{
#if (HAS_BGR8==1)
			// Optimized BGR8 implementation
			int loopWidth1 = ((WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8));
			int loopWidth = ((loopWidth1 + 2) / 3);
			int remainTrx = loopWidth1 % 3;
			int widthInPix = Width;
			int remainPix = widthInPix % (4 * (AXIMM_DATA_WIDTH / 32));
			remainPix = (remainPix == 0) ? (4 * (AXIMM_DATA_WIDTH / 32)) : remainPix;

		loop_BGR8:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320

				ap_uint<AXIMM_DATA_WIDTH> rd0, rd1, rd2;
				YUV_MULTI_PIXEL pix;

#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=8

					ap_uint<AXIMM_DATA_WIDTH * 3> rd;
					bytes.plane0 >> rd0;
					rd(AXIMM_DATA_WIDTH - 1, 0) = rd0;
					if (x < loopWidth - 1 || remainTrx != 1) bytes.plane0 >> rd1;
					rd(2 * AXIMM_DATA_WIDTH - 1, AXIMM_DATA_WIDTH) = rd1;
					if (x < loopWidth - 1 || remainTrx == 0) bytes.plane0 >> rd2;
					rd(3 * AXIMM_DATA_WIDTH - 1, 2 * AXIMM_DATA_WIDTH) = rd2;

					for (int i = 0; i < 4 * (AXIMM_DATA_WIDTH / 32) / SAMPLES_PER_CLOCK; i++)
					{
						for (int k = 0; k < SAMPLES_PER_CLOCK; k++)
						{
							pix.val[2 + k * 3] = (rd((SAMPLES_PER_CLOCK*i + k) * 24 + 7, (SAMPLES_PER_CLOCK*i + k) * 24));
							pix.val[1 + k * 3] = (rd((SAMPLES_PER_CLOCK*i + k) * 24 + 15, (SAMPLES_PER_CLOCK*i + k) * 24 + 8));
							pix.val[0 + k * 3] = (rd((SAMPLES_PER_CLOCK*i + k) * 24 + 23, (SAMPLES_PER_CLOCK*i + k) * 24 + 16));
							pix.val[0 + k * 3] = pix.val[0 + k * 3] << (MAX_DATA_WIDTH - 8);
							pix.val[1 + k * 3] = pix.val[1 + k * 3] << (MAX_DATA_WIDTH - 8);
							pix.val[2 + k * 3] = pix.val[2 + k * 3] << (MAX_DATA_WIDTH - 8);
						}
						if (x < loopWidth - 1 || i < remainPix / SAMPLES_PER_CLOCK) img << pix;
					}
				}
			}
#endif
		}
		else if (VideoFormat == RGBX8 || VideoFormat == YUVX8)
		{
#if (HAS_RGBX8_YUVX8==1)
			// Optimized RGBX8, YUVX8 implementation
			int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);
			int widthInPix = WidthInBytes / 4;
			int remainPix = widthInPix % ((AXIMM_DATA_WIDTH / 32));
			remainPix = (remainPix == 0) ? ((AXIMM_DATA_WIDTH / 32)) : remainPix;

		loop_RGBX8_YUVX8:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320

				YUV_MULTI_PIXEL pix;

#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=2

					ap_uint<AXIMM_DATA_WIDTH> rd;
					bytes.plane0 >> rd;
					for (int i = 0; i < (AXIMM_DATA_WIDTH / 32) / HSC_SAMPLES_PER_CLOCK; i++)
					{
						for (int k = 0; k < HSC_SAMPLES_PER_CLOCK; k++)
						{
							pix.val[0 + k * 3] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 7, (HSC_SAMPLES_PER_CLOCK*i + k) * 32) << (MAX_DATA_WIDTH - 8));
							pix.val[1 + k * 3] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 15, (HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 8) << (MAX_DATA_WIDTH - 8));
							pix.val[2 + k * 3] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 23, (HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 16) << (MAX_DATA_WIDTH - 8));
						}
						if (x < loopWidth - 1 || i < remainPix / HSC_SAMPLES_PER_CLOCK) img << pix;

					}
				}
			}
#endif
		}
		else if (VideoFormat == RGBX10 || VideoFormat == YUVX10)
		{
#if (HAS_RGBX10_YUVX10==1)
			// Optimized RGBX10, YUVX10 implementation
			int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);
			int widthInPix = WidthInBytes / 4;
			int remainPix = widthInPix % ((AXIMM_DATA_WIDTH / 32));
			remainPix = (remainPix == 0) ? ((AXIMM_DATA_WIDTH / 32)) : remainPix;

		loop_RGBX10_YUVX10:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320

				YUV_MULTI_PIXEL pix;

#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=2

					ap_uint<AXIMM_DATA_WIDTH> rd;
					bytes.plane0 >> rd;
					for (int i = 0; i < (AXIMM_DATA_WIDTH / 32) / HSC_SAMPLES_PER_CLOCK; i++)
					{
						for (int k = 0; k < HSC_SAMPLES_PER_CLOCK; k++)
						{
							pix.val[0 + k * 3] = rd((HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 9, (HSC_SAMPLES_PER_CLOCK*i + k) * 32);
							pix.val[1 + k * 3] = rd((HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 19, (HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 10);
							pix.val[2 + k * 3] = rd((HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 29, (HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 20);
						}
						if (x < loopWidth - 1 || i < remainPix / HSC_SAMPLES_PER_CLOCK) img << pix;
					}
				}
			}
#endif
		}
		else if (VideoFormat == Y8)
		{
#if (HAS_Y8==1)
			// Optimized Y8 implementation
			int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);
			int widthInPix = WidthInBytes;
			int remainPix = widthInPix % (4 * (AXIMM_DATA_WIDTH / 32));
			remainPix = (remainPix == 0) ? (4 * (AXIMM_DATA_WIDTH / 32)) : remainPix;

		loop_Y8:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320

				YUV_MULTI_PIXEL pix;

#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=8

					ap_uint<AXIMM_DATA_WIDTH> rd;
					bytes.plane0 >> rd;

					for (int i = 0; i < 4 * (AXIMM_DATA_WIDTH / 32) / HSC_SAMPLES_PER_CLOCK; i++)
					{
						for (int k = 0; k < HSC_SAMPLES_PER_CLOCK; k++)
						{
							pix.val[0 + k * 3] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 8 + 7, (HSC_SAMPLES_PER_CLOCK*i + k) * 8) << (MAX_DATA_WIDTH - 8));
							pix.val[1 + k * 3] = (128 << (MAX_DATA_WIDTH - 8));
							pix.val[2 + k * 3] = (128 << (MAX_DATA_WIDTH - 8));
						}
						if (x < loopWidth - 1 || i < remainPix / HSC_SAMPLES_PER_CLOCK) img << pix;
					}
				}
			}
#endif
		}
		else if (VideoFormat == Y10)
		{
#if (HAS_Y10==1)
			// Optimized Y10 implementation
			int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);
			int widthInPix = Width;
			int remainPix = widthInPix % (3 * (AXIMM_DATA_WIDTH / 32));
			remainPix = (remainPix == 0) ? (3 * (AXIMM_DATA_WIDTH / 32)) : remainPix;

		loop_Y10:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320

				YUV_MULTI_PIXEL pix;

#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=6

					ap_uint<AXIMM_DATA_WIDTH> rd_raw;
					ap_uint<AXIMM_DATA_WIDTH> rd;
					bytes.plane0 >> rd_raw;
					for (int j = 0; j < (AXIMM_DATA_WIDTH / 32); j++)
					{
						rd((30 * j) + 29, 30 * j) = rd_raw((32 * j) + 29, 32 * j);
					}

					for (int i = 0; i < 3 * (AXIMM_DATA_WIDTH / 32) / HSC_SAMPLES_PER_CLOCK; i++)
					{
						for (int k = 0; k < HSC_SAMPLES_PER_CLOCK; k++)
						{
							pix.val[0 + k * 3] = rd((HSC_SAMPLES_PER_CLOCK*i + k) * 10 + 9, (HSC_SAMPLES_PER_CLOCK*i + k) * 10);
							pix.val[1 + k * 3] = 512;
							pix.val[2 + k * 3] = 512;
						}
						if (x < loopWidth - 1 || i < remainPix / HSC_SAMPLES_PER_CLOCK) img << pix;
					}
				}
			}
#endif
		}
		else if (VideoFormat == RGBA8 || VideoFormat == YUVA8)
		{
#if (HAS_RGBA8_YUVA8==1)
			// Optimized RGBA8, YUVA8 implementation
			int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);
			int widthInPix = WidthInBytes / 4;
			int remainPix = widthInPix % ((AXIMM_DATA_WIDTH / 32));
			remainPix = (remainPix == 0) ? ((AXIMM_DATA_WIDTH / 32)) : remainPix;

		loop_RGBA8_YUVA8:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320

				YUV_MULTI_PIXEL pix;

#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=2

					ap_uint<AXIMM_DATA_WIDTH> rd;
					bytes.plane0 >> rd;
					for (int i = 0; i < (AXIMM_DATA_WIDTH / 32) / HSC_SAMPLES_PER_CLOCK; i++)
					{
						for (int k = 0; k < HSC_SAMPLES_PER_CLOCK; k++)
						{
							pix.val[0 + k * 4] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 7, (HSC_SAMPLES_PER_CLOCK*i + k) * 32) << (MAX_DATA_WIDTH - 8));
							pix.val[1 + k * 4] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 15, (HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 8) << (MAX_DATA_WIDTH - 8));
							pix.val[2 + k * 4] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 23, (HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 16) << (MAX_DATA_WIDTH - 8));
							pix.val[3 + k * 4] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 31, (HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 24) << (MAX_DATA_WIDTH - 8));
						}
						if (x < loopWidth - 1 || i < remainPix / HSC_SAMPLES_PER_CLOCK) img << pix;
					}
				}
			}
#endif
		}
		else if (VideoFormat == BGRA8)
		{
#if (HAS_BGRA8==1)
			// Optimized BGRA8 implementation
			int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);
			int widthInPix = WidthInBytes / 4;
			int remainPix = widthInPix % ((AXIMM_DATA_WIDTH / 32));
			remainPix = (remainPix == 0) ? ((AXIMM_DATA_WIDTH / 32)) : remainPix;

		loop_BGRA8:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320

				YUV_MULTI_PIXEL pix;

#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=2

					ap_uint<AXIMM_DATA_WIDTH> rd;
					bytes.plane0 >> rd;
					for (int i = 0; i < (AXIMM_DATA_WIDTH / 32) / HSC_SAMPLES_PER_CLOCK; i++)
					{
						for (int k = 0; k < HSC_SAMPLES_PER_CLOCK; k++)
						{
							pix.val[0 + k * 4] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 23, (HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 16) << (MAX_DATA_WIDTH - 8));
							pix.val[1 + k * 4] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 15, (HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 8) << (MAX_DATA_WIDTH - 8));
							pix.val[2 + k * 4] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 7, (HSC_SAMPLES_PER_CLOCK*i + k) * 32) << (MAX_DATA_WIDTH - 8));
							pix.val[3 + k * 4] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 31, (HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 24) << (MAX_DATA_WIDTH - 8));
						}
						if (x < loopWidth - 1 || i < remainPix / HSC_SAMPLES_PER_CLOCK) img << pix;
					}
				}
			}
#endif
		}
		else if (VideoFormat == BGRX8)
		{
#if (HAS_BGRX8==1)
			// Optimized BGRX8 implementation
			int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);
			int widthInPix = WidthInBytes / 4;
			int remainPix = widthInPix % ((AXIMM_DATA_WIDTH / 32));
			remainPix = (remainPix == 0) ? ((AXIMM_DATA_WIDTH / 32)) : remainPix;

		loop_BGRX8:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320

				YUV_MULTI_PIXEL pix;

#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=2

					ap_uint<AXIMM_DATA_WIDTH> rd;
					bytes.plane0 >> rd;
					for (int i = 0; i < (AXIMM_DATA_WIDTH / 32) / HSC_SAMPLES_PER_CLOCK; i++)
					{
						for (int k = 0; k < HSC_SAMPLES_PER_CLOCK; k++)
						{
							pix.val[0 + k * 3] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 23, (HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 16) << (MAX_DATA_WIDTH - 8));
							pix.val[1 + k * 3] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 15, (HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 8) << (MAX_DATA_WIDTH - 8));
							pix.val[2 + k * 3] = (rd((HSC_SAMPLES_PER_CLOCK*i + k) * 32 + 7, (HSC_SAMPLES_PER_CLOCK*i + k) * 32) << (MAX_DATA_WIDTH - 8));
						}
						if (x < loopWidth - 1 || i < remainPix / HSC_SAMPLES_PER_CLOCK) img << pix;
					}
				}
			}
#endif
		}
	}
}

void MultiPixStream2Bytes(HSC_STREAM_MULTIPIX& StrmMPix,
	STREAM_PLANES& bytes,
	U16 Height,
	U16 WidthInPix,
	U16 WidthInBytes,
	U16 StrideInBytes,
	U8 VideoFormat
)
{
#if (dbg == 1)
	int temp = 0;
#endif

	if ((VideoFormat == Y_U_V8_420) || (VideoFormat == R_G_B8))
	{
#if (MAX_NR_PLANES == 3)
#if ((HAS_Y_U_V8_420 == 1) || (HAS_R_G_B8 == 1))
		YUV_MULTI_PIXEL pix;
		const int rows = Height;
		const int cols = WidthInPix;
		const int videosize = rows*cols;
		//const int endindex = videosize/4%4;
		ap_uint<AXIMM_DATA_WIDTH> Yfifodata, Ufifodata, Vfifodata; // fix width!
		int xmodratio, uv_xmodratio;

		int ratio = AXIMM_DATA_WIDTH / AXIMM_DATA_WIDTH_420;
		int loopWidth = (videosize / HSC_SAMPLES_PER_CLOCK + ratio - 1) / ratio*ratio;
		int Yrem = videosize % (AXIMM_DATA_WIDTH / 8);

		ap_uint<AXIMM_DATA_WIDTH_420> y_fb_pix; // fix width!

		if (VideoFormat == Y_U_V8_420)
		{
#if (HAS_Y_U_V8_420 == 1)
			int UVrem = (videosize / 4) % (AXIMM_DATA_WIDTH / 8);
#if (HSC_SAMPLES_PER_CLOCK==1)
			ap_uint<AXIMM_DATA_WIDTH_420> u_fb_pix, v_fb_pix;
#else
			ap_uint<AXIMM_DATA_WIDTH_420 / 2> u_fb_pix, v_fb_pix;
#endif

			int uv_cnt = 0;
			int hor_cnt = 0;
			int ver_cnt = 0;

			for (int x = 0; x < loopWidth; x++)
			{
#pragma HLS pipeline II=1
				//#pragma HLS loop_flatten off

				xmodratio = (x&(ratio - 1));
				uv_xmodratio = (uv_cnt&(ratio * 2 - 1));

				//pop pix from input stream
				if (x < videosize / HSC_SAMPLES_PER_CLOCK)
					StrmMPix >> pix;

				for (int l = 0; l < HSC_SAMPLES_PER_CLOCK; l++)
				{
					y_fb_pix(8 * l + HSC_BITS_PER_COMPONENT - 1, 8 * l) = pix.val[l * HSC_NR_COMPONENTS];
					//pix.val[1 + l * 2] = u_fb_pix(8*(l+1)-1,8*l);
				}
#if (HSC_SAMPLES_PER_CLOCK==1)
				if ((x & 1) == 0)
					u_fb_pix = pix.val[1];
				else
					v_fb_pix = pix.val[1];
#elif (HSC_SAMPLES_PER_CLOCK==2)
				u_fb_pix = pix.val[1];
				v_fb_pix = pix.val[4];
#else
				u_fb_pix(7, 0) = pix.val[1];
				v_fb_pix(7, 0) = pix.val[4];
				u_fb_pix(15, 8) = pix.val[7];
				v_fb_pix(15, 8) = pix.val[10];
#endif

				Yfifodata(AXIMM_DATA_WIDTH_420*(xmodratio + 1) - 1, AXIMM_DATA_WIDTH_420*xmodratio) = y_fb_pix;

#if (HSC_SAMPLES_PER_CLOCK==1)
				if ((x & 1) == 0)
					Ufifodata(AXIMM_DATA_WIDTH_420*(uv_xmodratio / 2 + 1) - 1, AXIMM_DATA_WIDTH_420*(uv_xmodratio / 2)) = u_fb_pix;
				else
					Vfifodata(AXIMM_DATA_WIDTH_420*(uv_xmodratio / 2 + 1) - 1, AXIMM_DATA_WIDTH_420*(uv_xmodratio / 2)) = v_fb_pix;
#else
				Ufifodata(AXIMM_DATA_WIDTH_420 / 2 * (uv_xmodratio + 1) - 1, AXIMM_DATA_WIDTH_420 / 2 * uv_xmodratio) = u_fb_pix;
				Vfifodata(AXIMM_DATA_WIDTH_420 / 2 * (uv_xmodratio + 1) - 1, AXIMM_DATA_WIDTH_420 / 2 * uv_xmodratio) = v_fb_pix;
#endif

				if (xmodratio == ratio - 1)
					bytes.plane0 << Yfifodata;

				if (uv_xmodratio == ratio * 2 - 1 && (ver_cnt & 1) == 0)
				{
					bytes.plane1 << Ufifodata;
					bytes.plane2 << Vfifodata;
				}

				//update counters
				if ((ver_cnt & 1) == 0)
					uv_cnt++;

				if (hor_cnt == cols / HSC_SAMPLES_PER_CLOCK - 1)
				{
					hor_cnt = 0;
					ver_cnt++;
				}
				else
				{
					hor_cnt = hor_cnt + 1;
				}
			}
			//output remaining pixels
			if (Yrem != 0)
			{
				bytes.plane0 << Yfifodata;
			}

			if (UVrem != 0)
			{
				bytes.plane1 << Ufifodata;
				bytes.plane2 << Vfifodata;
			}
#endif
		}

		if (VideoFormat == R_G_B8)
			//ColorMode == rgb
		{
#if (HAS_R_G_B8 == 1)
			int UVrem = videosize % (AXIMM_DATA_WIDTH / 8);
			ap_uint<AXIMM_DATA_WIDTH_420> u_fb_pix, v_fb_pix;

			int uv_cnt = 0;
			int hor_cnt = 0;
			int ver_cnt = 0;

			for (int x = 0; x < loopWidth; x++)
			{
#pragma HLS pipeline II=1
				//#pragma HLS loop_flatten off

				xmodratio = (x&(ratio - 1));

				//pop pix from input stream
				if (x < videosize / HSC_SAMPLES_PER_CLOCK)
					StrmMPix >> pix;

				for (int l = 0; l < HSC_SAMPLES_PER_CLOCK; l++)
				{
					y_fb_pix(8 * l + HSC_BITS_PER_COMPONENT - 1, 8 * l) = pix.val[0 + (l * HSC_NR_COMPONENTS)];
					u_fb_pix(8 * l + HSC_BITS_PER_COMPONENT - 1, 8 * l) = pix.val[1 + (l * HSC_NR_COMPONENTS)];
					v_fb_pix(8 * l + HSC_BITS_PER_COMPONENT - 1, 8 * l) = pix.val[2 + (l * HSC_NR_COMPONENTS)];
				}

				Yfifodata(AXIMM_DATA_WIDTH_420*(xmodratio + 1) - 1, AXIMM_DATA_WIDTH_420*xmodratio) = y_fb_pix;
				Ufifodata(AXIMM_DATA_WIDTH_420*(xmodratio + 1) - 1, AXIMM_DATA_WIDTH_420*xmodratio) = u_fb_pix;
				Vfifodata(AXIMM_DATA_WIDTH_420*(xmodratio + 1) - 1, AXIMM_DATA_WIDTH_420*xmodratio) = v_fb_pix;

				if (xmodratio == ratio - 1)
				{
					bytes.plane0 << Yfifodata;
					bytes.plane1 << Ufifodata;
					bytes.plane2 << Vfifodata;
				}
			}
			//output remaining pixels
			if (Yrem != 0)
			{
				bytes.plane0 << Yfifodata;
				bytes.plane1 << Ufifodata;
				bytes.plane2 << Vfifodata;
			}

#if(dbg == 1)
			if (temp < 10)

			{
				printf("In MultiPixStream2Bytes: ##############################################################\n");

				printf("Yfifodata = 0x%llx,", (Yfifodata.to_int()));
				printf("Ufifodata = 0x%llx,", (Ufifodata.to_int()));
				printf("Vfifodata = 0x%llx\n", (Vfifodata.to_int()));
				temp = temp + 1;

				printf("Exit MultiPixStream2Bytes: ##############################################################\n");
			}
#endif

#endif
		}
#endif
#endif
	}
	else
	{
		YUV_MULTI_PIXEL pix;

		if (VideoFormat == RGBX8 || VideoFormat == YUVX8)
		{
#if (HAS_RGBX8_YUVX8==1)
			int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);
			int widthInPix = WidthInBytes / 4;
			int remainPix = widthInPix % (AXIMM_DATA_WIDTH / 32);
			remainPix = (remainPix == 0) ? (AXIMM_DATA_WIDTH / (32 * HSC_SAMPLES_PER_CLOCK)) : (remainPix / HSC_SAMPLES_PER_CLOCK);

			ap_uint<24> in_pix = 0;
			ap_uint<AXIMM_DATA_WIDTH> out_pix = 0;

		loop_RGBX8_YUVX8:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320
#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=4
					for (int i = 0; i < (AXIMM_DATA_WIDTH / (32 * HSC_SAMPLES_PER_CLOCK)); i++)
					{
						if (x < loopWidth - 1 || i < remainPix) StrmMPix >> pix;
						for (int l = 0; l < HSC_SAMPLES_PER_CLOCK; l++)
						{
							for (int k = 0; k < NR_COMPONENTS; k++)
							{
								in_pix((k + 1) * 8 - 1, k * 8) = pix.val[k + l * NR_COMPONENTS] >> (MAX_DATA_WIDTH - 8);
							}
							out_pix((32 * HSC_SAMPLES_PER_CLOCK*i) + (l * 32) + 23, (32 * HSC_SAMPLES_PER_CLOCK*i) + (l * 32)) = in_pix;
						}
					}
					bytes.plane0 << out_pix;
#if (dbg == 1)
					printf("out_pix = 0x%llx,", (out_pix.to_int()));
#endif
				}
			}
#endif
		}
		else if (VideoFormat == BGRX8)
		{
#if (HAS_BGRX8==1)
			int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);
			int widthInPix = WidthInBytes / 4;
			int remainPix = widthInPix % (AXIMM_DATA_WIDTH / 32);
			remainPix = (remainPix == 0) ? (AXIMM_DATA_WIDTH / (32 * HSC_SAMPLES_PER_CLOCK)) : (remainPix / HSC_SAMPLES_PER_CLOCK);

			ap_uint<24> in_pix = 0;
			ap_uint<AXIMM_DATA_WIDTH> out_pix = 0;

		loop_BGRX8:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320
#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=4
					for (int i = 0; i < (AXIMM_DATA_WIDTH / (32 * HSC_SAMPLES_PER_CLOCK)); i++)
					{
						if (x < loopWidth - 1 || i < remainPix) StrmMPix >> pix;
						for (int l = 0; l < HSC_SAMPLES_PER_CLOCK; l++)
						{
							in_pix(7, 0) = pix.val[2 + l * NR_COMPONENTS] >> (MAX_DATA_WIDTH - 8);
							in_pix(15, 8) = pix.val[1 + l * NR_COMPONENTS] >> (MAX_DATA_WIDTH - 8);
							in_pix(23, 16) = pix.val[0 + l * NR_COMPONENTS] >> (MAX_DATA_WIDTH - 8);
							out_pix((32 * HSC_SAMPLES_PER_CLOCK*i) + (l * 32) + 23, (32 * HSC_SAMPLES_PER_CLOCK*i) + (l * 32)) = in_pix;
						}
					}
					bytes.plane0 << out_pix;
				}
			}
#endif
		}
		else if (VideoFormat == RGBX10 || VideoFormat == YUVX10)
		{
#if (HAS_RGBX10_YUVX10==1)
			int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);
			int widthInPix = WidthInBytes / 4;
			int remainPix = widthInPix % (AXIMM_DATA_WIDTH / 32);
			remainPix = (remainPix == 0) ? (AXIMM_DATA_WIDTH / (32 * HSC_SAMPLES_PER_CLOCK)) : (remainPix / HSC_SAMPLES_PER_CLOCK);

			ap_uint<30> in_pix = 0;
			ap_uint<AXIMM_DATA_WIDTH> out_pix = 0;

		loop_RGBX10_YUVX10:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320
#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=4
					for (int i = 0; i < (AXIMM_DATA_WIDTH / (32 * HSC_SAMPLES_PER_CLOCK)); i++)
					{
						if (x < loopWidth - 1 || i < remainPix) StrmMPix >> pix;
						for (int l = 0; l < HSC_SAMPLES_PER_CLOCK; l++)
						{
							for (int k = 0; k < NR_COMPONENTS; k++)
							{
								in_pix((k + 1) * 10 - 1, k * 10) = pix.val[k + l * NR_COMPONENTS];
							}
							out_pix((32 * HSC_SAMPLES_PER_CLOCK*i) + (l * 32) + 29, (32 * HSC_SAMPLES_PER_CLOCK*i) + (l * 32)) = in_pix;
						}
					}
					bytes.plane0 << out_pix;
				}
			}
#endif
		}
		else if (VideoFormat == YUYV8)
		{
#if (HAS_YUYV8==1)
			int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);
			int widthInPix = WidthInBytes / 2;
			int remainPix = widthInPix % (AXIMM_DATA_WIDTH / 16);
			remainPix = (remainPix == 0) ? (AXIMM_DATA_WIDTH / (16 * HSC_SAMPLES_PER_CLOCK)) : (remainPix / HSC_SAMPLES_PER_CLOCK);

			ap_uint<16 * HSC_SAMPLES_PER_CLOCK> in_pix = 0;
			ap_uint<AXIMM_DATA_WIDTH> out_pix = 0;

		loop_YUYV8:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320
#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=4
					for (int i = 0; i < (AXIMM_DATA_WIDTH / (16 * HSC_SAMPLES_PER_CLOCK)); i++)
					{
						if (x < loopWidth - 1 || i < remainPix) StrmMPix >> pix;
						for (int l = 0; l < HSC_SAMPLES_PER_CLOCK; l++)
						{
							for (int k = 0; k < 2; k++)
							{
								in_pix(16 * l + (k + 1) * 8 - 1, 16 * l + k * 8) = pix.val[k + l * NR_COMPONENTS] >> (MAX_DATA_WIDTH - 8);
							}
						}
						out_pix((16 * HSC_SAMPLES_PER_CLOCK*(i + 1)) - 1, (16 * HSC_SAMPLES_PER_CLOCK*i)) = in_pix;
					}
					bytes.plane0 << out_pix;
				}
			}
#endif
		}
		else if (VideoFormat == UYVY8)
		{
#if (HAS_UYVY8==1)
			int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);
			int widthInPix = WidthInBytes / 2;
			int remainPix = widthInPix % (AXIMM_DATA_WIDTH / 16);
			remainPix = (remainPix == 0) ? (AXIMM_DATA_WIDTH / (16 * HSC_SAMPLES_PER_CLOCK)) : (remainPix / HSC_SAMPLES_PER_CLOCK);

			ap_uint<16 * HSC_SAMPLES_PER_CLOCK> in_pix = 0;
			ap_uint<AXIMM_DATA_WIDTH> out_pix = 0;

		loop_UYVY8:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320
#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=4
					for (int i = 0; i < (AXIMM_DATA_WIDTH / (16 * HSC_SAMPLES_PER_CLOCK)); i++)
					{
						if (x < loopWidth - 1 || i < remainPix) StrmMPix >> pix;
						for (int l = 0; l < HSC_SAMPLES_PER_CLOCK; l++)
						{
							in_pix(16 * l + 7, 16 * l) = pix.val[1 + l * NR_COMPONENTS] >> (MAX_DATA_WIDTH - 8);
							in_pix(16 * l + 15, 16 * l + 8) = pix.val[0 + l * NR_COMPONENTS] >> (MAX_DATA_WIDTH - 8);
						}
						out_pix((16 * HSC_SAMPLES_PER_CLOCK*(i + 1)) - 1, (16 * HSC_SAMPLES_PER_CLOCK*i)) = in_pix;
					}
					bytes.plane0 << out_pix;
				}
			}
#endif
		}
		else if (VideoFormat == Y_UV8 || VideoFormat == Y_UV8_420)
		{
#if (HAS_Y_UV8_Y_UV8_420==1)
			int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);
			int widthInPix = WidthInBytes;
			int remainPix = widthInPix % (AXIMM_DATA_WIDTH / 8);
			remainPix = (remainPix == 0) ? (AXIMM_DATA_WIDTH / (8 * HSC_SAMPLES_PER_CLOCK)) : (remainPix / HSC_SAMPLES_PER_CLOCK);

			ap_uint<8 * HSC_SAMPLES_PER_CLOCK> in_pix_Y = 0, in_pix_UV = 0;
			ap_uint<AXIMM_DATA_WIDTH> out_pix_Y = 0, out_pix_UV = 0;

		loop_Y_UV8_Y_UV8_420:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320
#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=8
					for (int i = 0; i < (AXIMM_DATA_WIDTH / (8 * HSC_SAMPLES_PER_CLOCK)); i++)
					{
						if (x < loopWidth - 1 || i < remainPix) StrmMPix >> pix;
						for (int l = 0; l < HSC_SAMPLES_PER_CLOCK; l++)
						{
							in_pix_Y(8 * (l + 1) - 1, 8 * l) = pix.val[0 + l * NR_COMPONENTS] >> (MAX_DATA_WIDTH - 8);
							in_pix_UV(8 * (l + 1) - 1, 8 * l) = pix.val[1 + l * NR_COMPONENTS] >> (MAX_DATA_WIDTH - 8);
						}
						out_pix_Y((8 * HSC_SAMPLES_PER_CLOCK*(i + 1)) - 1, (8 * HSC_SAMPLES_PER_CLOCK*i)) = in_pix_Y;
						out_pix_UV((8 * HSC_SAMPLES_PER_CLOCK*(i + 1)) - 1, (8 * HSC_SAMPLES_PER_CLOCK*i)) = in_pix_UV;
					}
					bytes.plane0 << out_pix_Y;
					if (!(y & 1) || !IS_420(VideoFormat))
						bytes.plane1 << out_pix_UV;
				}
			}
#endif
		}
		else if (VideoFormat == Y_UV10 || VideoFormat == Y_UV10_420)
		{
#if (HAS_Y_UV10_Y_UV10_420==1)
			//assumes AXIMM_DATA_WIDTH contains a multiple of HSC_SAMPLES_PER_CLOCK pixels (for example, 2 pix/clk needs at least 64 bits)
			int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);
			int widthInPix = WidthInPix;
			int remainPix = widthInPix % ((AXIMM_DATA_WIDTH * 3) / 32);
			remainPix = (remainPix == 0) ? ((AXIMM_DATA_WIDTH * 3) / (32 * HSC_SAMPLES_PER_CLOCK)) : (remainPix / HSC_SAMPLES_PER_CLOCK);

			ap_uint<10 * HSC_SAMPLES_PER_CLOCK> in_pix_Y = 0, in_pix_UV = 0;
			ap_uint<AXIMM_DATA_WIDTH> raw_pix_Y = 0, raw_pix_UV = 0;
			ap_uint<AXIMM_DATA_WIDTH> out_pix_Y = 0, out_pix_UV = 0;

		loop_Y_UV10_Y_UV10_420:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320
#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=6
					for (int i = 0; i < ((AXIMM_DATA_WIDTH * 3) / (32 * HSC_SAMPLES_PER_CLOCK)); i++)
					{
						if (x < loopWidth - 1 || i < remainPix) StrmMPix >> pix;
						for (int l = 0; l < HSC_SAMPLES_PER_CLOCK; l++)
						{
							in_pix_Y(10 * (l + 1) - 1, 10 * l) = pix.val[0 + l * NR_COMPONENTS];
							in_pix_UV(10 * (l + 1) - 1, 10 * l) = pix.val[1 + l * NR_COMPONENTS];
						}
						raw_pix_Y((10 * HSC_SAMPLES_PER_CLOCK*(i + 1)) - 1, (10 * HSC_SAMPLES_PER_CLOCK*i)) = in_pix_Y;
						raw_pix_UV((10 * HSC_SAMPLES_PER_CLOCK*(i + 1)) - 1, (10 * HSC_SAMPLES_PER_CLOCK*i)) = in_pix_UV;
					}
					for (int j = 0; j < (AXIMM_DATA_WIDTH / 32); j++)
					{
						out_pix_Y((32 * j) + 29, 32 * j) = raw_pix_Y((30 * j) + 29, 30 * j);
						out_pix_UV((32 * j) + 29, 32 * j) = raw_pix_UV((30 * j) + 29, 30 * j);
					}
					bytes.plane0 << out_pix_Y;
					if (!(y & 1) || !IS_420(VideoFormat))
						bytes.plane1 << out_pix_UV;
				}
			}
#endif
		}
		else if (VideoFormat == RGB8 || VideoFormat == YUV8)
		{
#if (HAS_RGB8_YUV8==1)
			//int loopWidth = ((WidthInBytes+((AXIMM_DATA_WIDTH*3)/8)-1)/((AXIMM_DATA_WIDTH*3)/8));
			int loopWidth1 = ((WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8));
			int loopWidth = ((loopWidth1 + 2) / 3);
			int widthInPix = WidthInPix;    //WidthInBytes/3
			int remPix = widthInPix % ((3 * AXIMM_DATA_WIDTH) / 24);
			int remainPix = (remPix == 0) ? ((AXIMM_DATA_WIDTH * 3) / (24 * HSC_SAMPLES_PER_CLOCK)) : (remPix / HSC_SAMPLES_PER_CLOCK);
			int remainTrx = (remPix == 0) ? 3 : ((remPix * 24) + (AXIMM_DATA_WIDTH - 1)) / AXIMM_DATA_WIDTH;

			ap_uint<24 * HSC_SAMPLES_PER_CLOCK> in_pix = 0;
			ap_uint<AXIMM_DATA_WIDTH * 3> out_pix = 0;

		loop_RGB8_YUV8:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320
#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=8
					for (int i = 0; i < ((AXIMM_DATA_WIDTH * 3) / (24 * HSC_SAMPLES_PER_CLOCK)); i++)
					{
						if (x < loopWidth - 1 || i < remainPix) StrmMPix >> pix;
						for (int l = 0; l < HSC_SAMPLES_PER_CLOCK; l++)
						{
							for (int k = 0; k < NR_COMPONENTS; k++)
							{
								in_pix(24 * l + (k + 1) * 8 - 1, 24 * l + k * 8) = pix.val[k + l * NR_COMPONENTS] >> (MAX_DATA_WIDTH - 8);
							}
						}
						out_pix((24 * HSC_SAMPLES_PER_CLOCK*(i + 1)) - 1, (24 * HSC_SAMPLES_PER_CLOCK*i)) = in_pix;
					}
					for (int j = 0; j < 3; j++)
					{
						if (x < loopWidth - 1 || j < remainTrx) bytes.plane0 << out_pix(AXIMM_DATA_WIDTH*(j + 1) - 1, AXIMM_DATA_WIDTH*j);
					}
				}
			}
#endif
		}
		else if (VideoFormat == BGR8)
		{
#if (HAS_BGR8==1)
			int loopWidth1 = ((WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8));
			int loopWidth = ((loopWidth1 + 2) / 3);
			int widthInPix = WidthInPix;    //WidthInBytes/3
			int remPix = widthInPix % ((3 * AXIMM_DATA_WIDTH) / 24);
			int remainPix = (remPix == 0) ? ((AXIMM_DATA_WIDTH * 3) / (24 * HSC_SAMPLES_PER_CLOCK)) : (remPix / HSC_SAMPLES_PER_CLOCK);
			int remainTrx = (remPix == 0) ? 3 : ((remPix * 24) + (AXIMM_DATA_WIDTH - 1)) / AXIMM_DATA_WIDTH;

			ap_uint<24 * HSC_SAMPLES_PER_CLOCK> in_pix = 0;
			ap_uint<AXIMM_DATA_WIDTH * 3> out_pix = 0;

		loop_BGR8:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320
#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=8
					for (int i = 0; i < ((AXIMM_DATA_WIDTH * 3) / (24 * HSC_SAMPLES_PER_CLOCK)); i++)
					{
						if (x < loopWidth - 1 || i < remainPix) StrmMPix >> pix;
						for (int l = 0; l < HSC_SAMPLES_PER_CLOCK; l++)
						{
							for (int k = 0; k < NR_COMPONENTS; k++)
							{
								in_pix(24 * l + (k + 1) * 8 - 1, 24 * l + k * 8) = pix.val[(2 - k) + l * NR_COMPONENTS] >> (MAX_DATA_WIDTH - 8);
							}
						}
						out_pix((24 * HSC_SAMPLES_PER_CLOCK*(i + 1)) - 1, (24 * HSC_SAMPLES_PER_CLOCK*i)) = in_pix;
					}
					for (int j = 0; j < 3; j++)
					{
						if (x < loopWidth - 1 || j < remainTrx) bytes.plane0 << out_pix(AXIMM_DATA_WIDTH*(j + 1) - 1, AXIMM_DATA_WIDTH*j);
					}
				}
			}
#endif
		}
		else if (VideoFormat == Y8)
		{
#if (HAS_Y8==1)
			int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);
			int widthInPix = WidthInBytes;
			int remainPix = widthInPix % (AXIMM_DATA_WIDTH / 8);
			remainPix = (remainPix == 0) ? (AXIMM_DATA_WIDTH / (8 * HSC_SAMPLES_PER_CLOCK)) : (remainPix / HSC_SAMPLES_PER_CLOCK);

			ap_uint<8 * HSC_SAMPLES_PER_CLOCK> in_pix = 0;
			ap_uint<AXIMM_DATA_WIDTH> out_pix;

		loop_Y8:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320
#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=8
					for (int i = 0; i < (AXIMM_DATA_WIDTH / (8 * HSC_SAMPLES_PER_CLOCK)); i++)
					{
						if (x < loopWidth - 1 || i < remainPix) StrmMPix >> pix;
						for (int l = 0; l < HSC_SAMPLES_PER_CLOCK; l++)
						{
							in_pix(8 * (l + 1) - 1, 8 * l) = pix.val[0 + l * HSC_NR_COMPONENTS] >> (MAX_DATA_WIDTH - 8);
						}
						out_pix((8 * HSC_SAMPLES_PER_CLOCK*(i + 1)) - 1, (8 * HSC_SAMPLES_PER_CLOCK*i)) = in_pix;
					}
					bytes.plane0 << out_pix;
				}
			}
#endif
		}
		else if (VideoFormat == Y10)
		{
#if (HAS_Y10==1)
			//assumes AXIMM_DATA_WIDTH contains a multiple of HSC_SAMPLES_PER_CLOCK pixels (for example, 2 pix/clk needs at least 64 bits)
			int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);
			int widthInPix = WidthInPix;
			int remainPix = widthInPix % ((AXIMM_DATA_WIDTH * 3) / 32);
			remainPix = (remainPix == 0) ? ((AXIMM_DATA_WIDTH * 3) / (32 * HSC_SAMPLES_PER_CLOCK)) : (remainPix / HSC_SAMPLES_PER_CLOCK);

			ap_uint<10 * HSC_SAMPLES_PER_CLOCK> in_pix = 0;
			ap_uint<AXIMM_DATA_WIDTH> raw_pix = 0;
			ap_uint<AXIMM_DATA_WIDTH> out_pix = 0;

		loop_Y10:
			for (int y = 0; y < Height; y++)
			{
#pragma HLS loop_tripcount max=4320
#pragma HLS loop_flatten off
				for (int x = 0; x < loopWidth; x++)
				{
#pragma HLS pipeline II=6
					for (int i = 0; i < ((AXIMM_DATA_WIDTH * 3) / (32 * HSC_SAMPLES_PER_CLOCK)); i++)
					{
						if (x < loopWidth - 1 || i < remainPix) StrmMPix >> pix;
						for (int l = 0; l < HSC_SAMPLES_PER_CLOCK; l++)
						{
							in_pix(10 * (l + 1) - 1, 10 * l) = pix.val[0 + l * NR_COMPONENTS];
						}
						raw_pix((10 * HSC_SAMPLES_PER_CLOCK*(i + 1)) - 1, (10 * HSC_SAMPLES_PER_CLOCK*i)) = in_pix;
					}
					for (int j = 0; j < (AXIMM_DATA_WIDTH / 32); j++)
					{
						out_pix((32 * j) + 29, 32 * j) = raw_pix((30 * j) + 29, 30 * j);
					}
					bytes.plane0 << out_pix;
				}
			}
#endif
		}
	}
}

#if (MAX_NR_PLANES == 1)
void Bytes2AXIMMvideo(
	STREAM_PLANES& bytes,
	AXIMM dstImg,
	AXIMM dstImg2,
	U16 Height,
	U16 WidthInBytes,
	U16 StrideInBytes,
	U8 VideoFormat)
{
	int loopWidth = (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8);

	ap_uint<AXIMM_DATA_WIDTH> mem_pix;

	int yoffset = 0;
loop_Bytes2AXIMMvideo_1plane:
	for (int y = 0; y < Height; y++)
	{
#pragma HLS loop_tripcount max=4320
#pragma HLS loop_flatten off
		for (int x = 0; x < loopWidth; x++)
		{
#pragma HLS pipeline II=1
			bytes.plane0 >> mem_pix;
			dstImg[yoffset + x] = mem_pix;
		}
		yoffset += StrideInBytes / (AXIMM_DATA_WIDTH / 8);
	}
}
#endif

#if(MAX_NR_PLANES == 2)
void Bytes2AXIMMvideo(
	STREAM_PLANES& bytes,
	AXIMM dstImg,
	AXIMM dstImg2,
#if (MAX_NR_PLANES == 3)
	AXIMM dstImg3,
#endif
	U16 Height,
#if (MAX_NR_PLANES == 3)
	U16 WidthOut,
#endif
	U16 WidthInBytes,
	U16 StrideInBytes,
	U8 VideoFormat)
{
	int offsetY = 0;
	int offsetUV = 0; //(Height)*(StrideInBytes/(AXIMM_DATA_WIDTH/8));

	ap_uint<AXIMM_DATA_WIDTH> fb_pix;

loop_Bytes2AXIMMvideo_2planes:
	for (int y = 0; y < Height; y++)
	{
#pragma HLS loop_tripcount max=4320
#pragma HLS loop_flatten off
		for (int x = 0; x < (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8); x++)
		{
#pragma HLS pipeline II=1
			bytes.plane0 >> fb_pix;
			dstImg[offsetY + x] = fb_pix;
		}
		offsetY += StrideInBytes / (AXIMM_DATA_WIDTH / 8);

		if (NR_PLANES(VideoFormat) == 2 && (!(y & 1) || !IS_420(VideoFormat)))
		{
			for (int x = 0; x < (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8); x++)
			{
#pragma HLS pipeline II=1
				bytes.plane1 >> fb_pix;
				dstImg2[offsetUV + x] = fb_pix;
			}
			offsetUV += StrideInBytes / (AXIMM_DATA_WIDTH / 8);
		}
	}
}
#endif

#if(MAX_NR_PLANES == 3)
void Bytes2AXIMMvideo(
	STREAM_PLANES& bytes,
	AXIMM dstImg,
	AXIMM dstImg2,
#if (MAX_NR_PLANES == 3)
	AXIMM dstImg3,
#endif
	U16 Height,
#if (MAX_NR_PLANES == 3)
	U16 WidthOut,
#endif
	U16 WidthInBytes,
	U16 StrideInBytes,
	U8 VideoFormat)
{
#if(dbg == 1)
	printf("In Bytes2AXIMMvideo begin: ##############################################################\n");
#endif
	if ((VideoFormat == Y_U_V8_420) || (VideoFormat == R_G_B8))
	{
#if(MAX_NR_PLANES == 3)
#if ((HAS_Y_U_V8_420 == 1) || (HAS_R_G_B8 == 1))

#if(dbg == 1)
		printf("In Line 1611 of Bytes2AXIMMvideo ##############################################################\n");
#endif
		ap_uint<AXIMM_DATA_WIDTH> *yBase, *uBase, *vBase;
		yBase = dstImg;
		uBase = dstImg2;
		vBase = dstImg3;

#if(dbg == 1)
		printf("In Line 1616 of Bytes2AXIMMvideo ##############################################################\n");
#endif
		const U16 rows = Height;
		const U16 cols = WidthOut;
		const int videosize = rows*cols;
		ap_uint<AXIMM_DATA_WIDTH> y, u, v; // fix width!
		ap_uint<AXIMM_DATA_WIDTH> U0, V0;
		ap_uint<AXIMM_DATA_WIDTH> Yout, Uout;
#if(dbg == 1)
		printf("In Line 1623 of Bytes2AXIMMvideo ##############################################################\n");
#endif
		int ratio = AXIMM_DATA_WIDTH / 8;
		int YnumTsf = (videosize + ratio - 1) / ratio;
		int YpixRem = videosize % ratio;
		int numBstInt = YnumTsf / AXIMM_BURST_LENGTH;
		int YremTsf = YnumTsf%AXIMM_BURST_LENGTH;
#if(dbg == 1)
		printf("In Line 1629 of Bytes2AXIMMvideo ##############################################################\n");
#endif

		for (int i = 0; i < numBstInt; i++)
		{
			//#pragma HLS pipeline II=1
			//#pragma HLS loop_flatten off
			int Yoffset = i*AXIMM_BURST_LENGTH;
			for (int j = 0;j < AXIMM_BURST_LENGTH;j++) {
#pragma HLS pipeline II=1
				bytes.plane0 >> y;
				yBase[Yoffset + j] = y;
#if(dbg == 1)
				printf("In Line 1640 of Bytes2AXIMMvideo ##############################################################\n");
#endif

			}
			if (VideoFormat == Y_U_V8_420)
			{
				int UVoffset = i / 4 * AXIMM_BURST_LENGTH;
				if (i % 4 == 3)
				{
					for (int j = 0;j < AXIMM_BURST_LENGTH;j++) {
#pragma HLS pipeline II=1
						bytes.plane1 >> u;
						uBase[UVoffset + j] = u;
						if (UVoffset == 0 && j == 0) U0 = u;
#if(dbg == 1)
						printf("In Line 1653 of Bytes2AXIMMvideo ##############################################################\n");
#endif

					}

					for (int j = 0;j < AXIMM_BURST_LENGTH;j++) {
#pragma HLS pipeline II=1
						bytes.plane2 >> v;
						vBase[UVoffset + j] = v;
						if (UVoffset == 0 && j == 0) V0 = v;
#if(dbg == 1)
						printf("In Line 1662 of Bytes2AXIMMvideo ##############################################################\n");
#endif

					}
				}
			}

			if (VideoFormat == R_G_B8)
				// ColorMode==rgb
			{
				int UVoffset = i*AXIMM_BURST_LENGTH;
				for (int j = 0;j < AXIMM_BURST_LENGTH;j++) {
#pragma HLS pipeline II=1
					bytes.plane1 >> u;
					uBase[UVoffset + j] = u;
					if (UVoffset == 0 && j == 0) U0 = u;

#if(dbg == 1)
					printf("In Line 1678 of Bytes2AXIMMvideo ##############################################################\n");
#endif

				}

				for (int j = 0;j < AXIMM_BURST_LENGTH;j++) {
#pragma HLS pipeline II=1
					bytes.plane2 >> v;
					vBase[UVoffset + j] = v;
					if (UVoffset == 0 && j == 0) V0 = v;
#if(dbg == 1)
					printf("In Line 1687 of Bytes2AXIMMvideo ##############################################################\n");
#endif

				}
			}

		}

		int Yoffset = numBstInt*AXIMM_BURST_LENGTH;
		for (int k = 0;k < YremTsf;k++) {
#pragma HLS pipeline II=1
			bytes.plane0 >> y;
#if(dbg == 1)
			printf("In Bytes2AXIMMvideo after exec:bytes.plane0 >> y ##############################################################\n");

			printf("y = 0x%llx,", (y.to_int()));
#endif

			if ((k == YremTsf - 1) && (YpixRem != 0))
			{
				Yout(AXIMM_DATA_WIDTH - 1, AXIMM_DATA_WIDTH - (YpixRem * 8)) = U0(AXIMM_DATA_WIDTH - 1, YpixRem * 8);
				Yout(AXIMM_DATA_WIDTH - (YpixRem * 8) - 1, 0) = y(YpixRem * 8 - 1, 0);
			}
			else
			{
				Yout = y;
			}
			yBase[Yoffset + k] = Yout;
#if(dbg == 1)
			printf("In Line 1711 of Bytes2AXIMMvideo ##############################################################\n");
#endif
		}
		if (VideoFormat == Y_U_V8_420)
		{
			int UVnumTsf = (videosize / 4 + ratio - 1) / ratio;
			int UpixRem = (videosize + (videosize / 4)) % ratio;
			int UremTsf = UVnumTsf%AXIMM_BURST_LENGTH;
			int VremTsf = UVnumTsf%AXIMM_BURST_LENGTH;
			int UVoffset = numBstInt / 4 * AXIMM_BURST_LENGTH;

			for (int k = 0;k < UremTsf;k++) {
#pragma HLS pipeline II=1
				bytes.plane1 >> u;
#if(dbg == 1)
				printf("In Line 1725 of Bytes2AXIMMvideo ##############################################################\n");
#endif

				if ((k == UremTsf - 1) && (UpixRem != 0))
				{
					Uout(AXIMM_DATA_WIDTH - 1, AXIMM_DATA_WIDTH - (UpixRem * 8)) = V0(AXIMM_DATA_WIDTH - 1, UpixRem * 8);
					Uout(AXIMM_DATA_WIDTH - (UpixRem * 8) - 1, 0) = u(UpixRem * 8 - 1, 0);
				}
				else
				{
					Uout = u;
				}
				uBase[UVoffset + k] = Uout;
#if(dbg == 1)
				printf("In Line 1737 of Bytes2AXIMMvideo ##############################################################\n");
#endif

			}
			for (int k = 0;k < VremTsf;k++) {
#pragma HLS pipeline II=1
				bytes.plane2 >> v;
				vBase[UVoffset + k] = v;
#if(dbg == 1)
				printf("In Line 1744 of Bytes2AXIMMvideo ##############################################################\n");
#endif

			}
		}

		if (VideoFormat == R_G_B8)
			//ColorMode==rgb
		{
			int UVnumTsf = (videosize + ratio - 1) / ratio;
			int UpixRem = (videosize + videosize) % ratio;
			int UremTsf = UVnumTsf%AXIMM_BURST_LENGTH;
			int VremTsf = UVnumTsf%AXIMM_BURST_LENGTH;
			int UVoffset = numBstInt*AXIMM_BURST_LENGTH;
#if(dbg == 1)
			printf("In Line 1757 of Bytes2AXIMMvideo ##############################################################\n");
#endif
			for (int k = 0;k < UremTsf;k++) {
#pragma HLS pipeline II=1
				bytes.plane1 >> u;
#if(dbg == 1)
				printf("v = 0x%llx,", (u.to_int()));
#endif

				if ((k == UremTsf - 1) && (UpixRem != 0))
				{
					Uout(AXIMM_DATA_WIDTH - 1, AXIMM_DATA_WIDTH - (UpixRem * 8)) = V0(AXIMM_DATA_WIDTH - 1, UpixRem * 8);
					Uout(AXIMM_DATA_WIDTH - (UpixRem * 8) - 1, 0) = u(UpixRem * 8 - 1, 0);
				}
				else
				{
					Uout = u;
				}
				uBase[UVoffset + k] = Uout;
			}
			for (int k = 0;k < VremTsf;k++) {
#pragma HLS pipeline II=1
				bytes.plane2 >> v;
#if(dbg == 1)
				printf("v = 0x%llx,", (v.to_int()));
				printf("Exiting Bytes2AXIMMvideo: ##############################################################\n");
#endif
				vBase[UVoffset + k] = v;

			}
		}
#endif
#endif
	}
	else
	{
		int offsetY = 0;
		int offsetUV = 0; //(Height)*(StrideInBytes/(AXIMM_DATA_WIDTH/8));

		ap_uint<AXIMM_DATA_WIDTH> fb_pix;

	loop_Bytes2AXIMMvideo_3planes:
		for (int y = 0; y < Height; y++)
		{
#pragma HLS loop_tripcount max=4320
#pragma HLS loop_flatten off
			for (int x = 0; x < (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8); x++)
			{
#pragma HLS pipeline II=1
				bytes.plane0 >> fb_pix;
				dstImg[offsetY + x] = fb_pix;
			}
			offsetY += StrideInBytes / (AXIMM_DATA_WIDTH / 8);

			if (NR_PLANES(VideoFormat) == 2 && (!(y & 1) || !IS_420(VideoFormat)))
			{
				for (int x = 0; x < (WidthInBytes + (AXIMM_DATA_WIDTH / 8) - 1) / (AXIMM_DATA_WIDTH / 8); x++)
				{
#pragma HLS pipeline II=1
					bytes.plane1 >> fb_pix;
					dstImg2[offsetUV + x] = fb_pix;
				}
				offsetUV += StrideInBytes / (AXIMM_DATA_WIDTH / 8);
			}
		}

	}
}
#endif
