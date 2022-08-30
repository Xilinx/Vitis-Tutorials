/* (c) Copyright 2020 Xilinx, Inc.
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
 
#include <adf.h>
#include "subsys.h" 

using namespace adf;

template<int COL0, int COL1, int COL2>
class TEST_BF: public graph{
  
public:

	port<input>  dldin[4*3];
	port<input>  dlcin[32*3];
	port<output> dlout[8*3];	

	port<input>  uldin[8*3];
	port<input>  ulcin[32*3];
	port<output> ulout[4*3];	

	// beamforming, 64 antenna, 32 layers
	DL64A32L<COL0+0, 0>  dlbf_0;
	DL64A32L<COL1+0, 0>  dlbf_1;
	DL64A32L<COL2+0, 0>  dlbf_2;
	UL64A32L<COL0+8, 0>  ulbf_0;
	UL64A32L<COL1+8, 0>  ulbf_1;
	UL64A32L<COL2+8, 0>  ulbf_2;

	
    TEST_BF(){
		
		// DLBF
		for(unsigned k=0;k<4; k++) connect<>(dldin[k], dlbf_0.din[k]);
		for(unsigned k=0;k<32;k++) connect<>(dlcin[k], dlbf_0.cin[k]);
		for(unsigned k=0;k<8; k++) connect<>(dlbf_0.out[k], dlout[k]);

		for(unsigned k=0;k<4; k++) connect<>(dldin[4+k], dlbf_1.din[k]);
		for(unsigned k=0;k<32;k++) connect<>(dlcin[32+k], dlbf_1.cin[k]);
		for(unsigned k=0;k<8; k++) connect<>(dlbf_1.out[k], dlout[8+k]);

		for(unsigned k=0;k<4; k++) connect<>(dldin[8+k], dlbf_2.din[k]);
		for(unsigned k=0;k<32;k++) connect<>(dlcin[64+k], dlbf_2.cin[k]);
		for(unsigned k=0;k<8; k++) connect<>(dlbf_2.out[k], dlout[16+k]);
		
		// ULBF
		for(unsigned k=0;k<8; k++) connect<>(uldin[k], ulbf_0.din[k]);
		for(unsigned k=0;k<32;k++) connect<>(ulcin[k], ulbf_0.cin[k]);
		for(unsigned k=0;k<4; k++) connect<>(ulbf_0.out[k], ulout[k]);

		for(unsigned k=0;k<8; k++) connect<>(uldin[8+k], ulbf_1.din[k]);
		for(unsigned k=0;k<32;k++) connect<>(ulcin[32+k], ulbf_1.cin[k]);
		for(unsigned k=0;k<4; k++) connect<>(ulbf_1.out[k], ulout[4+k]);

		for(unsigned k=0;k<8; k++) connect<>(uldin[16+k], ulbf_2.din[k]);
		for(unsigned k=0;k<32;k++) connect<>(ulcin[64+k], ulbf_2.cin[k]);
		for(unsigned k=0;k<4; k++) connect<>(ulbf_2.out[k], ulout[8+k]);
    };
	
	
}; // end of class

