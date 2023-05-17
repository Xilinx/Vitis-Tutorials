/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#include <ap_int.h>
#include <hls_stream.h>
#include <assert.h>

void read(const ap_int<512>         *input,
	  hls::stream<ap_int<512> > &inStream,
	  unsigned int              numInputs) {
  for(unsigned int i = 0; i < numInputs; i++) {
    #pragma HLS PIPELINE
    inStream.write(input[i]);
  }
}

void exec(hls::stream<ap_int<512> > &inStream,
	  hls::stream<ap_int<512> > &outStream,
	  unsigned int              numInputs,
	  unsigned int              processDelay) {
  for(int num = 0; num < numInputs; num++) {
    ap_int<512> in = inStream.read();
    for(int i = 0; i < processDelay; i++) {
      in += 1;
    }
    outStream.write(in);
  }
}
	  
void write(hls::stream<ap_int<512> > &outStream,
	   ap_int<512>               *output,
	   unsigned int              numInputs) {
  for(unsigned int i = 0; i < numInputs; i++) {
    #pragma HLS PIPELINE
    output[i] = outStream.read();
  }
}


void pass_dataflow(const ap_int<512> *input,
		   ap_int<512>       *output,
		   unsigned int      numInputs,
		   unsigned int      processDelay) {
  #pragma HLS DATAFLOW

  assert(numInputs >= 1);
  assert(processDelay >=1);

  hls::stream<ap_int<512> > inStream;
  hls::stream<ap_int<512> > outStream;

  read(input,      inStream,  numInputs);
  exec(inStream,   outStream, numInputs, processDelay);
  write(outStream, output,    numInputs);
  
}

extern "C" {
void pass(const ap_int<512> *input,
	  ap_int<512>       *output,
	  unsigned int      numInputs,
	  unsigned int      processDelay) {
  #pragma HLS INTERFACE m_axi     port=input     offset=slave bundle=p0
  #pragma HLS INTERFACE m_axi     port=output    offset=slave bundle=p1
  
  pass_dataflow(input, output, numInputs, processDelay);
}
}
	  
