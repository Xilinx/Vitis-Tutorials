/**
* Copyright (C) 2019-2021 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/



#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include "data.h"

int size=5120;

extern "C" {

void stream_out(hls::stream<ap_axis<32, 0, 0, 0>  >& s) {

#pragma HLS INTERFACE axis port=s
#pragma HLS INTERFACE ap_ctrl_none port=return

	for(int i = 0; i < size; i+=2) {
		ap_axis<32, 0, 0, 0> x;
	        x.data(15,0) =  cint16Input[i];
	        x.data(32,16) =  cint16Input[i+1];
                x.keep=-1;
                x.last=0;
		s.write(x);
	}
}
}
