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
#include "adf.h"

int32_t chess_storage(%chess_alignof(v8int32)) weights[8] = {1,2,3,4,5,6,7,8};

void vectorized_weighted_sum_with_margin(input_window_int32 * restrict in, output_window_int32 * restrict out)
{
	v8int32 coeffs = *(v8int32*) weights;

	v16int32 data = undef_v16int32();
	v4acc80 acc;
	v8int32 dataout = undef_v8int32();
	data = upd_w(data,0,window_readincr_v8(in));

	for(unsigned i=0; i<256/8; i++)
		chess_prepare_for_pipelining
		chess_loop_range(4,32)
	{

		acc = lmul4(    data, 1,0x3210,1,coeffs,0,0x0000,1);
		data = upd_w(data,1,window_read_v8(in));
		acc = lmac4(acc,data, 3,0x3210,1,coeffs,2,0x0000,1);
		acc = lmac4(acc,data, 5,0x3210,1,coeffs,4,0x0000,1);
		acc = lmac4(acc,data, 7,0x3210,1,coeffs,6,0x0000,1);

		dataout = upd_v(dataout,0,srs(acc,0));

		acc = lmul4(    data, 5,0x3210,1,coeffs,0,0x0000,1);
		acc = lmac4(acc,data, 7,0x3210,1,coeffs,2,0x0000,1);
		data = upd_w(data,0,window_readincr_v8(in));
		acc = lmac4(acc,data, 9,0x3210,1,coeffs,4,0x0000,1);
		acc = lmac4(acc,data,11,0x3210,1,coeffs,6,0x0000,1);

		dataout = upd_v(dataout,1,srs(acc,0));

		window_writeincr(out,dataout);

	}

}
