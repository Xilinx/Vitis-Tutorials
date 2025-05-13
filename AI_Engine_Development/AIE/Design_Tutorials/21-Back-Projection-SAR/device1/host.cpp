//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "sar_top_1engine_cfg.h"
#include "sar_top_1engine_graph.h"

#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <unistd.h>
#include <xrt/xrt_device.h>
#include <xrt/xrt_kernel.h>
#include <xrt/xrt_aie.h>
#include <xrt/xrt_graph.h>
#include <xrt/experimental/xrt_ip.h>

static const char*    STR_ERROR  = "ERROR:   ";
static const char*    STR_PASSED = "PASSED:  ";
static const char*    STR_USAGE  = "USAGE:   ";
static const char*    STR_INFO   = "INFO:    ";

alignas(16) static float coord_i[LEN_COORD_DATA];
alignas(16) static float R0_range_i[LEN_R0_RANGE_DATA];
static  cfloat golden_o[LEN_IMAGE_DATA*NENGINE];

// ------------------------------------------------------------
// Instantiate AIE graph
// ------------------------------------------------------------

static sar_top_1engine_graph<REPEAT,NSAMP,0> engine0;

// ------------------------------------------------------------
// Read Input File
// ------------------------------------------------------------

template<typename T>
void read_data_file( std::string fname, unsigned NSAMP, T* datavec )
{
  std::fstream fs;
  fs.open(fname, std::ifstream::in);
  if (fs.is_open() == 0) { std::cerr << "Cannot open " << fname << std::endl; exit(1); }
  for (unsigned ii=0; ii < NSAMP; ii++) {
    if constexpr( std::is_same<T,cfloat>::value ) {
      float val_r, val_i;
      fs >> val_r >> val_i;
      datavec[ii] = {val_r,val_i};
    }
    else if constexpr( std::is_same<T,float>::value) {
      T val;
      fs >> val;
      datavec[ii] = val;
    }
  }
  fs.close();
}

// ------------------------------------------------------------
// Dump Image
// ------------------------------------------------------------

void dump_image( cfloat* image_v, std::string fname, unsigned NSAMP )
{
  std::fstream fs;
  fs.open(fname,std::ofstream::out);
  fs.precision(9);
  fs.setf(std::ios::scientific,std::ios::floatfield);
  if (fs.is_open() == 0) { std::cerr << "Cannot open " << fname << std::endl; exit(1); }
  for (unsigned ii=0; ii < NSAMP; ii++) {
    fs << image_v[ii].real << " " << image_v[ii].imag << std::endl;
  }
  fs.close();
}

// ------------------------------------------------------------
// Regression
// ------------------------------------------------------------

