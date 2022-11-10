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
