/*
 * Copyright 2020 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <adf.h>
#include "subsys.h" 

using namespace adf;

template<int COL0>
class TEST_BF: public graph{
  
public:

	port<input>  dldin[4];
	port<input>  dlcin[32];
	port<output> dlout[8];	

	port<input>  uldin[8];
	port<input>  ulcin[32];
	port<output> ulout[4];	

	// beamforming, 64 antenna, 32 layers
	DL64A32L<COL0+0, 0>  dlbf;
	UL64A32L<COL0+8, 0>  ulbf;

	
    TEST_BF(){
		
		// DLBF
		for(unsigned k=0;k<4; k++) connect<>(dldin[k], dlbf.din[k]);
		for(unsigned k=0;k<32;k++) connect<>(dlcin[k], dlbf.cin[k]);
		for(unsigned k=0;k<8; k++) connect<>(dlbf.out[k], dlout[k]);
		
		// ULBF
		for(unsigned k=0;k<8; k++) connect<>(uldin[k], ulbf.din[k]);
		for(unsigned k=0;k<32;k++) connect<>(ulcin[k], ulbf.cin[k]);
		for(unsigned k=0;k<4; k++) connect<>(ulbf.out[k], ulout[k]);
    };
	
	
}; // end of class

