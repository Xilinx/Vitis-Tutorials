/*
 * (c) Copyright 2020 Xilinx, Inc. All rights reserved.
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
