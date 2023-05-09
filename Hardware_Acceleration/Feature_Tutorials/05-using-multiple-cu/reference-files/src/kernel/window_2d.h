/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#pragma once

#define HLS_NO_XIL_FPO_LIB
#include "ap_int.h"
#include "hls_stream.h"
#include "hls_video_mem.h"

template<unsigned MAX_LINE_SIZE, unsigned KERNEL_H_SIZE, unsigned KERNEL_V_SIZE, typename T>
class Window2D {

  public:

	Window2D(unsigned short width, unsigned short height) 
	{
#ifndef __SYNTHESIS__
		assert(width<=MAX_LINE_SIZE);
		assert(width>KERNEL_H_SIZE);
		assert(height>KERNEL_V_SIZE);
#endif	
		mWidth = width;
		mHeight = height;
	}		
	
	bool next(hls::stream<T>& srcImg, unsigned x, unsigned y)
	{
#pragma HLS inline	
	    T pix;
    	T pixBuf[KERNEL_V_SIZE]; //local storage for line buffer column (to avoid multiple read clients on BRAM)
#pragma HLS ARRAY_PARTITION variable=pixBuf      complete dim=0

    	// Get pixel from only active region of image, shift line buffer up, insert new pixel at bottom
    	if(x<mWidth)
    	{
        	mLineBuffer.shift_up(x);

        	if(y<mHeight)
        	{
            	srcImg >> pix;
            	mLineBuffer.insert_bottom_row(pix, x);
        	}
    	}

    	// Get a column out of the line buffer, shift the kernel window left, insert column on the right
    	mLineBuffer.get_col(pixBuf, x);
    	mPixelWindow.shift_pixels_left();
    	mPixelWindow.insert_right_col(pixBuf);

		// Clamp pixels to 0 when outside of image 
		for(int row=0; row<KERNEL_V_SIZE; row++)
		{
			for(int col=0; col<KERNEL_H_SIZE; col++)
			{
				int xoffset = x+col-KERNEL_H_SIZE+1;
				int yoffset = y+row-KERNEL_V_SIZE+1;
				
				if ( (xoffset<0) || (xoffset>=mWidth) ||
					 (yoffset<0) || (yoffset>=mHeight) ) {
					mPixelWindow.insert_pixel(0, row, col);
				}				
			}
		}		
		
		return ((y>=(KERNEL_V_SIZE/2)) && (x>=(KERNEL_H_SIZE/2)));	
	}
	
	T operator () (int row, int col) {
#pragma HLS inline	
		return mPixelWindow(row,col);
	}
	

	
  private:	
	unsigned short mWidth;
	unsigned short mHeight;	
	bool mValid;
    hls::Window<KERNEL_V_SIZE, KERNEL_H_SIZE, T>      mPixelWindow;
    hls::LineBuffer<KERNEL_V_SIZE, MAX_LINE_SIZE, T>  mLineBuffer;	
};