bool regression( cfloat* imageA, cfloat* imageG, unsigned NSAMP )
{
  static constexpr float level = 1e-6;
  bool flag = 0;
  for (unsigned ss=0; ss < NSAMP; ss++) {
    bool this_re = (std::fabs(imageG[ss].real-imageA[ss].real) > level);
    bool this_im = (std::fabs(imageG[ss].imag-imageA[ss].imag) > level);
    // if (this_re | this_im) {
    //   std::cerr << "image[" << ss <<"] Error:  Golden: (" << imageG[ss].real << ", " << imageG[ss].imag << ")"
    //             << "  Actual: (" << imageA[ss].real << ", " << imageA[ss].imag << ")" << std::endl;
    // }
    flag |= this_re;
    flag |= this_im;
  }
  return(flag);
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main(int argc, char* argv[])
{
  if (argc != 2) {
    // Must pass XCLBIN from the command line:
    std::cout << STR_USAGE << argv[0] <<" <xclbin>" << std::endl;
    return 1;
  }
  std::cout << STR_INFO << "Configuration: NSAMP = " << NSAMP << std::endl;
  std::cout << STR_INFO << "Configuration: REPEAT = " << REPEAT << std::endl;
  std::cout << STR_INFO << "Configuration: NPULSE_USE = " << NPULSE_USE << std::endl;
  std::cout << STR_INFO << "Configuration: NFRAME = " << NFRAME << std::endl;
  std::cout << STR_INFO << "Configuration: NENGINE = " << NENGINE << std::endl;

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
  // Declare SAR System Parameters
  // ------------------------------------------------------------

  // Declare GMIO buffers:
  auto bo_ifft_i = xrt::aie::bo(my_device,NUM_BYTES_PULSE_DATA,xrt::bo::flags::normal,0);
  std::cout << STR_PASSED << "Create GMIO buffer objects" << std::endl;

  // Malloc GMIO buffers:
  cfloat* ifft_i = bo_ifft_i.map<cfloat*>();
  std::cout << STR_PASSED << "Malloc GMIO buffers storage" << std::endl;

  // ------------------------------------------------------------
  // Initialize
  // ------------------------------------------------------------

  read_data_file<cfloat>("pulse_data.txt",LEN_PULSE_DATA,ifft_i);
  read_data_file< float>("antenna_coord_data.txt",LEN_COORD_DATA,coord_i);
  read_data_file< float>("R0_range_data.txt",LEN_R0_RANGE_DATA,R0_range_i);
  std::string fname_gld = "vfs_imA_raw_" + std::to_string(NPULSE_USE) + "_pulses.txt";
  read_data_file<cfloat>(fname_gld,LEN_IMAGE_DATA*NENGINE,golden_o);
  std::cout << STR_PASSED << "Read all input files" << std::endl;

  // ------------------------------------------------------------
  // Run Synthetic Aperture Radar Application
  // ------------------------------------------------------------

  auto my_graph0 = xrt::graph(my_device,xclbin_uuid,"engine0");
  my_graph0.reset();
  std::cout << STR_INFO << "Acquire AIE graph handles & reset graphs" << std::endl;

  // Transfer RTP's only once:
  my_graph0.update("engine0.engine.diff3d.kk.in[1]",coord_i);
  my_graph0.update("engine0.engine.dR_comp.kk.in[1]",R0_range_i);
  std::cout << STR_INFO << "Transferred design RTP's to AIE graphs" << std::endl;

  // Setup PL Image Buffer:
  auto image_buffer = xrt::kernel(my_device, xclbin_uuid, "image_buffer_1engine_wrapper:{image_buffer}");
  auto image_buffer_run = xrt::run(image_buffer);
  auto image_bo = xrt::bo(my_device,NUM_BYTES_IMAGE_DATA,image_buffer.group_id(0));
  cfloat* image_o = image_bo.map<cfloat*>();
  image_buffer_run.set_arg( 0, image_bo );
  image_buffer_run.set_arg( 1, NPULSE_USE );
  std::cout << STR_INFO << "Setup PL Image Buffer" << std::endl;

  // Start AIE graph iterations (all pulses in all frames):
  uint64_t tic = my_graph0.get_timestamp();
  my_graph0.run(NPULSE_USE*NFRAME);
  std::cout << STR_PASSED << "my_graph0.run(NPULSE_USE);" << std::endl;

  // Loop over a number of SAR image frames:
  std::cout << "\nRunning " << NFRAME << " frames of Synthetic Aperture Radar with " << NPULSE_USE << " radar pulses per frame" << std::endl;

  for (unsigned ff=0; ff < NFRAME; ff++) {
    std::cout << STR_INFO << "Starting Frame " << ff << " ..." << std::endl;

    // Start PL Image Buffer:
    image_buffer_run.start();
    std::cout << STR_PASSED << "image_buffer_run.start();" << std::endl;

    // Kick off IFFT transfers (all pulses in a frame):
    bo_ifft_i.async("engine0.ifft_i",XCL_BO_SYNC_BO_GMIO_TO_AIE,NUM_BYTES_PULSE_DATA,0);
    std::cout << STR_PASSED << "bo_ifft_i.async(\"engine0.ifft_i\",XCL_BO_SYNC_BO_GMIO_TO_AIE,NUM_BYTES_PULSE_DATA,0);" << std::endl;

     // Wait for processing:
    image_buffer_run.wait();
    std::cout << STR_PASSED << "image_buffer_run.wait();" << std::endl;

    // Upload final image for this frame:
    image_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    std::cout << STR_PASSED << "image_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);"  << std::endl;
    std::cout << STR_INFO << "Done Frame " << ff << std::endl;
  } // ff
  // If I uncomment out these lines I get a bus-error -- why???
  //   my_graph0.wait();
  //   std::cout << STR_PASSED << "my_graph0.wait();" << std::endl;
  uint64_t toc = my_graph0.get_timestamp();
  my_graph0.end();
  std::cout << STR_PASSED << "my_graph0.end();" << std::endl;

  // Dump final image to file:
  std::string fname_act = "hw_imA_raw_" + std::to_string(NPULSE_USE) + "_pulses.txt";
  dump_image(image_o,fname_act,LEN_IMAGE_DATA);
  std::cout << STR_PASSED << "dump_image(image_o,fname_act,LEN_IMAGE_DATA);" << std::endl;

  // ------------------------------------------------------------
  // Regression Test
  // ------------------------------------------------------------

  bool flag = regression(image_o,golden_o,LEN_IMAGE_DATA*NENGINE);

  // Compute approximate throughput:
  float fps = NFRAME * 1250.0e6 / ((float(toc)-float(tic))*NPULSE/NPULSE_USE);
  std::cout << STR_INFO << "tic: " << tic << " toc: " << toc << std::endl;
  std::cout << STR_INFO << "\nTotal run time: " << (float(toc)-float(tic))/1250.0e6 << " (sec)" << std::endl;
  std::cout << STR_INFO << "\nApproximate Throughput for SAR BP: " << fps << " fps" << std::endl;

  if (flag == 0) {
    std::cout << STR_INFO << "--- PASSED ---" << std::endl;
  }
  else {
    std::cout << STR_INFO << "*** FAILED ***" << std::endl;
  }

  return(0);
}
