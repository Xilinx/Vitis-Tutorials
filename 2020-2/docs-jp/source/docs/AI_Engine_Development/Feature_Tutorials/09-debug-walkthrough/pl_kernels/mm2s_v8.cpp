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

#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

const int STREAM_NUM=8;
const int STREAM_WIDTH=64;

extern "C" {

void mm2s_v8(ap_int<STREAM_WIDTH*STREAM_NUM>* mem, hls::stream<ap_axis<STREAM_WIDTH, 0, 0, 0>  > &s0, hls::stream<ap_axis<STREAM_WIDTH, 0, 0, 0>  > &s1, hls::stream<ap_axis<STREAM_WIDTH, 0, 0, 0>  > &s2, hls::stream<ap_axis<STREAM_WIDTH, 0, 0, 0>  > &s3, hls::stream<ap_axis<STREAM_WIDTH, 0, 0, 0>  > &s4, hls::stream<ap_axis<STREAM_WIDTH, 0, 0, 0>  > &s5, hls::stream<ap_axis<STREAM_WIDTH, 0, 0, 0>  > &s6, hls::stream<ap_axis<STREAM_WIDTH, 0, 0, 0>  > &s7, int size) {
#pragma HLS INTERFACE m_axi port=mem offset=slave bundle=gmem

#pragma HLS INTERFACE s_axilite port=mem bundle=control
#pragma HLS INTERFACE s_axilite port=size bundle=control
#pragma HLS interface s_axilite port=return bundle=control

	for(int i = 0; i < size; i++) {
#pragma HLS PIPELINE II=1
		ap_int<STREAM_WIDTH*STREAM_NUM> data=mem[i];
		ap_axis<STREAM_WIDTH, 0, 0, 0> x[STREAM_NUM];
		for(int j=0;j<STREAM_NUM;j++){
			x[j].data=data(STREAM_WIDTH*j+STREAM_WIDTH-1,STREAM_WIDTH*j);
			x[j].keep=-1;
			x[j].last=0;
		}
		s0.write(x[0]);
		s1.write(x[1]);
		s2.write(x[2]);
		s3.write(x[3]);
		s4.write(x[4]);
		s5.write(x[5]);
		s6.write(x[6]);
		s7.write(x[7]);
	}

}

}
