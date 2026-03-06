/*
Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

 // XRT APIS are used for both the AIE Graph and PL Kernels
#include "experimental/xrt_kernel.h" 
#include "experimental/xrt_graph.h" 
#include <cstring> 
#include <iostream> 
#include <fstream>
#include "input_data.h"
#include "input1_data.h"
#include "golden_data.h"

#define DATA_SIZE_IN0 16
#define DATA_SIZE_IN1 4
#define DATA_SIZE_OUT0 4
#define N_ITER 1

using namespace std;

int main(int argc, char** argv) 
{
	// Load the xclbin containing the design
	unsigned int dev_index = 0;
	char* xclbinFilename = argv[1];
	auto device = xrt::device(dev_index);
	auto xclbin_uuid = device.load_xclbin(xclbinFilename);
	int tmp1,tmp2;

	// Input prepration
	int sizePLIn0  = DATA_SIZE_IN0;
	int* DataInput0 = new int[sizePLIn0];
	for(int i=0;i<sizePLIn0;i++){
	DataInput0[i]= input_data[i];
		printf("DataInput0[%d]=%d\n", i,DataInput0[i]);
	}

	int sizePLIn1  = DATA_SIZE_IN1;
	int* DataInput1 = new int[sizePLIn1];
	for(int i=0;i<sizePLIn1;i++){
	DataInput1[i]= input1_data[i];
		printf("DataInput1[%d]=%d\n", i,DataInput1[i]);
	}

	int sizePLOut0 = DATA_SIZE_OUT0;
	int* goldenPL0     = new int[sizePLOut0];

	for(int i=0;i<sizePLOut0;i++){
        goldenPL0[i]=golden_data[i] ;
		printf("goldenPL0[%d]=%d\n", i,goldenPL0[i]);
	}

	// Buffer allocation and mapping
	auto in_bohdl0 = xrt::bo(device, sizePLIn0*sizeof(int), 0, 0);
	auto in_bomapped0 = in_bohdl0.map<uint32_t*>();
	memcpy(in_bomapped0, DataInput0, sizePLIn0 * sizeof(int));
	printf("Input memory virtual addr 0x%px\n", in_bomapped0);
	
	auto in_bohdl1 = xrt::bo(device, sizePLIn1*sizeof(int), 0, 0);
	auto in_bomapped1 = in_bohdl1.map<uint32_t*>();
	memcpy(in_bomapped1, DataInput1, sizePLIn1 * sizeof(int));
	printf("Input memory virtual addr 0x%px\n", in_bomapped1);
	
	auto out_bohdl0 = xrt::bo(device, sizePLOut0*sizeof(int), 0, 0);
	auto out_bomapped0 = out_bohdl0.map<uint32_t*>();
	memset(out_bomapped0, 0xABCDEF00, sizePLOut0 * sizeof(int));
	printf("Output memory virtual addr 0x%px\n", out_bomapped0);
	
	in_bohdl0.sync(XCL_BO_SYNC_BO_TO_DEVICE, sizePLIn0 * sizeof(int), 0);
	in_bohdl1.sync(XCL_BO_SYNC_BO_TO_DEVICE, sizePLIn1 * sizeof(int), 0);
	out_bohdl0.sync(XCL_BO_SYNC_BO_FROM_DEVICE, sizePLOut0 * sizeof(int), 0);
	
	// Open the Kernels
	auto mm2s_1 = xrt::kernel(device, xclbin_uuid, "mm2s:{mm2s_1}");
	auto mm2s_1_rhdl = mm2s_1(in_bohdl0, nullptr, sizePLIn0 );
	auto mm2s_2 = xrt::kernel(device, xclbin_uuid, "mm2s:{mm2s_2}");
	auto mm2s_2_rhdl = mm2s_2(in_bohdl1, nullptr, sizePLIn1 );
	auto s2mm_1 = xrt::kernel(device, xclbin_uuid, "s2mm:{s2mm}");
	auto s2mm_1_rhdl = s2mm_1(out_bohdl0, nullptr, sizePLOut0 );
	auto graph = xrt::graph(device, xclbin_uuid, "mygraph");
	
	// Run the Kernels
	graph.run(N_ITER);
	mm2s_1_rhdl.wait();
	mm2s_2_rhdl.wait();
	s2mm_1_rhdl.wait();

	// Check the result with golden output
	int errorCount = 0;
	for (int i = 0; i < sizePLOut0; i++) {
		printf("OUTPUT[%d]=%d\n", i,out_bomapped0[i]);
		if ((signed)out_bomapped0[i] != (signed)goldenPL0[i]) {
		printf("Error found @ %d, %ld != %ld\n", i, (long int)out_bomapped0[i], (long int)goldenPL0[i]);
		errorCount++;
	}
}
	
	// Closing the run and printing the results
	delete[] DataInput0;
	delete[] DataInput1;
	delete[] goldenPL0;
	graph.end();
	if (errorCount) {
	    printf("\n");
            printf("Test failed with %d errors\n", errorCount);
	    printf("\n"); }
  	else {
	    printf("\n"); 
     	    printf("TEST PASSED\n");
	    printf("\n"); }
	return errorCount;
}
