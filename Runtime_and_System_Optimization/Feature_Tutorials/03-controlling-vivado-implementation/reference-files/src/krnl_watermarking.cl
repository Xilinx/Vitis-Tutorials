/**********
Copyright (c) 2018, Xilinx, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********/

/*******************************************************************************
Description: 
    This example demonstrate to utilized both DDR and full bandwidth using 
    watermark Application. In Watermark application, kernel has to apply a fixed
    watermark (Here is it Xilinx First Character 'X') into a given Image and 
    write the output image.
    Inside Host code, input image is placed into Bank0 and Kernel will read
    input Image and write the output image to Bank1.
    To utilized the both Banks fully Kernel code do burst read input image with 
    full datawidth of 512 and do the burst write of output image with full 
    datawidth of 512. 
    As Kernel is accessing Sequentially from both the DDR, so kernel with get the
    Best memory access bandwidth from both DDRs and will do watermark with good 
    performance.
*******************************************************************************/

//Water Mark Defines
#define CHANNELS  3     //Three Channels (R, G and B)
#define WATERMARK_HEIGHT 16
#define WATERMARK_WIDTH 16

//Using Datatype uint16 to get the full memory datawidth 512
#define TYPE uint16

//Per Memory Access getting 16 pixels
#define DATA_SIZE 16


//function declaration
uint saturatedAdd(uint x, uint y);

__kernel __attribute__ ((reqd_work_group_size(1, 1, 1)))
void apply_watermark(__global const TYPE * __restrict input, __global TYPE * __restrict output, int width, int height) {
    
    //WaterMark Image of 16x16 size
    int watermark[WATERMARK_HEIGHT][WATERMARK_WIDTH] = 
    {
      { 0, 0,        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,        0 },
      { 0, 0x0f0f0f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0 },
      { 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0, 0 }, 
      { 0, 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0 },
      { 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0x0f0f0f, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0x0f0f0f, 0, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0 },
      { 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0, 0 },
      { 0, 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0, 0, 0 },
      { 0, 0, 0x0f0f0f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0, 0 }, 
      { 0, 0x0f0f0f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0f0f0f, 0 },
      { 0, 0,        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,        0 },
    } ;
    
    uint imageSize = width * height; // Total Number of Pixels
    //As memory access is int16 type so total 16 pixels can be access at a time,
    // so calculating total number of Memory accesses which are needed to entire
    // Image
    uint size = ( (imageSize-1) / DATA_SIZE ) + 1; 


    // Process the whole image 
    __attribute__((xcl_pipeline_loop))
    image_traverse: for (uint idx = 0, x = 0 , y = 0  ; idx < size ; ++idx, x+= DATA_SIZE)
    {
      // Read the next 16 Pixels
      TYPE tmp = input[idx];

      // Row Boundary Check for x 
      if (x >= width){
        x = x -width;
        ++y;
      }
      //Unrolling below loop to process all 16 pixels concurrently
      __attribute__((opencl_unroll_hint))
      watermark: for ( int i = 0 ; i < DATA_SIZE ; i++)
      {
          uint tmp_x = x+i;
          uint tmp_y = y;
          // Row Boundary Check for x 
          if (tmp_x > width) {
            tmp_x = tmp_x -width;
            tmp_y +=1;
          }

          uint w_idy = tmp_y % WATERMARK_HEIGHT; 
          uint w_idx = tmp_x % WATERMARK_WIDTH;
          tmp[i]     = saturatedAdd(tmp[i], watermark[w_idy][w_idx]) ;
      }

      //Write the Next 16 Pixels result to output memory
      output[idx] = tmp ;
    }
}

uint saturatedAdd(uint x, uint y)
{
    // Separate into the different channels

    //Red Channel
    uchar redX = x & 0xff ;
    uchar redY = y & 0xff ;
    uchar redOutput ;
    
    //Green Channel
    uchar greenX = (x & 0xff00) >> 8 ;
    uchar greenY = (y & 0xff00) >> 8 ;
    uchar greenOutput ;
    
    //Blue Channel
    uchar blueX = (x & 0xff0000) >> 16 ;
    uchar blueY = (y & 0xff0000) >> 16 ;
    uchar blueOutput ;
    
    //Calculating Red 
    if (redX + redY > 255){
        redOutput = 255 ;
    }else{
      redOutput = redX + redY ;
    }
    
    //Calculating Green
    if (greenX + greenY > 255){
      greenOutput = 255 ;
    }else{
      greenOutput = greenX + greenY ;
    }
    
    //Calculating Blue
    if (blueX + blueY > 255){
      blueOutput = 255 ;
    }else {
      blueOutput = blueX + blueY ;
    }
   
    // Combining all channels into one 
    uint combinedOutput = 0 ;
    combinedOutput |= (uint) redOutput ;
    combinedOutput |= (uint) (greenOutput << 8) ;
    combinedOutput |= (uint) (blueOutput << 16) ;
    return combinedOutput ;
}
