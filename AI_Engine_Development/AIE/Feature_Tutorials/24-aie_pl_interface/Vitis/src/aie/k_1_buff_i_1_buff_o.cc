/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/
#include <adf.h>
#include "aie_api/aie.hpp"
#include "aie_api/utils.hpp"
#include "aie_api/aie_adf.hpp"
#include "k_1_buff_i_1_buff_o.h"

#define FIR_LEN 16
#define SRS_SHIFT 15

using namespace adf;

alignas(32) int16_t taps[FIR_LEN] = {-1000, 2000, -3000, 4000, -5000, 6000, -7000, 8000, 8000, -7000, 6000, -5000, 4000, -3000, 2000, -1000};

void k_1_buff_i_1_buff_o(in_type & in,out_type & out) {

    constexpr unsigned Lanes=4, Points=16, CoeffStep=1, DataStepXY=1;
	using mulop = aie::sliding_mul_sym_xy_ops<Lanes, Points, CoeffStep, DataStepXY,int16,cint16>;

	aie::vector<cint16,32> sbuff;
	aie::vector<cint16,8> dataout;

    auto inIter=aie::begin_vector_random_circular<8>(in);
	auto outIter=aie::begin_vector_random_circular<8>(out);

    const unsigned samples  = out.size();
	const int shift         = SRS_SHIFT ;

	const aie::vector<int16,FIR_LEN> coeff = aie::load_v<FIR_LEN>((int16 *)taps) ;

	const unsigned LSIZE = (samples / 4); // assuming # of samples is integer power of 2 and >= 64

	for ( unsigned i=0; i<LSIZE; i+=2)
	{
		sbuff.insert(0, *inIter++); 	// 00++|04++|____|____    ____|____|____|____
		sbuff.insert(1, *inIter++);		// 00..|04..|08++|12++    ____|____|____|____
		sbuff.insert(2, *inIter); 		// 00..|04..|08..|12..    16++|20++|____|____


		auto acc = mulop::mul_sym(coeff,0,sbuff,0); // o0..3  =f(c0..7, d0..10,  d8..18)
		dataout.insert(0, acc.to_vector<cint16>(shift));

		acc = mulop::mul_sym(coeff,0,sbuff,4); // o4..7  =f(c0..7, d4..14,  d12..22)
		dataout.insert(1, acc.to_vector<cint16>(shift));
		*outIter++=dataout;

		inIter=inIter-1;
	}
}
