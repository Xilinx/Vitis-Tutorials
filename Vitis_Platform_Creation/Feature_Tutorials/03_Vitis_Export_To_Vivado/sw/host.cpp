/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include <fstream>
#include <cstring>

#include "experimental/xrt_kernel.h"
#include "experimental/xrt_graph.h"
#include "data.h" //Contains input and golden output data 

#define SAMPLES 256

int main(int argc, char* argv[])
{
	//////////////////////////////////////////
	// Open xclbin
	//////////////////////////////////////////	

	char* xclbinFile=argv[1];
	auto device = xrt::device(0);
    	if(device == nullptr)
		throw std::runtime_error("No valid device handle found. Make sure using right xclOpen index.");
    	auto xclbin_uuid = device.load_xclbin(xclbinFile);

    	int sizeIn = SAMPLES/2;
	int sizeOut = SAMPLES;

	
	//Obtains the graph handle from the XCLBIN that is loaded into the device
	auto cghdl = xrt::graph(device,xclbin_uuid,"clipgraph");
	
	printf("graph run\n");
	//Run th graph for 1 iteration
	cghdl.run(1);
	
	//Graph end
	cghdl.end();
	printf("graph end\n");	
	
	printf("in number:");
	system("devmem 0xa4010010");
	printf("error number:");
	system("devmem 0xa4010020");
	printf("TEST IS DONE");
	/*if (in_num != 0 && err_num == 0)
	/{
		printf("TEST IS PASSED");
	}
	else
	{	
		printf("TEST IS FAIL");
	}*/

}
