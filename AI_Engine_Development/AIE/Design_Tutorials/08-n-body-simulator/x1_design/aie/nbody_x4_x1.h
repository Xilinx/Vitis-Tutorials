/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "nbody_subsystem.h"

#define NUM_INSTANCES 1

using namespace adf;

template <int COL_OFFSET>
class nbodySystem : public adf::graph {
	
public:
	input_port in[1 + NUM_INSTANCES];
	output_port out[NUM_INSTANCES];

	nbodySubsystem<COL_OFFSET+0, 0>  nbody_inst0;

	nbodySystem() {
	
		connect< pktstream, window<WINDOW_SIZE_I> >(in[0], nbody_inst0.in[0]);	
		connect< stream >(in[1], nbody_inst0.in[1]);
		
		connect< pktstream >(nbody_inst0.out[0], out[0]);

		
  	}
};
