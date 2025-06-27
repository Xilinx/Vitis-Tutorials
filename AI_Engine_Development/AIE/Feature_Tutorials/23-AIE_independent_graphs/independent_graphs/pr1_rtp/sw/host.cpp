/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <math.h>
#include "xrt/xrt_kernel.h"
#include "xrt/xrt_graph.h"

const int ITERATION=4;
const int ELEM_per_iter=256;
int run(int argc, char* argv[]){

	size_t output_size_in_bytes = ELEM_per_iter*4*ITERATION;

	if(argc != 2) {
		std::cout << "Usage: " << argv[0] <<" <xclbin>" << std::endl;
		return EXIT_FAILURE;
	}
	char* xclbinFilename = argv[1];
	
	int ret;
	// Open xclbin
	auto device = xrt::device(0); //device index=0
	auto uuid = device.load_xclbin(xclbinFilename);

	// s2mm & datagen kernel handle
	auto s2mm = xrt::kernel(device, uuid, "s2mm:{s2mm_1}");
	auto datagen = xrt::kernel(device, uuid, "datagen");

	// output memory
	auto out_bo = xrt::bo(device, output_size_in_bytes,s2mm.group_id(0));
	auto host_out=out_bo.map<int*>();

	//kernel run
	auto s2mm_run = s2mm(out_bo, nullptr, ELEM_per_iter*ITERATION);//1st run for s2mm has started
	auto datagen_run = datagen(nullptr, ELEM_per_iter*ITERATION,0);

	auto ghdl=xrt::graph(device,uuid,"pr1_gr");
	ghdl.run(ITERATION);
	ghdl.update("pr1_gr.k.in[1]",10);
	ghdl.end();

	s2mm_run.wait();
	out_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

	int match = 0;	
	int base=10;
	for (int i = 0; i < ITERATION; i++) {
		for(int j=0;j<ELEM_per_iter;j++){
			if(host_out[ELEM_per_iter*i+j]!=base+i*ELEM_per_iter+j){
				match=1;
			}
		}
	}

	return match;
}

int main(int argc, char* argv[])
{
	try {
		auto match = run(argc, argv);
		std::cout << "TEST " << (match ? "FAILED" : "PASSED") << "\n";
		return (match ? EXIT_FAILURE :  EXIT_SUCCESS);
	}	
		catch (std::exception const& e) {
		std::cout << "Exception: " << e.what() << "\n";
		std::cout << "FAILED TEST\n";
		return 1;
	}
}
