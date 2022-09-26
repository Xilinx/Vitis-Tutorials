/**
* Copyright (C) 2019-2021 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/
// TRIPCOUNT indentifier


#include <ap_int.h>
#include <hls_stream.h>
#ifndef __SYNTHESIS__
#include <iostream>
#endif

const unsigned int c_min = 4096;
const unsigned int c_max = 10 * 300 * 4096;
const unsigned int num = 3;



extern "C"{

void search(float* result,float* longitude,float* latitude,float* out,int size){

    #ifndef __SYNTHESIS__    
    std::cout << "kernel_search call success" << std::endl;
    #endif
    
	for (int i =0; i < size; i++){
	#pragma HLS LOOP_TRIPCOUNT min = c_min max = c_max
		out[num*i] = longitude[i];
		out[num*i+1] = latitude[i];
		out[num*i+2] = result[i];
	}

    #ifndef __SYNTHESIS__
        std::cout << "kernel_search call finish" << std::endl;
    #endif

}


}
