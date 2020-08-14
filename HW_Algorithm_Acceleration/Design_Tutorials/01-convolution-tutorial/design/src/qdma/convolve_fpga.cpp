 #include "constants.h"
 #include "kernels.h"
 #include "types.h"
 #include "ap_fixed.h"
 #include <hls_stream.h>
#include<iostream>
 #include <cmath>
 #include <cstring>
 
#define dd 14848
 
  int offset_1;
 #include <algorithm>
 
 typedef ap_fixed<16,9> fixed;
 #define COEFFICIENT_SIZE 3
 
 extern "C"
 {
   static const RGBPixel zero = {0, 0, 0, 0};

 void read_dataflow(hls::stream<RGBPixel>& read_stream, const RGBPixel *in,
                   int img_width, int elements, int half,
                   int top_padding, int bottom_padding) {
     while(top_padding--) {
         read_stream << zero;
     }
     static int p=0;
     int pixel = 0;
     int i =0;
    RGBPixel a;
  
   while(elements--) {
         a = in[pixel++];
         read_stream << a;
  
         i++;

     }
     while(bottom_padding--) {
         read_stream << zero;
     }
        p++;
 }
 
 void compute_dataflow(hls::stream<RGBPixel>& write_stream, hls::stream<RGBPixel>& read_stream,
                       const float* coefficient,
                       int img_width, int elements, int center) {
       static RGBPixel window_mem[COEFFICIENT_SIZE][MAX_WIDTH];
 #pragma HLS data_pack variable=window_mem
 #pragma HLS array_partition variable=window_mem complete dim=1
       static fixed coef[COEFFICIENT_SIZE * COEFFICIENT_SIZE];
 #pragma HLS array_partition variable=coef complete
 float a;
       for(int i  = 0; i < COEFFICIENT_SIZE*COEFFICIENT_SIZE; i++) {
           a  = coefficient[i];
        coef[i] = a;
 //     std::cout <<" coeff " << a <<std::endl;
 
     //      std::cout<<"coef is " << a <<std::endl;
       }
      // std::cout <<"center is "<<center <<std::endl;
       int line_idx = 0;
 static int z=0;
       while(line_idx < COEFFICIENT_SIZE - 1) {
           for(int ii = 0; ii < img_width; ii++) {
               read_stream >> window_mem[line_idx][ii];
           }
           line_idx++;
       }
 
       for(int ii = 0; ii < COEFFICIENT_SIZE; ii++) {
           read_stream >> window_mem[line_idx][ii];
       }
 
       int top_idx = 0;
       int insert_idx = line_idx;
       int window_line_idx = top_idx;
       int j = 0;
       int insert_column_idx = COEFFICIENT_SIZE;
      // std::cout <<"while loop confition << " << elements <<std::endl;
      // std::cout <<"img width "<< img_width <<std::endl;
       while(elements--) {
           fixed sum_r = 0, sum_g=0, sum_b=0;
           for(int m = 0; m < COEFFICIENT_SIZE; ++m) {
               for(int n = 0; n < COEFFICIENT_SIZE; ++n) {
                   int jj = j + n - center;
                   RGBPixel tmp = (jj >= 0 && jj < img_width) ? window_mem[window_line_idx][jj] : zero;
                  fixed coef_tmp = coef[m * COEFFICIENT_SIZE + n] * (jj >= 0 && jj < img_width);
                   sum_r += tmp.r * coef_tmp;
                   sum_g += tmp.g * coef_tmp;
                   sum_b += tmp.b * coef_tmp;
              }
               window_line_idx = ((window_line_idx + 1) == COEFFICIENT_SIZE) ? 0 : window_line_idx + 1;
           }
           window_line_idx = top_idx;
           RGBPixel out = {sum_r.to_int(), sum_g.to_int(), sum_b.to_int(), 0};
           RGBPixel a = out;
          write_stream << out;
           j++;
           if(j >= img_width) {
               j = 0;
               top_idx = ((top_idx + 1) == COEFFICIENT_SIZE) ? 0 : top_idx + 1;
               window_line_idx = top_idx;
           }
           read_stream >> window_mem[insert_idx][insert_column_idx++];
           if (insert_column_idx >= img_width) {
               insert_column_idx = 0;
               insert_idx = ((insert_idx + 1) == COEFFICIENT_SIZE) ? 0 : insert_idx + 1;
           }
       }
    z++;  
 }
 
 void write_dataflow(RGBPixel* outFrame, hls::stream<RGBPixel>& write_stream,
                     int elements) {
     int pixel = 0;
   
RGBPixel a;
    while(elements--) {
         write_stream >> a ; //outFrame[pixel++];
         outFrame[pixel++] = a; 
   }
 }


 void convolve_fpga(const RGBPixel* inFrame, RGBPixel* outFrame,
                                 const float* coefficient, int coefficient_size,
                                 int img_width, int img_height,
                                 int line_offset, int num_lines) {

     int half = COEFFICIENT_SIZE / 2;
 
     hls::stream<RGBPixel> read_stream("read");
     hls::stream<RGBPixel> write_stream("write");
 
     int elements = img_width * num_lines;
     int offset;
     if(line_offset-half > 0)
     {
        offset = (line_offset - half)*img_width;
     }
     else
     {
        offset = 0;
     }
 
     
     
    int top_padding = 0;
     int bottom_padding = 0;
     int padding = 0;
     if(line_offset == 0) {
         top_padding = half * img_width;
     } else {
         padding = img_width *  half;
     }
     if(line_offset + num_lines < img_height) {
         padding += img_width * half + COEFFICIENT_SIZE;
     }else {
         bottom_padding = img_width * (half) + COEFFICIENT_SIZE;
     }
     offset_1 = offset;
 #pragma HLS dataflow

     read_dataflow(read_stream, inFrame + offset, img_width, elements + padding, half, top_padding, bottom_padding);
     compute_dataflow(write_stream, read_stream, coefficient, img_width, elements, half);
     write_dataflow(outFrame + line_offset * img_width, write_stream, elements);
   }
 }
