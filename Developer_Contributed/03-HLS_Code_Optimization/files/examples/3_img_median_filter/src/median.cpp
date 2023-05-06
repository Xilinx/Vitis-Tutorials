/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*/

#include <stdio.h>
#include <stdlib.h>
#include "median.h"



pix_t sorting_network(pix_t window[KMED*KMED])
{
//#pragma HLS PIPELINE II=1
#pragma HLS INLINE

	int const N=KMED*KMED;
	pix_t t[N];
	pix_t z[N];

	pix_t pixel;
	pix_t med_val;
	char i, k, stage;


for (i=0; i<KMED*KMED; i++)
	{
		z[i] = window[i];
	}

	//sorting_network<N>(z);
L2: for (stage = 1; stage <= N; stage++)
	{
//#pragma HLS LOOP_TRIPCOUNT min=1 max=9

		if ((stage%2)==1)
			k=0;
		if ((stage%2)==0)
			k=1;

L3:for (i = k; i<N-1; i=i+2)
	{
//#pragma HLS LOOP_TRIPCOUNT min=1 max=4
		t[(i  )] = MIN(z[(i)], z[(i+1)]);
		t[(i+1)] = MAX(z[(i)], z[(i+1)]);
		z[(i  )] = t[(i  )];
		z[(i+1)] = t[(i+1)];
	}

	}

	med_val = z[N/2]; // the median value is in location N/2+1,
	//but in C the address starts from 0, so decrease by 1 again

	pixel = med_val;


#ifdef DEBUG_MEDIAN
	for (i=0; i<KMED*KMED; i++)
		printf("%3d ",z[i]);
	printf("\n");
#endif

	return pixel;

}



//Main function for Median Filtering
//This function includes a line buffer for a streaming implementation
void top_median(pix_t in_pix[MAX_HEIGHT][MAX_WIDTH],
	pix_t out_pix[MAX_HEIGHT][MAX_WIDTH],
	short height, short width) //, FILE *fp)
{

#pragma HLS INTERFACE ap_none port=width
#pragma HLS INTERFACE ap_none port=height
#pragma HLS INTERFACE ap_fifo port=out_pix

#pragma HLS INTERFACE ap_fifo port=in_pix



	short int row;
	short int  col;
	static int  cnt=0;
	pix_t med, pix;

	pix_t pixel[KMED];

	static pix_t line_buffer[KMED][MAX_WIDTH];
#pragma HLS ARRAY_PARTITION variable=line_buffer complete dim=1

	pix_t window[KMED*KMED];
#pragma HLS ARRAY_PARTITION variable=window complete dim=1


L1: for(row = 0; row < height+KKMED; row++)
	{
	#pragma HLS LOOP_TRIPCOUNT min=600 max=1080 avg=720
	L2: for(col = 0; col < width+KKMED; col++)
	{
	#pragma HLS LOOP_TRIPCOUNT min=800 max=1920 avg=1280


		// Line Buffer fill
	if(col < width)
		for(int ii = 0; ii < KMED-1; ii++)
		{
			pixel[ii] = line_buffer[ii][col] = line_buffer[ii+1][col];
		}

		//There is an offset to accomodate the active pixel region
		if((col < width) && (row < height)){
			pix = in_pix[row][col];
			pixel[KMED-1] = line_buffer[KMED-1][col] = pix;
		}


		//Shift right the processing window to make room for the new column
		for(int ii = 0; ii < KMED; ii++)
			for(int jj = 0; jj < KMED-1; jj++)
				window[ii*KMED+jj] = window[ii*KMED+jj+1];

		//	  if(col < width)
		for(int ii = 0; ii < KMED; ii++)
			window[ii*KMED+KMED-1] = pixel[ii];

		//This design assumes there are no edges on the boundary of the image
		if ( (row >= KKMED) & (col >= KKMED) & (row<(height-KKMED))  & (col<(width-KKMED)) )
		{
			med = sorting_network(window);
		}
		else
		{
		 med = 0;
		}


		//The output image is offset from the input to account for the line buffer
		if ((row >= KKMED) & (col >= KKMED) )
		{
			pix_t output_pixel;
			output_pixel = med;

			out_pix[row-KKMED][col-KKMED] = output_pixel;
		}



	}
	}


}




void ref_median(pix_t in_pix[MAX_HEIGHT][MAX_WIDTH],
	pix_t ref_pix[MAX_HEIGHT][MAX_WIDTH],
	short height, short width) //, FILE *fp)
{
	short int row;
	short int  col;
	static int  cnt=0;
	pix_t pix, med;
	signed char x,y;

	pix_t window[KMED*KMED];

	L1: for(row = 0; row < height+KKMED; row++)
	{
#pragma HLS LOOP_TRIPCOUNT min=600 max=1080 avg=720
		L2: for(col = 0; col < width+KKMED; col++)
		{
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=800 max=1920 avg=1280

			//This design assumes there are no edges on the boundary of the image
			if ( (row >= KMED-1) & (col >= KMED-1) &  (row < height)  & (col< width) )
			{
				   L3:for (y=-KMED+1; y<=0; y++)
				   //L3:for (y=-2; y<=0; y++)
				   {

					   //#pragma HLS LOOP_TRIPCOUNT min=3 max=3
					   L4: for (x=-KMED+1; x<=0; x++)
					   //L4: for (x=-2; x<=0; x++)
					   {
						//#pragma HLS LOOP_TRIPCOUNT min=3 max=3

						   window[(KMED-1+y)*KMED+(KMED-1+x)] = in_pix[row+y][col+x];
						   //printf("%3d ", window[(KMED-1+y)*KMED+(KMED-1+x)]);
					   }
					   //printf("\n");
				   }
				   pix = sorting_network(window);
				   ref_pix[row-KKMED][col-KKMED] = pix;

			}

		} // end of L2
	} // end of L1


}
