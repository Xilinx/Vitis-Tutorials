//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#pragma once
#include "aie_api/aie_adf.hpp"

// -------------------------------------------------------------
// channle filter
// -------------------------------------------------------------
#define MAC0 lbuff.insert(0, buffer_read(ovlp));buffer256_incr_v8(ovlp,1); \
		     acc1 = aie::sliding_mac_sym<4,4,1,1,1>(acc1,coe,4,lbuff,10,rbuff,8); \
             rbuff.insert(1, buffer_read(ovlp_sym));buffer256_incr_v8(ovlp_sym,-1); \
		     acc0 = aie::sliding_mac_sym<4,4,1,1,1>(acc0,coe,8,lbuff,10,rbuff,0); \
		     acc1 = aie::sliding_mac_sym<4,4,1,1,1>(acc1,coe,8,lbuff,14,rbuff,4); \
		     acc0 = aie::sliding_mac_sym<4,4,1,1,1>(acc0,coe,12,lbuff,14,rbuff,12);

#define MAC1 lbuff.insert(1, buffer_read(ovlp));buffer256_incr_v8(ovlp,1); \
		     acc1 = aie::sliding_mac_sym<4,4,1,1,1>(acc1,coe,12,lbuff,2,rbuff,0);coe = *coeff_++; \
		     rbuff.insert(0, buffer_read(ovlp_sym));buffer256_incr_v8(ovlp_sym,-1); \
		     acc0 = aie::sliding_mac_sym<4,4,1,1,1>(acc0,coe,0,lbuff,2,rbuff,8); \
		     acc1 = aie::sliding_mac_sym<4,4,1,1,1>(acc1,coe,0,lbuff,6,rbuff,12); \
		     acc0 = aie::sliding_mac_sym<4,4,1,1,1>(acc0,coe,4,lbuff,6,rbuff,4);

typedef v8cint16 buffer_datatype;

struct buffer_internal
{
	buffer_datatype * restrict head;
	buffer_datatype * restrict ptr;
};

inline __attribute__((always_inline)) void buffer_init(buffer_internal *w, buffer_datatype * buffer)
{
	w->head = buffer;
	w->ptr = buffer;
}


inline __attribute__((always_inline)) void buffer256_incr_v8(buffer_internal * w, int count) {
    w->ptr=cyclic_add(w->ptr, count, w->head, 32);
  }

inline __attribute__((always_inline)) void buffer128_incr_v8(buffer_internal * w, int count) {
    w->ptr=cyclic_add(w->ptr, count, w->head, 16);
  }

inline __attribute__((always_inline)) void buffer_write(buffer_internal * w, v8cint16 value) {
    *((v8cint16 * restrict)(w->ptr)) = value;
  }

inline __attribute__((always_inline)) aie::vector<cint16,8> buffer_read(buffer_internal * w) {
	aie::vector<cint16,8> value;
    value = *((v8cint16 * restrict)(w->ptr));
	return value;
  }

inline __attribute__((always_inline)) void buffer_copy(buffer_internal *w, buffer_internal * buffer)
{
      w->head = buffer->head;
      w->ptr = chess_copy(buffer->ptr);
}



inline __attribute__((always_inline)) void fir_89t_sym_buf(int position,v8cint16 * restrict pi,v4cint16 * restrict po,buffer_internal * restrict buff_,const v16int16 * restrict coeff_ ,unsigned samples)

