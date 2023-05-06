/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "my_defines.h"
#include "ref_histogram.h"


/* ######################################################################### */

// computes the Cumulative Density Function cdf[] from the Histrogram hist[]
void ref_compute_cdf(uint19_t hist[GRAY_LEVELS], uint25_t cdf[GRAY_LEVELS])
{
	uint8_t i, k;

	CDF_L1:	for (k = 0; k < GRAY_LEVELS-1; k++)
	{
		uint25_t tmp_cdf=0;
		CDF_L2: for (i = 0; i<=k; i++)
		{
			tmp_cdf = tmp_cdf + hist[i];
		}
		cdf[k] = tmp_cdf;
	}

	return;

}

/* ######################################################################### */

// Computes the histogram of Y plane
void ref_compute_hist( uint11_t width, uint11_t height, uint19_t hist[GRAY_LEVELS], RGB_t inp_img[MAX_WIDTH*MAX_HEIGHT])
{

	int i;
	uint11_t  x, y;
	uint8_t   Y, U, V;


	Ly: for (y=0; y<height; y++)
	{
		Lx: for (x=0; x<width; x++)
		{
			Y = inp_img[y*MAX_WIDTH +x].R;
			U = inp_img[y*MAX_WIDTH +x].G;
			V = inp_img[y*MAX_WIDTH +x].B;
			hist[Y] = hist[Y] + 1;
		} // end of Lx loop
	} // end of Ly loop
}


/* ######################################################################### */

void ref_yuv2rgb( uint11_t width, uint11_t height, RGB_t inp_img[MAX_WIDTH*MAX_HEIGHT], RGB_t out_img[MAX_WIDTH*MAX_HEIGHT])
{

	int i;
	uint11_t  x, y;
	int9_t    R, G, B, Y, U, V;
	int9_t    C, D, E;

	const int11_t	Wyuv[3][3] = {
			{298,    0,  409},
			{298, -100, -208},
			{298,  516,    0}
	};

	Ly: for (y=0; y<height; y++)
	{
		Lx: for (x=0; x<width; x++)
		{
			Y = inp_img[y*MAX_WIDTH +x].R; // in the input image Y is stored into the R channel
			U = inp_img[y*MAX_WIDTH +x].G;
			V = inp_img[y*MAX_WIDTH +x].B;
			// YUV to RGB conversion of current image I(n)
			C = Y - 16;
			D = U - 128;
			E = V - 128;
			short int tmp_R = ( Wyuv[0][0] * C                  + Wyuv[0][2] * E + 128);
			short int tmp_G = ( Wyuv[1][0] * C + Wyuv[1][1] * D + Wyuv[1][2] * E + 128);
			short int tmp_B = ( Wyuv[2][0] * C + Wyuv[2][1] * D                  + 128);
			tmp_R = tmp_R >> 8;
			tmp_G = tmp_G >> 8;
			tmp_B = tmp_B >> 8;
			R = CLIP( tmp_R);
			G = CLIP( tmp_G);
			B = CLIP( tmp_B);
			out_img[y*MAX_WIDTH +x].R = R;
			out_img[y*MAX_WIDTH +x].G = G;
			out_img[y*MAX_WIDTH +x].B = B;

		} // end of Lx loop
	} // end of Ly loop
}


/* ######################################################################### */

