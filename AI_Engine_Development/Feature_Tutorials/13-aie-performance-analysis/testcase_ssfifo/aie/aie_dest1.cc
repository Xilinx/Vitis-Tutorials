/**********
© Copyright 2021-2022 Xilinx, Inc.
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
__attribute__ ((noinline)) void aie_dest1(input_window<int32> *in, 
        output_stream<int32> *out, output_window<int32> *outm){
	aie::vector<int32,4> tmp;
	for(int i=0;i<8;i++)
	chess_prepare_for_pipelining
	{
		tmp=window_readincr_v<4>(in);
		writeincr(out,tmp);
		window_writeincr(outm,tmp);
	}
}

