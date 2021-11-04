/**********
© Copyright 2021 Xilinx, Inc.

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


#include "graphs/mygraph.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include "data.h"

// This is used for the PL Kernels
#include "xrt/xrt.h"
#include "xrt/experimental/xrt_kernel.h"

// Using the ADF API that call XRT API
#include "adf/adf_api/XRTConfig.h"
#include "fft_twiddle_lut_dit.h"
#include "fft_kernel_bufs.h"
//mygraph_float<WIN_SAMPS>  mygraph_top;
mygraph<WIN_SAMPS>  mygraph_top;

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

	//////////////////////////////////////////
	// graph execution for AIE
	//////////////////////////////////////////	
	
	printf("graph init. This does nothing because CDO in boot PDI already configures AIE.\n");
	mygraph_top.init();
	
	printf("graph run -\n");
	mygraph_top.run(1);
	
	mygraph_top.end();

	printf("graph end\n");
	printf("TEST PASSED\n");
	xrtDeviceClose(dhdl);
	
	
	return  0;
}

