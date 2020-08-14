 #include "constants.h"
// #include "kernels.h"
 #include "kernel_types.h"
 #include "ap_fixed.h"
#include <hls_stream.h>
#include <iostream>
 #include <cmath>
 #include <cstring>
 
#define dd 14848
 typedef qdma_axis<DWIDTH, 0, 0, 0> pkt;

 
 #include <algorithm>
typedef ap_fixed<16,9> fixed;
 #define COEFFICIENT_SIZE 3

 union test{
int x;
float y;
};
 extern "C"
 {
   static const RGBPixel zero = {0, 0, 0};

 void read_dataflow(hls::stream<RGBPixel>& read_stream, hls::stream<pkt>& in,
                   int img_width, int elements, int half,
                   int top_padding, int bottom_padding) {
  

     while(top_padding--) {
         read_stream << zero;
     }
    RGBPixel a_temp;
     int pixel = 0;
     ap_int<1> last;
     bool eos = false;
     do{

    pkt a_t = in.read();
    ap_int<32> in1 = a_t.get_data();
    last = a_t.get_last();
    if(last)
    {
        eos = true;
    }
      a_temp.init(in1);
       RGBPixel a;
       a = a_temp; 
       read_stream << a_temp;

     }while(eos ==false);

        while(bottom_padding--) {
         read_stream << zero;
     }
 }
 
 void compute_dataflow(hls::stream<RGBPixel>& write_stream, hls::stream<RGBPixel>& read_stream,
                        hls::stream<pkt>& coefficient,
                       int img_width, int elements, int center) {
       static RGBPixel window_mem[COEFFICIENT_SIZE][MAX_WIDTH];
//#pragma HLS BIND_STORAGE variable=window_mem type=ram_t2p impl=bram
 #pragma HLS AGGREGATE variable=window_mem
 #pragma HLS array_partition variable=window_mem complete dim=1
 //#pragma HLS array_partition variable=window_mem complete dim=2

//#pragma HLS ARRAY_PARTITION variable=window_mem dim=2 factor=10 cyclic

       static fixed coef[COEFFICIENT_SIZE * COEFFICIENT_SIZE];
 #pragma HLS array_partition variable=coef complete
 union test p1;

       for(int i  = 0; i < COEFFICIENT_SIZE*COEFFICIENT_SIZE; i++) {
    	   pkt a_t =  coefficient.read(); //>> p1.x;
    	   p1.x = a_t.get_data();
           coef[i] = p1.y;
       }
      int line_idx = 0;
 
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
      
       while(elements--) {
           fixed sum_r = 0, sum_g=0, sum_b=0;
           for(int m = 0; m < COEFFICIENT_SIZE; ++m) {
               for(int n = 0; n < COEFFICIENT_SIZE; ++n) {
                   int jj = j + n - center;
                   // 1 st - 0- 9
                   //      - 1-9
                   RGBPixel tmp = (jj >= 0 && jj < img_width) ? window_mem[window_line_idx][jj] : zero;
                   fixed coef_tmp = coef[m * COEFFICIENT_SIZE + n] * (jj >= 0 && jj < img_width);
                   sum_r += tmp.r * coef_tmp;
                   sum_g += tmp.g * coef_tmp;
                   sum_b += tmp.b * coef_tmp;
               }
               window_line_idx = ((window_line_idx + 1) == COEFFICIENT_SIZE) ? 0 : window_line_idx + 1;
           }
           window_line_idx = top_idx;
           RGBPixel out = {sum_r.to_int(), sum_g.to_int(), sum_b.to_int()};
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
  }
 
 void write_dataflow(hls::stream<pkt>& outFrame, hls::stream<RGBPixel>& write_stream,
                     int elements) 
 {
     int pixel = 0;
     pkt t_out;
     ap_int<32> a_out;
     RGBPixel tmpout;
     int i=0;
     std::cout <<"elements -----------------"<<elements <<std::endl;
     while(elements--) {
        RGBPixel a;
        write_stream >> tmpout;
        a_out = tmpout.func1();
        a = tmpout;
       t_out.set_data(a_out);
       t_out.set_keep(-1);
        i++;
        if(elements ==0)
        {
            std::cout << "inside the tlast " << std::endl;
            t_out.set_last(1);
            std::cout <<"tlast for the data " <<t_out.get_last() <<std::endl;
        }
        else
        {
            t_out.set_last(0);
        }  
             outFrame.write(t_out);

     
     }
  }

void init(int *elements,int* offset, int img_width, int img_height, int num_lines, int line_offset,int *half,
            int *top_padding, int *bottom_padding, int *padding )
{
  *half = COEFFICIENT_SIZE / 2;

     *elements = img_width * num_lines;
     // offset;
     *bottom_padding =0;
     *top_padding = 0;
     *padding = 0;

     if(line_offset-*half > 0)
     {
        *offset = (line_offset - *half)*img_width;
     }
     else
     {
        *offset = 0;
     }

     if(line_offset == 0) {
         *top_padding = *half * img_width;
     } else {
         *padding = img_width *  (*half);
     }
     if(line_offset + num_lines < img_height) {
         *padding += img_width * (*half) + COEFFICIENT_SIZE;
     }else {
         *bottom_padding = img_width * (*half) + COEFFICIENT_SIZE;
     }
}

void convolve_fpga( hls::stream<pkt>& inFrame, hls::stream<pkt>& outFrame,
		hls::stream<pkt>& coefficient,
		int coefficient_size, int img_width, int img_height,
                                 int line_offset, int num_lines)
{
 #pragma HLS dataflow

    int half; 
     static hls::stream<RGBPixel> read_stream("read");
     static hls::stream<RGBPixel> write_stream("write");
#pragma HLS STREAM variable=read_stream depth=8192
#pragma HLS STREAM variable=write_stream depth=8192
    int elements;
    int offset;
     int top_padding;
     int bottom_padding;
     int padding;
   
    init(&elements,&offset, img_width, img_height, num_lines, line_offset,&half,&top_padding, &bottom_padding, &padding );
    read_dataflow(read_stream, inFrame, img_width, elements + padding, half, top_padding, bottom_padding);
    compute_dataflow(write_stream, read_stream, coefficient, img_width, elements, half);
    write_dataflow(outFrame, write_stream, elements);

}
 }