{
    const int DDC_SHIFT = 15;
	const int shift = DDC_SHIFT ;
	const unsigned LSIZE = (samples / 4 / 2);
	const unsigned SSIZE=LSIZE%16;

	aie::vector<cint16,16> lbuff;
	aie::vector<cint16,16> rbuff;


	#ifndef __X86SIM__
	alignas(32) static  char ddc_fir89_loc[32] ={0,10,11,0,4,14,15,0,8,2,3,0,12,6,7,0,0,10,11,0,4,14,15,0,8,2,3,0,12,6,7,0};
	#else
	alignas(32) static thread_local char ddc_fir89_loc[32] ={0,10,11,0,4,14,15,0,8,2,3,0,12,6,7,0,0,10,11,0,4,14,15,0,8,2,3,0,12,6,7,0};
	#endif
	char * restrict loc_ = (char * )ddc_fir89_loc;
	char * restrict swift= loc_+position;
	loc_=loc_+(*swift);
	*swift = ((*swift)+SSIZE)&0xF;


	buffer_internal _ovlp;     buffer_copy(&_ovlp, buff_);
    buffer_internal _ovlp_sym; buffer_copy(&_ovlp_sym, buff_);
    buffer_internal _ovlp_wr;  buffer_copy(&_ovlp_wr, buff_);

    buffer_internal *  ovlp     = &_ovlp;     buffer128_incr_v8(ovlp,*loc_++);
    buffer_internal *  ovlp_sym = &_ovlp_sym; buffer128_incr_v8(ovlp_sym,*loc_++);
    buffer_internal *  ovlp_wr  = &_ovlp_wr;  buffer128_incr_v8(ovlp_wr,*loc_);

	aie::accum<cacc48,4> acc,acc1;

	lbuff.insert(0, buffer_read(ovlp));buffer128_incr_v8(ovlp,1);              //ovlp 0->=8
	lbuff.insert(1, buffer_read(ovlp));buffer128_incr_v8(ovlp,1);              //ovlp 8->=16


	for ( unsigned l=0; l<LSIZE; ++l )
        chess_prepare_for_pipelining
        chess_loop_range(8,)
    {

        aie::vector<int16,16> coe = *coeff_++;                            //coeff->16
        rbuff.insert(1, aie::vector<cint16,8>(*pi++));
		acc1 = aie::sliding_mul_sym<4,4,1,1,1>(coe,0,lbuff,4,rbuff,12);

        rbuff.insert(0, buffer_read(ovlp_sym));buffer128_incr_v8(ovlp_sym,-1); //ovlp_sym 80->72
		acc = aie::sliding_mul_sym<4,4,1,1,1>(coe,0,lbuff,0,rbuff,8);
		acc1 = aie::sliding_mac_sym<4,4,1,1,1>(acc1,coe,4,lbuff,8,rbuff,8);
		buffer_write(ovlp_wr,rbuff.extract<8>(1));buffer128_incr_v8(ovlp_wr,1);            //ovlp_wr 88 ->96

        rbuff.insert(1, buffer_read(ovlp_sym));buffer128_incr_v8(ovlp_sym,-1); //ovlp_sym 72->64
		acc = aie::sliding_mac_sym<4,4,1,1,1>(acc,coe,4,lbuff,4,rbuff,4);
		acc = aie::sliding_mac_sym<4,4,1,1,1>(acc,coe,8,lbuff,8,rbuff,0);

        lbuff.insert(0, buffer_read(ovlp));buffer128_incr_v8(ovlp,1);          //ovlp 16->24
		acc1 = aie::sliding_mac_sym<4,4,1,1,1>(acc1,coe,8,lbuff,12,rbuff,4);
		acc1 = aie::sliding_mac_sym<4,4,1,1,1>(acc1,coe,12,lbuff,0,rbuff,0);

        rbuff.insert(0, buffer_read(ovlp_sym));buffer128_incr_v8(ovlp_sym,-1); //ovlp_sym 64->56
		acc = aie::sliding_mac_sym<4,4,1,1,1>(acc,coe,12,lbuff,12,rbuff,12);
        coe = *coeff_++;                                      //coeff->32
		acc = aie::sliding_mac_sym<4,4,1,1,1>(acc,coe,0,lbuff,0,rbuff,8);

        lbuff.insert(1, buffer_read(ovlp));buffer128_incr_v8(ovlp,1);          //ovlp 24->32
		acc1 = aie::sliding_mac_sym<4,4,1,1,1>(acc1,coe,0,lbuff,4,rbuff,12);
		acc1 = aie::sliding_mac_sym<4,4,1,1,1>(acc1,coe,4,lbuff,8,rbuff,8);

        rbuff.insert(1, buffer_read(ovlp_sym));buffer128_incr_v8(ovlp_sym,-1); //ovlp_sym 56->48
		acc = aie::sliding_mac_sym<4,4,1,1,1>(acc,coe,4,lbuff,4,rbuff,4);
		acc = aie::sliding_mac_sym<4,4,1,1,1>(acc,coe,8,lbuff,8,rbuff,0);

        lbuff.insert(0, buffer_read(ovlp));buffer128_incr_v8(ovlp,1);          //ovlp 32->40
		acc = aie::sliding_mac_sym<4,4,1,1,1>(acc,coe,12,lbuff,12,rbuff,12);
		acc1 = aie::sliding_mac_sym<4,4,1,1,1>(acc1,coe,8,lbuff,12,rbuff,4);

        lbuff.insert(1, buffer_read(ovlp));buffer128_incr_v8(ovlp,1);          //ovlp 40->48
		acc1 = aie::sliding_mac_sym<4,4,1,1,1>(acc1,coe,12,lbuff,0,rbuff,0);

		rbuff.insert(0, buffer_read(ovlp_sym));buffer128_incr_v8(ovlp_sym,-1); //ovlp_sym 48->40
        coe = *coeff_-- ;                                      //coeff->16
		acc1 = aie::sliding_mac_sym<4,4,1,1,1>(acc1,coe,0,lbuff,4,rbuff,12);
		acc1 = aie::sliding_mac_sym<4,4,1,1,1>(acc1,coe,4,lbuff,8,rbuff,8);
		acc = aie::sliding_mac_sym<4,4,1,1,1>(acc,coe,0,lbuff,0,rbuff,8);

        rbuff.insert(1, buffer_read(ovlp_sym));                             //ovlp_sym 40->40
		acc = aie::sliding_mac_sym<4,4,1,1,1>(acc,coe,4,lbuff,4,rbuff,4);

        lbuff.insert(0, buffer_read(ovlp));buffer128_incr_v8(ovlp, -5);        //ovlp 48->8
		acc1 = aie::sliding_mac_sym<4,4,1,1,1>(acc1,coe,8,lbuff,12,rbuff,4);
		acc = aie::sliding_mac_sym<4,4,1,1,1>(acc,coe,8,lbuff,8,rbuff,0);

		lbuff.insert(0, buffer_read(ovlp));buffer128_incr_v8(ovlp, 1);         //ovlp 8->16
		acc =   aie::sliding_mul_ops<4,4,1,1,1,int16,cint16,cacc48>::mac(acc,coe,12,rbuff,12);
		*po++ = acc.to_vector<cint16>(shift);

		lbuff.insert(1, buffer_read(ovlp));buffer128_incr_v8(ovlp, 1);         //ovlp 16->24
		acc1 =   aie::sliding_mul_ops<4,4,1,1,1,int16,cint16,cacc48>::mac(acc1,coe,12,rbuff,0);
		*po++ = acc1.to_vector<cint16>(shift);
		buffer128_incr_v8(ovlp_sym, 6);                                               //ovlp_sym 40->88
	    coe = *coeff_-- ;                                      //coeff->0
	}
}


