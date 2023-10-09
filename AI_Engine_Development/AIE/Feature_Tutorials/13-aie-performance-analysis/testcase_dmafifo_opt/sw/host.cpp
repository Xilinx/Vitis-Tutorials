/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <complex>
#include "adf/adf_api/XRTConfig.h"
#include "experimental/xrt_kernel.h"

#include "graph.cpp"

using namespace adf;

int run(int argc, char* argv[]){
	size_t iterations = 100;
	size_t bytes_per_iteration=4096;
	size_t output_size_in_bytes = iterations * bytes_per_iteration;
	size_t OUTPUT_SIZE = output_size_in_bytes / 16; //128bits width interface

	//TARGET_DEVICE macro needs to be passed from gcc command line
	if(argc != 2) {
		std::cout << "Usage: " << argv[0] <<" <xclbin>" << std::endl;
		return EXIT_FAILURE;
	}
	char* xclbinFilename = argv[1];
	
	int ret;
	// Open xclbin
	auto device = xrt::device(0); //device index=0
	auto uuid = device.load_xclbin(xclbinFilename);
	auto dhdl = xrtDeviceOpenFromXcl(device);

	// s2mm & mm2s kernel handle
	auto s2mm = xrt::kernel(device, uuid, "s2mm");
	auto mm2s = xrt::kernel(device, uuid, "mm2s");

	// output memory
	auto out_bo = xrt::bo(device, output_size_in_bytes, s2mm.group_id(0));
	auto host_out=out_bo.map<int*>();

	// input memory
	auto in_bo = xrt::bo(device, output_size_in_bytes, mm2s.group_id(0));
	auto host_in=in_bo.map<int*>();

	// Initialization input memory
	for(int i=0;i<output_size_in_bytes/4;i++){
		host_in[i]=i;
	}

	//kernel run
	auto s2mm_run = s2mm(out_bo, nullptr, OUTPUT_SIZE);//1st run for s2mm has started
	auto mm2s_run = mm2s(in_bo, nullptr, OUTPUT_SIZE);

	// graph run
	adf::registerXRT(dhdl, uuid.get());
	std::cout<<"Register XRT"<<std::endl;

	event::handle handle = event::start_profiling(gr.dataout, event::io_stream_start_to_bytes_transferred_cycles, output_size_in_bytes);
	if(handle==event::invalid_handle){
		printf("ERROR:Invalid handle. Only two performance counter in a AIE-PL interface tile\n");
		return 1;
	}

	gr.run(iterations);
	gr.end();
	s2mm_run.wait();
	// Wait graph for some cycles
	long long cycle_count = event::read_profiling(handle);
	std::cout<<"cycle count:"<<cycle_count<<std::endl;
	event::stop_profiling(handle);
	double throughput = (double)output_size_in_bytes / (cycle_count *0.8 * 1e-3); //Every AIE cycle is 0.8ns in production board
	std::cout<<"Throughput of the graph: "<<throughput<<" MB/s"<<std::endl;
	out_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

	std::ofstream out("out.txt",std::ofstream::out);
	int match = 0;
	for (int i = 0; i < OUTPUT_SIZE; i++) {
		if(host_in[i]*2!=host_out[i]){
			std::cout<<"ERROR: i="<<i<<" gold="<<host_in[i]*2<<" out="<<host_out[i]<<std::endl;
			match=1;
			break;
		}
		out<<host_out[i]<<std::endl;
	}
	out.close();

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
