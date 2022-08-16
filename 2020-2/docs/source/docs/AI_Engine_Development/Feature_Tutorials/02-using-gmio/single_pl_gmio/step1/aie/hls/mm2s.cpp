/**********
© Copyright 2020 Xilinx, Inc.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**********/
#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>


extern "C" {

void mm2s(const ap_int<128>* mem, hls::stream<ap_axis<128, 0, 0, 0>  >& s) {
#pragma HLS INTERFACE m_axi port=mem offset=slave bundle=gmem

#pragma HLS interface axis port=s

#pragma HLS INTERFACE s_axilite port=mem bundle=control
#pragma HLS interface s_axilite port=return bundle=control

	for(int i = 0; i < 128; i++) {//2048 Bytes per run
#pragma HLS PIPELINE II=1 
		ap_axis<128, 0, 0, 0> x;
		x.data=mem[i];
		x.keep=-1;
		x.last=0;
		s.write(x);
	}

}

}
