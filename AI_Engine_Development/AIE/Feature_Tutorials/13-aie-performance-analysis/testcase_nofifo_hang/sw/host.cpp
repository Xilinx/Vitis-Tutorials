/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <complex>
#include "xrt/xrt_kernel.h"
#include "xrt/xrt_aie.h"

#define OUTPUT_SIZE 2048

int run(int argc, char* argv[]){
	size_t output_size_in_bytes = OUTPUT_SIZE * sizeof(int);

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
	for(int i=0;i<OUTPUT_SIZE;i++){
		host_in[i]=i;
	}

	xrt::aie::profiling handle(device), handle2(device);
	handle.start(xrt::aie::profiling::profiling_option::io_stream_running_event_count, "gr.dataout", "", 0);
	handle2.start(xrt::aie::profiling::profiling_option::io_stream_running_event_count, "gr.in", "", 0);

	//kernel run
	auto s2mm_run = s2mm(out_bo, nullptr, OUTPUT_SIZE);//1st run for s2mm has started
	auto mm2s_run = mm2s(in_bo, nullptr, OUTPUT_SIZE);
	auto ghdl=xrt::graph(device,uuid,"gr");
	ghdl.run(4);
	// Wait graph for some cycles
	ghdl.end(5000); // wait for AIE kernel to complete or 5000 AIE cycles

	long long data_out_count = handle.read();
	long long data_in_count = handle2.read();
	handle.stop();
	handle2.stop();
	std::cout<<"Output data received:"<<data_out_count<<std::endl;
	std::cout<<"Input data sent:"<<data_in_count<<std::endl;

	out_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

	std::ofstream out("out.txt",std::ofstream::out);
	int match = 0;
	for (int i = 0; i < OUTPUT_SIZE; i++) {
		if(host_in[i]*2!=host_out[i]){
			std::cout<<"Expected mis-match: i="<<i<<" gold="<<host_in[i]*2<<" out="<<host_out[i]<<std::endl;
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
		std::cout << "TEST " << (match ? "PASSED expected by timeout" : "FAILED") << "\n";
		return (match ?  EXIT_SUCCESS :  EXIT_FAILURE);
	}	
		catch (std::exception const& e) {
		std::cout << "Exception: " << e.what() << "\n";
		std::cout << "FAILED TEST\n";
		return 1;
	}
}
