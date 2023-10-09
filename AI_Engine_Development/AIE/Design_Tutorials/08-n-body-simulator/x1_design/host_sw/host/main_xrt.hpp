/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/




#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <chrono>
#include <map>
#include <cstring>

#include "log.hpp" 
#include "nbody.hpp" 

// This is used for the PL Kernels
//#include "xrt/xrt.h"
#include "experimental/xrt_kernel.h"

// Using the ADF API that call XRT API
#include "adf/adf_api/XRTConfig.h"

#define OFF false
#define ON true
#define NUM_CU 1

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