inline void fir_199t_sym_buf(int position,v8cint16 * restrict pi,v4cint16 * restrict po,buffer_internal * restrict buff_,const v16int16 * restrict coeff_ ,unsigned samples)

{
    const int DDC_SHIFT = 15;
	const int shift = DDC_SHIFT ;
	const unsigned LSIZE = (samples / 4 / 2);
	const unsigned SSIZE=LSIZE/2;

	aie::vector<cint16,16> lbuff;
	aie::vector<cint16,16> rbuff;

#ifndef __X86SIM__
	alignas(32) static  char ddc_fir199_loc[16]={0,24,25,0,16,8,9,0,0,24,25,0,16,8,9,0};
	#else
	alignas(32) static thread_local char ddc_fir199_loc[16]={0,24,25,0,16,8,9,0,0,24,25,0,16,8,9,0};
	#endif
	char * restrict loc_ = (char * )ddc_fir199_loc;
	char * restrict swift= loc_+position;
	loc_=loc_+(*swift);
	*swift = ((*swift)+SSIZE)&0xF;

	buffer_internal _ovlp;     buffer_copy(&_ovlp,     buff_);
    buffer_internal _ovlp_sym; buffer_copy(&_ovlp_sym, buff_);
    buffer_internal _ovlp_wr;  buffer_copy(&_ovlp_wr,  buff_);

    buffer_internal *  ovlp     = &_ovlp;     buffer256_incr_v8(ovlp,    *loc_++);
    buffer_internal *  ovlp_sym = &_ovlp_sym; buffer256_incr_v8(ovlp_sym,*loc_++);
    buffer_internal *  ovlp_wr  = &_ovlp_wr;  buffer256_incr_v8(ovlp_wr, *loc_);


	for ( unsigned l=0; l<LSIZE; ++l )
        chess_prepare_for_pipelining
        chess_loop_range(32,)
    {

        aie::accum<cacc48,4> acc0,acc1;
        aie::vector<int16,16> coe = *coeff_++;                            //coeff->16

		lbuff.insert(0, buffer_read(ovlp));buffer256_incr_v8(ovlp,1);              //ovlp 0->=8
		lbuff.insert(1, buffer_read(ovlp));buffer256_incr_v8(ovlp,1);              //ovlp 8->=16

        rbuff.insert(1, aie::vector<cint16,8>(*pi++));
		acc1 = aie::sliding_mul_sym<4,4,1,1,1>(coe,0,lbuff,6,rbuff,12);

		rbuff.insert(0, buffer_read(ovlp_sym));buffer256_incr_v8(ovlp_sym,-1);    //ovlp_sym 192->184
		acc0 = aie::sliding_mul_sym<4,4,1,1,1>(coe,0,lbuff,2,rbuff,8);
		acc0 = aie::sliding_mac_sym<4,4,1,1,1>(acc0,coe,4,lbuff,6,rbuff,4);
		buffer_write(ovlp_wr,rbuff.extract<8>(1));buffer256_incr_v8(ovlp_wr,1);               //ovlp_wr  200 ->208

		MAC0   //ovlp 16->24  ovlp_sym 184->176
		MAC1   //ovlp 24->32  ovlp_sym 176->168 coef 16->32

		MAC0   //ovlp 32->40  ovlp_sym 168->160
		MAC1   //ovlp 40->48  ovlp_sym 160->152 coef 32->48

		MAC0   //ovlp 48->56  ovlp_sym 152->144
		MAC1   //ovlp 56->64  ovlp_sym 144->136 coef 48->64

		MAC0   //ovlp 64->72  ovlp_sym 136->128
		MAC1   //ovlp 72->80  ovlp_sym 128->120 coef 64->80

		MAC0   //ovlp 80->88  ovlp_sym 120->112
		MAC1   //ovlp 88->96  ovlp_sym 112->104 coef 80->96

		lbuff.insert(0, buffer_read(ovlp));buffer256_incr_v8(ovlp,1);             //ovlp 96->104
		acc1 = aie::sliding_mac_sym<4,4,1,1,1>(acc1,coe,4,lbuff,10,rbuff,8);

		rbuff.insert(1, buffer_read(ovlp_sym));buffer256_incr_v8(ovlp_sym,12);      //ovlp_sym 104->200
		acc0 = aie::sliding_mac_sym<4,4,1,1,1>(acc0,coe,8,lbuff,10,rbuff,0);
		acc1 = aie::sliding_mac_sym<4,4,1,1,1>(acc1,coe,8,lbuff,14,rbuff,4);
		acc0 = aie::sliding_mac_sym<4,4,1,1,1>(acc0,coe,12,lbuff,14,rbuff,12);

		lbuff.insert(1, buffer_read(ovlp));buffer256_incr_v8(ovlp,-12);           //ovlp 104->8
		acc1 = aie::sliding_mac_sym<4,4,1,1,1>(acc1,coe,12,lbuff,2,rbuff,0);
		coe = *coeff_; coeff_=coeff_-6;//coeff 96->0

		acc0 = aie::sliding_mac_sym<4,7,1,1,1>(acc0,coe,0,lbuff,2);
		*po++ = acc0.to_vector<cint16>(shift);

		acc1 = aie::sliding_mac_sym<4,7,1,1,1>(acc1,coe,0,lbuff,6);
		*po++ = acc1.to_vector<cint16>(shift);

	}
}


