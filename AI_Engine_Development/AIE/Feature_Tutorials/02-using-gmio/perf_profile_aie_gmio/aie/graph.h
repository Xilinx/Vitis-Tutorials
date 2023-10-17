/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <adf.h>
#include "kernel.h"

#if __AIEARCH__==10 or __AIE_ARCH__==10
static const int col[8]={2,6,10,18,26,34,42,46};
#elif __AIEARCH__==20 or __AIE_ARCH__==20
static const int col[8]={2,6,14,22,30,34};
#else
static const int col[8]={0};//for PS compilation
#endif
static const int NUM=4;

class topgraph: public adf::graph
{
public:
	adf::kernel k[NUM];
	adf::input_gmio gmioIn[NUM];	
	adf::output_gmio gmioOut[NUM];
	
	topgraph(){
		for(int i=0;i<NUM;i++){
			k[i] = adf::kernel::create(vec_incr);
			adf::source(k[i]) = "vec_incr.cc";
			adf::runtime<adf::ratio>(k[i])= 1;
			gmioIn[i]=adf::input_gmio::create("gmioIn"+std::to_string(i),/*size_t burst_length*/256,/*size_t bandwidth*/100);
			gmioOut[i]=adf::output_gmio::create("gmioOut"+std::to_string(i),/*size_t burst_length*/256,/*size_t bandwidth*/100);
			adf::connect<>(gmioIn[i].out[0], k[i].in[0]);	
			adf::connect<>(k[i].out[0], gmioOut[i].in[0]);

			adf::location<adf::kernel>(k[i])=adf::tile(col[i],0);
			location<GMIO>(gmioIn[i]) = location<kernel>(k[i]) + relative_offset({.col_offset=0});	
			location<GMIO>(gmioOut[i]) = location<kernel>(k[i]) + relative_offset({.col_offset=1});
		}
	}
};
#endif
