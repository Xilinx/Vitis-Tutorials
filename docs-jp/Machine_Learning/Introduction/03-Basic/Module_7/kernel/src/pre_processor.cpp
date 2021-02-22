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
#include "pre_processor.h"

namespace xf {
namespace cv {
// =====================================================================================
// Function to split xf::cv::Mat into __ROI_PARALLEL_PORTS__ xf::cv::Mat
// =====================================================================================
template<int TYPE, int ROWS, int COLS, int NPPC>
void DuplicateMat_rpp(xf::cv::Mat<TYPE, ROWS, COLS, NPPC> &in_mat,
		xf::cv::Mat<TYPE, ROWS, COLS, NPPC> &out_mat00,
		xf::cv::Mat<TYPE, ROWS, COLS, NPPC> &out_mat01) {
#pragma HLS INLINE OFF

	const int c_TRIP_COUNT = ROWS * (COLS >> XF_BITSHIFT(NPPC));
	int loopcount = in_mat.rows * (in_mat.cols >> XF_BITSHIFT(NPPC));

	for (int i = 0; i < loopcount; i++) {
#pragma HLS LOOP_TRIPCOUNT min = c_TRIP_COUNT max = c_TRIP_COUNT
#pragma HLS pipeline II                           = 1
		XF_TNAME(TYPE, NPPC)
		tmp = in_mat.read(i);

		out_mat00.write(i, tmp);
		out_mat01.write(i, tmp);

	}

} // End of DuplicateMat_rpp()
// =====================================================================================

void kernel(ap_uint<AXI_WIDTH> *image_in, ap_uint<AXI_WIDTH> *image_out,
		ap_uint<AXI_WIDTH> *image_out_full, int width_in, int height_in,
		int width_out, int height_out) {
#pragma HLS INLINE OFF

	xf::cv::Mat < XF_16UC1, MAX_IN_HEIGHT, MAX_IN_WIDTH, NPC
			> in_mat(height_in, width_in);
#pragma HLS stream variable = in_mat.data depth = 2

	xf::cv::Mat < XF_8UC3, MAX_IN_HEIGHT, MAX_IN_WIDTH, NPC
			> in_rgb(height_in, width_in);
#pragma HLS stream variable = in_rgb.data depth = 2

	xf::cv::Mat < XF_8UC3, MAX_IN_HEIGHT, MAX_IN_WIDTH, NPC
			> in_rgb_copy0(height_in, width_in);
#pragma HLS stream variable = in_rgb_copy0.data depth = 2

	xf::cv::Mat < XF_8UC3, MAX_IN_HEIGHT, MAX_IN_WIDTH, NPC
			> in_rgb_copy1(height_in, width_in);
#pragma HLS stream variable = in_rgb_copy1.data depth = 2

	xf::cv::Mat < XF_8UC3, MAX_IN_HEIGHT, MAX_IN_WIDTH, NPC
			> out_rgb0(height_out, width_out);
#pragma HLS stream variable = out_rgb0.data depth = 2

	xf::cv::Mat < XF_8UC3, MAX_IN_HEIGHT, MAX_IN_WIDTH, NPC
			> out_rgb1(1080, 1920);
#pragma HLS stream variable = out_rgb1.data depth = 2

#pragma HLS DATAFLOW

	xf::cv::Array2xfMat<AXI_WIDTH, XF_16UC1, MAX_IN_HEIGHT, MAX_IN_WIDTH, NPC>(
			image_in, in_mat);

//        convert_color(is_uyvy, in_mat, in_rgb);
	xf::cv::uyvy2bgr<XF_16UC1, XF_8UC3, MAX_IN_HEIGHT, MAX_IN_WIDTH, NPC>(
			in_mat, in_rgb);
	printf("Color Conversion Complete\n");

	xf::cv::DuplicateMat_rpp<XF_8UC3, MAX_IN_HEIGHT, MAX_IN_WIDTH, NPC>(in_rgb,
			in_rgb_copy0, in_rgb_copy1);
	printf("RGB Frame Duplicated\n");

	xf::cv::resize<XF_INTERPOLATION_AREA, XF_8UC3,
	MAX_IN_HEIGHT,
	MAX_IN_WIDTH,
	MAX_IN_HEIGHT,
	MAX_IN_WIDTH,
	NPC,
	MAX_DOWN_SCALE>(in_rgb_copy0, out_rgb0);

	xf::cv::resize<XF_INTERPOLATION_AREA, XF_8UC3,
	MAX_IN_HEIGHT,
	MAX_IN_WIDTH,
	MAX_IN_HEIGHT,
	MAX_IN_WIDTH,
	NPC,
	MAX_DOWN_SCALE>(in_rgb_copy1, out_rgb1);

	xf::cv::xfMat2Array<AXI_WIDTH, XF_8UC3, MAX_OUT_HEIGHT, MAX_OUT_WIDTH, NPC>(
			out_rgb0, image_out);
	xf::cv::xfMat2Array<AXI_WIDTH, XF_8UC3, MAX_OUT_HEIGHT, MAX_OUT_WIDTH, NPC>(
			out_rgb1, image_out_full);
}

} // namespace cv
} // namespace xf

extern "C" {
void pre_processor(ap_uint<AXI_WIDTH> *image_in, ap_uint<AXI_WIDTH> *image_out,
		ap_uint<AXI_WIDTH> *image_out_full, int width_in, int height_in,
		int width_out, int height_out) {
#pragma HLS INTERFACE m_axi port=image_in        bundle=image_in_gmem
#pragma HLS INTERFACE m_axi port=image_out       bundle=image_out_gmem
#pragma HLS INTERFACE m_axi port=image_out_full  bundle=image_out_full_gmem
#pragma HLS INTERFACE s_axilite port=width_in   bundle=control
#pragma HLS INTERFACE s_axilite port=height_in  bundle=control
#pragma HLS INTERFACE s_axilite port=width_out  bundle=control
#pragma HLS INTERFACE s_axilite port=height_out bundle=control
#pragma HLS INTERFACE s_axilite port = return bundle = control
#pragma HLS INTERFACE s_axilite port=image_in        bundle=control
#pragma HLS INTERFACE s_axilite port=image_out       bundle=control
#pragma HLS INTERFACE s_axilite port=image_out_full  bundle=control
#pragma HLS INTERFACE s_axilite port=width_in             bundle=control


	kernel(image_in, image_out, image_out_full, (uint32_t) width_in,
			(uint32_t) height_in, (uint32_t) width_out, (uint32_t) height_out);

}
}