void ref_rgb2yuv( uint11_t width, uint11_t height, RGB_t inp_img[MAX_WIDTH*MAX_HEIGHT], RGB_t out_img[MAX_WIDTH*MAX_HEIGHT])
{
	int i;
	uint11_t  x, y;
	int9_t   R, G, B, Y, U, V;

	const int9_t	Wrgb[3][3] = {
			{ 66, 129,  25},
			{-38, -74, 112},
			{122, -94, -18},
	};

	Ly: for (y=0; y<height; y++)
	{
		Lx: for (x=0; x<width; x++)
		{
			R = inp_img[y*MAX_WIDTH +x].R;
			G = inp_img[y*MAX_WIDTH +x].G;
			B = inp_img[y*MAX_WIDTH +x].B;
			short int tmp_Y = ( Wrgb[0][0] * R + Wrgb[0][1] * G + Wrgb[0][2] * B + 128);
			short int tmp_U = ( Wrgb[1][0] * R + Wrgb[1][1] * G + Wrgb[1][2] * B + 128);
			short int tmp_V = ( Wrgb[2][0] * R + Wrgb[2][1] * G + Wrgb[2][2] * B + 128);
			tmp_Y = ( tmp_Y >> 8) +  16;
			tmp_U = ( tmp_U >> 8) + 128;
			tmp_V = ( tmp_V >> 8) + 128;
			Y = CLIP(tmp_Y);
			U = CLIP(tmp_U);
			V = CLIP(tmp_V);
			out_img[y*MAX_WIDTH +x].R = Y;
			out_img[y*MAX_WIDTH +x].G = U;
			out_img[y*MAX_WIDTH +x].B = V;

		} // end of Lx loop
	} // end of Ly loop

}

/* ######################################################################### */

void ref_equalization( uint11_t width, uint11_t height, uint25_t cdf[GRAY_LEVELS],
		RGB_t inp_img[MAX_WIDTH*MAX_HEIGHT], RGB_t out_img[MAX_WIDTH*MAX_HEIGHT])
{

	int i;
	uint11_t  x, y;
	uint8_t   Y, U, V;

	Ly: for (y=0; y<height; y++)
	{
		Lx: for (x=0; x<width; x++)
		{
			Y = inp_img[y*MAX_WIDTH +x].R;
			U = inp_img[y*MAX_WIDTH +x].G;
			V = inp_img[y*MAX_WIDTH +x].B;

			// histogram equalization via CDF
			/*
			float gain = (float) (GRAY_LEVELS-1) / ((float) (height * width -1));
			float cdf_mult = (cdf[Y]-1);
			cdf_mult = cdf_mult*gain;
			int cdf_int = cdf_mult; //truncation
			*/
			int cdf_int =  (cdf[Y]-1);
			cdf_int = (cdf_int*(GRAY_LEVELS-1))/(height * width -1);
			// clipping
			if (cdf_int >= (GRAY_LEVELS-1))
				cdf_int = (GRAY_LEVELS-1);
			else if (cdf_int <= 0 )
				cdf_int = 0;
			Y = (uint8_t) cdf_int;
			out_img[y*MAX_WIDTH +x].R = Y;
			out_img[y*MAX_WIDTH +x].G = U;
			out_img[y*MAX_WIDTH +x].B = V;

		} // end of Lx loop
	} // end of Ly loop

}


/* ######################################################################### */

void ref_img_hist_equaliz( uint11_t width, uint11_t height, uint19_t hist[GRAY_LEVELS],
		RGB_t inp_img[MAX_WIDTH*MAX_HEIGHT], RGB_t out_img[MAX_WIDTH*MAX_HEIGHT])
{
	uint25_t cdf[GRAY_LEVELS];

	RGB_t *yuv = (RGB_t *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(RGB_t));
	RGB_t *yeq = (RGB_t *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(RGB_t));

	// RGB to YUV conversion of current image I(n)
	ref_rgb2yuv(width, height, inp_img, yuv);

	// compute histogram of current image I(n)
	//// commented to avoid overwriting the reference histogram pre-computed in MATLAB
	////ref_compute_hist(width, height, hist, yuv);

	//compute CDF
	ref_compute_cdf(hist, cdf);

	// equalize the current image I(n)
	ref_equalization(width, height, cdf, yuv, yeq);

	// YUV to RGB conversion of current image I(n)
	ref_yuv2rgb(width, height, yeq, out_img);

	free(yuv);
	free(yeq);

}
