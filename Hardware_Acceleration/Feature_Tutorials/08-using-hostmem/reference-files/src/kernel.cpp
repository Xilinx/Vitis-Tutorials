/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#include <ap_int.h>

#define BUFFER_SIZE 128
#define DATAWIDTH 512
#define DATATYPE_SIZE 32
#define VECTOR_SIZE (DATAWIDTH / DATATYPE_SIZE) 

typedef ap_uint<DATAWIDTH> uint512_dt;
typedef ap_uint<DATATYPE_SIZE> din_type;
typedef ap_uint<DATATYPE_SIZE + 1> dout_type;

extern "C" {
void vadd(const uint512_dt *in1, 
          const uint512_dt *in2, 
          uint512_dt *out,       
          int size              
          ) {
#pragma HLS INTERFACE m_axi port=in1 bundle=gmem num_write_outstanding=32 max_write_burst_length=64  num_read_outstanding=32 max_read_burst_length=64  offset=slave
#pragma HLS INTERFACE m_axi port=in2 bundle=gmem num_write_outstanding=32 max_write_burst_length=64  num_read_outstanding=32 max_read_burst_length=64  offset=slave
#pragma HLS INTERFACE m_axi port=out bundle=gmem num_write_outstanding=32 max_write_burst_length=64 num_read_outstanding=32 max_read_burst_length=64 offset=slave

  uint512_dt v1_local[BUFFER_SIZE];    
  uint512_dt result_local[BUFFER_SIZE]; 

  int size_in16 = size / VECTOR_SIZE ;

  for (int i = 0; i < size_in16; i += BUFFER_SIZE) {

  v1_rd: for (int j = 0; j < BUFFER_SIZE; j++) {
      v1_local[j] = in1[i + j];
    }

  v2_rd_add: for (int j = 0; j < BUFFER_SIZE; j++) {
      uint512_dt tmpV1 = v1_local[j];
      uint512_dt tmpV2 = in2[i + j];
      uint512_dt tmpOut = 0;
      din_type val1, val2;
      dout_type res;

    v2_parallel_add: for (int i = 0; i < VECTOR_SIZE; i++) {
        #pragma HLS UNROLL
        val1 = tmpV1.range(DATATYPE_SIZE * (i + 1) - 1, i * DATATYPE_SIZE);
        val2 = tmpV2.range(DATATYPE_SIZE * (i + 1) - 1, i * DATATYPE_SIZE);
        res = val1 + val2; 
        tmpOut.range(DATATYPE_SIZE * (i + 1) - 1, i * DATATYPE_SIZE) = res; 
      }
      result_local[j] = tmpOut;
    }

  out_write: for (int j = 0; j < BUFFER_SIZE; j++) {
      out[i + j] = result_local[j];
    }
  }
}
}
