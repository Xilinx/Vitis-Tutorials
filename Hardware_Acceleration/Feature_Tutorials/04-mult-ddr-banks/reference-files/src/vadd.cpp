/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

/*******************************************************************************
Description:
    HLS pragmas can be used to optimize the design : improve throughput, reduce
latency and
    device resource utilization of the resulting RTL code
    This is vector addition example to demonstrate how HLS optimizations are
used in kernel.
*******************************************************************************/

#define BUFFER_SIZE 1024

/*
    Vector Addition Kernel Implementation
    Arguments:
        in1   (input)     --> Input Vector1
        in2   (input)     --> Input Vector2
        out   (output)    --> Output Vector
        size  (input)     --> Size of Vector in Integer
   */
extern "C" {
void vadd(const unsigned int *in1, // Read-Only Vector 1
          const unsigned int *in2, // Read-Only Vector 2
          unsigned int *out,       // Output Result
          int size                 // Size in integer
          ) {
#pragma HLS INTERFACE m_axi port = in1 offset = slave bundle = gmem0
#pragma HLS INTERFACE m_axi port = in2 offset = slave bundle = gmem1
#pragma HLS INTERFACE m_axi port = out offset = slave bundle = gmem2
  unsigned int v1_buffer[BUFFER_SIZE];   // Local memory to store vector1
  unsigned int v2_buffer[BUFFER_SIZE];   // Local memory to store vector2
  unsigned int vout_buffer[BUFFER_SIZE]; // Local Memory to store result

  // Per iteration of this loop perform BUFFER_SIZE vector addition
  for (int i = 0; i < size; i += BUFFER_SIZE) {
    int chunk_size = BUFFER_SIZE;
    // boundary checks
    if ((i + BUFFER_SIZE) > size)
      chunk_size = size - i;

    // Transferring data in bursts hides the memory access latency as well as
    // improves bandwidth utilization and efficiency of the memory controller.
    // It is recommended to infer burst transfers from successive requests of
    // data from consecutive address locations.
    // A local memory vl_local is used for buffering the data from a single
    // burst. The entire input vector is read in multiple bursts.
    // The choice of LOCAL_MEM_SIZE depends on the specific applications and
    // available on-chip memory on target FPGA.
    // burst read of v1 and v2 vector from global memory
    for (int j = 0; j < chunk_size; j++) {
      v1_buffer[j] = in1[i + j];
    }
    for (int j = 0; j < chunk_size; j++) {
      v2_buffer[j] = in2[i + j];
    }

  // PIPELINE pragma reduces the initiation interval for loop by allowing the
  // concurrent executions of operations
  vadd:
    for (int j = 0; j < chunk_size; j++) {
      // perform vector addition
      vout_buffer[j] = v1_buffer[j] + v2_buffer[j];
    }
    // burst write the result
    for (int j = 0; j < chunk_size; j++) {
      out[i + j] = vout_buffer[j];
    }
  }
}
}
