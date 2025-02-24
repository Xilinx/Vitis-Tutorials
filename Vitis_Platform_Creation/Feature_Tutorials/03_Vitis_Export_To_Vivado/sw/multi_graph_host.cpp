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
	
	auto out_bohdl_1 = xrt::bo(device, sizeOut * sizeof(int), 0, 0);
	auto out_bomapped_1 = out_bohdl_1.map<uint32_t*>();
	memset(out_bomapped_1, 0xABCDEF00, sizeOut * sizeof(int));
	printf("Output memory virtual addr 0x%px\n", out_bomapped_1);

	auto out_bohdl_2 = xrt::bo(device, sizeOut * sizeof(int), 0, 0);
	auto out_bomapped_2 = out_bohdl_2.map<uint32_t*>();
	memset(out_bomapped_2, 0xABCDEF00, sizeOut * sizeof(int));
	printf("Output memory virtual addr 0x%px\n", out_bomapped_2);
	////////////////////////////////////////////////////////
	// stream_out ip - Creating kernel handle using xrt::kernel API
	///////////////////////////////////////////////////////	
	
	auto stream_out_khdl = xrt::kernel(device, xclbin_uuid, "aie_stream_in");
	auto stream_out_rhdl = stream_out_khdl(in_bohdl, nullptr, sizeIn);
	printf("run aie_stream_in\n");

	////////////////////////////////////////////////////////
	// stream_in ip - Creating kernel handle using xrt::kernel API
	///////////////////////////////////////////////////////		
	
	auto stream_in_khdl_1 = xrt::kernel(device, xclbin_uuid, "stream_in:{stream_in1}");
	auto stream_in_rhdl_1 = stream_in_khdl_1(out_bohdl_1, nullptr, sizeOut);
	printf("run stream_in1\n");

	auto stream_in_khdl_2 = xrt::kernel(device, xclbin_uuid, "stream_in:{stream_in2}");
	auto stream_in_rhdl_2 = stream_in_khdl_2(out_bohdl_2, nullptr, sizeOut);
	printf("run stream_in2\n");

	//////////////////////////////////////////
	// graph execution for AIE
	//////////////////////////////////////////	
	
	//Obtains the graph handle from the XCLBIN that is loaded into the device
	auto cghdl_1 = xrt::graph(device,xclbin_uuid,"clipgraph1");
	auto cghdl_2 = xrt::graph(device,xclbin_uuid,"clipgraph2");
	
	printf("graph run\n");
	//Run th graph for 1 iteration
	cghdl_1.run(1);
	cghdl_2.run(1);
	
	//Graph end
	cghdl_1.end();
	cghdl_2.end();
	printf("graph end\n");	
	
	
	//////////////////////////////////////////
	// wait for stream_out done
	//////////////////////////////////////////	
	
	stream_out_rhdl.wait();

	//////////////////////////////////////////
	// wait for stream_in done
	//////////////////////////////////////////	
	
	stream_in_rhdl_1.wait();
	stream_in_rhdl_2.wait();

	out_bohdl_1.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
	out_bohdl_2.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
	
	//////////////////////////////////////////
	// Comparing the execution data to the golden data
	//////////////////////////////////////////	
	
	int errorCount1 = 0;
	int errorCount2 = 0;
	{
		for (int i = 0; i < sizeOut; i++)
		{
				if ((signed)out_bomapped_1[i] != golden[i])
				{
					printf("Error found @ %d, %d != %d\n", i, out_bomapped_1[i], golden[i]);
					errorCount1++;
				}
				
				if ((signed)out_bomapped_2[i] != golden[i])
				{
					printf("Error found @ %d, %d != %d\n", i, out_bomapped_2[i], golden[i]);
					errorCount2++;
				}
		}

		if (errorCount1 or errorCount2)
			printf("Test failed with %d  %d errors\n", errorCount1, errorCount2);
		else
			printf("TEST PASSED\n");
	}
	
    
	std::cout << "Releasing remaining XRT objects...\n";
        int errorCount = errorCount1 + errorCount2 ;	
	return errorCount;
}
