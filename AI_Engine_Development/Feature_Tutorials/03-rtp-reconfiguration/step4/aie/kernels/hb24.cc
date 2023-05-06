/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
#include <aie_api/utils.hpp>
// needs chess storage on coeffs in testbench
using namespace adf;
void fir24_sym(input_buffer<cint16,extents<64>,margin<24>> &iwin, output_buffer<cint16,extents<64>> &owin,  const  int32  (&coeffs)[12]) {
	auto iwinIter=aie::begin_vector_random_circular<4>(iwin);
	auto iwinIter8=aie::begin_vector_random_circular<8>(iwin);
	auto owinIter=aie::begin_vector_circular<4>(owin);
	aie::vector<cint16,16> xbuff;
	aie::vector<cint16,16> ybuff;

	aie::vector<cint16,8> wtmp;
	aie::vector<cint16,4> vtmp;
	wtmp=**(decltype(iwinIter8)*)&iwinIter;
	iwinIter+=2;
	xbuff.insert(0,wtmp); // D0  D1  D2  D3  | D4  D5  D6  D7 |  x x x x | x x x x

	wtmp=**(decltype(iwinIter8)*)&iwinIter;
	iwinIter+=2;
	xbuff.insert(1,wtmp); // D0  D1  D2  D3  | D4  D5  D6  D7 |  D8 D9 D10 D11 | D12 D13 D14 D15
	ybuff.insert(0, wtmp.extract<4>(1));
	vtmp=*iwinIter++;
	ybuff.insert(1, vtmp);
	wtmp=**(decltype(iwinIter8)*)&iwinIter;
	iwinIter+=2;
	ybuff.insert(1, wtmp);

	//D0  D1  D2  D3  | D4  D5  D6  D7 |  D8  D9  D10 D11 | D12 D13 D14 D15
	//D12 D13 D14 D15 | D16 D17 D18 D19 | D20 D21 D22 D23 | D24 D25 D26 D27
     
	aie::vector<int16,16> zbuff ; 
	for (unsigned i = 0; i < 12 ; i++) { 
		//zbuff.push( (int16) coeffs[11 - i]);
		zbuff[i]=coeffs[i];
	}

	const unsigned BLOCKSIZE = 64;

	// every loop consumes 16 input samples and produces 16 output samples
	for (unsigned i = 0 ; i < BLOCKSIZE >> 4 ; i++) { 
	
		aie::accum<cacc48,4> acc;
		aie::vector<cint16,4> isamples;
		aie::vector<cint16,4> osamples;
	
		acc=aie::sliding_mul_sym<4,4>(zbuff,0,xbuff,0,ybuff,11);
		acc=aie::sliding_mac_sym<4,4>(acc,zbuff,4,xbuff,4,ybuff,7);
		acc=aie::sliding_mac_sym<4,4>(acc,zbuff,8,xbuff,8,ybuff,3);
		osamples = acc.to_vector<cint16>(9);
	
		isamples=*iwinIter++;
		*owinIter++=osamples;
	
		// copy values 16,17,18,19 from b to a             // D16 D17 D18 D19  | D4  D5  D6  D7 |  D8 D9 D10 D11 | D12 D13 D14 D15
		xbuff.insert(0, ybuff.extract<4>(1));
		// update 4 new values (28,29,30,31) into ybuff    // D28 D29 D30 D31  | D16 D17 D18 D19 | D20 D21 D22 D23 | D24 D25 D26 D27
		ybuff.insert(0, isamples);
		
		acc=aie::sliding_mul_sym<4,4>(zbuff,0,xbuff,4,ybuff,15);
		acc=aie::sliding_mac_sym<4,4>(acc,zbuff,4,xbuff,8,ybuff,11);
		acc=aie::sliding_mac_sym<4,4>(acc,zbuff,8,xbuff,12,ybuff,7);
		osamples = acc.to_vector<cint16>(9);
	
		isamples=*iwinIter++;
		*owinIter++=osamples;
		// copy values 20,21,22,23 from b to a            // D16 D17 D18 D19  | D20 D21 D22 D23 |  D8 D9 D10 D11 | D12 D13 D14 D15
	
		xbuff.insert(1, ybuff.extract<4>(2));
		// update 4 new values (28,29,30,31) into ybuff   // D28 D29 D30 D31  | D32 D33 D34 D35 | D20 D21 D22 D23 | D24 D25 D26 D27
		ybuff.insert(1, isamples);
		
		acc=aie::sliding_mul_sym<4,4>(zbuff,0,xbuff,8,ybuff,3);
		acc=aie::sliding_mac_sym<4,4>(acc,zbuff,4,xbuff,12,ybuff,15);
		acc=aie::sliding_mac_sym<4,4>(acc,zbuff,8,xbuff,0,ybuff,11);
		osamples = acc.to_vector<cint16>(9);
	
		isamples=*iwinIter++;
		*owinIter++=osamples;
	
	 	// copy values 24,25,26,27 from b to a           // D16 D17 D18 D19  | D20 D21 D22 D23 |  D24 D25 D26 D27 | D12 D13 D14 D15
	
	 	xbuff.insert(2, ybuff.extract<4>(3));
		// update 4 new values (40,41,42,32)             // D28 D29 D30 D31  | D32 D33 D34 D35 |  D36 D27 D38 D39 | D24 D25 D26 D27
		ybuff.insert(2, isamples);
	
		acc=aie::sliding_mul_sym<4,4>(zbuff,0,xbuff,12,ybuff,7);
		acc=aie::sliding_mac_sym<4,4>(acc,zbuff,4,xbuff,0,ybuff,3);
		acc=aie::sliding_mac_sym<4,4>(acc,zbuff,8,xbuff,4,ybuff,15);
		osamples = acc.to_vector<cint16>(9);
	
		isamples=*iwinIter++;
		*owinIter++=osamples;
	
		// copy values 28, 29, 30, 31 from b to a           // D16 D17 D18 D19  | D20 D21 D22 D23 |  D24 D25 D26 D27 | D28 D29 D30 D31
	
		xbuff.insert(3, ybuff.extract<4>(0));
		// update 4 new values (40,41,42,32)             // D28 D29 D30 D31  | D32 D33 D34 D35 |  D36 D27 D38 D39 | D40 D41 D42 D43
		ybuff.insert(3, isamples);
	};
};



