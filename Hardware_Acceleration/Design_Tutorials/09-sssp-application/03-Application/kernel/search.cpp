/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
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
