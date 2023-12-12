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
class SimpleGraph : public adf::graph {

public :
	output_plio out;
	input_plio in;
private:
	kernel k_mean;
	kernel k_deviation;
	kernel k_norm;
	shared_buffer<bfloat16> mtxA;

public:
	SimpleGraph() {
		in=input_plio::create("Datain0", plio_128_bits,  "data/input0.csv");
		out=output_plio::create("Dataout0", plio_128_bits,  "data/output0.txt");

		k_mean = adf::kernel::create(mean<K_COL,K_ROW, ROW*COL/K_ROW/K_COL>);
		source(k_mean) = "mean.cc";
		runtime<ratio>(k_mean) = 0.9;
		k_deviation = adf::kernel::create(deviation<K_COL,K_ROW, ROW*COL/K_ROW/K_COL>);
		source(k_deviation) = "deviation.cc";
		runtime<ratio>(k_deviation) = 0.9;
		k_norm = adf::kernel::create(norm<K_COL,K_ROW, ROW*COL/K_ROW/K_COL>);
		source(k_norm) = "norm.cc";
		runtime<ratio>(k_norm) = 0.9;

		repetition_count(k_mean)=ROW*COL/K_ROW/K_COL;
		repetition_count(k_deviation)=ROW*COL/K_ROW/K_COL;
		repetition_count(k_norm)=ROW*COL/K_ROW/K_COL;

		mtxA = shared_buffer<bfloat16>::create({COL,ROW}, 1, 1);
		write_access(mtxA.in[0]) = tiling({.buffer_dimension={COL,ROW}, .tiling_dimension={COL,ROW}, .offset={0,0} });
		read_access(mtxA.out[0]) = tiling({.buffer_dimension={COL,ROW}, .tiling_dimension={COL,ROW}, .offset={0,0} });

		connect(in.out[0], mtxA.in[0]);
    	connect(mtxA.out[0], k_mean.in[0]);
    	connect(mtxA.out[0], k_deviation.in[0]);
    	connect(mtxA.out[0], k_norm.in[0]);
		connect(k_mean.out[0], k_deviation.in[1]);
		connect(k_deviation.out[0], k_norm.in[1]);
		connect(k_norm.out[0], out.in[0]);

		dimensions(k_mean.in[0])={K_ROW*K_COL};//elements
		dimensions(k_mean.out[0])={1};//elements
		dimensions(k_deviation.in[0])={K_ROW*K_COL};//elements
		dimensions(k_deviation.in[1])={1};//elements
		dimensions(k_deviation.out[0])={2};//elements
		dimensions(k_norm.in[0])={K_ROW*K_COL};//elements
		dimensions(k_norm.in[1])={2};//elements
		dimensions(k_norm.out[0])={K_ROW*K_COL};//elements
			
	}

};

#endif
