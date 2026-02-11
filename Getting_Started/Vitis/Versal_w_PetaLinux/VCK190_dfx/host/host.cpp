// dfx_host_compare_two_pass_opt.cpp
// Usage: ./dfx_host_compare_two_pass_opt task1.xclbin task2.xclbin
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

// --- tiny helpers ---
#define CHECK_0(expr, msg) do { if ((expr) != 0) { throw std::runtime_error(msg); } } while(0)
#define CHECK_PTR(p, msg)  do { if (!(p))        { throw std::runtime_error(msg); } } while(0)

// Load xclbin (same API as your example)
static std::vector<char>
load_xclbin(xrtDeviceHandle device, const std::string& fnm)
{
  if (fnm.empty())
    throw std::runtime_error("No xclbin specified");

  std::ifstream stream(fnm, std::ios::binary);
  if (!stream)
    throw std::runtime_error("Failed to open xclbin: " + fnm);

  stream.seekg(0, stream.end);
  size_t size = static_cast<size_t>(stream.tellg());
  stream.seekg(0, stream.beg);

  std::vector<char> header(size);
  stream.read(header.data(), size);

  auto top = reinterpret_cast<const axlf*>(header.data());
  CHECK_0(xrtDeviceLoadXclbin(device, top), "Bitstream download failed");
  return header;
}

// Run exactly one region (one DFX pass), return host output buffer.
// - does NOT close the device
// - closes only the handles it opens
static std::vector<int>
run_one_pass(xrtDeviceHandle dhdl,
             const xuid_t& uuid,
             xrtBufferHandle in_bohdl,  int sizeInWords,
             xrtBufferHandle out_bohdl, int sizeOutWords,
             long itr)
{
  // Open PL kernels
  xrtKernelHandle mm2s_khdl       = xrtPLKernelOpen(dhdl, uuid, "mm2s");
  xrtKernelHandle s2mm_khdl       = xrtPLKernelOpen(dhdl, uuid, "s2mm");
  xrtKernelHandle polar_clip_khdl = xrtPLKernelOpen(dhdl, uuid, "polar_clip");
  CHECK_PTR(mm2s_khdl, "open mm2s failed");
  CHECK_PTR(s2mm_khdl, "open s2mm failed");
  CHECK_PTR(polar_clip_khdl, "open polar_clip failed");

  xrtRunHandle mm2s_rhdl = xrtRunOpen(mm2s_khdl);
  xrtRunHandle s2mm_rhdl = xrtRunOpen(s2mm_khdl);
  xrtRunHandle polar_clip_rhdl = xrtRunOpen(polar_clip_khdl);
  CHECK_PTR(mm2s_rhdl, "runOpen mm2s failed");
  CHECK_PTR(s2mm_rhdl, "runOpen s2mm failed");
  CHECK_PTR(polar_clip_rhdl, "runOpen polar_clip failed");

  // Args (your convention: 0 = BO, 2 = length in 32-bit words)
  CHECK_0(xrtRunSetArg(mm2s_rhdl, 0, in_bohdl),  "mm2s arg0 failed");
  CHECK_0(xrtRunSetArg(mm2s_rhdl, 2, sizeInWords), "mm2s arg2 failed");
  CHECK_0(xrtRunSetArg(s2mm_rhdl, 0, out_bohdl), "s2mm arg0 failed");
  CHECK_0(xrtRunSetArg(s2mm_rhdl, 2, sizeOutWords), "s2mm arg2 failed");
  CHECK_0(xrtRunSetArg(polar_clip_rhdl, 2, sizeOutWords), "polar_clip arg2 failed");

  // Coherency (push input)
  {
    size_t in_bytes = static_cast<size_t>(sizeInWords) * sizeof(int);
    CHECK_0(xrtBOSync(in_bohdl, XCL_BO_SYNC_BO_TO_DEVICE, in_bytes, 0), "BOSync TO_DEVICE failed");
  }

  // Open graph
  auto ghdl = xrtGraphOpen(dhdl, uuid, "clipgraph");
  CHECK_PTR(ghdl, "xrtGraphOpen(clipgraph) failed");

  // Start order: consumer first
  xrtRunStart(s2mm_rhdl);        printf("run s2mm\n");
  xrtRunStart(polar_clip_rhdl);  printf("run polar_clip\n");
  printf("xrtGraphRun\n");
  xrtGraphRun(ghdl, itr);
  xrtRunStart(mm2s_rhdl);        printf("run mm2s\n");

  // Wait
  int st = xrtRunWait(mm2s_rhdl);
  std::cout << "mm2s completed with status(" << st << ")\n";
  st = xrtRunWait(polar_clip_rhdl);
  std::cout << "polar_clip completed with status(" << st << ")\n";
  st = xrtRunWait(s2mm_rhdl);
  std::cout << "s2mm completed with status(" << st << ")\n";

  // Graph end/close
  xrtGraphEnd(ghdl, 0);
  printf("xrtGraphEnd..\n");
  xrtGraphClose(ghdl);

  // Pull output
  size_t out_bytes = static_cast<size_t>(sizeOutWords) * sizeof(int);
  CHECK_0(xrtBOSync(out_bohdl, XCL_BO_SYNC_BO_FROM_DEVICE, out_bytes, 0), "BOSync FROM_DEVICE failed");
  auto out_ptr = reinterpret_cast<int*>(xrtBOMap(out_bohdl));
  std::vector<int> host_out(sizeOutWords);
  std::memcpy(host_out.data(), out_ptr, out_bytes);

  // Close handles opened here
  xrtRunClose(s2mm_rhdl);
  xrtKernelClose(s2mm_khdl);
  xrtRunClose(mm2s_rhdl);
  xrtKernelClose(mm2s_khdl);
  xrtRunClose(polar_clip_rhdl);
  xrtKernelClose(polar_clip_khdl);

  return host_out;
}

