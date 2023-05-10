/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/


#include <fstream>
#include <cstring>
#include "data.h"
#include "xrt/xrt.h"
#include "xrt/experimental/xrt_kernel.h"
#include "experimental/xrt_graph.h"


int main(int argc, char ** argv)
{
        char* xclbinFile=argv[1];
	auto device = xrt::device(0);
    	if(device == nullptr)
		throw std::runtime_error("No valid device handle found. Make sure using right xclOpen index.");
    	auto xclbin_uuid = device.load_xclbin(xclbinFile);
	//////////////////////////////////////////
	// graph execution for AIE
	//////////////////////////////////////////	
	
	auto cghdl = xrt::graph(device,xclbin_uuid,"mygraph_top");
	
	//Run th graph for 1 iteration
	printf("graph run\n");
	cghdl.run(1);
	
	//Graph end
	cghdl.end();
	printf("graph end\n");		
	printf("TEST PASSED\n");
	
	return  0;
}

