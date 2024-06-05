/*Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "nbody_subsystem.h"

#define NUM_INSTANCES 10

using namespace adf;


template <int COL_OFFSET>
class nbodySystem : public adf::graph {
	
public:
	input_plio in_i[NUM_INSTANCES];
	input_plio in_j; 	
	output_plio out_i[NUM_INSTANCES];
            //COL 0-4, ROW 0
            nbodySubsystem<COL_OFFSET+0, 0>  nbody_inst0;
            nbodySubsystem<COL_OFFSET+1, 0>  nbody_inst1;
            nbodySubsystem<COL_OFFSET+2, 0>  nbody_inst2;
            nbodySubsystem<COL_OFFSET+3, 0>  nbody_inst3;
            nbodySubsystem<COL_OFFSET+4, 0>  nbody_inst4;

            //COL 0-4, ROW 4
            nbodySubsystem<COL_OFFSET+0, 4>  nbody_inst5;
            nbodySubsystem<COL_OFFSET+1, 4>  nbody_inst6;
            nbodySubsystem<COL_OFFSET+2, 4>  nbody_inst7;
            nbodySubsystem<COL_OFFSET+3, 4>  nbody_inst8;
            nbodySubsystem<COL_OFFSET+4, 4>  nbody_inst9;

	nbodySystem() {
		//input j PLIO
		in_j  = input_plio::create("in_j",  adf::plio_32_bits, "../../data/input_j.txt", 400);
			
		//input i PLIOs
		in_i[0]  = input_plio::create("in_i0",  adf::plio_32_bits, "../../data/input_i0.txt", 400);
		in_i[1]  = input_plio::create("in_i1",  adf::plio_32_bits, "../../data/input_i1.txt", 400);
		in_i[2]  = input_plio::create("in_i2",  adf::plio_32_bits, "../../data/input_i2.txt", 400);
		in_i[3]  = input_plio::create("in_i3",  adf::plio_32_bits, "../../data/input_i3.txt", 400);
		in_i[4]  = input_plio::create("in_i4",  adf::plio_32_bits, "../../data/input_i4.txt", 400);
		in_i[5]  = input_plio::create("in_i5",  adf::plio_32_bits, "../../data/input_i5.txt", 400);
		in_i[6]  = input_plio::create("in_i6",  adf::plio_32_bits, "../../data/input_i6.txt", 400);
		in_i[7]  = input_plio::create("in_i7",  adf::plio_32_bits, "../../data/input_i7.txt", 400);
		in_i[8]  = input_plio::create("in_i8",  adf::plio_32_bits, "../../data/input_i8.txt", 400);
		in_i[9]  = input_plio::create("in_i9",  adf::plio_32_bits, "../../data/input_i9.txt", 400);

		//output PLIOs
		out_i[0] = output_plio::create("out_i0", adf::plio_32_bits, "./data/output_i0.txt", 400);
		out_i[1] = output_plio::create("out_i1", adf::plio_32_bits, "./data/output_i1.txt", 400);
		out_i[2] = output_plio::create("out_i2", adf::plio_32_bits, "./data/output_i2.txt", 400);
		out_i[3] = output_plio::create("out_i3", adf::plio_32_bits, "./data/output_i3.txt", 400);
		out_i[4] = output_plio::create("out_i4", adf::plio_32_bits, "./data/output_i4.txt", 400);
		out_i[5] = output_plio::create("out_i5", adf::plio_32_bits, "./data/output_i5.txt", 400);
		out_i[6] = output_plio::create("out_i6", adf::plio_32_bits, "./data/output_i6.txt", 400);
		out_i[7] = output_plio::create("out_i7", adf::plio_32_bits, "./data/output_i7.txt", 400);
		out_i[8] = output_plio::create("out_i8", adf::plio_32_bits, "./data/output_i8.txt", 400);
		out_i[9] = output_plio::create("out_i9", adf::plio_32_bits, "./data/output_i9.txt", 400);
		//Connect all input j ports
		connect(in_j.out[0], nbody_inst0.data_in[1]);
		connect(in_j.out[0], nbody_inst1.data_in[1]);
		connect(in_j.out[0], nbody_inst2.data_in[1]);
		connect(in_j.out[0], nbody_inst3.data_in[1]);
		connect(in_j.out[0], nbody_inst4.data_in[1]);
		connect(in_j.out[0], nbody_inst5.data_in[1]);
		connect(in_j.out[0], nbody_inst6.data_in[1]);
		connect(in_j.out[0], nbody_inst7.data_in[1]);
		connect(in_j.out[0], nbody_inst8.data_in[1]);
		connect(in_j.out[0], nbody_inst9.data_in[1]);

		//Connect all input i ports
		connect(in_i[0].out[0], nbody_inst0.data_in[0]);	
		connect(in_i[1].out[0], nbody_inst1.data_in[0]);	
		connect(in_i[2].out[0], nbody_inst2.data_in[0]);	
		connect(in_i[3].out[0], nbody_inst3.data_in[0]);	
		connect(in_i[4].out[0], nbody_inst4.data_in[0]);	
		connect(in_i[5].out[0], nbody_inst5.data_in[0]);	
		connect(in_i[6].out[0], nbody_inst6.data_in[0]);	
		connect(in_i[7].out[0], nbody_inst7.data_in[0]);	
		connect(in_i[8].out[0], nbody_inst8.data_in[0]);	
		connect(in_i[9].out[0], nbody_inst9.data_in[0]);	

		//Connect all output ports
		connect(nbody_inst0.data_out[0], out_i[0].in[0]);
		connect(nbody_inst1.data_out[0], out_i[1].in[0]);
		connect(nbody_inst2.data_out[0], out_i[2].in[0]);
		connect(nbody_inst3.data_out[0], out_i[3].in[0]);
		connect(nbody_inst4.data_out[0], out_i[4].in[0]);
		connect(nbody_inst5.data_out[0], out_i[5].in[0]);
		connect(nbody_inst6.data_out[0], out_i[6].in[0]);
		connect(nbody_inst7.data_out[0], out_i[7].in[0]);
		connect(nbody_inst8.data_out[0], out_i[8].in[0]);
		connect(nbody_inst9.data_out[0], out_i[9].in[0]);
        
	}
};
