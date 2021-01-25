# ALPHA_MIX HLS C Kernel Creation

According to the design specification, the *alpha_mix* kernel will include following functions:

* Receive monochrome (one channel 8-bit depth) digital clock images via AXI Stream slave interface
* Read the background image from global memory via AXI master interface
* Resize the digital clock image to designated scale
* Color the clock digit characters as well as the background layer for the clock images
* Mix the clock digit, clock background layer and the background image with opacity setting
* Output the mixed image via AXI Stream master interface

Following code block is the top level function declaration for *alpha_mix* kernel. You can see that: the hardware AXI stream port (master or slave) is represented by *hls::stream* data type; the hardware AXI master port (read or write) is represented by *ap_uint* (actually array pointer) data type; those kernel arguments are represented by normal *int* data type, which will be grouped up to map to an AXI slave interface.

~~~c++
void alpha_mix(hls::stream<ap_axiu<64, 0, 0, 0>> &time_img_input,   // time image input
               ap_uint<256> *bgr_img_input,                         // background image input
               hls::stream<ap_axiu<64, 0, 0, 0>> &mix_img_output,   // mixed image output
               int     time_img_rows_in,                            // input time image height
               int     time_img_cols_in,                            // input time image width
               int     time_img_rows_rsz,                           // resized time image height
               int     time_img_cols_rsz,                           // resized time image width
               int     time_img_pos_row,                            // resized time image position - Y
               int     time_img_pos_col,                            // resized time image position - X
               ap_uint<32> time_char_color,                         // [31:0] = [xRGB]
               ap_uint<32> time_bgr_color,                          // [31:0] = [xRGB]
               int     time_bgr_opacity,                            // time image background opacity，[7:0] used
               int     bgr_img_rows,                                // background image height
               int     bgr_img_cols                                 // background image width
)
~~~


Following is the sub-function and data flow diagram for *alpha_mix* kernel.

<div align="center">
<img src="./images/alpha_mix_flow.png" alt="alpha_mix flow" >
</div>

In the diagram, the sub-functions filled with red are from Vitis Vision Library, and those with blue are hand-written. *xf::cv::Mat* is the counterpart for *cv::Mat* in OpenCV software library, it is very useful for handling image data. In the hardware implementation, if we want to handle the image with in-order pixel level (no need to randomly access the pixel data), we can use *#pragma HLS stream* to indicate the compiler to map the *xf::cv::Mat* object to array. This is the case for our *alpha_mix* kernel. Many functions in Vitis Vision Library support *xf::cv::Mat* as the input and output data. In the HLS C code, we can use for-loop to process data in *xf::cv::mat* stream easily, you can refer to the source code of sub-function *mixing* for alpha-mixing operation applied to the input and output *xf::cv::Mat* objects.

We use Vitis *v++* command to compile the HLS C source code to kernel file (xo). For example, use following command to compile *alpha_mix.c* to hardware kernel with *xilinx_u50_gen3x16_xdma_201920_3* platform.

~~~
v++ --platform xilinx_u50_gen3x16_xdma_201920_3                                                     \
    --target hw                                                                                     \
    --kernel alpha_mix                                                                              \
    --include ./include                                                                             \
    --advanced.prop kernel.alpha_mix.kernel_flags="-std=c++0x -D__SDSVHLS__ -DHLS_NO_XIL_FPO_LIB"   \
    --compile                                                                                       \
    --output alpha_mix.xo                                                                           \
    alpha_mix.c
~~~

<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020-2021 Xilinx</sup></p>
