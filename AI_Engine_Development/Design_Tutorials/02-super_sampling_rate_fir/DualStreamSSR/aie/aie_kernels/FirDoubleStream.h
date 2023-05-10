/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

 #pragma once

#include <adf.h>

namespace DoubleStream {


template<int NSamples,int ShiftAcc,bool DiscardSample,bool SwapRead>
class FIR_MultiKernel_cout {
private:
	alignas(32) cint16 weights[4];
	alignas(32) cint16 delay_line[16];

public:
	FIR_MultiKernel_cout(const cint16 (&taps)[4])
	{
		for(int i=0;i<4;i++) weights[i] = taps[i];
		for(int i=0;i<16;i++) delay_line[i] = (cint16){0,0};
	};

	void filter(input_stream_cint16*  restrict sin1,input_stream_cint16*  restrict sin2,output_stream_cacc48*  cout);

	static void registerKernelClass()
	{
		REGISTER_FUNCTION(FIR_MultiKernel_cout::filter);
	};
};


template<int NSamples,int ShiftAcc,bool DiscardSample,bool SwapRead>
class FIR_MultiKernel_cincout {
private:
	alignas(32) cint16 weights[4];
	alignas(32) cint16 delay_line[16];

public:
	FIR_MultiKernel_cincout(const cint16 (&taps)[4])
	{
		for(int i=0;i<4;i++) weights[i] = taps[i];
		for(int i=0;i<16;i++) delay_line[i] = (cint16){0,0};
	};

	void filter(input_stream_cint16*  restrict sin1,input_stream_cint16*  restrict sin2,input_stream_cacc48*  cin,output_stream_cacc48*  cout);

	static void registerKernelClass()
	{
		REGISTER_FUNCTION(FIR_MultiKernel_cincout::filter);
	};
};


template<int NSamples,int ShiftAcc,bool DiscardSample,bool SwapRead>
class FIR_MultiKernel_cin {
private:
	alignas(32) cint16 weights[4];
	alignas(32) cint16 delay_line[16];

public:
	FIR_MultiKernel_cin(const cint16 (&taps)[4])
	{
		for(int i=0;i<4;i++) weights[i] = taps[i];
		for(int i=0;i<16;i++) delay_line[i] = (cint16){0,0};
	};

	void filter(input_stream_cint16*  restrict sin1,input_stream_cint16*  restrict sin2,input_stream_cacc48*  cin,output_stream_cint16*  restrict sout1, output_stream_cint16*  restrict sout2);

	static void registerKernelClass()
	{
		REGISTER_FUNCTION(FIR_MultiKernel_cin::filter);
	};
};


template<int Delay>
void FIRinit();



}
