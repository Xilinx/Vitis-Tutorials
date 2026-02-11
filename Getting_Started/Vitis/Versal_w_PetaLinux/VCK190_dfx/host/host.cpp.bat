// dfx_host_compare_two_pass.cpp
// Usage: ./dfx_host_compare_two_pass task1.xclbin task2.xclbin
//
// Pass 1: cint16input -> mm2s -> polar_clip -> graph("clipgraph") -> s2mm -> output1
// Pass 2: cint16input -> mm2s -> polar_clip -> graph("clipgraph") -> s2mm -> output2
// Compare: output1 vs output2 (no golden)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>

#include "input.h"                 // cint16input[]
#include "experimental/xrt_kernel.h"
#include "adf/adf_api/XRTConfig.h"

#define INPUT_SIZE   128
#define OUTPUT_SIZE  256
#define NO_OF_ITERATIONS  4

static std::vector<char>
load_xclbin(xrtDeviceHandle device, const std::string& fnm)
{
  if (fnm.empty())
    throw std::runtime_error("No xclbin speified");

  std::ifstream stream(fnm, std::ios::binary);
  if (!stream)
    throw std::runtime_error("Failed to open xclbin: " + fnm);

  stream.seekg(0,stream.end);
  size_t size = static_cast<size_t>(stream.tellg());
  stream.seekg(0,stream.beg);

  std::vector<char> header(size);
  stream.read(header.data(),size);

  auto top = reinterpret_cast<const axlf*>(header.data());
  if (xrtDeviceLoadXclbin(device, top))
    throw std::runtime_error("Bitstream download failed");

  return header;
}

static void run_one_pass(xrtDeviceHandle dhdl,
                         const xuid_t& uuid,
                         xrtBufferHandle in_bohdl,  int sizeIn,
                         xrtBufferHandle out_bohdl, int sizeOut,
                         long itr)
{
  // Open kernels (same names & sequence as your sample)
  xrtKernelHandle mm2s_khdl = xrtPLKernelOpen(dhdl, uuid, "mm2s");
  xrtKernelHandle s2mm_khdl = xrtPLKernelOpen(dhdl, uuid, "s2mm");
  xrtKernelHandle polar_clip_khdl = xrtPLKernelOpen(dhdl, uuid, "polar_clip");

  xrtRunHandle mm2s_rhdl = xrtRunOpen(mm2s_khdl);
  xrtRunHandle s2mm_rhdl = xrtRunOpen(s2mm_khdl);
  xrtRunHandle polar_clip_rhdl = xrtRunOpen(polar_clip_khdl);

  // Set args (keep your arg indices: 0=BO, 2=len)
  xrtRunSetArg(mm2s_rhdl, 0, in_bohdl);
  xrtRunSetArg(mm2s_rhdl, 2, sizeIn);

  xrtRunSetArg(s2mm_rhdl, 0, out_bohdl);
  xrtRunSetArg(s2mm_rhdl, 2, sizeOut);

  xrtRunSetArg(polar_clip_rhdl, 2, sizeOut);

  

  // Start order (robust): s2mm -> polar_clip -> graph -> mm2s

  xrtRunStart(mm2s_rhdl);
  printf("run mm2s\n");

  xrtRunStart(s2mm_rhdl);
  printf("run s2mm\n");

  xrtRunStart(polar_clip_rhdl);
  printf("run polar_clip\n");
// Graph open/run (keep your graph name and calls)
  auto ghdl = xrtGraphOpen(dhdl, uuid, "clipgraph");
  printf("xrtGraphRun\n");
  xrtGraphRun(ghdl, itr);

 

  // Wait
  auto state = xrtRunWait(mm2s_rhdl);
  std::cout << "mm2s completed with status(" << state << ")\n";

  state = xrtRunWait(polar_clip_rhdl);
  std::cout << "polar_clip completed with status(" << state << ")\n";

  state = xrtRunWait(s2mm_rhdl);
  std::cout << "s2mm completed with status(" << state << ")\n";

  xrtGraphEnd(ghdl,0);
  printf("xrtGraphEnd..\n");
  xrtGraphClose(ghdl);

  // Close
  xrtRunClose(s2mm_rhdl);
  xrtKernelClose(s2mm_khdl);

  xrtRunClose(mm2s_rhdl);
  xrtKernelClose(mm2s_khdl);

  xrtRunClose(polar_clip_rhdl);
  xrtKernelClose(polar_clip_khdl);
    
}

