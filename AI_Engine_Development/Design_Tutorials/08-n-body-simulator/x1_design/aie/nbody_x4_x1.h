/* (c) Copyright 2021 Xilinx, Inc.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 * 		http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
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
