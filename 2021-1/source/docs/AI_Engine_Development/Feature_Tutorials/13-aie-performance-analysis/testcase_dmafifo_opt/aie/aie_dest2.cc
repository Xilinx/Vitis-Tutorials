/**********
© Copyright 2021 Xilinx, Inc.
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
#include <adf.h>
__attribute__ ((noinline)) void aie_dest2(input_stream_int32 * __restrict in, input_window_int32 * __restrict inm, 
        output_stream_int32 * __restrict outm){
	v4int32 tmp;
	v4int32 tmp2;
	for(int i=0;i<128;i++)
	chess_prepare_for_pipelining
	{
		tmp=readincr_v4(in);
		tmp2=window_readincr_v4(inm);
		writeincr_v4(outm,tmp+tmp2);
		tmp=readincr_v4(in);
		tmp2=window_readincr_v4(inm);
		writeincr_v4(outm,tmp+tmp2);
	}
}

