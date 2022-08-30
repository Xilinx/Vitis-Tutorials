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
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include "aie_api/utils.hpp"

alignas(aie::vector_decl_align) int32 weights[8] = {1,2,3,4,5,6,7,8};

void vectorized_weighted_sum_with_margin(input_window<int32> * restrict in, output_window<int32> * restrict out)
{
	aie::vector<int32,8> coeffs = aie::load_v<8>(weights);

	aie::vector<int32,16> data;
	aie::accum<acc80,4> acc;
	aie::vector<int32,8> dataout;
	data.insert(0,window_readincr_v<8>(in));

	for(unsigned i=0; i<256/16; i++)
		chess_prepare_for_pipelining
		chess_loop_range(4,32)
	{
		data.insert(1,window_readincr_v<8>(in));
		acc=aie::sliding_mul<4,8>(coeffs,0,data,1);
		dataout.insert(0,acc.to_vector<int32>());
		acc=aie::sliding_mul<4,8>(coeffs,0,data,5);
		dataout.insert(1,acc.to_vector<int32>());
		window_writeincr(out,dataout);

		data.insert(0,window_readincr_v<8>(in));
		acc=aie::sliding_mul<4,8>(coeffs,0,data,9);
		dataout.insert(0,acc.to_vector<int32>());
		acc=aie::sliding_mul<4,8>(coeffs,0,data,13);
		dataout.insert(1,acc.to_vector<int32>());
		window_writeincr(out,dataout);
	}
}
