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
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>

void vec_incr(input_window<int32>* data,output_window<int32>* out){
	aie::vector<int32,16> vec1=aie::broadcast<int32>(1);
	for(int i=0;i<16;i++)
	chess_prepare_for_pipelining
	chess_loop_range(4,)
	{
		aie::vector<int32,16> vdata=window_readincr_v<16>(data);
		aie::vector<int32,16> vresult=aie::add(vdata,vec1);
		window_writeincr(out,vresult);
	}
	aie::tile tile=aie::tile::current();
	unsigned long long time=tile.cycles();//cycle counter of the AI Engine tile
	window_writeincr(out,time);
}
