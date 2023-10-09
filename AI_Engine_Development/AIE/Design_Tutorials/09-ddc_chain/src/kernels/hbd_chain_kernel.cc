// 
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
// 

#include "hbd_chain_kernel.h"
#include "ddc_kernel_stage.h"


// -------------------------------------------------------------
// channle filter
// -------------------------------------------------------------
hbd_chain_kernel::hbd_chain_kernel( cint16 (&hbfd_init)[OVLP_SIZE] ) : hbfd_ovlp(hbfd_init)
{
	aie::set_rounding(aie::rounding_mode::symmetric_inf);
	aie::set_saturation(aie::saturation_mode::saturate);
};


void hbd_chain_kernel::run(input_buffer<cint16> & __restrict cb_input,output_buffer<cint16> & __restrict cb_output)
{


    //bypass ctrl word to next kernel
	auto inputw_lItr = aie::begin_vector<8>(cb_input);
	aie::vector<cint16,8> in = *inputw_lItr++;
	aie::vector<int32,8> hdr = aie::vector_cast<int32>(in);
	
	char mode = hdr.get(0)&0x3;

	auto outputwItr = aie::begin_vector<8>(cb_output);
	*outputwItr++ = aie::vector_cast<cint16>(hdr);
	
	// initialize the overlap memory if the reset flag is set
	if ((hdr.get(0)>>16)&1){

		v8cint16 *pt = (v8cint16 *)hbfd_ovlp;

		for(unsigned k=0; k<(HBFD11_BUFF_SIZE*DDC_NCH_LTE); ++k){
			*pt++ = null_v8cint16();
		}
	}

	cint16_t *xbuff = (cint16_t *)cb_input.data();xbuff+=8;
	cint16_t *ybuff = (cint16_t *)cb_output.data();ybuff+=8;

	v8cint16 * xbuf    = (v8cint16 *)xbuff;
	v4cint16 * ybuf    = (v4cint16 *)ybuff;
	v8cint16 * ybuf_   = (v8cint16 *)ybuff;

	v8cint16 * restrict hb11_ovlp = (v8cint16 *)hbfd_ovlp;


	if (mode==0)
	{
		for(char carrid=0; carrid<DDC_NCH_LTE; carrid++)
		chess_flatten_loop
		{
			//x-> a
			fir_11t_sym_hb_2d_inv(xbuf,ybuf,hb11_ovlp,DDC_LTE_SAMPLES_PERCH2D);hb11_ovlp+=2;

			xbuf+=32;
			ybuf+=32;
		}
	}

	if (mode==1)
	{
		for(unsigned short l=0; l<32; ++l) *ybuf_++ = *xbuf++;
	}


}
