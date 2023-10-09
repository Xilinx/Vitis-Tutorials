//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "m16_ssr8_graph.h"

// Instantiate AIE graph:
m16_ssr8_graph aie_dut;

#include <stdlib.h>
#include <fstream>
#include <iostream>

#include <unistd.h>
#include <xrt/xrt_device.h>
#include <xrt/xrt_kernel.h>

#include <experimental/xrt_aie.h>
#include <experimental/xrt_graph.h>
#include <experimental/xrt_ip.h>

static const char*    STR_ERROR  = "ERROR:   ";
static const char*    STR_PASSED = "PASSED:  ";
static const char*    STR_USAGE  = "USAGE:   ";
static const char*    STR_INFO   = "INFO:    ";

// ------------------------------------------------------------
// DDR Parameters
// ------------------------------------------------------------

typedef int TT_DATA;            // Assume cint16 data

static constexpr unsigned         NUM_ITER = 2;
static constexpr int32_t          DFT_PERM = 1;
static constexpr int32_t        LOOP_CNT_I = 8; // 512 x 4 x 4 = 8K samples per stream (1 graph iterations)
static constexpr int32_t        LOOP_CNT_O = 4; // Try to stop it early
static constexpr int32_t          LOOP_SEL = 0; // ID of loop to capture by DDR SNK PL HLS block
static constexpr unsigned        NSTREAM_I = 7;
static constexpr unsigned        NSTREAM_O = 8;
static constexpr unsigned            DEPTH = 512;
static constexpr unsigned DDR_WORD_DEPTH_I = NSTREAM_I * DEPTH;
static constexpr unsigned DDR_WORD_DEPTH_O = NSTREAM_O * DEPTH;
static constexpr unsigned    NUM_SAMPLES_I = DDR_WORD_DEPTH_I * 4; // 32-bit (cint16) x 4 = 128-bit PL bus (4 samples)
static constexpr unsigned    NUM_SAMPLES_O = DDR_WORD_DEPTH_O * 4; // 32-bit (cint16) x 4 = 128-bit PL bus (4 samples)

static constexpr unsigned DDR_BUFFSIZE_I_BYTES = NUM_SAMPLES_I * 4; // Each sample is 4 bytes (32-bits)
static constexpr unsigned DDR_BUFFSIZE_O_BYTES = NUM_SAMPLES_O * 4; // Each sample is 4 bytes (32-bits)


// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main(int argc, char* argv[])
{
  //TARGET_DEVICE macro needs to be passed from gcc command line
  if (argc != 2) {
    std::cout << STR_USAGE << argv[0] <<" <xclbin>" << std::endl;
    return 1;
  }

  // ------------------------------------------------------------
  // Load XCLBIN
  // ------------------------------------------------------------

  char* xclbinFilename = argv[1];
  unsigned dev_index = 0;
  auto my_device = xrt::device(dev_index);
  std::cout << STR_PASSED << "auto my_device = xrt::device(" << dev_index << ")" << std::endl;

  auto xclbin_uuid = my_device.load_xclbin(xclbinFilename);
  std::cout << STR_PASSED << "auto xclbin_uuid = my_device.load_xclbin(" << xclbinFilename << ")" << std::endl;

  // ------------------------------------------------------------
  // Load and Run AIE graph
  // ------------------------------------------------------------

  auto my_graph  = xrt::graph(my_device, xclbin_uuid, "aie_dut");
  std::cout << STR_PASSED << "auto my_graph  = xrt::graph(my_device, xclbin_uuid, \"aie_dut\")" << std::endl;

  my_graph.reset();
  std::cout << STR_PASSED << "my_graph.reset()" << std::endl;

  my_graph.run(NUM_ITER);
  std::cout << STR_PASSED << "my_graph.run( NUM_ITER=" << NUM_ITER << " )" << std::endl;

  // ------------------------------------------------------------
  // Load and Start DDR Source/Sink PL Kernels
  // ------------------------------------------------------------

  auto dma_src = xrt::kernel(my_device, xclbin_uuid, "dma_stream_src_wrapper:{dma_src}");
  std::cout << STR_PASSED << "auto dma_src = xrt::kernel(my_device, xclbin_uuid, \"dma_stream_src_wrapper:{dma_src}\")" << std::endl;

  auto dma_snk = xrt::kernel(my_device, xclbin_uuid, "dma_stream_snk_wrapper:{dma_snk}");
  std::cout << STR_PASSED << "auto dma_snk = xrt::kernel(my_device, xclbin_uuid, \"dma_stream_snk_wrapper:{dma_snk}\")" << std::endl;

  xrt::run dma_src_run = xrt::run(dma_src);
  std::cout << STR_PASSED << "xrt::run dma_src_run = xrt::run(dma_src)" << std::endl;

  xrt::run dma_snk_run = xrt::run(dma_snk);
  std::cout << STR_PASSED << "xrt::run dma_snk_run = xrt::run(dma_snk)" << std::endl;

  // ------------------------------------------------------------
  // Configure DDR Buffer Objects
  // ------------------------------------------------------------

  auto dma_src_bo = xrt::bo(my_device, DDR_BUFFSIZE_I_BYTES, dma_src.group_id(0));
  std::cout << STR_PASSED << "dma_src_bo = xrt::bo(my_device, DDR_BUFFSIZE_I_BYTES, dma_src.group_id(0) (=" << dma_src.group_id(0) << "))" << std::endl;

  auto dma_src_bo_mapped = dma_src_bo.map<TT_DATA*>();
  std::cout << STR_PASSED << "auto dma_src_bo_mapped = dma_src_bo.map<TT_DATA*>()" << std::endl;

  auto dma_snk_bo = xrt::bo(my_device, DDR_BUFFSIZE_O_BYTES, dma_snk.group_id(0));
  std::cout << STR_PASSED << "dma_snk_bo = xrt::bo(my_device, DDR_BUFFSIZE_O_BYTES, dma_snk.group_id(0) (=" << dma_snk.group_id(0) << "))" << std::endl;

  auto dma_snk_bo_mapped = dma_snk_bo.map<TT_DATA*>();
  std::cout << STR_PASSED << "auto dma_snk_bo_mapped = dma_snk_bo.map<TT_DATA*>()" << std::endl;

  // Open stimulus input file:
  std::ifstream ss_i;
  ss_i.open("sig_i.txt",std::ifstream::in);
  if ( ss_i.is_open() == 0 ) {
    std::cout << STR_ERROR << "failed to open sig_i.txt" << std::endl;
    return(1);
  }

  // Read data from input file:
  for (unsigned ss=0; ss < NUM_SAMPLES_I; ss++) {
    int16_t val_re, val_im;
    ss_i >> val_re >> val_im;
    TT_DATA buff = (uint16_t) val_re;
    buff |= (uint32_t) ((uint16_t)val_im << 16);
    dma_src_bo_mapped[ss] = buff;
  }
  ss_i.close();
  std::cout << STR_PASSED << "Successfully read input file sig_i.txt" << std::endl;

  // ------------------------------------------------------------
  // Load and start PL kernels
  // ------------------------------------------------------------

  dma_src_run.set_arg( 0, dma_src_bo );
  std::cout << STR_PASSED << "dma_src_run.set_arg( 0, dma_src_bo )" << std::endl;

  dma_src_run.set_arg( 1, LOOP_CNT_I );
  std::cout << STR_PASSED << "dma_src_run.set_arg( 1, LOOP_CNT_I=" << LOOP_CNT_I << " )" << std::endl;

  dma_snk_run.set_arg( 0, dma_snk_bo );
  std::cout << STR_PASSED << "dma_snk.run.set_arg( 0, dma_snk_bo )" << std::endl;

  dma_snk_run.set_arg( 1, LOOP_SEL );
  std::cout << STR_PASSED << "dma_snk_run.set_arg( 1, LOOP_SEL=" << LOOP_SEL << " )" << std::endl;

  dma_snk_run.set_arg( 2, LOOP_CNT_O );
  std::cout << STR_PASSED << "dma_snk_run.set_arg( 2, LOOP_CNT_O=" << LOOP_CNT_O << " )" << std::endl;

  dma_snk_run.set_arg( 3, DFT_PERM );
  std::cout << STR_PASSED << "dma_snk_run.set_arg( 3, DFT_PERM=" << DFT_PERM << " )" << std::endl;

  dma_src_run.start();
  std::cout << STR_PASSED << "dma_src_run.start()" << std::endl;

  dma_snk_run.start();
  std::cout << STR_PASSED << "dma_snk_run.start()" << std::endl;


  // Wait for all kernels to end:
  std::cout << std::endl << STR_INFO << "Waiting for kernels to end..." << std::endl << std::endl;

  // We only wait for the SNK that is setup for 1 iteration. The SRC and graph are setup to run 2 iterations.
  // dma_src_run.wait();
  // std::cout << STR_PASSED << "dma_src_run.wait()" << std::endl;

  dma_snk_run.wait();
  std::cout << STR_PASSED << "dma_snk_run.wait()" << std::endl;

  // my_graph.end();
  // std::cout << STR_PASSED << "my_graph.end()" << std::endl;

  // ------------------------------------------------------------
  // Retrieve Results
  // ------------------------------------------------------------

  dma_snk_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
  std::cout << STR_PASSED << "dma_snk_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE)" << std::endl;

  // Open GOLDEN results file:
  std::ifstream ss_o;
  ss_o.open("sig_o.txt",std::ifstream::in);
  if ( ss_o.is_open() == 0 ) {
    std::cout << STR_ERROR << "failed to open sig_o.txt" << std::endl;
    return(1);
  }

  // Open ACTUAL results file:
  std::ofstream ss_a;
  ss_a.open("sig_a.txt",std::ofstream::out);
  if ( ss_a.is_open() == 0 ) {
    std::cout << STR_ERROR << "failed to open sig_a.txt" << std::endl;
    return(1);
  }

  // Validate results:
  bool flag = 0;
  for (unsigned ss=0; ss < NUM_SAMPLES_O; ss++) {
    int16_t val_g_re, val_g_im;
    ss_o >> val_g_re >> val_g_im;
    int16_t val_a_re = (int16_t) ( dma_snk_bo_mapped[ss]       ) & 0x0000FFFF;
    int16_t val_a_im = (int16_t) ( dma_snk_bo_mapped[ss] >> 16 ) & 0x0000FFFF;
    ss_a << val_a_re << " " << val_a_im << std::endl;
    int16_t err_re = abs(val_g_re - val_a_re);
    int16_t err_im = abs(val_g_im - val_a_im);
    flag |= ( err_re > 1 ) || ( err_im > 1 );
    std::cout << ss << "\t:Gold\t" << val_g_re << "\t" << val_g_im << "\t:Actual\t" << val_a_re << "\t" << val_a_im << std::endl;
  }
  ss_o.close();
  ss_a.close();

  // Done:
  if ( flag == 0 )
    std::cout << std::endl << "--- PASSED ---" << std::endl;
  else
    std::cout << std::endl << "*** FAILED ***" << std::endl;
  return(flag);
}