inline __attribute__((always_inline)) void fir_23t_sym_hb_2d(v8cint16 * restrict pi, v4cint16 * restrict po, v8cint16 * restrict buff, unsigned samples)
{
    const int DDC_SHIFT = 15;
    const int sft = DDC_SHIFT;
	aie::vector<cint16,32> sbuff;

    static constexpr unsigned HB23_2D_COEFFICIENTS = 8;
    alignas(32) static constexpr int16_t coeffs_hb23_2d[HB23_2D_COEFFICIENTS] ={
    		-45,  251, -848, 2256, -5603, 20372,    0, 32767
    };
	const aie::vector<int16,16> coe = *((const v16int16 *) coeffs_hb23_2d);

    const unsigned LSIZE = (samples / 4 /4);

	sbuff.insert(0, aie::vector<cint16,8>(*buff++));// 0:7|X|X|X
	sbuff.insert(1, aie::vector<cint16,8>(*buff++));// 0:7|8:15|X|X
	sbuff.insert(2, aie::vector<cint16,8>(*buff));  // 0:7|8:15|16:23|

    for ( unsigned l=0; l<LSIZE; ++l )
    chess_prepare_for_pipelining
    chess_loop_range(4,)
    {
        aie::accum<cacc48,4> acc;

		sbuff.insert(3, aie::vector<cint16,8>(*pi++));

        acc = mul4_sym(sbuff, 2, 0x6420, 2, 24, coe, 0, 0x0000, 1);
        acc = mac4_sym_ct(acc, sbuff, 10, 0x6420, 2, 16, 3, coe, 4, 0x0000, 1);
		*po++ = acc.to_vector<cint16>(sft);

		sbuff.insert(0, aie::vector<cint16,8>(*pi++));
        acc = mul4_sym(sbuff, 10, 0x6420, 2, 32, coe, 0, 0x0000, 1);
        acc = mac4_sym_ct(acc, sbuff, 18, 0x6420, 2, 24, 3, coe, 4, 0x0000, 1);
		*po++ = acc.to_vector<cint16>(sft);

		sbuff.insert(1, aie::vector<cint16,8>(*pi++));
        acc = mul4_sym(sbuff, 18, 0x6420, 2, 40, coe, 0, 0x0000, 1);
        acc = mac4_sym_ct(acc, sbuff, 26, 0x6420, 2, 32, 3, coe, 4, 0x0000, 1);
		*po++ = acc.to_vector<cint16>(sft);

		sbuff.insert(2, aie::vector<cint16,8>(*pi++));
        acc = mul4_sym(sbuff, 26, 0x6420, 2, 48, coe, 0, 0x0000, 1);
        acc = mac4_sym_ct(acc, sbuff, 34, 0x6420, 2, 40, 3, coe, 4, 0x0000, 1);
		*po++ = acc.to_vector<cint16>(sft);
    }

	*buff--=sbuff.extract<8>(2);
	*buff--=sbuff.extract<8>(1);
	*buff=sbuff.extract<8>(0);
}

