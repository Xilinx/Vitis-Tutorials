/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef GRAPH_MODULE_H
#define GRAPH_MODULE_H

#include "kernels.h"
#include <adf.h>
using namespace adf;

const int COL=256;
const int ROW=384;
const int K_COL=256;
const int K_ROW=64;
const int NUM=6;
class SimpleGraph : public adf::graph {

public :
	output_plio out;
	input_plio in;
private:
	kernel k[NUM];
	shared_buffer<bfloat16> mtxA,mtxB;

public:
	SimpleGraph() {
		in=input_plio::create("Datain0", plio_128_bits,  "data/input0.csv");
		out=output_plio::create("Dataout0", plio_128_bits,  "data/output0.txt");

		mtxA = shared_buffer<bfloat16>::create({COL,ROW}, 1, NUM);
		mtxB = shared_buffer<bfloat16>::create({COL,ROW}, NUM, 1);

		for(int i=0;i<NUM;i++){
			if(i==0){
				k[i]=kernel::create(mean_dev_norm_first<K_COL,K_ROW>);
				dimensions(k[i].in[0])={K_ROW*K_COL};//elements
				dimensions(k[i].out[0])={K_ROW*K_COL};//elements
				source(k[i])="mean_dev_norm_first.cc";
			}else if(i<NUM-1){
				k[i]=kernel::create(mean_dev_norm_middle<K_COL,K_ROW>);
				dimensions(k[i].in[0])={K_ROW*K_COL};//elements
				dimensions(k[i].out[0])={K_ROW*K_COL};//elements
				source(k[i])="mean_dev_norm_middle.cc";
			}else{
				k[i]=kernel::create(mean_dev_norm_last<K_COL,K_ROW,NUM>);
				dimensions(k[i].in[0])={K_ROW*K_COL};//elements
				dimensions(k[i].out[0])={K_ROW*K_COL};//elements
				source(k[i])="mean_dev_norm_last.cc";
			}

    		connect(mtxA.out[i], k[i].in[0]);
    		connect(k[i].out[0], mtxB.in[i]);
			runtime<ratio>(k[i]) = 0.9;
		}
		connect(in.out[0],mtxA.in[0]);
		connect(mtxB.out[0],out.in[0]);
		write_access(mtxA.in[0]) = tiling({.buffer_dimension={COL,ROW}, .tiling_dimension={COL,ROW}, .offset={0,0} });
		read_access(mtxB.out[0]) = tiling({.buffer_dimension={COL,ROW}, .tiling_dimension={COL,ROW}, .offset={0,0} });
		for(int i=0;i<NUM;i++){
			read_access(mtxA.out[i]) = tiling({.buffer_dimension={COL,ROW}, .tiling_dimension={K_COL,K_ROW}, .offset={0,K_ROW*i} });
			write_access(mtxB.in[i]) = tiling({.buffer_dimension={COL,ROW}, .tiling_dimension={K_COL,K_ROW}, .offset={0,K_ROW*i} });
		}

		for(int i=0;i<NUM-1;i++){
			connect(k[NUM-1].out[1],k[i].in[1]);//connection for mean and deviation
			if(i==NUM-2){//connection for partial accumulation
				connect(k[i].out[1],k[i+1].in[1]);
			}else{
				connect(k[i].out[1],k[i+1].in[2]);
			}
		}

	}

};

#endif
