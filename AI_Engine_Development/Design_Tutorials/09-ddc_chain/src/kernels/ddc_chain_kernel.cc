// 
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
// 

#include "ddc_chain_kernel.h"
#include "ddc_kernel_stage.h"

// -------------------------------------------------------------
// channle filter
// -------------------------------------------------------------
ddc_chain_kernel::ddc_chain_kernel( cint16 (&ovlp_init)[OVLP_SIZE], cint16 (&dmBufA_init)[DDC_LTE_SAMPLES_PERCH4D] )
  : ddcchanfir_ovlp(ovlp_init), dmBufA_DDC(dmBufA_init)
{
	aie::set_rounding(aie::rounding_mode::symmetric_inf);
	aie::set_saturation(aie::saturation_mode::saturate);
};


void ddc_chain_kernel::run(input_buffer<cint16> & __restrict cb_input,output_buffer<cint16> & __restrict cb_output)
{
	auto inputw_lItr = aie::begin_vector<8>(cb_input);
    aie::vector<int32,8> hdr = aie::vector_cast<int32>(aie::vector<cint16,8>(*inputw_lItr++));
	cint16_t *xbuff = ((cint16_t *)(cb_input.data())); xbuff+=8;
	cint16_t *ybuff = ((cint16_t *)cb_output.data());

	v8cint16 * xbuf    = (v8cint16 *)xbuff;
	v4cint16 * ybuf    = (v4cint16 *)ybuff;

    //--------------------------------------------
	// Set pointers to internal buffers
	//--------------------------------------------
	cint16_t * restrict dmBufA_ = dmBufA_DDC;

	buffer_internal firLng_buff_; buffer_init(&firLng_buff_,(v8cint16 *)ddcchanfir_ovlp);
	buffer_internal fir89_buff0_; buffer_init(&fir89_buff0_,(v8cint16 *)ddcchanfir_ovlp+32);
	buffer_internal fir89_buff1_; buffer_init(&fir89_buff1_,(v8cint16 *)ddcchanfir_ovlp+48);
	buffer_internal fir89_buff2_; buffer_init(&fir89_buff2_,(v8cint16 *)ddcchanfir_ovlp+64);
	buffer_internal fir89_buff3_; buffer_init(&fir89_buff3_,(v8cint16 *)ddcchanfir_ovlp+80);
	buffer_internal fir89_buff4_; buffer_init(&fir89_buff4_,(v8cint16 *)ddcchanfir_ovlp+96);

	buffer_internal * restrict firLng_buf ={&firLng_buff_};
	buffer_internal * restrict fir89_ovlp[5]= {&fir89_buff0_,&fir89_buff1_,&fir89_buff2_,&fir89_buff3_,&fir89_buff4_};

	v8cint16 * restrict hb23_ovlp = (v8cint16 *)ddcchanfir_ovlp+112;

	const v16int16 * restrict coeff_fir89  =  (const v16int16 *)coeffs_fir89;
	const v16int16 * restrict coeff_fir199 =  (const v16int16 *)coeffs_fir199;
	v4cint16 *dmbufa = (v4cint16*)dmBufA_;
	v8cint16 *abuf =  (v8cint16*)dmBufA_;

	char mode = hdr.get(0)&0x3;
	if ((hdr.get(0)>>16)&1){

		v8cint16 *pt = (v8cint16 *)ddcchanfir_ovlp;

		for(unsigned k=0; k<((DDC199_BUFF_SIZE)*DDC_NCH_NR+(DDC89_BUFF_SIZE+HBFD23_BUFF_SIZE)*DDC_NCH_LTE); ++k){
			*pt++ = null_v8cint16();
		}
	}

	// start process NR channle
	if(mode==1){

		// only one NR carrier
		fir_199t_sym_buf(3,xbuf,ybuf,firLng_buf,coeff_fir199,DDC_NR_SAMPLES_PERCH);

	}else{

		// filter the LTE carriers
		int cnt_pos=3;

		// loop for LTE carriers
		for(char carrid=0; carrid<DDC_NCH_LTE; carrid++)
	    chess_flatten_loop
		{
			//x-> a
			fir_23t_sym_hb_2d(xbuf,dmbufa,hb23_ovlp,DDC_LTE_SAMPLES_PERCH4D);hb23_ovlp+=3; xbuf+=16;
			//a->y
			fir_89t_sym_buf(cnt_pos, abuf,ybuf,fir89_ovlp[carrid],coeff_fir89,DDC_LTE_SAMPLES_PERCH4D); cnt_pos+=4; ybuf+=16;
		}
	}
}
