/*Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "nbody_subsystem.h"

#define NUM_INSTANCES 10

using namespace adf;


template <int COL_OFFSET>
class nbodySystem : public adf::graph {
	
public:
	input_port in_i[NUM_INSTANCES];
	input_port in_j; 	
	output_port out[NUM_INSTANCES];
        
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

		//Connect all input j ports
		connect< stream > (in_j, nbody_inst0.in[1]);
		connect< stream > (in_j, nbody_inst1.in[1]);
		connect< stream > (in_j, nbody_inst2.in[1]);
		connect< stream > (in_j, nbody_inst3.in[1]);
		connect< stream > (in_j, nbody_inst4.in[1]);
		connect< stream > (in_j, nbody_inst5.in[1]);
		connect< stream > (in_j, nbody_inst6.in[1]);
		connect< stream > (in_j, nbody_inst7.in[1]);
		connect< stream > (in_j, nbody_inst8.in[1]);
		connect< stream > (in_j, nbody_inst9.in[1]);

		//Connect all input i ports
		connect< pktstream, window<WINDOW_SIZE_I > >(in_i[0], nbody_inst0.in[0]);	
		connect< pktstream, window<WINDOW_SIZE_I > >(in_i[1], nbody_inst1.in[0]);	
		connect< pktstream, window<WINDOW_SIZE_I > >(in_i[2], nbody_inst2.in[0]);	
		connect< pktstream, window<WINDOW_SIZE_I > >(in_i[3], nbody_inst3.in[0]);	
		connect< pktstream, window<WINDOW_SIZE_I > >(in_i[4], nbody_inst4.in[0]);	
		connect< pktstream, window<WINDOW_SIZE_I > >(in_i[5], nbody_inst5.in[0]);	
		connect< pktstream, window<WINDOW_SIZE_I > >(in_i[6], nbody_inst6.in[0]);	
		connect< pktstream, window<WINDOW_SIZE_I > >(in_i[7], nbody_inst7.in[0]);	
		connect< pktstream, window<WINDOW_SIZE_I > >(in_i[8], nbody_inst8.in[0]);	
		connect< pktstream, window<WINDOW_SIZE_I > >(in_i[9], nbody_inst9.in[0]);	

		//Connect all output ports
		connect< pktstream > (nbody_inst0.out[0], out[0]);
		connect< pktstream > (nbody_inst1.out[0], out[1]);
		connect< pktstream > (nbody_inst2.out[0], out[2]);
		connect< pktstream > (nbody_inst3.out[0], out[3]);
		connect< pktstream > (nbody_inst4.out[0], out[4]);
		connect< pktstream > (nbody_inst5.out[0], out[5]);
		connect< pktstream > (nbody_inst6.out[0], out[6]);
		connect< pktstream > (nbody_inst7.out[0], out[7]);
		connect< pktstream > (nbody_inst8.out[0], out[8]);
		connect< pktstream > (nbody_inst9.out[0], out[9]);
        
	}
};
        
