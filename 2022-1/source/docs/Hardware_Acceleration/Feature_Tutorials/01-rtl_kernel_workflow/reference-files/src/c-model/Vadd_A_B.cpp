/**
* Copyright (C) 2020 Xilinx, Inc
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
#include <iostream>

#define BUFFER_SIZE 1024
#define DATA_SIZE 4096

// TRIPCOUNT identifier
const unsigned int c_len = DATA_SIZE / BUFFER_SIZE;
const unsigned int c_size = BUFFER_SIZE;

/*
    Vector Addition Kernel Implementation
    Arguments:
        size  (input)     --> Size of Vector in Integer
        A   (input)     --> Input Vector1
        B   (inout)     --> Input/Output Vector2
*/

extern "C" {
void Vadd_A_B(int size, // Size in integer
	      const unsigned int* A, // Read-Only Vector 1
	      unsigned int* B     // Output Result
	      ) {
    // In Vitis flow, a kernel can be defined without interface pragma. For such
    // case, it follows default behavioral.
    // All pointer arguments (in1,in2,out_r) will be referred as Memory Mapped
    // pointers using two M_AXI interfaces.
    // All the scalar arguments (size) and return argument will be associated to
    // single s_axilite interface.
    #pragma HLS INTERFACE m_axi port=A offset=slave bundle=m00_axi
    #pragma HLS INTERFACE m_axi port=B offset=slave bundle=m01_axi


    unsigned int A_buffer[BUFFER_SIZE];   // Local memory to store A
    unsigned int B_buffer[BUFFER_SIZE];   // Local memory to store B
    unsigned int vout_buffer[BUFFER_SIZE]; // Temporary to store result
/*
    // Per iteration of this loop perform BUFFER_SIZE vector addition
    for (int i = 0; i < size; i += BUFFER_SIZE) {
#pragma HLS LOOP_TRIPCOUNT min = c_len max = c_len
	std::cout << "Outer Loop: i = " << i << std::endl; 
        int chunk_size = BUFFER_SIZE;
        // boundary checks
        if ((i + BUFFER_SIZE) > size) chunk_size = size - i;
    // Transferring data in bursts hides the memory access latency as well as
    // improves bandwidth utilization and efficiency of the memory controller.
    // It is recommended to infer burst transfers from successive requests of data
    // from consecutive address locations.
    // A local memory vl_local is used for buffering the data from a single burst.
    // The entire input vector is read in multiple bursts.
    // The choice of LOCAL_MEM_SIZE depends on the specific applications and
    // available on-chip memory on target FPGA.
    // burst read of v1 and v2 vector from global memory
    read1:
        for (int j = 0; j < chunk_size; j++) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
#pragma HLS PIPELINE II = 1
            A_buffer[j] = A[i + j];
	    std::cout << "ReadA: j = " << j << " A_buffer[j]: " << A_buffer[j] << std::endl; 
        }
    read2:
        for (int j = 0; j < chunk_size; j++) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
#pragma HLS PIPELINE II = 1
            B_buffer[j] = B[i + j];
	    std::cout << "ReadB: j = " << j << " B_buffer[j]: " << B_buffer[j] << std::endl; 
        }
    // PIPELINE pragma reduces the initiation interval for loop by allowing the
    // concurrent executions of operations
    vadd:
        for (int j = 0; j < chunk_size; j++) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
#pragma HLS PIPELINE II = 1
            // perform vector addition
	    std::cout << "j = " << j << std::endl; 
            vout_buffer[j] = A_buffer[j] + B_buffer[j];
	    std::cout << "Vadd: vout_buffer[j] = " << vout_buffer[j] << std::endl;
	    std::cout << "Vadd: j = " << j << " vout_buffer[j]: " << vout_buffer[j] << std::endl; 
        }
    // burst write the result
    write:
        for (int j = 0; j < chunk_size; j++) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
#pragma HLS PIPELINE II = 1
	    std::cout << "Write: [i + j] = " << i + j << std::endl; 
	    std::cout << "Write: B[i + j] = " << vout_buffer[j] << std::endl;
            B[i + j] = vout_buffer[j];
        }
	}*/

    // PIPELINE pragma reduces the initiation interval for loop by allowing the
    // concurrent executions of operations
    vadd:
        for (int i = 0; i < BUFFER_SIZE; i++) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
#pragma HLS PIPELINE II = 1
            // perform vector addition
            vout_buffer[i] = A[i] + B[i];
	    std::cout << "Vadd: vout_buffer[i] = " << vout_buffer[i] << std::endl;
	    //d::cout << "Vadd: j = " << j << " vout_buffer[j]: " << vout_buffer[j] << std::endl; 
        }

    // burst write the result
    write:
        for (int j = 0; j < BUFFER_SIZE; j++) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
#pragma HLS PIPELINE II = 1
            B[j] = vout_buffer[j];
        }

    }
}
