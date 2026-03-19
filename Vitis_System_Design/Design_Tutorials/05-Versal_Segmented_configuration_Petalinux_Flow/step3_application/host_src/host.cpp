/*
Copyright (C) 2020-2022 Xilinx, Inc. All rights reserved.
Copyright (C) 2022-2026 Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

/*
 * main.cpp : AI Engine A-to-Z Linux Application (Linux XRT)
 */

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "experimental/xrt_kernel.h"
#include "adf/adf_api/XRTConfig.h"

// Define sync direction constants if not available in headers
// XCL_BO_SYNC_BO_TO_DEVICE = 0 (host to device)
// XCL_BO_SYNC_BO_FROM_DEVICE = 1 (device to host)
#ifndef XCL_BO_SYNC_BO_FROM_DEVICE
#define XCL_BO_SYNC_BO_FROM_DEVICE 1
#endif
#ifndef XCL_BO_SYNC_BO_TO_DEVICE
#define XCL_BO_SYNC_BO_TO_DEVICE 0
#endif

// Use same approach as reference host: generate input data dynamically
#define INPUT_SIZE  128
#define OUTPUT_SIZE 128
#define NO_OF_ITERATIONS  4

static std::vector<char>
load_xclbin(xrtDeviceHandle device, const std::string& fnm)
{
  if (fnm.empty())
    throw std::runtime_error("No xclbin specified");

  // load bit stream
  std::ifstream stream(fnm, std::ios::binary | std::ios::in);
  stream.seekg(0, stream.end);
  size_t size = static_cast<size_t>(stream.tellg());
  stream.seekg(0, stream.beg);

  std::vector<char> header(size);
  stream.read(header.data(), size);

  auto top = reinterpret_cast<const axlf*>(header.data());
  if (xrtDeviceLoadXclbin(device, top))
    throw std::runtime_error("Bitstream download failed");

  return header;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <xclbin>" << std::endl;
        return EXIT_FAILURE;
    }

    char* xclbin_filename = argv[1];
    long itr = NO_OF_ITERATIONS;
    xuid_t uuid;
    auto dhdl = xrtDeviceOpen(0);
    auto xclbin = load_xclbin(dhdl, xclbin_filename);
    xrtDeviceGetXclbinUUID(dhdl, uuid);

    int size_in_samples = INPUT_SIZE;
    int size_out_samples = OUTPUT_SIZE;

    size_t input_size_in_bytes = static_cast<size_t>(size_in_samples) * sizeof(int32_t);
    size_t output_size_in_bytes = static_cast<size_t>(size_out_samples) * sizeof(int32_t);

    std::vector<int32_t> input(size_in_samples);
    std::vector<int32_t> output(size_out_samples);
    
    // Generate input data using same method as reference host
    int32_t data = 0x00010000;
    for (auto& v : input) {
        v = data;
        data += 0x00020002;
    }
    
    // Initialize output buffer
    for (auto& v : output)
        v = 0xABCDEF00;

    xrtBufferHandle in_bohdl = xrtBOAlloc(dhdl, input_size_in_bytes, 0, 0);
    auto in_bomapped = reinterpret_cast<int32_t*>(xrtBOMap(in_bohdl));
    std::memcpy(in_bomapped, input.data(), input_size_in_bytes);
    std::printf("Input memory virtual addr 0x%llx\n",
                static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(in_bomapped)));

    xrtBufferHandle out_bohdl = xrtBOAlloc(dhdl, output_size_in_bytes, 0, 0);
    auto out_bomapped = reinterpret_cast<int32_t*>(xrtBOMap(out_bohdl));
    std::memcpy(out_bomapped, output.data(), output_size_in_bytes);
    std::printf("Output memory virtual addr 0x%llx\n",
                static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(out_bomapped)));
    
    xrtKernelHandle mm2s_khdl = xrtPLKernelOpen(dhdl, uuid, "mm2s");
    xrtRunHandle mm2s_rhdl = xrtRunOpen(mm2s_khdl);
    int rval = xrtRunSetArg(mm2s_rhdl, 0, in_bohdl);
    rval = xrtRunSetArg(mm2s_rhdl, 2, size_in_samples);
    (void)rval;
    xrtRunStart(mm2s_rhdl);
    std::printf("run mm2s\n");
    
    xrtKernelHandle s2mm_khdl = xrtPLKernelOpen(dhdl, uuid, "s2mm");
    xrtRunHandle s2mm_rhdl = xrtRunOpen(s2mm_khdl);
    rval = xrtRunSetArg(s2mm_rhdl, 0, out_bohdl);
    rval = xrtRunSetArg(s2mm_rhdl, 2, size_out_samples);
    (void)rval;
    xrtRunStart(s2mm_rhdl);
    std::printf("run s2mm\n");

    std::printf("xrtGraphOpen\n");
    auto ghdl = xrtGraphOpen(dhdl, uuid, "mygraph");
    std::printf("xrtGraphRun\n");
    xrtGraphRun(ghdl, itr);
     
    auto state = xrtRunWait(mm2s_rhdl);
    std::cout << "mm2s completed with status(" << state << ")\n";

    state = xrtRunWait(s2mm_rhdl);
    std::cout << "s2mm completed with status(" << state << ")\n";

    xrtGraphEnd(ghdl, 0);
    std::printf("xrtGraphEnd..\n");
    xrtGraphClose(ghdl);

    xrtRunClose(s2mm_rhdl);
    xrtKernelClose(s2mm_khdl);

    xrtRunClose(mm2s_rhdl);
    xrtKernelClose(mm2s_khdl);

    
    // Calculate golden data using same method as reference host
    int err = 0;
    for (int i = 0; i < size_out_samples; i++) {
        int32_t data_in = input[i % size_in_samples];
        int32_t real_temp = (data_in & 0xFFFF) + ((data_in & 0xFFFF0000) >> 16);
        int32_t im_temp = (data_in & 0xFFFF) - ((data_in & 0xFFFF0000) >> 16);
        int32_t golden = ((real_temp - im_temp) << 16) + real_temp + im_temp;

        if (golden != out_bomapped[i]) {
            err++;
            std::printf("Output Error: Golden = %x Output = %x\n", golden, out_bomapped[i]);
        }
    }

    xrtBOFree(in_bohdl);
    xrtBOFree(out_bohdl);
    xrtDeviceClose(dhdl);

    if (err > 0)
        std::printf("The AI Engine A to Z Application failed with %d errors\n", err);
    else
        std::printf("Successfully ran AI Engine simple Application with no error\n");

    return (err ? EXIT_FAILURE : EXIT_SUCCESS);
}
