//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "demixer_5c_kernel0.h"

demixer_5c_kernel0::demixer_5c_kernel0(void)
{
	aie::set_rounding(aie::rounding_mode::symmetric_inf);
	aie::set_saturation(aie::saturation_mode::saturate);
}



void demixer_5c_kernel0::run( input_buffer<cint16> & __restrict cb_input, output_buffer<cint16> & __restrict cb_output)
{
	#ifndef __X86SIM__
 	static int32_t PHASE_INDEX00 = 0;
	static int32_t PHASE_INDEX01 = 0;
	static int32_t PHASE_INDEX02 = 0;
	static int32_t PHASE_INDEX03 = 0;
	static int32_t PHASE_INDEX04 = 0;
	alignas(32) static cint16 nco_fine_buf0[80];
	#else
	static thread_local int32_t PHASE_INDEX00 = 0;
	static thread_local int32_t PHASE_INDEX01 = 0;
	static thread_local int32_t PHASE_INDEX02 = 0;
	static thread_local int32_t PHASE_INDEX03 = 0;
	static thread_local int32_t PHASE_INDEX04 = 0;
	alignas(32) static thread_local cint16 nco_fine_buf0[80];
	#endif

	auto inputw_lItr = aie::begin_vector<8>(cb_input);
	aie::vector<cint16,8> in = *inputw_lItr++;
	aie::vector<int32,8> hdr = aie::vector_cast<int32>(in);
	//bypass the header
	auto outputwItr = aie::begin_vector<8>(cb_output);
	*outputwItr++ = aie::vector_cast<cint16>(hdr);

	if ((hdr.get(0)>>16)&1){
		PHASE_INDEX00 = 0;
		PHASE_INDEX01 = 0;
		PHASE_INDEX02 = 0;
		PHASE_INDEX03 = 0;
		PHASE_INDEX04 = 0;
	}
	uint32_t phase_step[5];

	char mode = hdr.get(0)&0x3;

	for(unsigned char i=0;i<5;i++) phase_step[i]=hdr.get(i+1);

    aie::vector<cint16,8> dds_rough  = aie::zeros<cint16,8>();

    aie::vector<cint16,16> vdata = aie::zeros<cint16,16>();

    aie::accum<cacc48,8> chess_storage(bm0) acc0a;
    aie::accum<cacc48,8> chess_storage(bm1) acc0b;
	aie::accum<cacc48,8> chess_storage(bm2) acc1;

	const unsigned input_samples = cb_input.size();

	const unsigned LSIZE = (input_samples / 8 /2);

	const int shift=16;

	// ----------------------------------
	// every data block size is N_blkin*8
	// ----------------------------------
	v8cint16 * restrict pi = (v8cint16 *)(cb_input.data()); pi+=1;    // pointing to the start of carrier
	v8cint16 * restrict po = (v8cint16 *)(cb_output.data()); po+=1;   // pointing to the start of carrier

	uint32_t phstp0=0;
	uint32_t phstp1=0;
	uint32_t phstp2=0;
	uint32_t phstp3=0;
	uint32_t phstp4=0;

	v8cint16 * buf_wr= (v8cint16 *) nco_fine_buf0;
	chess_protect_access v8cint16 * buf_rd= (v8cint16 *) nco_fine_buf0;

	aie::vector<cint16,16> buftmp;
	aie::vector<cint16,32> buf_fine;

	// initialize fine phase rotation buffer --------------------------------------
    for(unsigned char k=0; k<16;k++){buftmp.set(aie::sincos_complex(phstp0),15-k); phstp0 = chess_copy(phstp0 + phase_step[0]);}
	*buf_wr++ = buftmp.extract<8>(0);
	*buf_wr++ = buftmp.extract<8>(1);
    for(unsigned char k=0; k<16;k++){buftmp.set(aie::sincos_complex(phstp1),15-k); phstp1 = chess_copy(phstp1 + phase_step[1]);}
	*buf_wr++ = buftmp.extract<8>(0);
	*buf_wr++ = buftmp.extract<8>(1);
    for(unsigned char k=0; k<16;k++){buftmp.set(aie::sincos_complex(phstp2),15-k); phstp2 = chess_copy(phstp2 + phase_step[2]);}
	*buf_wr++ = buftmp.extract<8>(0);
	*buf_wr++ = buftmp.extract<8>(1);
    for(unsigned char k=0; k<16;k++){buftmp.set(aie::sincos_complex(phstp3),15-k); phstp3 = chess_copy(phstp3 + phase_step[3]);}
	*buf_wr++ = buftmp.extract<8>(0);
	*buf_wr++ = buftmp.extract<8>(1);
    for(unsigned char k=0; k<16;k++){buftmp.set(aie::sincos_complex(phstp4),15-k); phstp4 = chess_copy(phstp4 + phase_step[4]);}
	*buf_wr++ = buftmp.extract<8>(0);
	*buf_wr++ = buftmp.extract<8>(1);

	//----------------------------------------------------------------------------

	buf_fine.insert(0,aie::vector<cint16,8>(*buf_rd++));

	if (mode==1) {

		for (unsigned l=0; l<LSIZE; ++l)
			chess_prepare_for_pipelining
            chess_loop_range(8,)
		{
	        //f0
		    // calculate sin/cos values and save it to dds_out
            dds_rough.set(sincos(PHASE_INDEX00),0); PHASE_INDEX00+=phstp0;
			// multiply refined and coarse DDS values
			buf_fine.insert(1,aie::vector<cint16,8>(*buf_rd--));
			acc0a = aie::mul(aie::reverse(buf_fine.extract<8>(1)), dds_rough.get(0));
			vdata.insert(0, aie::vector<cint16,8>(*pi++));
			acc0b = aie::mul(aie::reverse(buf_fine.extract<8>(0)), dds_rough.get(0));

			// dot-multiply with the data
			vdata.insert(1, aie::vector<cint16,8>(*pi++));
			acc1 = aie::mul(vdata.extract<8>(0), aie::vector<cint16,8>(acc0a.to_vector<cint16>(15)));
	        *po++= acc1.to_vector<cint16>(shift);

			buf_fine.insert(0,aie::vector<cint16,8>(*buf_rd++));
			acc1 = aie::mul(vdata.extract<8>(1), aie::vector<cint16,8>(acc0b.to_vector<cint16>(15)));
	        *po++= acc1.to_vector<cint16>(shift);
		}
	}

	else {


		for (unsigned l=0; l<LSIZE; ++l)
			chess_prepare_for_pipelining
            chess_loop_range(8,)
	        //chess_unroll_loop(4)
		{
			vdata.insert(0, aie::vector<cint16,8>(*pi++));
			vdata.insert(1, aie::vector<cint16,8>(*pi++));
	        //f0
	        // calculate sin/cos values and save it to dds_out
            dds_rough.set(aie::sincos_complex(PHASE_INDEX00),0); PHASE_INDEX00+=phstp0;
	        // multiply refined and coarse DDS values
			buf_fine.insert(1,aie::vector<cint16,8>(*buf_rd++));
	        acc0a = mul8(buf_fine, 1, 0x02468ace, dds_rough, 0, 0x00000000);
			//acc0a = aie::mul(aie::filter_even(aie::reverse(buf_fine.extract<16>(0)),1), dds_rough.get(0)); // Functionally correct to line above, but causes degradation in throughput because of filter_even API.
	        acc0b = mul8(buf_fine, 0, 0x02468ace, dds_rough, 0, 0x00000000);
			// acc0b = aie::mul(aie::filter_odd(aie::reverse(buf_fine.extract<16>(0)),1), dds_rough.get(0)); // Functionally correct to line above, but causes degradation in throughput because of filter_odd API.

	        // dot-multiply with the data
			buf_fine.insert(2,aie::vector<cint16,8>(*buf_rd++));
	        acc1 = mul8( vdata, 0, 0xeca86420, acc0a.to_vector<cint16>(15), 0, 0x76543210);
			//acc1 = aie::mul(aie::filter_even(vdata,1), aie::vector<cint16,8>(acc0a.to_vector<cint16>(15))); // Functionally correct to line above, but causes degradation in throughput because of filter_even API.
	        *po++= acc1.to_vector<cint16>(shift);

	        acc1 = mul8( vdata, 1, 0xeca86420, acc0b.to_vector<cint16>(15), 0, 0x76543210);
			// acc1 = aie::mul(aie::filter_odd(vdata,1), aie::vector<cint16,8>(acc0b.to_vector<cint16>(15))); // Functionally correct to line above, but causes degradation in throughput because of filter_odd API.
	        *po  = acc1.to_vector<cint16>(shift); po+=DEMIXER_CARR1P;

	        //f1
	        // calculate sin/cos values and save it to dds_out
            dds_rough.set(aie::sincos_complex(PHASE_INDEX01),0); PHASE_INDEX01+=phstp1;
	        // multiply refined and coarse DDS values
			buf_fine.insert(3,aie::vector<cint16,8>(*buf_rd++));
	        acc0a = mul8(buf_fine,17, 0x02468ace, dds_rough, 0, 0x00000000);
	        acc0b = mul8(buf_fine,16, 0x02468ace, dds_rough, 0, 0x00000000);

	        // dot-multiply with the data
			buf_fine.insert(0,aie::vector<cint16,8>(*buf_rd++));
	        acc1 = mul8( vdata, 0, 0xeca86420, acc0a.to_vector<cint16>(15), 0, 0x76543210);
	        *po++= acc1.to_vector<cint16>(shift);

	        acc1 = mul8( vdata, 1, 0xeca86420, acc0b.to_vector<cint16>(15), 0, 0x76543210);
	        *po  = acc1.to_vector<cint16>(shift); po+=DEMIXER_CARR1P;

	        //f2
	        // calculate sin/cos values and save it to dds_out
            dds_rough.set(aie::sincos_complex(PHASE_INDEX02),0); PHASE_INDEX02+=phstp2;
	        // multiply refined and coarse DDS values
			buf_fine.insert(1,aie::vector<cint16,8>(*buf_rd++));
	        acc0a = mul8(buf_fine, 1, 0x02468ace, dds_rough, 0, 0x00000000);
	        acc0b = mul8(buf_fine, 0, 0x02468ace, dds_rough, 0, 0x00000000);

	        // dot-multiply with the data
			buf_fine.insert(2,aie::vector<cint16,8>(*buf_rd++));
	        acc1 = mul8( vdata, 0, 0xeca86420, acc0a.to_vector<cint16>(15), 0, 0x76543210);
	        *po++= acc1.to_vector<cint16>(shift);

	        acc1 = mul8( vdata, 1, 0xeca86420, acc0b.to_vector<cint16>(15), 0, 0x76543210);
	        *po  = acc1.to_vector<cint16>(shift); po+=DEMIXER_CARR1P;

	        //f3
	        // calculate sin/cos values and save it to dds_out
            dds_rough.set(aie::sincos_complex(PHASE_INDEX03),0); PHASE_INDEX03+=phstp3;
	        // multiply refined and coarse DDS values
			buf_fine.insert(3,aie::vector<cint16,8>(*buf_rd++));
	        acc0a = mul8(buf_fine,17, 0x02468ace, dds_rough, 0, 0x00000000);
	        acc0b = mul8(buf_fine,16, 0x02468ace, dds_rough, 0, 0x00000000);

	        // dot-multiply with the data
			buf_fine.insert(0,aie::vector<cint16,8>(*buf_rd++));
	        acc1 = mul8( vdata, 0, 0xeca86420, acc0a.to_vector<cint16>(15), 0, 0x76543210);
	        *po++= acc1.to_vector<cint16>(shift);

	        acc1 = mul8( vdata, 1, 0xeca86420, acc0b.to_vector<cint16>(15), 0, 0x76543210);
	        *po  = acc1.to_vector<cint16>(shift); po+=DEMIXER_CARR1P;

	        //f4
	        // calculate sin/cos values and save it to dds_out
            dds_rough.set(aie::sincos_complex(PHASE_INDEX04),0); PHASE_INDEX04+=phstp4;
	        // multiply refined and coarse DDS values
			buf_fine.insert(1,aie::vector<cint16,8>(*buf_rd));buf_rd-=9;
	        acc0a = mul8(buf_fine, 1, 0x02468ace, dds_rough, 0, 0x00000000);
	        acc0b = mul8(buf_fine, 0, 0x02468ace, dds_rough, 0, 0x00000000);

	        // dot-multiply with the data
			buf_fine.insert(0,aie::vector<cint16,8>(*buf_rd++));
	        acc1 = mul8( vdata, 0, 0xeca86420, acc0a.to_vector<cint16>(15), 0, 0x76543210);
	        *po++= acc1.to_vector<cint16>(shift);

	        acc1 = mul8( vdata, 1, 0xeca86420, acc0b.to_vector<cint16>(15), 0, 0x76543210);
	        *po  = acc1.to_vector<cint16>(shift); po-=DEMIXER_CARR4P;

        }
	}
}
