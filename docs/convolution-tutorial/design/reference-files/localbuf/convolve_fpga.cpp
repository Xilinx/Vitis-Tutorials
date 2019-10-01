
#include "constants.h"
#include "kernels.h"
#include "types.h"

#include <cmath>



#include <cstring>

extern "C"
{
void convolve_fpga(const RGBPixel* inFrame, RGBPixel* outFrame,
                    const float* coefficient, int coefficient_size,
                    int img_width, int img_height)
 {
 #pragma HLS INTERFACE s_axilite port=return bundle=control
 #pragma HLS INTERFACE s_axilite port=inFrame bundle=control
 #pragma HLS INTERFACE s_axilite port=outFrame bundle=control
 #pragma HLS INTERFACE s_axilite port=coefficient bundle=control
 #pragma HLS INTERFACE s_axilite port=coefficient_size bundle=control
 #pragma HLS INTERFACE s_axilite port=img_width bundle=control
 #pragma HLS INTERFACE s_axilite port=img_height bundle=control
 #pragma HLS INTERFACE m_axi port=inFrame offset=slave bundle=gmem1
 #pragma HLS INTERFACE m_axi port=outFrame offset=slave bundle=gmem1
 #pragma HLS INTERFACE m_axi port=coefficient offset=slave bundle=gmem2
 
 #pragma HLS data_pack variable=inFrame
 #pragma HLS data_pack variable=outFrame

     int center = coefficient_size / 2;

     RGBPixel window_mem[MAX_FILTER][MAX_WIDTH];
     RGBPixel out_line[MAX_WIDTH];
 #pragma HLS data_pack variable=window_mem
 #pragma HLS data_pack variable=out_line
 
     float coef[MAX_FILTER * MAX_FILTER];
     memcpy(coef, coefficient, coefficient_size * coefficient_size * sizeof(float));
 
     static const RGBPixel zero = {0, 0, 0, 0};

     for(int line = 0; line < center; line++) {
         for(int pixel = 0; pixel < MAX_WIDTH; pixel++) {
             window_mem[line][pixel] = zero;
         }
     }
 
     for(int line = center; line < coefficient_size-1; line++) {
         memcpy(window_mem[line], inFrame + ((line-1) * img_width), img_width * sizeof(RGBPixel));
     }
     for(int line = 0; line < img_height; ++line) {
         int next_line = (line + coefficient_size - 1) % MAX_FILTER;
         if(line + 1 < img_height) {
             memcpy(window_mem[next_line], inFrame + ((line+center) * img_width), img_width * sizeof(RGBPixel));
         }
         else {
             for(int pixel = 0; pixel < MAX_WIDTH; pixel++) {
                 window_mem[next_line][pixel] = zero;
             }
         }
         int window_line_idx = next_line - center - 1;
         if(window_line_idx < 0) window_line_idx += MAX_FILTER;
         int top_idx = window_line_idx;


         for(int pixel = 0; pixel < img_width; ++pixel)
         {
             float sum_r = 0, sum_g=0, sum_b=0;
             for(int m = 0; m < coefficient_size; ++m)
             {
                 for(int n = 0; n < coefficient_size; ++n)
                 {
                     int jj = pixel + n - center;
                     if(jj >= 0 && jj < img_width)
                     {
                         sum_r += window_mem[window_line_idx][jj].r * coef[m * coefficient_size + n];
                         sum_g += window_mem[window_line_idx][jj].g * coef[m * coefficient_size + n];
                         sum_b += window_mem[window_line_idx][jj].b * coef[m * coefficient_size + n];
                     }
                 }
                 window_line_idx=(window_line_idx + 1) == MAX_FILTER ? 0 : window_line_idx + 1;
             }
             window_line_idx = top_idx;
             out_line[pixel].r =  fabsf(sum_r);
             out_line[pixel].g =  fabsf(sum_g);
             out_line[pixel].b =  fabsf(sum_b);
         }
         memcpy(outFrame+(line * img_width), out_line, img_width * sizeof(RGBPixel));
     }
  }
}
