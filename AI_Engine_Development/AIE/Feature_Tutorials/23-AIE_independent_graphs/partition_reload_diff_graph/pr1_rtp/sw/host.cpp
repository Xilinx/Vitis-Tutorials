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
int run(char* xclbinFilename_pl, char* xclbinFilename_aie, int rtp_type){

	size_t output_size_in_bytes = ELEM_per_iter*4*ITERATION;
	int ret;
	// Open xclbin
	auto dhdl = xrt::device(0); //device index=0
	auto xclbin_pl = xrt::xclbin(std::string(xclbinFilename_pl));
	auto uuid_pl = dhdl.register_xclbin(xclbin_pl);
	xrt::hw_context hwctx_pl{dhdl, uuid_pl};
	auto xclbin_aie = xrt::xclbin(std::string(xclbinFilename_aie));
	auto uuid_aie = dhdl.register_xclbin(xclbin_aie);
	xrt::hw_context hwctx_aie{dhdl, uuid_aie};

	// s2mm & datagen kernel handle
	auto s2mm = xrt::kernel(hwctx_pl, "s2mm:{s2mm_1}");
	auto datagen = xrt::kernel(hwctx_pl, "datagen");

	// output memory
	auto out_bo = xrt::bo(hwctx_pl, output_size_in_bytes,static_cast<xrt::bo::flags>(0),s2mm.group_id(0));
	auto host_out=out_bo.map<int*>();

	//kernel run
	auto s2mm_run = s2mm(out_bo, nullptr, ELEM_per_iter*ITERATION);//1st run for s2mm has started
	auto datagen_run = datagen(nullptr, ELEM_per_iter*ITERATION,0);

	auto ghdl=xrt::graph(hwctx_aie,"gr");
	ghdl.run(ITERATION);
	int value[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	if(rtp_type==0){
		ghdl.update("gr.k.in[1]",10);
	}else if(rtp_type==1){
		ghdl.update("gr.k.in[1]",value);
	}
	ghdl.end();

	s2mm_run.wait();
	out_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

	int match = 0;	
	int base=10;
	for (int i = 0; i < ITERATION; i++) {
		for(int j=0;j<ELEM_per_iter;j++){
			if(rtp_type==0){
				if(host_out[ELEM_per_iter*i+j]!=base+i*ELEM_per_iter+j){
					match=1;
				}
			}else if(rtp_type==1){
				if(host_out[ELEM_per_iter*i+j]!=value[j%16]+i*ELEM_per_iter+j){
					match=1;
				}
			}
		}
	}

	return match;
}

int main(int argc, char* argv[])
{
	try {
		if(argc != 3 && argc != 4) {
			std::cout << "Usage: " << argv[0] <<" <pl.xclbin> <aie_only.xclbin> <aie_only_second.xclbin>" << std::endl;
			return EXIT_FAILURE;
		}
		char* xclbinFilename_pl = argv[1];
		char* xclbinFilename_aie = argv[2];
		auto match = run(xclbinFilename_pl, xclbinFilename_aie,0);
		std::cout << "TEST PR1 Graph0 " << (match ? "FAILED" : "PASSED") << "\n";
		if(argc==4){//Do second xclbin reload
			xclbinFilename_aie = argv[3];
			match = run(xclbinFilename_pl, xclbinFilename_aie,1);
			std::cout << "TEST PR1 SECOND GRAPH " << (match ? "FAILED" : "PASSED") << "\n";
		}
		return (match ? EXIT_FAILURE :  EXIT_SUCCESS);
	}	
		catch (std::exception const& e) {
		std::cout << "Exception: " << e.what() << "\n";
		std::cout << "FAILED TEST\n";
		return 1;
	}
}
