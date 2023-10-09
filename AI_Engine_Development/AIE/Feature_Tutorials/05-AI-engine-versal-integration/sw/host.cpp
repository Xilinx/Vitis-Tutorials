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

	//////////////////////////////////////////
	// input memory
	// Allocating the input size of sizeIn to MM2S
	// MM2S module transfers input data from PL to the AI Engine
	//////////////////////////////////////////
		
	auto in_bohdl = xrt::bo(device, sizeIn * sizeof(int16_t) * 2, 0, 0);
	auto in_bomapped = in_bohdl.map<uint32_t*>();
	memcpy(in_bomapped, cint16Input, sizeIn * sizeof(int16_t) * 2);
	printf("Input memory virtual addr 0x%px\n", in_bomapped);

	in_bohdl.sync(XCL_BO_SYNC_BO_TO_DEVICE);

	//////////////////////////////////////////
	// output memory
	// Allocating the output size of sizeOut to S2MM
	// S2MM module receives the output data from AI Engine 
	//////////////////////////////////////////
	
	auto out_bohdl = xrt::bo(device, sizeOut * sizeof(int), 0, 0);
	auto out_bomapped = out_bohdl.map<uint32_t*>();
	memset(out_bomapped, 0xABCDEF00, sizeOut * sizeof(int));
	printf("Output memory virtual addr 0x%px\n", out_bomapped);

	////////////////////////////////////////////////////////
	// mm2s ip - Creating kernel handle using xrt::kernel API
	///////////////////////////////////////////////////////	
	
	auto mm2s_khdl = xrt::kernel(device, xclbin_uuid, "mm2s");
	auto mm2s_rhdl = mm2s_khdl(in_bohdl, nullptr, sizeIn);
	printf("run mm2s\n");

	////////////////////////////////////////////////////////
	// s2mm ip - Creating kernel handle using xrt::kernel API
	///////////////////////////////////////////////////////		
	
	auto s2mm_khdl = xrt::kernel(device, xclbin_uuid, "s2mm");
	auto s2mm_rhdl = s2mm_khdl(out_bohdl, nullptr, sizeOut);
	printf("run s2mm\n");

	//////////////////////////////////////////
	// graph execution for AIE
	//////////////////////////////////////////	
	
	//Obtains the graph handle from the XCLBIN that is loaded into the device
	auto cghdl = xrt::graph(device,xclbin_uuid,"clipgraph");
	
	printf("graph run\n");
	//Run th graph for 1 iteration
	cghdl.run(1);
	
	//Graph end
	cghdl.end();
	printf("graph end\n");	
	
	
	//////////////////////////////////////////
	// wait for mm2s done
	//////////////////////////////////////////	
	
	mm2s_rhdl.wait();

	//////////////////////////////////////////
	// wait for s2mm done
	//////////////////////////////////////////	
	
	s2mm_rhdl.wait();

	out_bohdl.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
	
	//////////////////////////////////////////
	// Comparing the execution data to the golden data
	//////////////////////////////////////////	
	
	int errorCount = 0;
	{
		for (int i = 0; i < sizeOut; i++)
		{
				if ((signed)out_bomapped[i] != golden[i])
				{
					printf("Error found @ %d, %d != %d\n", i, out_bomapped[i], golden[i]);
					errorCount++;
				}
		}

		if (errorCount)
			printf("Test failed with %d errors\n", errorCount);
		else
			printf("TEST PASSED\n");
	}
	
    
	std::cout << "Releasing remaining XRT objects...\n";
	
	return errorCount;
}
