#include "constants.h"
#include "kernels.h"
#include "types.h"

#include "ap_fixed.h"
#include <hls_stream.h>

#include <cmath>
#include <cstring>

typedef ap_fixed<16,9> fixed;
#define COEFFICIENT_SIZE 3


extern "C"
{

  static const RGBPixel zero = {0, 0, 0, 0};

  void read_dataflow(hls::stream<RGBPixel>& read_stream, const RGBPixel *in,
                     int img_width, int elements, int half) {
  int pixel = 0;
  while(elements--) {
        read_stream <<  in[pixel++];
  }
  int padding = img_width * half + COEFFICIENT_SIZE;
  while(padding--) {
        read_stream << zero;
  }
}

  void compute_dataflow(hls::stream<RGBPixel>& write_stream, hls::stream<RGBPixel>& read_stream,
                        const float* coefficient, int img_width, int elements, int center) {
      static RGBPixel window_mem[COEFFICIENT_SIZE][MAX_WIDTH];
#pragma HLS data_pack variable=window_mem
#pragma HLS array_partition variable=window_mem complete dim=1
      static fixed coef[COEFFICIENT_SIZE * COEFFICIENT_SIZE];
#pragma HLS array_partition variable=coef complete

      for(int i  = 0; i < COEFFICIENT_SIZE*COEFFICIENT_SIZE; i++) {
          coef[i] = coefficient[i];
      }

      int line_idx = 0;
      while(line_idx < center) {
          for(int i = 0; i < img_width; i++) {
              window_mem[line_idx][i] = zero;
          }
          line_idx++;
      }

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

  void write_dataflow(RGBPixel* outFrame, hls::stream<RGBPixel>& write_stream,
                      int elements) {
    int pixel = 0;
    while(elements--) {
        write_stream >> outFrame[pixel++];
    }
  }

  void convolve_fpga(const RGBPixel* inFrame, RGBPixel* outFrame,
                     const float* coefficient, int coefficient_size,
                     int img_width, int img_height)
  {
#pragma HLS INTERFACE s_axilite port=return bundle=control
#pragma HLS INTERFACE s_axilite port=inFrame bundle=control
#pragma HLS INTERFACE s_axilite port=outFrame bundle=control
#pragma HLS INTERFACE s_axilite port=img_height bundle=control
#pragma HLS INTERFACE s_axilite port=img_width bundle=control
#pragma HLS INTERFACE s_axilite port=coefficient bundle=control
#pragma HLS INTERFACE s_axilite port=coefficient_size bundle=control
#pragma HLS INTERFACE m_axi port=inFrame bundle=gmem1
#pragma HLS INTERFACE m_axi port=outFrame bundle=gmem2
#pragma HLS INTERFACE m_axi port=coefficient bundle=gmem3
#pragma HLS data_pack variable=inFrame
#pragma HLS data_pack variable=outFrame

    int half = COEFFICIENT_SIZE / 2;

    hls::stream<RGBPixel> read_stream("read");
    hls::stream<RGBPixel> write_stream("write");
    int elements = img_width * img_height;

#pragma HLS dataflow
    read_dataflow(read_stream, inFrame, img_width, elements, half);
    compute_dataflow(write_stream, read_stream, coefficient, img_width, elements, half);
    write_dataflow(outFrame, write_stream, elements);

  }
}
