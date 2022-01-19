/*
 * Copyright 2020 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <hls_stream.h>
#include <ap_int.h>
#include <ap_axi_sdata.h>
#include "common/xf_common.hpp"
#include "common/xf_utility.hpp"
#include "imgproc/xf_resize.hpp"

#define _XF_SYNTHESIS_ 1

// Max input/output Image Dimensions
#define MAX_WIDTH 1920
#define MAX_HEIGHT 1080

// resizer max down scale ratio
#define MAXDOWNSCALE 9

extern "C"
{

    // mixed value calculation function: image layer Y1 is over image layer Y2
    // y1 is color value of Y1, y2 is color value of Y2, alpha1 is alpha value of Y1
    uint8_t mix_calc (uint8_t y1, uint8_t alpha1, uint8_t y2) 
    {
        return (y1 * alpha1 + y2 * (255 - alpha1)) >> 8;
    }

    // major alpha mixing function
    void mixing(xf::cv::Mat<XF_8UC1, MAX_HEIGHT, MAX_WIDTH, XF_NPPC8> &time_img,
                xf::cv::Mat<XF_8UC3, MAX_HEIGHT, MAX_WIDTH, XF_NPPC8> &bgr_img,
                xf::cv::Mat<XF_8UC3, MAX_HEIGHT, MAX_WIDTH, XF_NPPC8> &mixed_img,
                int time_img_pos_row,
                int time_img_pos_col,
                ap_uint<8> time_char_color_r,
                ap_uint<8> time_char_color_g,
                ap_uint<8> time_char_color_b,
                ap_uint<8> time_bgr_color_r,
                ap_uint<8> time_bgr_color_g,
                ap_uint<8> time_bgr_color_b,
                ap_uint<8> time_bgr_opacity)
    {
        // clang-format off
        #pragma HLS INLINE
        // clang-format on

        for (int row = 0; row < bgr_img.rows; row++)
        {
            for (int col = 0; col < (bgr_img.cols >> XF_BITSHIFT(XF_NPPC8)); col++)
            {
                ap_uint<24 * XF_NPPC8> din_bgr;
                ap_uint<8 * XF_NPPC8> din_time;
                ap_uint<24 * XF_NPPC8> dout_mix;

                // clang-format off
                #pragma HLS PIPELINE
                // clang-format on

                din_bgr = bgr_img.read(row * (bgr_img.cols >> XF_BITSHIFT(XF_NPPC8)) + col);

                if ((row >= time_img_pos_row) && 
                    (row <  (time_img_pos_row + time_img.rows)) &&
                    (col >= (time_img_pos_col >> XF_BITSHIFT(XF_NPPC8))) && 
                    (col <  ((time_img_pos_col >> XF_BITSHIFT(XF_NPPC8)) + (time_img.cols >> XF_BITSHIFT(XF_NPPC8)))))
                {
                    din_time = time_img.read((row - time_img_pos_row) * (time_img.cols >> XF_BITSHIFT(XF_NPPC8)) + (col - (time_img_pos_col >> XF_BITSHIFT(XF_NPPC8))));
                    for (int i = 0; i < (1 << XF_BITSHIFT(XF_NPPC8)); i++)
                    {
                        // clang-format off
                        #pragma HLS UNROLL
                        // clang-format on
                        dout_mix.range(23 + i * 24, 16 + i * 24) = mix_calc(time_char_color_r, din_time(7 + i * 8, i * 8),     // red channel
                                                                    mix_calc(time_bgr_color_r, time_bgr_opacity, din_bgr.range(23 + i * 24, 16 + i * 24)));

                        dout_mix.range(15 + i * 24, 8 + i * 24) = mix_calc(time_char_color_g, din_time(7 + i * 8, i * 8),       // green channel
                                                                    mix_calc(time_bgr_color_g, time_bgr_opacity, din_bgr.range(15 + i * 24, 8 + i * 24)));

                        dout_mix.range(7 + i * 24, 0 + i * 24) = mix_calc(time_char_color_b, din_time(7 + i * 8, i * 8),        // blue channel
                                                                    mix_calc(time_bgr_color_b, time_bgr_opacity, din_bgr.range(7 + i * 24, 0 + i * 24)));                        
                    }
                }
                else
                {
                    dout_mix = din_bgr; 
                }
                mixed_img.write(row * (bgr_img.cols >> XF_BITSHIFT(XF_NPPC8)) + col, dout_mix);

            }
        }
    }


    // function to stream out the cv::Mat data
    // input hls::stream data width is 24*8, output AXI stream data width is 64
    void cvmat_stream_out(xf::cv::Mat<XF_8UC3, MAX_HEIGHT, MAX_WIDTH, XF_NPPC8> &input_img, 
                       hls::stream<ap_axiu<64, 0, 0, 0>> &stream_out)
    {
        for (int i = 0; i < input_img.rows * (input_img.cols >> XF_BITSHIFT(XF_NPPC8)); i++)
        {
            ap_uint<24 * XF_NPPC8> data;
            ap_axiu<64, 0, 0, 0> v;
            data = input_img.read(i);
            v.data = data.range(63,0);
            stream_out.write(v);
            v.data = data.range(127,64);
            stream_out.write(v);
            v.data = data.range(191,128);
            stream_out.write(v);
        }
    }

    // Top level kernel function
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
                   int     time_bgr_opacity,                            // time image background opacity [7:0] is used
                   int     bgr_img_rows,                                // background image height
                   int     bgr_img_cols                                 // background image width
    )
    {
        xf::cv::Mat<XF_8UC1, MAX_HEIGHT, MAX_WIDTH, XF_NPPC8> time_img;
        // clang-format off
        #pragma HLS stream variable=time_img.data
        // clang-format on
        time_img.rows = time_img_rows_in;
        time_img.cols = time_img_cols_in;

        xf::cv::Mat<XF_8UC1, MAX_HEIGHT, MAX_WIDTH, XF_NPPC8> resized_time_img;
        // clang-format off
        #pragma HLS stream variable=resized_time_img.data
        // clang-format on
        resized_time_img.rows = time_img_rows_rsz;
        resized_time_img.cols = time_img_cols_rsz;

        xf::cv::Mat<XF_8UC3, MAX_HEIGHT, MAX_WIDTH, XF_NPPC8> bgr_img;
        // clang-format off
        #pragma HLS stream variable=bgr_img.data
        // clang-format on
        bgr_img.rows = bgr_img_rows;
        bgr_img.cols = bgr_img_cols;

        xf::cv::Mat<XF_8UC3, MAX_HEIGHT, MAX_WIDTH, XF_NPPC8> mixed_img;
        // clang-format off
        #pragma HLS stream variable=mixed_img.data
        // clang-format on
        mixed_img.rows = bgr_img_rows;
        mixed_img.cols = bgr_img_cols;

        // clang-format off
        #pragma HLS DATAFLOW
        // clang-format on

        // convert axi stream to xfMat
        xf::cv::axiStrm2xfMat<64, XF_8UC1, MAX_HEIGHT, MAX_WIDTH, XF_NPPC8>(time_img_input, time_img);

        // convert array (axi master) to xfMat
        //xf::cv::Array2xfMat<512, XF_8UC3, MAX_HEIGHT, MAX_WIDTH, XF_NPPC8>(bgr_img_input, bgr_img); 
        xf::cv::Array2xfMat<256, XF_8UC3, MAX_HEIGHT, MAX_WIDTH, XF_NPPC8>(bgr_img_input, bgr_img); // yuang

        // Resize time image
        xf::cv::resize<XF_INTERPOLATION_BILINEAR, XF_8UC1, MAX_HEIGHT, MAX_WIDTH, MAX_HEIGHT, MAX_WIDTH, XF_NPPC8, MAXDOWNSCALE>(time_img, resized_time_img);

        // alpha-mixing with colored time image
        ap_uint<8> time_char_color_r = time_char_color.range(23,16);
        ap_uint<8> time_char_color_g = time_char_color.range(15,8);
        ap_uint<8> time_char_color_b = time_char_color.range(7,0);

        ap_uint<8> time_bgr_color_r = time_bgr_color.range(23,16);
        ap_uint<8> time_bgr_color_g = time_bgr_color.range(15,8);
        ap_uint<8> time_bgr_color_b = time_bgr_color.range(7,0);

        mixing(resized_time_img, bgr_img, mixed_img, 
               time_img_pos_row, time_img_pos_col,
               time_char_color_r, time_char_color_g, time_char_color_b,
               time_bgr_color_r, time_bgr_color_g, time_bgr_color_b,
               time_bgr_opacity);

        // convert xfMat to AXI stream
        //xf::cv::xfMat2axiStrm<64, XF_8UC3, MAX_HEIGHT, MAX_WIDTH, XF_NPPC8>(mixed_img, mix_img_output);
        cvmat_stream_out(mixed_img, mix_img_output);
    }
}
