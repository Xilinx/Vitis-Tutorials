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
#define NITER 4

int main(int argc, char ** argv)
{

	//////////////////////////////////////////
	// Open xclbin
	//////////////////////////////////////////

	char* xclbinFile=argv[1];
	auto device = xrt::device(0);
    if(device == nullptr)
		throw std::runtime_error("No valid device handle found. Make sure using right xclOpen index.");
    auto xclbin_uuid = device.load_xclbin(xclbinFile);

	int sizeIn = NITER*SAMPLES/2;
	int sizeOut = NITER*SAMPLES;
	int errorCount = 0;


	#ifndef EXTERNAL_IO
	//////////////////////////////////////////
	// input memory
	// Allocating the input size of sizeIn to MM2S
	// This is using low-level XRT call xclAllocBO to allocate the memory
	//////////////////////////////////////////

	auto in_bohdl = xrt::bo(device, sizeIn * sizeof(int16_t) * 2, 0, 0);
	auto in_bomapped = in_bohdl.map<uint32_t*>();
	memcpy(in_bomapped, cint16Input, sizeIn * sizeof(int16_t) * 2);
	printf("Input memory virtual addr 0x%px\n", in_bomapped);


	//////////////////////////////////////////
	// output memory
	// Allocating the output size of sizeOut to S2MM
	// This is using low-level XRT call xclAllocBO to allocate the memory
	//////////////////////////////////////////

	auto out_bohdl = xrt::bo(device, sizeOut * sizeof(int), 0, 0);
	auto out_bomapped = out_bohdl.map<uint32_t*>();
	memset(out_bomapped, 0xABCDEF00, sizeOut * sizeof(int));
	printf("Output memory virtual addr 0x%px\n", out_bomapped);


	#if defined(__SYNCBO_ENABLE__)
	printf("Input Buffer Object Synchronization\n");
	in_bohdl.sync(XCL_BO_SYNC_BO_TO_DEVICE);
	#endif


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
	// polar_clip IP
	// Using the xrtPLKernelOpen function to manually control the PL Kernel
	// that is outside of the AI Engine graph
	//////////////////////////////////////////

	auto polar_clip_khdl = xrt::kernel(device, xclbin_uuid, "polar_clip");
	// Need to provide the kernel handle, and the argument order of the kernel arguments
	// Here the out_bohdl is the output buffer, the nullptr is the streaming interface and must be null,
	// lastly, the size of the data. This info can be found in the kernel definition.
	auto polar_clip_rhdl = polar_clip_khdl( nullptr, nullptr);
	printf("run polar_clip\n");






	#endif


	//////////////////////////////////////////
	// graph execution for AIE
	//////////////////////////////////////////

	//Obtains the graph handle from the XCLBIN that is loaded into the device
	auto cghdl = xrt::graph(device,xclbin_uuid,"clipgraph");

	printf("graph run\n");
	cghdl.run(NITER);

	//Graph end
	cghdl.end();
	printf("graph end\n");


	#ifndef EXTERNAL_IO
	//////////////////////////////////////////
	// wait for mm2s done
	//////////////////////////////////////////

	auto state_mm2s = mm2s_rhdl.wait();

	std::cout << "mm2s (DataIn1) completed with status(" << state_mm2s << ")\n";

	//////////////////////////////////////////
	// wait for polar_clip done
	//////////////////////////////////////////

	auto state_polar_clip = polar_clip_rhdl.wait();
	std::cout << "polar_clip completed with status(" << state_polar_clip << ")\n";

	//////////////////////////////////////////
	// wait for s2mm done
	//////////////////////////////////////////

	auto state_s2mm = s2mm_rhdl.wait();
	std::cout << "s2mm completed with status(" << state_s2mm << ")\n";

	#if defined(__SYNCBO_ENABLE__)
	printf("Output Buffer Object Synchronization\n");
	in_bohdl.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
	#endif



	//////////////////////////////////////////
	// Comparing the execution data to the golden data
	//////////////////////////////////////////


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


	#endif

	return errorCount;
}
