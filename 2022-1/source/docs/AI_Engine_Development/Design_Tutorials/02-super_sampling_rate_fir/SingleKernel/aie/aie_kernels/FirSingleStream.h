/*
 * (c) Copyright 2021-2022 Xilinx, Inc. All rights reserved.
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
 *
 */
 #pragma once

#include <adf.h>

namespace SingleStream {

template<int NSamples,int ShiftAcc>
class FIR_SingleStream {
private:
	alignas(32) cint16 weights[32];
	alignas(32) cint16 delay_line[32];

public:
	FIR_SingleStream(const cint16 (&taps)[32])
	{
		for(int i=0;i<32;i++)
		{
			weights[i] = taps[i];
			delay_line[i] = (cint16){0,0};
		}
	};

	void filter(input_stream_cint16*  sin,output_stream_cint16*  sout);

	static void registerKernelClass()
	{
		REGISTER_FUNCTION(FIR_SingleStream::filter);
	};
};

}