int main(int argc, char ** argv)
{
  try {
    if(argc < 3) {
      std::cout << "Usage: " << argv[0] <<" <task1.xclbin> <task2.xclbin>\n";
      return EXIT_FAILURE;
    }

    const char* xclbin1 = argv[1];
    const char* xclbin2 = argv[2];

    // Open device & load first xclbin
    xuid_t uuid;
    auto dhdl = xrtDeviceOpen(0);
    auto xb1 = load_xclbin(dhdl, xclbin1);
    xrtDeviceGetXclbinUUID(dhdl, uuid);

    // Derived sizes (keep your macro meaning)
    long itr = NO_OF_ITERATIONS;
    int sizeIn  = INPUT_SIZE  * itr;
    int sizeOut = OUTPUT_SIZE * itr;

    size_t input_size_in_bytes  = static_cast<size_t>(sizeIn)  * sizeof(int);
    size_t output_size_in_bytes = static_cast<size_t>(sizeOut) * sizeof(int);

    // Buffers for Region1
    xrtBufferHandle in_bohdl1  = xrtBOAlloc(dhdl, input_size_in_bytes,  0, 0);
    xrtBufferHandle out_bohdl1= xrtBOAlloc(dhdl, output_size_in_bytes, 0, 0);

    auto in_bomapped1  = reinterpret_cast<short int*>(xrtBOMap(in_bohdl1));
    auto out_bomapped1 = reinterpret_cast<int*>(xrtBOMap(out_bohdl1));

    
    // Host copies of outputs for compare
    std::vector<int> host_out1(sizeOut);
    std::vector<int> host_out2(sizeOut);

    // ---------------- PASS 1 ----------------
    // Copy same original input to BO
    memcpy(in_bomapped1, cint16input, input_size_in_bytes);
    printf("[PASS 1] load %s\n", xclbin1);
    run_one_pass(dhdl, uuid, in_bohdl1, sizeIn, out_bohdl1, sizeOut, itr);

    // Save output1
    memcpy(host_out1.data(), out_bomapped1, output_size_in_bytes);

    //Release XRT resources
    std::cout << "Releasing remaining XRT objects after first region...\n";
    xrtBOFree(in_bohdl1);
    xrtBOFree(out_bohdl1);
    xrtDeviceClose(dhdl);

    
    // ---------------- PASS 2 ----------------
    // Load second xclbin
    auto dhdl2 = xrtDeviceOpen(0);
    auto xb2 = load_xclbin(dhdl2, xclbin2);
    xrtDeviceGetXclbinUUID(dhdl, uuid);

    //Buffers for reload region
     xrtBufferHandle in_bohdl2  = xrtBOAlloc(dhdl2, input_size_in_bytes,  0, 0);
    xrtBufferHandle out_bohdl2= xrtBOAlloc(dhdl2, output_size_in_bytes, 0, 0);

    auto in_bomapped2  = reinterpret_cast<short int*>(xrtBOMap(in_bohdl2));
    auto out_bomapped2 = reinterpret_cast<int*>(xrtBOMap(out_bohdl2));
    // Copy the SAME original input again
    memcpy(in_bomapped2, cint16input, input_size_in_bytes);
    printf("[PASS 2] load %s\n", xclbin2);
    run_one_pass(dhdl2, uuid, in_bohdl2, sizeIn, out_bohdl2, sizeOut, itr);

    // Save output2
    memcpy(host_out2.data(), out_bomapped2, output_size_in_bytes);

     std::cout << "Releasing remaining XRT objects after reloading region...\n";
    xrtBOFree(in_bohdl2);
    xrtBOFree(out_bohdl2);

    xrtDeviceClose(dhdl2);
    // ---------------- Compare output1 vs output2 ----------------
    int errCnt = 0;
    for (int i = 0; i < sizeOut; ++i) {
      if (host_out1[i] != host_out2[i]) {
        if (errCnt < 16) {
          printf("DIFF @%d: out1=0x%08x  out2=0x%08x\n",
                 i, host_out1[i], host_out2[i]);
        }
        ++errCnt;
      }
    }

    
   

    std::cout << "COMPARE " << (errCnt ? "FAILED" : "PASSED")
              << " (mismatches=" << errCnt << ")\n";
    return errCnt ? EXIT_FAILURE : EXIT_SUCCESS;
  }
  catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
    return EXIT_FAILURE;
  }

  
}
