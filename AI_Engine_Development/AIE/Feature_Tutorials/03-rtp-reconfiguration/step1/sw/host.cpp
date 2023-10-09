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
#include "xrt/xrt_graph.h"

#define OUTPUT_SIZE 512

int run(int argc, char* argv[]){

	size_t output_size_in_bytes = OUTPUT_SIZE * sizeof(int);

	if(argc != 2) {
		std::cout << "Usage: " << argv[0] <<" <xclbin>" << std::endl;
		return EXIT_FAILURE;
	}
	char* xclbinFilename = argv[1];
	
	int ret;
	// Open xclbin
	auto device = xrt::device(0); //device index=0
	auto uuid = device.load_xclbin(xclbinFilename);

	// s2mm & random_noise kernel handle
	auto s2mm = xrt::kernel(device, uuid, "s2mm");

	// output memory
	auto out_bo = xrt::bo(device, output_size_in_bytes,s2mm.group_id(0));
	auto host_out=out_bo.map<std::complex<short>*>();

	//kernel run
	auto s2mm_run = s2mm(out_bo, nullptr, OUTPUT_SIZE);//1st run for s2mm has started

	int coeffs_readback[12]={0,0,0,0,0,0,0,0,0,0,0,0};

	auto ghdl=xrt::graph(device,uuid,"gr");
	ghdl.run(4);
	ghdl.update("gr.dds.in[0]",10);
	ghdl.update("gr.dds.in[0]",10);
	ghdl.update("gr.dds.in[0]",100);
	ghdl.update("gr.dds.in[0]",100);
	ghdl.end();

	s2mm_run.wait();
	out_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

	std::ofstream out("out.txt",std::ofstream::out);
	std::ifstream golden("data/golden.txt",std::ifstream::in);
	short g_real=0,g_imag=0;
	int match = 0;
	for (int i = 0; i < OUTPUT_SIZE; i++) {
		golden >> std::dec >> g_real;
		golden >> std::dec >> g_imag;
		if(g_real!=host_out[i].real() || g_imag!=host_out[i].imag()){
			std::cout<<"ERROR: i="<<i<<" gold.real="<<g_real<<" gold.imag="<<g_imag<<" out.real="<<host_out[i].real()<<" out.imag="<<host_out[i].imag()<<std::endl;
			match=1;
		}
		out<<host_out[i].real()<<" "<<host_out[i].imag()<<" "<<std::endl;
	}
	out.close();
	golden.close();

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
