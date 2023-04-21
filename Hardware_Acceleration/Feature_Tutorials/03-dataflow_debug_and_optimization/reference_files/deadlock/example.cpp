/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/
#include "example.h"

void example(hls::stream<int>& A, hls::stream<int>& B){
#pragma HLS dataflow
#pragma HLS INTERFACE ap_fifo port=&A
#pragma HLS INTERFACE ap_fifo port=&B
    hls::stream<int> data_channel1;
    hls::stream<int> data_channel2;

    proc_1(A, data_channel1, data_channel2);
    proc_2(data_channel1, data_channel2, B);
}

void proc_1(hls::stream<int>& A, hls::stream<int>& B, hls::stream<int>& C){
#pragma HLS dataflow
    hls::stream<int> data_channel1;
    hls::stream<int> data_channel2;

    proc_1_1(A, data_channel1, data_channel2);
    proc_1_2(B, C, data_channel1, data_channel2);
}

void proc_1_1(hls::stream<int>& A, hls::stream<int>& data_channel1, hls::stream<int>& data_channel2){
  int i;
  int tmp;
  for(i = 0; i < 10; i++){
    tmp = A.read();
    data_channel1.write(tmp); 
  }
  for(i = 0; i < 10; i++){
    data_channel2.write(tmp); 
  }
}

void proc_1_2(hls::stream<int>& B, hls::stream<int>& C, hls::stream<int>& data_channel1, hls::stream<int>& data_channel2){
  int i;
  int tmp;
  
  for(i = 0; i < 10; i++){
    tmp = data_channel2.read() + data_channel1.read();
    B.write(tmp);
  }
  for(i = 0; i < 10; i++){
    C.write(tmp); 
  }
}

void proc_2(hls::stream<int>& A, hls::stream<int>& B, hls::stream<int>& C){
#pragma HLS dataflow
    hls::stream<int> data_channel1;
    hls::stream<int> data_channel2;

    proc_2_1(A, B, data_channel1, data_channel2);
    proc_2_2(C, data_channel1, data_channel2);
}

void proc_2_1(hls::stream<int>& A, hls::stream<int>& B, hls::stream<int>& data_channel1, hls::stream<int>& data_channel2){
  int i;
  int tmp;
  for(i = 0; i < 10; i++){
    tmp = A.read() + B.read();
    data_channel1.write(tmp); 
  }
  for(i = 0; i < 10; i++){
    data_channel2.write(tmp); 
  }
}

void proc_2_2(hls::stream<int>& C, hls::stream<int>& data_channel1, hls::stream<int>& data_channel2){
  int i;
  int tmp;
  for(i = 0; i < 10; i++){
    tmp = data_channel2.read() + data_channel1.read();
    C.write(tmp);
  }
}

