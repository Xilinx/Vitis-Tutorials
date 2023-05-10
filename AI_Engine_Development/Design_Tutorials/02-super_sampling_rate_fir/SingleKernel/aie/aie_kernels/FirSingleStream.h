/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
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