inline __attribute__((always_inline)) void fir_11t_sym_hb_2d_inv(v8cint16 * restrict pi, v4cint16 * restrict po, v8cint16 * restrict buff, unsigned samples)
{
    const int DDC_SHIFT = 15;
    const int sft = DDC_SHIFT;
	aie::vector<cint16,32> sbuff;

    static constexpr unsigned HB11_2D_COEFFICIENTS = 16;
    alignas(32) static constexpr int16_t coeffs_hb11_2d[HB11_2D_COEFFICIENTS] ={
    		502, -3517, 19401, 32767,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0
    };
	const aie::vector<int16,16> coe = *((const v16int16 *) coeffs_hb11_2d);


    const unsigned LSIZE = (samples / 4 / 8);

	sbuff.insert(0, aie::vector<cint16,8>(*buff++));  // 0:14|X|X|X
	sbuff.insert(2, aie::vector<cint16,8>(*buff));  // 0:14|X|X|X

    for ( unsigned l=0; l<LSIZE; ++l )
    chess_prepare_for_pipelining
    chess_loop_range(4,)
    {
        sbuff.insert(1, aie::vector<cint16,8>(*pi++));    // 0:14|16:30|X|X
		//1
        sbuff.insert(3, aie::vector<cint16,8>(*pi++));
        *po++ = srs( mul4_sym_ct(sbuff, 3,0x3210, 1, 8,18, coe, 0, 0x0000, 1), sft);
        //2
        sbuff.insert(2, aie::vector<cint16,8>(*pi++));
        *po++ = srs( mul4_sym_ct(sbuff, 7,0x3210, 1,12,18, coe, 0, 0x0000, 1), sft);
        //3
        sbuff.insert(0, aie::vector<cint16,8>(*pi++));
        *po++ = srs( mul4_sym_ct(sbuff,11,0x3210, 1,16,18, coe, 0, 0x0000, 1), sft);
        //4
        sbuff.insert(3, aie::vector<cint16,8>(*pi++));
        *po++ = srs( mul4_sym_ct(sbuff,15,0x3210, 1,20,18, coe, 0, 0x0000, 1), sft);
		//5
        sbuff.insert(1, aie::vector<cint16,8>(*pi++));
        *po++ = srs( mul4_sym_ct(sbuff,19,0x3210, 1,24,18, coe, 0, 0x0000, 1), sft);
		//6
        sbuff.insert(0, aie::vector<cint16,8>(*pi++));
        *po++ = srs( mul4_sym_ct(sbuff,23,0x3210, 1,28,18, coe, 0, 0x0000, 1), sft);
		//7
        sbuff.insert(2, aie::vector<cint16,8>(*pi++));
        *po++ = srs( mul4_sym_ct(sbuff,27,0x3210, 1, 0,18, coe, 0, 0x0000, 1), sft);
		//8
        *po++ = srs( mul4_sym_ct(sbuff,31,0x3210, 1, 4,18, coe, 0, 0x0000, 1), sft);
    }
	*buff--=sbuff.extract<8>(2);
	*buff=sbuff.extract<8>(0);
}
