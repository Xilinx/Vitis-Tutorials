/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "my_defines.h"
#include "histogram.h"

const int IMG_DEPTH = MAX_WIDTH*MAX_HEIGHT;
const int CDF_DEPTH = GRAY_LEVELS;

/* ######################################################################### */

// computes the Cumulative Density Function cdf[] from the Histrogram hist[]
void compute_cdf(uint19_t hist[GRAY_LEVELS], uint25_t cdf[GRAY_LEVELS])
{
  uint8_t i, k;

 CDF_L1:for (k = 0; k < GRAY_LEVELS-1; k++)
 {
   uint25_t tmp_cdf=0;
   CDF_L2:for (i = 0; i<=k; i++)
   {
     #pragma HLS LOOP_TRIPCOUNT  min=1 max=256 avg=256
     // cdf for current picture from past picture histogram
     tmp_cdf = tmp_cdf + hist[i];
   }
   cdf[k] = tmp_cdf;
 }

  return;

}



/* ######################################################################### */

void top_histogram_equaliz( uint11_t width, uint11_t height,
			    uint25_t cdf[GRAY_LEVELS],
//RGB_t inp_img[MAX_WIDTH*MAX_HEIGHT], RGB_t out_img[MAX_WIDTH*MAX_HEIGHT])
			    uint8_t inp_R[MAX_WIDTH*MAX_HEIGHT],
			    uint8_t inp_G[MAX_WIDTH*MAX_HEIGHT],
			    uint8_t inp_B[MAX_WIDTH*MAX_HEIGHT],
			    uint8_t out_R[MAX_WIDTH*MAX_HEIGHT],
			    uint8_t out_G[MAX_WIDTH*MAX_HEIGHT],
			    uint8_t out_B[MAX_WIDTH*MAX_HEIGHT])
{
#pragma HLS INTERFACE mode=ap_fifo depth=IMG_DEPTH port=inp_R
#pragma HLS INTERFACE mode=ap_fifo depth=IMG_DEPTH port=inp_G
#pragma HLS INTERFACE mode=ap_fifo depth=IMG_DEPTH port=inp_B
#pragma HLS INTERFACE mode=ap_fifo depth=IMG_DEPTH port=out_R
#pragma HLS INTERFACE mode=ap_fifo depth=IMG_DEPTH port=out_G
#pragma HLS INTERFACE mode=ap_fifo depth=IMG_DEPTH port=out_B
#pragma HLS INTERFACE mode=ap_fifo depth=CDF_DEPTH port=cdf

  int i;
  uint11_t  x, y;
  uint8_t   Y, U, V;
  uint8_t   cdf_temp;
  uint25_t  cdf_mult, gain;
  uint32_t acc = 0;
  uint8_t  old, val;
  //from rgb2yuv()
  uint8_t   R, G, B;
  const int9_t	Wrgb[3][3] = {
    { 66, 129,  25},
    {-38, -74, 112},
    {122, -94, -18},
  };
  //from yuv2rgb()
  int9_t    C, D, E;
  const int11_t	Wyuv[3][3] = {
    {298,    0,  409},
    {298, -100, -208},
    {298,  516,    0}
  };

 Ly:for (y=0; y<height; y++)
 {
#pragma HLS LOOP_TRIPCOUNT min=480 max=1080 avg=720

 Lx:for (x=0; x<width; x++)
 {
#pragma HLS PIPELINE
#pragma HLS LOOP_TRIPCOUNT min=640 max=1920 avg=1280

   R = inp_R[y*MAX_WIDTH +x];
   G = inp_G[y*MAX_WIDTH +x];
   B = inp_B[y*MAX_WIDTH +x];
   short int tmp_Y = ( Wrgb[0][0] * R + Wrgb[0][1] * G + Wrgb[0][2] * B + 128) >> 8;
   short int tmp_U = ( Wrgb[1][0] * R + Wrgb[1][1] * G + Wrgb[1][2] * B + 128) >> 8;
   short int tmp_V = ( Wrgb[2][0] * R + Wrgb[2][1] * G + Wrgb[2][2] * B + 128) >> 8;
   tmp_Y = tmp_Y +  16;
   tmp_U = tmp_U + 128;
   tmp_V = tmp_V + 128;
   Y = CLIP(tmp_Y);
   U = CLIP(tmp_U);
   V = CLIP(tmp_V);

   // histogram equalization
   int numer   = ( (cdf[Y]-1) *(GRAY_LEVELS-1));
   int denomin = (height * width -1);
   cdf_mult = numer / denomin;
   // clipping (due to the approssimation done above)
   if (cdf_mult >= (GRAY_LEVELS-1))
     cdf_temp = (GRAY_LEVELS-1);
   else if (cdf_mult <= 0 )
     cdf_temp = 0;

   cdf_temp = (uint8_t) cdf_mult;
   Y = cdf_temp;

   // YUV to RGB conversion of current image I(n)
   C = Y - 16;
   D = U - 128;
   E = V - 128;
   short int tmp_R = (Wyuv[0][0] * C                  + Wyuv[0][2] * E + 128);
   short int tmp_G = (Wyuv[1][0] * C + Wyuv[1][1] * D + Wyuv[1][2] * E + 128);
   short int tmp_B = (Wyuv[2][0] * C + Wyuv[2][1] * D                  + 128);
   tmp_B = tmp_B >> 8;
   tmp_G = tmp_G >> 8;
   tmp_R = tmp_R >> 8;
   R = CLIP( tmp_R );
   G = CLIP( tmp_G );
   B = CLIP( tmp_B );
   out_R[y*MAX_WIDTH +x] = R;
   out_G[y*MAX_WIDTH +x] = G;
   out_B[y*MAX_WIDTH +x] = B;

 } // end of Lx loop
 } // end of Ly loop

}


/* ######################################################################### */

void wrapper_histogram_equaliz( uint11_t width, uint11_t height,
				uint19_t hist[GRAY_LEVELS],
				uint8_t inp_R[MAX_WIDTH*MAX_HEIGHT],
				uint8_t inp_G[MAX_WIDTH*MAX_HEIGHT],
				uint8_t inp_B[MAX_WIDTH*MAX_HEIGHT],
				uint8_t out_R[MAX_WIDTH*MAX_HEIGHT],
				uint8_t out_G[MAX_WIDTH*MAX_HEIGHT],
				uint8_t out_B[MAX_WIDTH*MAX_HEIGHT])
//RGB_t inp_img[MAX_WIDTH*MAX_HEIGHT], RGB_t out_img[MAX_WIDTH*MAX_HEIGHT])
{

  uint25_t cdf[GRAY_LEVELS];

  // compute the  CDF of current image I(n) from histogram of previous image I(n-1)
  compute_cdf(hist, cdf);

  //Histogram Equalization of current image I(n)
  top_histogram_equaliz(width, height, cdf, inp_R,inp_G,inp_B, out_R,out_G,out_B);

  return;

}
