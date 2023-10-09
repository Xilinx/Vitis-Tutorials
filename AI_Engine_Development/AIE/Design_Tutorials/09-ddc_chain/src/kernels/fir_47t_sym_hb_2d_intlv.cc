//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "fir_47t_sym_hb_2d_intlv.h"

fir_47t_sym_hb_2d::fir_47t_sym_hb_2d(void)
{
	aie::set_rounding(aie::rounding_mode::symmetric_inf);
	aie::set_saturation(aie::saturation_mode::saturate);
}


void fir_47t_sym_hb_2d::run( input_buffer<cint16> & __restrict cb_input, output_buffer<cint16> & __restrict cb_output)
{
    const int HBF47D_SHIFT = 15;
    const int sft = HBF47D_SHIFT;
    aie::vector<cint16,32> sbuff;
    const unsigned output_samples = HBF47D_OUTPUT_SAMPLES;
    const unsigned LSIZE = (output_samples / 4 / 4)-1;

    aie::vector<int16,16> coe0 = *(v16int16*)coeffs_hb47_2d;
	#ifndef __X86SIM__
	alignas(32) static  cint16 ovlp_buf[48];
	#else
	alignas(32) static thread_local cint16 ovlp_buf[48];
	#endif

    //bypass ctrl word to next kernel
	auto inputw_lItr = aie::begin_vector_random_circular<8>(cb_input);
	auto cb_odd_lItr = aie::begin_vector_random_circular<4>(cb_input);
	auto outputwItr = aie::begin_vector<4>(cb_output);
	*outputwItr++ = (*inputw_lItr).extract<4>(0);
	*outputwItr++ = (*inputw_lItr).extract<4>(1);
	inputw_lItr++;
	cb_odd_lItr+=2;          // Need to do this to keep the iterators synchronized (two v4's equals one v8)

    v8cint16 *ovlp    = (v8cint16 *)ovlp_buf;
	v4cint16 *ovlp_odd= (v4cint16 *)ovlp_buf+7; //ovlp_odd:0->25

	aie::accum<cacc48,4> acc0;
	aie::accum<cacc48,4> acc1;
	aie::accum<cacc48,4> acc2;
	aie::accum<cacc48,4> acc3;

	cb_odd_lItr+=2;

	//ovlp data processing
	sbuff.insert(0, aie::vector<cint16,8>(*ovlp));ovlp=ovlp+2;     //ovlp: 0->16      // |00:14|XX:XX|XX:XX|XX:XX|
	sbuff.insert(1, aie::vector<cint16,8>(*ovlp));ovlp=ovlp+2;     //ovlp:16->32      // |00:14|16:30|XX:XX|XX:XX|
	sbuff.insert(2, aie::vector<cint16,8>(*ovlp));                 //ovlp:32->32      // |00:14|16:30|32:46|XX:XX|
    acc0 = aie::sliding_mac_sym<4,8,1,1,1>(aie::accum<cacc48,4>(ups(*ovlp_odd,sft)),coe0,8,sbuff,9);ovlp_odd=ovlp_odd+3;//ovlp_odd:25->33
    acc1 = aie::sliding_mac_sym<4,8,1,1,1>(aie::accum<cacc48,4>(ups(*ovlp_odd,sft)),coe0,8,sbuff,13);ovlp_odd=ovlp_odd+1;//ovlp_odd:33->41

	sbuff.insert(3, aie::vector<cint16,8>(*inputw_lItr));inputw_lItr += 2;    //cb:48->64      // |00:14|16:30|32:46|48:62|
	acc0  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(acc0,coe0,0,sbuff,1,24);
	acc1  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(acc1,coe0,0,sbuff,5,28);
	acc0  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(acc0,coe0,4,sbuff,5,20);*outputwItr++ = acc0.to_vector<cint16>(sft);
	acc1  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(acc1,coe0,4,sbuff,9,24);*outputwItr++ = acc1.to_vector<cint16>(sft);

	sbuff.insert(0, aie::vector<cint16,8>(*inputw_lItr));inputw_lItr -= 2;    //cb:64->48      // |64:78|16:30|32:46|48:62|
	acc2  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(aie::accum<cacc48,4>(ups(*ovlp_odd,sft)),coe0,0,sbuff,9,0);                          //cb_odd:41->49
	acc3  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(aie::accum<cacc48,4>(ups(*cb_odd_lItr,sft)),coe0,0,sbuff,13,4);cb_odd_lItr+=1;//cb_odd:49->57
	acc2  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(acc2,coe0,4,sbuff,13,28);

	sbuff.insert(1, aie::vector<cint16,8>(*inputw_lItr));inputw_lItr += 2;    //cb:48->64      // |64:78|48:62|32:46|48:62|
	acc3  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(acc3,coe0,4,sbuff,17,0);

	sbuff.insert(0, aie::vector<cint16,8>(*ovlp++));//ovlp:32->33                                   //cb:64->64      // |32:46|48:62|32:46|48:62|
	acc2  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(acc2,coe0,8,sbuff,17,24);*outputwItr++ = acc2.to_vector<cint16>(sft);
	acc3  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(acc3,coe0,8,sbuff,21,28);*outputwItr++ = acc3.to_vector<cint16>(sft);


    for ( unsigned l=0; l<LSIZE; ++l )
        chess_prepare_for_pipelining
    {
		sbuff.insert(2, aie::vector<cint16,8>(*inputw_lItr));inputw_lItr += 2;    //cb:32->48      // |00:14|16:30|32:46|XX:XX|
		acc0  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(aie::accum<cacc48,4>(ups(*cb_odd_lItr,sft)),coe0,8,sbuff,9,16);cb_odd_lItr+=3;//cb_odd:25->33
		acc1  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(aie::accum<cacc48,4>(ups(*cb_odd_lItr,sft)),coe0,8,sbuff,13,20);cb_odd_lItr+=1;//cb_odd:25->33

        sbuff.insert(3, aie::vector<cint16,8>(*inputw_lItr));inputw_lItr += 2;    //cb:48->64      // |00:14|16:30|32:46|48:62|
		acc0  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(acc0,coe0,0,sbuff,1,24);
		acc1  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(acc1,coe0,0,sbuff,5,28);
		acc0  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(acc0,coe0,4,sbuff,5,20);*outputwItr++ = acc0.to_vector<cint16>(sft);
		acc1  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(acc1,coe0,4,sbuff,9,24);*outputwItr++ = acc1.to_vector<cint16>(sft);

		sbuff.insert(0, aie::vector<cint16,8>(*inputw_lItr));inputw_lItr -= 2;    //cb:64->48      // |64:78|16:30|32:46|48:62|
		acc2  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(aie::accum<cacc48,4>(ups(*cb_odd_lItr,sft)),coe0,0,sbuff,9,0);cb_odd_lItr+=3;//cb_odd:41->49
		acc3  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(aie::accum<cacc48,4>(ups(*cb_odd_lItr,sft)),coe0,0,sbuff,13,4);cb_odd_lItr+=1;//cb_odd:49->57
		acc2  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(acc2,coe0,4,sbuff,13,28);

		sbuff.insert(1, aie::vector<cint16,8>(*inputw_lItr));inputw_lItr -= 2;    //cb:48->32      // |64:78|48:62|32:46|48:62|
		acc3  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(acc3,coe0,4,sbuff,17,0);

		sbuff.insert(0, aie::vector<cint16,8>(*inputw_lItr));inputw_lItr += 4;    //cb:32->64      // |32:46|48:62|32:46|48:62|
		acc2  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(acc2,coe0,8,sbuff,17,24);*outputwItr++ = acc2.to_vector<cint16>(sft);
		acc3  = aie::sliding_mul_sym_ops<4,8,1,1,1,int16,cint16,cacc48>::mac_sym(acc3,coe0,8,sbuff,21,28);*outputwItr++ = acc3.to_vector<cint16>(sft);

    }

	inputw_lItr++;
	*ovlp-- = *inputw_lItr--;
	*ovlp-- = *inputw_lItr--;
	*ovlp-- = *inputw_lItr--;
	*ovlp-- = *inputw_lItr--;
	*ovlp-- = *inputw_lItr--;
	*ovlp-- = *inputw_lItr--;

}
