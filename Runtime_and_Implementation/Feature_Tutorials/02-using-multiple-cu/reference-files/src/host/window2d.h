#pragma once

template<unsigned KERNEL_H_SIZE, unsigned KERNEL_V_SIZE, typename T>
class Window2D {

  public:

	Window2D(unsigned short width, unsigned short height, unsigned short stride) 
	{
		mWidth  = width;
		mHeight = height;
		mStride = stride;
	}		
	
	bool next(T *srcImg, unsigned x, unsigned y)
	{
		// Clamp pixels to 0 when outside of image 
		for(int row=0; row<KERNEL_V_SIZE; row++)
		{
			for(int col=0; col<KERNEL_H_SIZE; col++)
			{
				int xoffset = (x+col-(KERNEL_H_SIZE/2));
				int yoffset = (y+row-(KERNEL_V_SIZE/2));
				
				if ( (xoffset<0) || (xoffset>=mWidth) ||
					 (yoffset<0) || (yoffset>=mHeight) ) {
					mPixelWindow[row][col] = 0;
				} else {
					mPixelWindow[row][col] = srcImg[yoffset*mStride+xoffset];
				}	
			}
		}
		
		return true;	
	}
	
	T operator () (int row, int col) {
		return mPixelWindow[row][col];
	}
	
  private:	
	int mWidth;
	int mHeight;	
	int mStride;	
    T mPixelWindow[KERNEL_V_SIZE][KERNEL_H_SIZE];
};