int main(int argc, char ** argv)
{
  try {
    if (argc < 3) {
      std::cout << "Usage: " << argv[0] <<" <task1.xclbin> <task2.xclbin>\n";
      return EXIT_FAILURE;
    }

    const char* xclbin1 = argv[1];
    const char* xclbin2 = argv[2];

    // Sizes (same meaning as your example)
    const long itr = NO_OF_ITERATIONS;
    const int  sizeInWords  = INPUT_SIZE  * itr;
    const int  sizeOutWords = OUTPUT_SIZE * itr;
    const size_t in_bytes   = static_cast<size_t>(sizeInWords)  * sizeof(int);
    const size_t out_bytes  = static_cast<size_t>(sizeOutWords) * sizeof(int);

    // ---------- PASS 1 ----------
    xuid_t uuid1{};
    auto dhdl1 = xrtDeviceOpen(0);                        CHECK_PTR(dhdl1, "xrtDeviceOpen(0) failed");
    auto xb1   = load_xclbin(dhdl1, xclbin1);
    CHECK_0(xrtDeviceGetXclbinUUID(dhdl1, uuid1), "Get UUID #1 failed");

    // Allocate BOs
    xrtBufferHandle in_bohdl1  = xrtBOAlloc(dhdl1, in_bytes,  0, 0); CHECK_PTR(in_bohdl1, "BOAlloc in #1 failed");
    xrtBufferHandle out_bohdl1 = xrtBOAlloc(dhdl1, out_bytes, 0, 0); CHECK_PTR(out_bohdl1, "BOAlloc out #1 failed");

    // Initialize input (same input for both passes)
    auto in_map1 = reinterpret_cast<short int*>(xrtBOMap(in_bohdl1));
    CHECK_PTR(in_map1, "BOMap in #1 failed");
    std::memcpy(in_map1, cint16input, in_bytes);

    std::cout << "[PASS 1] load " << xclbin1 << "\n";
    std::vector<int> out1 = run_one_pass(dhdl1, uuid1, in_bohdl1, sizeInWords, out_bohdl1, sizeOutWords, itr);

    // Release BOs, keep device policy consistent with your flow (close device after region)
    xrtBOFree(in_bohdl1);
    xrtBOFree(out_bohdl1);
    xrtDeviceClose(dhdl1);

    // ---------- PASS 2 ----------
    xuid_t uuid2{};
    auto dhdl2 = xrtDeviceOpen(0);                        CHECK_PTR(dhdl2, "xrtDeviceOpen(0) failed (pass 2)");
    auto xb2   = load_xclbin(dhdl2, xclbin2);
    CHECK_0(xrtDeviceGetXclbinUUID(dhdl2, uuid2), "Get UUID #2 failed");   // NOTE: use dhdl2 (bug fix)

    xrtBufferHandle in_bohdl2  = xrtBOAlloc(dhdl2, in_bytes,  0, 0); CHECK_PTR(in_bohdl2, "BOAlloc in #2 failed");
    xrtBufferHandle out_bohdl2 = xrtBOAlloc(dhdl2, out_bytes, 0, 0); CHECK_PTR(out_bohdl2, "BOAlloc out #2 failed");

    auto in_map2 = reinterpret_cast<short int*>(xrtBOMap(in_bohdl2));
    CHECK_PTR(in_map2, "BOMap in #2 failed");
    std::memcpy(in_map2, cint16input, in_bytes);

    std::cout << "[PASS 2] load " << xclbin2 << "\n";
    std::vector<int> out2 = run_one_pass(dhdl2, uuid2, in_bohdl2, sizeInWords, out_bohdl2, sizeOutWords, itr);

    xrtBOFree(in_bohdl2);
    xrtBOFree(out_bohdl2);
    xrtDeviceClose(dhdl2);

    // ---------- Compare ----------
    int errCnt = 0;
    if (out1.size() != out2.size()) {
      std::cerr << "Size mismatch: out1=" << out1.size() << " out2=" << out2.size() << "\n";
      errCnt = 1;
    } else {
      for (size_t i = 0; i < out1.size(); ++i) {
        if (out1[i] != out2[i]) {
          if (errCnt < 16)
            printf("DIFF @%zu: out1=0x%08x  out2=0x%08x\n", i, out1[i], out2[i]);
          ++errCnt;
        }
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
