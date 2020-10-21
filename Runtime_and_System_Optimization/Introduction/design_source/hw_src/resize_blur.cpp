/**********
Copyright (c) 2019, Xilinx, Inc.
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


#include "ap_int.h"
#include "common/xf_common.hpp"
#include "common/xf_utility.hpp"
#include "imgproc/xf_gaussian_filter.hpp"
#include "imgproc/xf_resize.hpp"


#define AXI_WIDTH 512
#define TYPE XF_8UC3
#define NPC XF_NPPC8

#define PRAGMA_SUB(x) _Pragma(#x)
#define DYN_PRAGMA(x) PRAGMA_SUB(x)

#define MAX_IN_WIDTH 1920
#define MAX_IN_HEIGHT 1080
#define MAX_OUT_WIDTH 1920
#define MAX_OUT_HEIGHT 1080

#define STREAM_DEPTH 8
#define MAX_DOWN_SCALE 7
#define FILTER_WIDTH 7

extern "C"
{
    void resize_blur_rgb(ap_uint<AXI_WIDTH> *image_in,
                         ap_uint<AXI_WIDTH> *image_out,
                         int width_in,
                         int height_in,
                         int width_out,
                         int height_out,
                         float sigma)
    {
#pragma HLS INTERFACE m_axi port = image_in offset = slave bundle = image_in_gmem max_read_burst_length=256
#pragma HLS INTERFACE m_axi port = image_out offset = slave bundle = image_out_gmem max_write_burst_length=256
#pragma HLS INTERFACE s_axilite port = image_in bundle = control
#pragma HLS INTERFACE s_axilite port = image_out bundle = control
#pragma HLS INTERFACE s_axilite port = width_in bundle = control
#pragma HLS INTERFACE s_axilite port = height_in bundle = control
#pragma HLS INTERFACE s_axilite port = width_out bundle = control
#pragma HLS INTERFACE s_axilite port = height_out bundle = control
#pragma HLS INTERFACE s_axilite port = sigma bundle = control
#pragma HLS INTERFACE s_axilite port = return bundle = control

        xf::cv::Mat<TYPE, MAX_IN_HEIGHT, MAX_IN_WIDTH, NPC> in_mat(height_in, width_in);
        DYN_PRAGMA(HLS stream variable = in_mat.data depth = STREAM_DEPTH)

        xf::cv::Mat<TYPE, MAX_OUT_HEIGHT, MAX_OUT_WIDTH, NPC> resized_mat(height_out, width_out);
        DYN_PRAGMA(HLS stream variable = resized_mat.data depth = STREAM_DEPTH);

        xf::cv::Mat<TYPE, MAX_OUT_HEIGHT, MAX_OUT_WIDTH, NPC> out_mat(height_out, width_out);
        DYN_PRAGMA(HLS stream variable = out_mat.data depth = STREAM_DEPTH)

#pragma HLS DATAFLOW

        xf::cv::Array2xfMat<AXI_WIDTH,
                            TYPE,
                            MAX_IN_HEIGHT,
                            MAX_IN_WIDTH,
                            NPC>(image_in, in_mat);
        xf::cv::resize<XF_INTERPOLATION_AREA,
                       TYPE,
                       MAX_IN_HEIGHT,
                       MAX_IN_WIDTH,
                       MAX_OUT_HEIGHT,
                       MAX_OUT_WIDTH,
                       NPC,
                       MAX_DOWN_SCALE>(in_mat, resized_mat);
        xf::cv::GaussianBlur<FILTER_WIDTH,
                             XF_BORDER_CONSTANT,
                             TYPE,
                             MAX_OUT_HEIGHT,
                             MAX_OUT_WIDTH,
                             NPC>(resized_mat, out_mat, sigma);
        xf::cv::xfMat2Array<AXI_WIDTH,
                            TYPE,
                            MAX_OUT_HEIGHT,
                            MAX_OUT_WIDTH,
                            NPC>(out_mat, image_out);
    }
}
