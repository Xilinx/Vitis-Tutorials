//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Mark Rollins

#include "sar_top_8engine_cfg.h"
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

alignas(16) static  float coord_i[LEN_COORD_DATA];
alignas(16) static  float R0_range_i[LEN_R0_RANGE_DATA];
static  cfloat golden_o[LEN_IMAGE_DATA*NENGINE];

// ------------------------------------------------------------
// Instantiate AIE graph
// ------------------------------------------------------------

static sar_top_1engine_graph<REPEAT,NSAMP,0> engine0;
static sar_top_1engine_graph<REPEAT,NSAMP,1> engine1;
static sar_top_1engine_graph<REPEAT,NSAMP,2> engine2;
static sar_top_1engine_graph<REPEAT,NSAMP,3> engine3;
static sar_top_1engine_graph<REPEAT,NSAMP,4> engine4;
static sar_top_1engine_graph<REPEAT,NSAMP,5> engine5;
static sar_top_1engine_graph<REPEAT,NSAMP,6> engine6;
static sar_top_1engine_graph<REPEAT,NSAMP,7> engine7;

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

template<unsigned NENGINE>
void dump_image( cfloat* (&image)[NENGINE], std::string fname, unsigned NSAMP )
{
  std::fstream fs;
  fs.open(fname,std::ofstream::out);
  fs.precision(9);
  fs.setf(std::ios::scientific,std::ios::floatfield);
  if (fs.is_open() == 0) { std::cerr << "Cannot open " << fname << std::endl; exit(1); }
  for (unsigned ee=0; ee < NENGINE; ee++) {
    for (unsigned ii=0; ii < NSAMP; ii++) { fs << image[ee][ii].real << " " << image[ee][ii].imag << std::endl; }
  }
  fs.close();
}

// ------------------------------------------------------------
// Regression
// ------------------------------------------------------------

