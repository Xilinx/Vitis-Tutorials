/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <aie_api/aie.hpp>
#include <aie_api/aie_adf.hpp>
const uint32 pktType=0;

void aie_core3(input_pktstream *in,output_pktstream *out){
	readincr(in);//read header and discard
	uint32 ID=getPacketid(out,0);//for output pktstream
	writeHeader(out,pktType,ID); //Generate header for output

	bool tlast;
	for(int i=0;i<8;i++){
		int32 tmp=readincr(in,tlast);
		tmp+=3;
		writeincr(out,tmp,i==7);//TLAST=1 for last word
	}
}
