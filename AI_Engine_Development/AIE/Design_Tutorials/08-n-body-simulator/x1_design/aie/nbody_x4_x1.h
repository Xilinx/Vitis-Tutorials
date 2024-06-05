/*
Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "src/nbody_subsystem.h"

#define NUM_INSTANCES 1

using namespace adf;

template <int COL_OFFSET>
class nbodySystem : public adf::graph {
	
public:
	input_plio   i_in[NUM_INSTANCES]; 
	input_plio   j_in[NUM_INSTANCES];
	output_plio  out[NUM_INSTANCES];   

	
	nbodySubsystem<COL_OFFSET+0, 0>  nbody_inst0;

	nbodySystem() {
		i_in[0] =  input_plio::create("in_i",  adf::plio_32_bits, "../../data/input_i0.txt", 400);
		j_in[0] =  input_plio::create("in_j",  adf::plio_32_bits, "../../data/input_j.txt", 400);
		out[0] =   output_plio::create("out_i", adf::plio_32_bits, "./data/output_i0.txt", 400);
		
		connect(i_in[0].out[0], nbody_inst0.data_in[0]);
		connect(j_in[0].out[0], nbody_inst0.data_in[1]);

		connect(nbody_inst0.data_out[0], out[0].in[0]);

  	}
};
