#pragma once 

#ifdef KERNEL_HEADERS
#include "ap_axi_sdata.h"
#include "ap_int.h"
#include "hls_stream.h"
#define DWIDTH 32

typedef qdma_axis<DWIDTH, 0, 0, 0> pkt;
#endif


struct RGBPixel
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
//    unsigned char a;
#ifdef KERNEL_HEADERS

void init(ap_int<32> d )
{
#pragma HLS INLINE
    r = d.range(7,0);
    g = d.range(15,8);
    b = d.range(23,16);

}
ap_int<32> func1()
{
#pragma HLS INLINE 
  ap_int<32> c = (ap_int<8>(b) , ap_int<8>(g), ap_int<8>(r));
    return c; 
}

#endif

}__attribute__((aligned(4)));

typedef unsigned char GrayPixel;





extern "C"
{
    
    // Convolve RGB video frame with input filter
  #ifdef HOST_CODE_OPT 
  void convolve_fpga(const RGBPixel* inFrame, RGBPixel* outFrame, const float* coefficient, int coefficient_size, int img_width, int img_height, 
                                 int line_offset, int num_lines);
  #elif KERNEL_HEADERS
  void convolve_fpga(hls::stream<pkt>& inFrame, hls::stream<pkt>& outFrame, hls::stream<pkt>& coefficient, int coefficient_size, int img_width, int img_height,
                                 int line_offset, int num_lines);
   #else
  void convolve_fpga(const RGBPixel* inFrame, RGBPixel* outFrame, const float* filter, int filter_size, int img_width, int img_height);  
  #endif
}
