/**********
© Copyright 2020 Xilinx, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**********/

#include "graph.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include "data.h"

// This is used for the PL Kernels
#include "xrt/xrt.h"
#include "xrt/experimental/xrt_kernel.h"

// Using the Cardano API that call XRT API
#include "adf/adf_api/XRTConfig.h"

static std::vector<char>
load_xclbin(xrtDeviceHandle device, const std::string& fnm)
{
	if (fnm.empty())
		throw std::runtime_error("No xclbin specified");

	// load bit stream
	std::ifstream stream(fnm);
	stream.seekg(0,stream.end);
	size_t size = stream.tellg();
	stream.seekg(0,stream.beg);

	std::vector<char> header(size);
	stream.read(header.data(),size);

	auto top = reinterpret_cast<const axlf*>(header.data());
	if (xrtDeviceLoadXclbin(device, top))
		throw std::runtime_error("Xclbin loading failed");

	return header;
}

#define SAMPLES 256

int main(int argc, char ** argv)
{
	//////////////////////////////////////////
	// Open xclbin
	//////////////////////////////////////////
	auto dhdl = xrtDeviceOpen(0); // Open Device the local device
	if(dhdl == nullptr)
		throw std::runtime_error("No valid device handle found. Make sure using right xclOpen index.");
    auto xclbin = load_xclbin(dhdl, "a.xclbin");
    auto top = reinterpret_cast<const axlf*>(xclbin.data());
    adf::registerXRT(dhdl, top->m_header.uuid);

	int sizeIn = SAMPLES/2;
	int sizeOut = SAMPLES;
	
	//////////////////////////////////////////
	// input memory
	// Allocating the input size of sizeIn to MM2S
	// This is using low-level XRT call xclAllocBO to allocate the memory
	//////////////////////////////////////////	
    
	xrtBufferHandle in_bohdl = xrtBOAlloc(dhdl, sizeIn * sizeof(int16_t) * 2, 0, 0);
	auto in_bomapped = reinterpret_cast<uint32_t*>(xrtBOMap(in_bohdl));
	memcpy(in_bomapped, cint16Input, sizeIn * sizeof(int16_t) * 2);
	printf("Input memory virtual addr 0x%px\n", in_bomapped);
	
	//////////////////////////////////////////
	// output memory
	// Allocating the output size of sizeOut to S2MM
	// This is using low-level XRT call xclAllocBO to allocate the memory
	//////////////////////////////////////////
	
	xrtBufferHandle out_bohdl = xrtBOAlloc(dhdl, sizeOut * sizeof(int), 0, 0);
	auto out_bomapped = reinterpret_cast<uint32_t*>(xrtBOMap(out_bohdl));
	memset(out_bomapped, 0xABCDEF00, sizeOut * sizeof(int));
	printf("Output memory virtual addr 0x%px\n", out_bomapped);
	
	//////////////////////////////////////////
	// mm2s ip
	// Using the xrtPLKernelOpen function to manually control the PL Kernel
	// that is outside of the AI Engine graph
	//////////////////////////////////////////

	xrtKernelHandle mm2s_khdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, "mm2s");
	// Need to provide the kernel handle, and the argument order of the kernel arguments
	// Here the in_bohdl is the input buffer, the nullptr is the streaming interface and must be null,
	// lastly, the size of the data. This info can be found in the kernel definition. 
	xrtRunHandle mm2s_rhdl = xrtKernelRun(mm2s_khdl, in_bohdl, nullptr, sizeIn);
	printf("run mm2s\n");
	
	//////////////////////////////////////////
	// s2mm ip
	// Using the xrtPLKernelOpen function to manually control the PL Kernel
	// that is outside of the AI Engine graph
	//////////////////////////////////////////
	
	xrtKernelHandle s2mm_khdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, "s2mm");
	// Need to provide the kernel handle, and the argument order of the kernel arguments
	// Here the out_bohdl is the output buffer, the nullptr is the streaming interface and must be null,
	// lastly, the size of the data. This info can be found in the kernel definition. 
	xrtRunHandle s2mm_rhdl = xrtKernelRun(s2mm_khdl, out_bohdl, nullptr, sizeOut);
	printf("run s2mm\n");

	//////////////////////////////////////////
	// graph execution for AIE
	//////////////////////////////////////////	
	printf("graph init. This does nothing because CDO in boot PDI already configures AIE.\n");
	clipgraph.init();
	
	printf("graph run\n");
	clipgraph.run(1);
	
	clipgraph.end();
	printf("graph end\n");
	
	//////////////////////////////////////////
	// wait for mm2s done
	//////////////////////////////////////////	
	
	auto state = xrtRunWait(mm2s_rhdl);
	std::cout << "mm2s completed with status(" << state << ")\n";
	xrtRunClose(mm2s_rhdl);
	xrtKernelClose(mm2s_khdl);

	//////////////////////////////////////////
	// wait for s2mm done
	//////////////////////////////////////////	
	
	state = xrtRunWait(s2mm_rhdl);
	std::cout << "s2mm completed with status(" << state << ")\n";
	xrtRunClose(s2mm_rhdl);
	xrtKernelClose(s2mm_khdl);

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
			printf("Test passed\n");
	}
	
	//////////////////////////////////////////
	// clean up XRT
	//////////////////////////////////////////	
    
	std::cout << "Releasing remaining XRT objects...\n";
	//xrtBOUnmap(dhdl, in_bohdl, in_bomapped);
	//xrtBOUnmap(dhdl, out_bohdl, out_bomapped);
	xrtBOFree(in_bohdl);
	xrtBOFree(out_bohdl);
	xrtDeviceClose(dhdl);
	
	return errorCount;
}
