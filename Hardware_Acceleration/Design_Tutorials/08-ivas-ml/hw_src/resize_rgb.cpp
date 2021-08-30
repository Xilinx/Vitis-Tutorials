/*
 * Copyright 2019 Xilinx, Inc.
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


#include "ap_int.h"
#include "common/xf_common.hpp"
#include "common/xf_utility.hpp"
#include "hls_stream.h"

// Need to include these in a particular order at the moment
// clang-format off
#include "imgproc/xf_resize.hpp"
#include "imgproc/xf_cvt_color.hpp"
// clang-format on

#ifdef ALVEO
#define AXI_WIDTH 512
#define NPC XF_NPPC8
// NPC for the Y channel must be 2x the NPC for the UV channel
#define NPC_UV XF_NPPC4
#define TYPE XF_8UC3
#define MAX_DOWN_SCALE 7
#else
#define AXI_WIDTH 128
#define NPC XF_NPPC4
// NPC for the Y channel must be 2x the NPC for the UV channel
#define NPC_UV XF_NPPC2
#define TYPE XF_8UC3
#define MAX_DOWN_SCALE 7
#endif

#define PRAGMA_SUB(x) _Pragma(#x)
#define DYN_PRAGMA(x) PRAGMA_SUB(x)

#define MAX_IN_WIDTH 1920
#define MAX_IN_HEIGHT 1080
#define MAX_OUT_WIDTH 1920
#define MAX_OUT_HEIGHT 1080
#define STREAM_DEPTH 3

namespace xf {
    namespace cv {

        template <int BUS_WIDTH, int NPPC>
        void Yptr2Mat(int stride_in_x,
                      ap_uint<BUS_WIDTH> *_yptr,
                      xf::cv::Mat<XF_8UC1, MAX_IN_HEIGHT, MAX_IN_WIDTH, NPPC> &_y_mat)
        {
#pragma HLS INLINE OFF
#pragma HLS DATAFLOW

            const int ch_width = XF_DTPIXELDEPTH(XF_8UC1, NPPC);

            // clang-format off
            hls::stream<ap_uint<BUS_WIDTH> > strm;
            #pragma HLS STREAM variable=strm depth=128
            // clang-format on

            int rows                  = _y_mat.rows;
            int cols                  = _y_mat.cols;
            int dstMat_cols_align_npc = ((_y_mat.cols + (NPPC - 1)) >> XF_BITSHIFT(NPPC)) << XF_BITSHIFT(NPPC);
            int WidthInBytes          = (cols);

            int pixel_width  = XF_CHANNELS(XF_8UC1, NPPC) * ch_width;
            int line_count   = ((cols * pixel_width) + BUS_WIDTH - 1) / BUS_WIDTH;
            int stride_count = (((stride_in_x * pixel_width) + BUS_WIDTH - 1) / BUS_WIDTH) - 1;
            int loop_count   = (((stride_in_x * rows * pixel_width)) + BUS_WIDTH - 1) / BUS_WIDTH;
            int offsetY      = 0;

            for (int i = 0; i < rows; i++) {
#pragma HLS loop_tripcount max = 4320
#pragma HLS loop_flatten off
                for (int x = 0; x < (WidthInBytes + (BUS_WIDTH / 8) - 1) / (BUS_WIDTH / 8); x++) {
#pragma HLS PIPELINE II = 1
                    ap_uint<BUS_WIDTH> fb_pix = _yptr[offsetY + x];
                    strm << fb_pix;
                }
                offsetY += (stride_in_x) / (BUS_WIDTH / 8);
            }

            xf::cv::accel_utils au;
            au.hlsStrm2xfMat<BUS_WIDTH, XF_8UC1, MAX_IN_HEIGHT, MAX_IN_WIDTH, NPPC, (MAX_IN_HEIGHT * MAX_IN_WIDTH) / NPPC>(strm, _y_mat, dstMat_cols_align_npc);
        }

        template <int BUS_WIDTH, int NPPC>
        void UVptr2Mat(int stride_in_x,
                       ap_uint<BUS_WIDTH> *_uvptr,
                       xf::cv::Mat<XF_8UC2, MAX_IN_HEIGHT / 2, MAX_IN_WIDTH / 2, NPPC> &_uv_mat)
        {
#pragma HLS INLINE OFF
#pragma HLS DATAFLOW

            const int ch_width = XF_DTPIXELDEPTH(XF_8UC2, NPPC);

            // clang-format off
            hls::stream<ap_uint<BUS_WIDTH> > strm;
            #pragma HLS STREAM variable=strm depth=128
            // clang-format on

            int rows                  = _uv_mat.rows;
            int cols                  = _uv_mat.cols;
            int dstMat_cols_align_npc = ((_uv_mat.cols + (NPPC - 1)) >> XF_BITSHIFT(NPPC)) << XF_BITSHIFT(NPPC);
            int offsetUV              = 0;
            int WidthInBytes          = cols * 2;

            int pixel_width  = XF_CHANNELS(XF_8UC2, NPPC) * ch_width;
            int line_count   = ((cols * pixel_width) + BUS_WIDTH - 1) / BUS_WIDTH;
            int stride_count = ((((stride_in_x * pixel_width) >> 1) + BUS_WIDTH - 1) / BUS_WIDTH) - 1;
            int loop_count   = (((stride_in_x * rows * pixel_width) >> 1) + BUS_WIDTH - 1) / BUS_WIDTH;

            for (int i = 0; i < rows; i++) {
// clang-format off
#pragma HLS loop_tripcount max=4320
// clang-format on
#pragma HLS loop_flatten off
                for (int x = 0; x < (WidthInBytes + (BUS_WIDTH / 8) - 1) / (BUS_WIDTH / 8); x++) {

                    // clang-format off
#pragma HLS pipeline II=1
                    // clang-format on
                    ap_uint<BUS_WIDTH> fb_pix = _uvptr[offsetUV + x];
                    strm << fb_pix;
                }

                offsetUV += (stride_in_x) / (BUS_WIDTH / 8); //StrideInBytes/(BUS_WIDTH/8);
            }
            xf::cv::accel_utils au;
            au.hlsStrm2xfMat<BUS_WIDTH, XF_8UC2, MAX_IN_HEIGHT / 2, MAX_IN_WIDTH / 2, NPPC, (MAX_IN_HEIGHT * MAX_IN_WIDTH) / 2 / NPPC>(strm, _uv_mat, dstMat_cols_align_npc);
        }


    } // namespace cv
} // namespace xf

extern "C"
{
    void image_normalize(xf::cv::Mat<XF_8UC3, MAX_OUT_HEIGHT, MAX_OUT_WIDTH, NPC> &_src,
                         xf::cv::Mat<XF_8UC3, MAX_OUT_HEIGHT, MAX_OUT_WIDTH, NPC> &_dest,
                         int use_mean,
                         int mean_val)
    {
        ap_uint<24 * NPC> din;
        ap_uint<24 * NPC> dout;
        ap_uint<8 * NPC> r, g, b;
#pragma HLS INLINE
        for (int x = 0; x < _dest.rows; x++) {
            //clang-format off
            DYN_PRAGMA(HLS LOOP TRIPCOUNT max = MAX_OUT_HEIGHT)
            //clang-format on
            for (int y = 0; y < (_dest.cols >> XF_BITSHIFT(NPC)); y++) {
                //clang-format off
                DYN_PRAGMA(HLS LOOP TRIPCOUNT max = MAX_OUT_WIDTH)
                //clang-format on
                din = _src.read(x * (_src.cols >> XF_BITSHIFT(NPC)) + y);
                for (int i = 0; i < (1 << XF_BITSHIFT(NPC)); i++) {
#pragma HLS UNROLL
                    r(i * 8 + 7, i * 8) = din.range(23 + i * 24, 16 + i * 24);
                    g(i * 8 + 7, i * 8) = din.range(15 + i * 24, 8 + i * 24);
                    b(i * 8 + 7, i * 8) = din.range(7 + i * 24, 0 + i * 24);
                }
#pragma HLS PIPELINE
                for (int i = 0; i < (1 << XF_BITSHIFT(NPC)); i++) {
#pragma HLS UNROLL
                    if (use_mean == 0) {
                        dout(23 + i * 24, i * 24) = (r(i * 8 + 7, i * 8),
                                                     g(i * 8 + 7, i * 8),
                                                     b(i * 8 + 7, i * 8));
                    }
                    else {
                        dout(23 + i * 24, i * 24) = ((ap_int<9>(r(i * 8 + 7, i * 8)) - mean_val)(7, 0),
                                                     (ap_int<9>(g(i * 8 + 7, i * 8)) - mean_val)(7, 0),
                                                     (ap_int<9>(b(i * 8 + 7, i * 8)) - mean_val)(7, 0));
                    }
                }
                _dest.write(x * (_dest.cols >> XF_BITSHIFT(NPC)) + y, dout);
            }
        }
    }

    void resize_accel_rgb(ap_uint<AXI_WIDTH> *image_in_y,
                          ap_uint<AXI_WIDTH> *image_in_uv,
                          ap_uint<AXI_WIDTH> *image_out,
                          int width_in,
                          int height_in,
                          int stride_in_x,
                          int width_out,
                          int height_out,
                          int use_mean,
                          int mean)
    {
        // clang-format off
#pragma HLS INTERFACE m_axi port=image_in_y offset=slave bundle=image_in_y_gmem depth=139264
#pragma HLS INTERFACE m_axi port=image_in_uv offset=slave bundle=image_in_uv_out_rgb_gmem depth=69632
#pragma HLS INTERFACE m_axi port=image_out offset=slave bundle=image_in_uv_out_rgb_gmem depth=43200
#pragma HLS INTERFACE s_axilite port=image_in
#pragma HLS INTERFACE s_axilite port=image_out
#pragma HLS INTERFACE s_axilite port=width_in
#pragma HLS INTERFACE s_axilite port=height_in
#pragma HLS INTERFACE s_axilite port=stride_in_x
#pragma HLS INTERFACE s_axilite port=width_out
#pragma HLS INTERFACE s_axilite port=height_out
#pragma HLS INTERFACE s_axilite port=use_mean
#pragma HLS INTERFACE s_axilite port=mean
#pragma HLS INTERFACE s_axilite port=return
        // clang-format on

#pragma HLS DATAFLOW

        xf::cv::Mat<XF_8UC1, MAX_IN_HEIGHT, MAX_IN_WIDTH, NPC> in_mat_y(height_in, width_in);
        //clang-format off
        DYN_PRAGMA(HLS stream variable = in_mat_y.data depth = STREAM_DEPTH)
        //clang-format on

        xf::cv::Mat<XF_8UC2, MAX_IN_HEIGHT / 2, MAX_IN_WIDTH / 2, NPC_UV> in_mat_uv(height_in / 2, width_in / 2);
        //clang-format off
        DYN_PRAGMA(HLS stream variable = in_mat_uv.data depth = STREAM_DEPTH)
        //clang-format on

        xf::cv::Mat<XF_8UC3, MAX_IN_HEIGHT, MAX_IN_WIDTH, NPC> in_rgb(height_in, width_in);
        //clang-format off
        DYN_PRAGMA(HLS stream variable = in_rgb.data depth = STREAM_DEPTH)
        //clang-format on

        xf::cv::Mat<XF_8UC3, MAX_OUT_HEIGHT, MAX_OUT_WIDTH, NPC> out_rgb(height_out, width_out);
        //clang-format off
        DYN_PRAGMA(HLS stream variable = out_rgb.data depth = STREAM_DEPTH)
        //clang-format on

        xf::cv::Mat<XF_8UC3, MAX_OUT_HEIGHT, MAX_OUT_WIDTH, NPC> out_mat(height_out, width_out);
        //clang-format off
        DYN_PRAGMA(HLS stream variable = out_mat.data depth = STREAM_DEPTH)
        //clang-format on

        // These are in a separate namespace to work around an HLS synthesis bug
        // where multiple instantiations of the array to mat function with different
        // template parameters results in a compile-time error.
        xf::cv::Yptr2Mat<AXI_WIDTH, NPC>(stride_in_x, image_in_y, in_mat_y);
        xf::cv::UVptr2Mat<AXI_WIDTH, NPC_UV>(stride_in_x, image_in_uv, in_mat_uv);

        xf::cv::nv122bgr<XF_8UC1, XF_8UC2, XF_8UC3, MAX_IN_HEIGHT, MAX_IN_WIDTH, NPC, NPC_UV>(in_mat_y, in_mat_uv, in_rgb);

        xf::cv::resize<XF_INTERPOLATION_AREA,
                       XF_8UC3,
                       MAX_IN_HEIGHT,
                       MAX_IN_WIDTH,
                       MAX_OUT_HEIGHT,
                       MAX_OUT_WIDTH,
                       NPC,
                       MAX_DOWN_SCALE>(in_rgb, out_rgb);
        image_normalize(out_rgb, out_mat, use_mean, mean);

        xf::cv::xfMat2Array<AXI_WIDTH, XF_8UC3, MAX_OUT_HEIGHT, MAX_OUT_WIDTH, NPC>(out_mat, image_out);
    }
}