template <unsigned NENGINE>
bool regression( cfloat* (&image)[NENGINE], cfloat* imageG, unsigned NSAMP )
{
  static constexpr float level = 1e-6;
  bool flag = 0;
  unsigned gg=0;
  for (unsigned ee=0, gg=0; ee < NENGINE; ee++) {
    for (unsigned ss=0; ss < NSAMP; ss++) {
      bool this_re = (std::fabs(imageG[gg].real-image[ee][ss].real) > level);
      bool this_im = (std::fabs(imageG[gg].imag-image[ee][ss].imag) > level);
      flag |= this_re;
      flag |= this_im;
      gg++;
    }
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
  xrt::aie::bo bo_ifft_i[NENGINE];
  cfloat* ifft_i[NENGINE];
  for (unsigned ee=0; ee < NENGINE; ee++) {
    bo_ifft_i[ee]  = xrt::aie::bo(my_device,NUM_BYTES_PULSE_DATA,xrt::bo::flags::normal,0);
    ifft_i[ee]  = bo_ifft_i[ee].map<cfloat*>();
  }
  std::cout << STR_INFO << "Created GMIO buffer objects" << std::endl;

  // ------------------------------------------------------------
  // Initialize
  // ------------------------------------------------------------

  for (unsigned ee=0; ee < NENGINE; ee++)
    read_data_file<cfloat>("pulse_data.txt",LEN_PULSE_DATA,ifft_i[ee]);
  read_data_file< float>("antenna_coord_data.txt",LEN_COORD_DATA,coord_i);
  read_data_file< float>("R0_range_data.txt",LEN_R0_RANGE_DATA,R0_range_i);
  std::string fname_gld = "vfs_imA_raw_" + std::to_string(NPULSE_USE) + "_pulses.txt";
  read_data_file<cfloat>(fname_gld,LEN_IMAGE_DATA*NENGINE,golden_o);
  std::cout << STR_INFO << "Read all input files" << std::endl;

  // ------------------------------------------------------------
  // Run Synthetic Aperture Radar Application
  // ------------------------------------------------------------

  auto my_graph0 = xrt::graph(my_device,xclbin_uuid,"engine0");
  auto my_graph1 = xrt::graph(my_device,xclbin_uuid,"engine1");
  auto my_graph2 = xrt::graph(my_device,xclbin_uuid,"engine2");
  auto my_graph3 = xrt::graph(my_device,xclbin_uuid,"engine3");
  auto my_graph4 = xrt::graph(my_device,xclbin_uuid,"engine4");
  auto my_graph5 = xrt::graph(my_device,xclbin_uuid,"engine5");
  auto my_graph6 = xrt::graph(my_device,xclbin_uuid,"engine6");
  auto my_graph7 = xrt::graph(my_device,xclbin_uuid,"engine7");
  my_graph0.reset();
  my_graph1.reset();
  my_graph2.reset();
  my_graph3.reset();
  my_graph4.reset();
  my_graph5.reset();
  my_graph6.reset();
  my_graph7.reset();
  std::cout << STR_INFO << "Acquire AIE graph handles & reset graphs" << std::endl;

  // Transfer RTP's only once:
  my_graph0.update("engine0.engine.diff3d.kk.in[1]",coord_i);
  my_graph1.update("engine1.engine.diff3d.kk.in[1]",coord_i);
  my_graph2.update("engine2.engine.diff3d.kk.in[1]",coord_i);
  my_graph3.update("engine3.engine.diff3d.kk.in[1]",coord_i);
  my_graph4.update("engine4.engine.diff3d.kk.in[1]",coord_i);
  my_graph5.update("engine5.engine.diff3d.kk.in[1]",coord_i);
  my_graph6.update("engine6.engine.diff3d.kk.in[1]",coord_i);
  my_graph7.update("engine7.engine.diff3d.kk.in[1]",coord_i);

  my_graph0.update("engine0.engine.dR_comp.kk.in[1]",R0_range_i);
  my_graph1.update("engine1.engine.dR_comp.kk.in[1]",R0_range_i);
  my_graph2.update("engine2.engine.dR_comp.kk.in[1]",R0_range_i);
  my_graph3.update("engine3.engine.dR_comp.kk.in[1]",R0_range_i);
  my_graph4.update("engine4.engine.dR_comp.kk.in[1]",R0_range_i);
  my_graph5.update("engine5.engine.dR_comp.kk.in[1]",R0_range_i);
  my_graph6.update("engine6.engine.dR_comp.kk.in[1]",R0_range_i);
  my_graph7.update("engine7.engine.dR_comp.kk.in[1]",R0_range_i);
  std::cout << STR_INFO << "Transferred design RTP's to AIE graphs" << std::endl;

  // Setup PL Image Buffers:
  auto buff0 = xrt::kernel(my_device, xclbin_uuid, "image_buffer_8engine_wrapper:{buff0}");
  auto buff1 = xrt::kernel(my_device, xclbin_uuid, "image_buffer_8engine_wrapper:{buff1}");
  auto buff2 = xrt::kernel(my_device, xclbin_uuid, "image_buffer_8engine_wrapper:{buff2}");
  auto buff3 = xrt::kernel(my_device, xclbin_uuid, "image_buffer_8engine_wrapper:{buff3}");
  auto buff4 = xrt::kernel(my_device, xclbin_uuid, "image_buffer_8engine_wrapper:{buff4}");
  auto buff5 = xrt::kernel(my_device, xclbin_uuid, "image_buffer_8engine_wrapper:{buff5}");
  auto buff6 = xrt::kernel(my_device, xclbin_uuid, "image_buffer_8engine_wrapper:{buff6}");
  auto buff7 = xrt::kernel(my_device, xclbin_uuid, "image_buffer_8engine_wrapper:{buff7}");

  auto buff0_run = xrt::run(buff0);
  auto buff1_run = xrt::run(buff1);
  auto buff2_run = xrt::run(buff2);
  auto buff3_run = xrt::run(buff3);
  auto buff4_run = xrt::run(buff4);
  auto buff5_run = xrt::run(buff5);
  auto buff6_run = xrt::run(buff6);
  auto buff7_run = xrt::run(buff7);

  auto image0_bo = xrt::bo(my_device,NUM_BYTES_IMAGE_DATA,buff0.group_id(0));
  auto image1_bo = xrt::bo(my_device,NUM_BYTES_IMAGE_DATA,buff1.group_id(0));
  auto image2_bo = xrt::bo(my_device,NUM_BYTES_IMAGE_DATA,buff2.group_id(0));
  auto image3_bo = xrt::bo(my_device,NUM_BYTES_IMAGE_DATA,buff3.group_id(0));
  auto image4_bo = xrt::bo(my_device,NUM_BYTES_IMAGE_DATA,buff4.group_id(0));
  auto image5_bo = xrt::bo(my_device,NUM_BYTES_IMAGE_DATA,buff5.group_id(0));
  auto image6_bo = xrt::bo(my_device,NUM_BYTES_IMAGE_DATA,buff6.group_id(0));
  auto image7_bo = xrt::bo(my_device,NUM_BYTES_IMAGE_DATA,buff7.group_id(0));

  cfloat* image_o[NENGINE];
  image_o[0] = image0_bo.map<cfloat*>();
  image_o[1] = image1_bo.map<cfloat*>();
  image_o[2] = image2_bo.map<cfloat*>();
  image_o[3] = image3_bo.map<cfloat*>();
  image_o[4] = image4_bo.map<cfloat*>();
  image_o[5] = image5_bo.map<cfloat*>();
  image_o[6] = image6_bo.map<cfloat*>();
  image_o[7] = image7_bo.map<cfloat*>();

  buff0_run.set_arg( 0, image0_bo );
  buff1_run.set_arg( 0, image1_bo );
  buff2_run.set_arg( 0, image2_bo );
  buff3_run.set_arg( 0, image3_bo );
  buff4_run.set_arg( 0, image4_bo );
  buff5_run.set_arg( 0, image5_bo );
  buff6_run.set_arg( 0, image6_bo );
  buff7_run.set_arg( 0, image7_bo );

  buff0_run.set_arg( 1, NPULSE_USE );
  buff1_run.set_arg( 1, NPULSE_USE );
  buff2_run.set_arg( 1, NPULSE_USE );
  buff3_run.set_arg( 1, NPULSE_USE );
  buff4_run.set_arg( 1, NPULSE_USE );
  buff5_run.set_arg( 1, NPULSE_USE );
  buff6_run.set_arg( 1, NPULSE_USE );
  buff7_run.set_arg( 1, NPULSE_USE );

  std::cout << STR_INFO << "Setup PL Image Buffers" << std::endl;

  // Start AIE graph iterations (all pulses in all frames):
  uint64_t tic = my_graph0.get_timestamp();
  my_graph0.run(NPULSE_USE*NFRAME);
  my_graph1.run(NPULSE_USE*NFRAME);
  my_graph2.run(NPULSE_USE*NFRAME);
  my_graph3.run(NPULSE_USE*NFRAME);
  my_graph4.run(NPULSE_USE*NFRAME);
  my_graph5.run(NPULSE_USE*NFRAME);
  my_graph6.run(NPULSE_USE*NFRAME);
  my_graph7.run(NPULSE_USE*NFRAME);
  std::cout << STR_INFO << "All AIE graphs are running" << std::endl;

  // Loop over a number of SAR image frames:
  std::cout << "\nRunning " << NFRAME << " frames of Synthetic Aperture Radar with " << NPULSE_USE << " radar pulses per frame" << std::endl;

  for (unsigned ff=0; ff < NFRAME; ff++) {
    std::cout << STR_INFO << "Starting Frame " << ff << " ..." << std::endl;

    // Start PL Image Buffer:
    buff0_run.start();
    buff1_run.start();
    buff2_run.start();
    buff3_run.start();
    buff4_run.start();
    buff5_run.start();
    buff6_run.start();
    buff7_run.start();
    std::cout << STR_INFO << "All PL image buffers started" << std::endl;

    // Kick off IFFT transfers (all pulses in a frame):
    bo_ifft_i[0].async("engine0.ifft_i",XCL_BO_SYNC_BO_GMIO_TO_AIE,NUM_BYTES_PULSE_DATA,0);
    bo_ifft_i[1].async("engine1.ifft_i",XCL_BO_SYNC_BO_GMIO_TO_AIE,NUM_BYTES_PULSE_DATA,0);
    bo_ifft_i[2].async("engine2.ifft_i",XCL_BO_SYNC_BO_GMIO_TO_AIE,NUM_BYTES_PULSE_DATA,0);
    bo_ifft_i[3].async("engine3.ifft_i",XCL_BO_SYNC_BO_GMIO_TO_AIE,NUM_BYTES_PULSE_DATA,0);
    bo_ifft_i[4].async("engine4.ifft_i",XCL_BO_SYNC_BO_GMIO_TO_AIE,NUM_BYTES_PULSE_DATA,0);
    bo_ifft_i[5].async("engine5.ifft_i",XCL_BO_SYNC_BO_GMIO_TO_AIE,NUM_BYTES_PULSE_DATA,0);
    bo_ifft_i[6].async("engine6.ifft_i",XCL_BO_SYNC_BO_GMIO_TO_AIE,NUM_BYTES_PULSE_DATA,0);
    bo_ifft_i[7].async("engine7.ifft_i",XCL_BO_SYNC_BO_GMIO_TO_AIE,NUM_BYTES_PULSE_DATA,0);
    std::cout << STR_INFO << "All GMIO IFFT transfers started" << std::endl;

    // Wait for processing:
    buff0_run.wait();
    buff1_run.wait();
    buff2_run.wait();
    buff3_run.wait();
    buff4_run.wait();
    buff5_run.wait();
    buff6_run.wait();
    buff7_run.wait();
    std::cout << STR_INFO << "Done waiting for all PL image buffers" << std::endl;

    // Upload final image for this frame:
    image0_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    image1_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    image2_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    image3_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    image4_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    image5_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    image6_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    image7_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    std::cout << STR_INFO << "Done uploading images from PL image buffers"  << std::endl;
    std::cout << STR_INFO << "Done Frame " << ff << std::endl;
  } // ff
  uint64_t toc = my_graph7.get_timestamp();
  my_graph0.end();
  my_graph1.end();
  my_graph2.end();
  my_graph3.end();
  my_graph4.end();
  my_graph5.end();
  my_graph6.end();
  my_graph7.end();
  std::cout << STR_INFO << "Done ending all graphs" << std::endl;

  // Compute approximate throughput:
  float fps = NFRAME * 1250.0e6 / ((float(toc)-float(tic))*NPULSE/NPULSE_USE);
  std::cout << STR_INFO << "tic: " << tic << " toc: " << toc << std::endl;
  std::cout << STR_INFO << "\nTotal run time: " << (float(toc)-float(tic))/1250.0e6 << " (sec)" << std::endl;
  std::cout << STR_INFO << "\nApproximate Throughput for SAR BP: " << fps << " fps" << std::endl;

  std::string fname_act = "hw_imA_raw_" + std::to_string(NPULSE_USE) + "_pulses.txt";
  dump_image<NENGINE>(image_o,fname_act,LEN_IMAGE_DATA);
  std::cout << STR_PASSED << "dump_image(image_o,fname_act,LEN_IMAGE_DATA);" << std::endl;

  // ------------------------------------------------------------
  // Regression Test
  // ------------------------------------------------------------

  bool flag = regression<NENGINE>(image_o,golden_o,LEN_IMAGE_DATA);

  if (flag == 0) {
    std::cout << STR_INFO << "--- PASSED ---" << std::endl;
  }
  else {
    std::cout << STR_INFO << "*** FAILED ***" << std::endl;
  }

  return(0);
}
