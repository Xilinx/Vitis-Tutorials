/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/
// TRIPCOUNT indentifier

#include <ap_int.h>
#include <hls_stream.h>
#include <cmath>
#ifndef __SYNTHESIS__
#include <iostream>
#endif


const unsigned int c_min = 4096;
const unsigned int c_max = 10 * 300 * 4096;
const float b = 0.15;
const unsigned int p = 4;


static void compute(float* cost,float* flow_free_time,float* Volume,float* capacity,float* weight, 
                    int size){

    for (int i =0; i < size; i++){
    #pragma HLS LOOP_TRIPCOUNT min = c_min max = c_max   
        weight[i] = 0.6 * cost[i] +  0.4 * flow_free_time[i]*(1 + b*powf(Volume[i]/capacity[i],p));

    }

}


extern "C" {

void weight_average(float* cost,float* flow_free_time,float* Volume,float* capacity,float* weight, 
                    int size) {

    #ifndef __SYNTHESIS__
        std::cout << "kernel_wa call success" << std::endl;
    #endif

        compute(cost,flow_free_time,Volume,capacity,weight,size);
        

    #ifndef __SYNTHESIS__
        std::cout << "kernel_wa call finish" << std::endl;
    #endif  

}
}
