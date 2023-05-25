<table class="sphinxhide" width="100%">
 <tr>
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>Vitis™ Application Acceleration Development Flow Tutorials</h1>
   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# ALPHA_MIX HLS C Kernel Creation

According to the design specification, the *alpha_mix* kernel will include the following functions:

* Receive monochrome (one channel 8-bit depth) digital clock images via the AXI4-Stream slave interface
* Read the background image from the global memory via the AXI master interface
* Resize the digital clock image to the designated scale
* Color the clock digit characters as well as the background layer for the clock images
* Mix the clock digit, clock background layer, and the background image with opacity setting
* Output the mixed image via the AXI4-Stream master interface

The following code block is the top-level function declaration for *alpha_mix* kernel. You can see that: the hardware AXI stream port (master or slave) is represented by the *hls::stream* data type; the hardware AXI master port (read or write) is represented by the *ap_uint* (actually array pointer) data type; those kernel arguments are represented by the normal *int* data type, which will be grouped up to map to an AXI slave interface.

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

The following is the sub-function and data flow diagram for the *alpha_mix* kernel.

![alpha_mix flow](alpha_mix flow)

In the diagram, the sub-functions filled with red are from the Vitis Vision Library, and those with blue are handwritten. *xf::cv::Mat* is the counterpart for *cv::Mat* in the OpenCV software library; it is very useful for handling image data. In the hardware implementation, if you want to handle the image with in-order pixel level (no need to randomly access the pixel data), you can use the *#pragma HLS stream* to indicate the compiler to map the *xf::cv::Mat* object to the array. This is the case for your *alpha_mix* kernel. Many functions in Vitis Vision Library support *xf::cv::Mat* as the input and output data. In the HLS C code, you can use the for-loop to process data in the *xf::cv::mat* stream easily; you can refer to the source code of sub-function *mixing* for the alpha-mixing operation applied to the input and output *xf::cv::Mat* objects.

Use Vitis *v++* command to compile the HLS C source code to the kernel file (xo). For example, use the following command to compile *alpha_mix.c* to the hardware kernel with a *xilinx_u50_gen3x16_xdma_201920_3* platform.

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

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
