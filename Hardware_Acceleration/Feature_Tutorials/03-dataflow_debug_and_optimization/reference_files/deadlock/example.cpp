/*
 * Copyright 2021 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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

