/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "src/kernels.h"
#include "src/include.h"
#include "src/globals.h"


extern float chess_storage(%chess_alignof(v8float))  particles_i[NUM_I*7];
extern float chess_storage(%chess_alignof(v8float))  particles_i_new[NUM_I*3];
extern float chess_storage(%chess_alignof(v8float))  particles_j[NUM_J*4];

using namespace adf;

template <int COL_OFFSET, int ROW_OFFSET>
class nbodySubsystem : public adf::graph {
private:
	kernel nbody_kernel[NUM_ENGINES_PER_COL];
	kernel transmit_new_i_kernel[NUM_ENGINES_PER_COL];
	pktsplit<NUM_ENGINES_PER_COL>  pkt_split_i;
	pktmerge<NUM_ENGINES_PER_COL>  pkt_merge_i;
	parameter global_particles_i[NUM_ENGINES_PER_COL];
	parameter global_particles_i_new[NUM_ENGINES_PER_COL];
	parameter global_particles_j[NUM_ENGINES_PER_COL];
	
public:
	port<input> data_in[2];
	port<output> data_out[1];

	nbodySubsystem() {
    
		// packet stream to different engines
		pkt_split_i  = pktsplit<NUM_ENGINES_PER_COL>::create();
		connect<adf::pktstream>(data_in[0], pkt_split_i.in[0]);//pktstream

		// output of nbody subsystem to pkt merge
		pkt_merge_i  = pktmerge<NUM_ENGINES_PER_COL>::create();

		// create NUM_ENGINES_PER_COL get_particles_i and n-body kernels
		for (int row =0; row<NUM_ENGINES_PER_COL; row++)  {
			nbody_kernel[row]   = kernel::create(nbody);
			source(nbody_kernel[row])   = "src/nbody.cc";
			runtime<ratio>(nbody_kernel[row]) = 0.7;
			location<kernel>(nbody_kernel[row]) = tile(COL_OFFSET,ROW_OFFSET+row);
	
			transmit_new_i_kernel[row]   = kernel::create(transmit_new_i);
			source(transmit_new_i_kernel[row])   = "src/transmit_new_i.cc";
			runtime<ratio>(transmit_new_i_kernel[row]) = 0.1;
			location<kernel>(transmit_new_i_kernel[row]) = tile(COL_OFFSET,ROW_OFFSET+row);
			
			connect(pkt_split_i.out[row], nbody_kernel[row].in[0]);//pktstream, window<WINDOW_SIZE_I>
			dimensions( nbody_kernel[row].in[0])={WINDOW_SIZE_I};
			 
                        connect(data_in[1], nbody_kernel[row].in[1]);// stream, window<WINDOW_SIZE_J> broadcast 
			dimensions( nbody_kernel[row].in[1])={WINDOW_SIZE_J}; 

			connect(nbody_kernel[row].out[0], transmit_new_i_kernel[row].in[0]);// adf::window<WINDOW_SIZE_I>
			dimensions( nbody_kernel[row].out[0])={WINDOW_SIZE_I};
			dimensions( transmit_new_i_kernel[row].in[0])={WINDOW_SIZE_I};  
			connect(transmit_new_i_kernel[row].out[0], pkt_merge_i.in[row]);  //i//adf::window<WINDOW_SIZE_I>, adf::pktstream
			//connect(nbody_kernel[row].out[0],pkt_merge_i.in[row]);
			
                        dimensions( transmit_new_i_kernel[row].out[0])={WINDOW_SIZE_I};
			
			global_particles_j[row] = parameter::array(particles_j);
			connect(global_particles_j[row], nbody_kernel[row]);
			
			global_particles_i[row] = parameter::array(particles_i);
			connect(global_particles_i[row], nbody_kernel[row]);
			
			global_particles_i_new[row] = parameter::array(particles_i_new);
			connect(global_particles_i_new[row], nbody_kernel[row]);
			
						
		}

		connect<adf::pktstream,adf::pktstream>(pkt_merge_i.out[0], data_out[0]);  // adf::pktstream
		
	}
};

