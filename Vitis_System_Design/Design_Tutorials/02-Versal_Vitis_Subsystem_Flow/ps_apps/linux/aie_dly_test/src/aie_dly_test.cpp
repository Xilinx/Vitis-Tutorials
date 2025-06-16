//
// Copyright (C) 2020-2022, Xilinx, Inc. All rights reserved.
// Copyright (C) 2022-2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//

#include "datamove_app.cpp"

#include <fstream>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <vector>

#include <experimental/xrt_aie.h>
#include <experimental/xrt_graph.h>
#include <experimental/xrt_ip.h>
#include <xrt/xrt_bo.h>
#include <xrt/xrt_device.h>
#include <xrt/xrt_kernel.h>

#define  TYPE_DATA  int16_t
#define  TYPE_COE   int16_t
#define  TYPE_DATAO int32_t
#define  SHIFT      14 
#define  NUM_COE    16 

static const int      SAMPLES     = 1536;
static const std::string   DEFAULT_INPUT  = "input.txt";
static const int      REF_MOD_OFFS  = 4;

static const char*    STR_ERROR   = "ERROR:   ";
static const char*    STR_FAILED  = "FAILED:  ";
static const char*    STR_PASSED  = "PASSED:  ";
static const char*    STR_INFO    = "INFO:    ";
static const char*    STR_USAGE   = "USAGE:   ";

int main(int argc, char* argv[]) {

  if ((argc < 2) || (argc > 5)) {
    std::cout << STR_USAGE << argv[0] <<" <xclbin> <samples> <input_data.txt> <ref_model_delay>" << std::endl;
    return EXIT_FAILURE;
  }

  char* xclbinFilename = argv[1];

  // Calculating samples / bsize
  int samples     = SAMPLES;
  std::string in_filename  = DEFAULT_INPUT;
  std::cout << STR_INFO << "argc: " << argc <<std::endl;
  if(argc >= 3) {
    samples     = atoi(argv[2]);
    std::cout << STR_INFO << "Number of samples: " << samples <<std::endl;
  }
  if(argc >= 4) {
    in_filename    = argv[3];
    std::cout << STR_INFO << "Using data from:  " << in_filename <<std::endl;

  }
  int ref_mod_offs     = REF_MOD_OFFS;
  if(argc >= 5) {
    ref_mod_offs  = atoi(argv[4]);
    std::cout << STR_INFO << "Adjust ref model delay with samples: " << ref_mod_offs <<std::endl;
  }

  int bsize       = 2 * samples * sizeof(TYPE_DATA);  // Adjust size for complex data in

  // Create device, graph and kernel handles
  unsigned int dev_index = 0;
  auto my_device = xrt::device(dev_index);
  std::cout << STR_PASSED << "auto my_device = xrt::device(" << dev_index << ")" << std::endl;

  auto xclbin_uuid = my_device.load_xclbin(xclbinFilename);
  std::cout << STR_PASSED << "auto xclbin_uuid = my_device.load_xclbin(" << xclbinFilename << ")" << std::endl;

  auto my_rtl_ip = xrt::ip(my_device, xclbin_uuid, "subtractor:{subtractor_0}");
  std::cout << STR_PASSED << "auto my_rtl_ip = xrt::ip(my_device, xclbin_uuid, \"subtractor:{subtractor_0}\")" << std::endl;

  auto my_graph  = xrt::graph(my_device, xclbin_uuid, "mygraph_top");
  std::cout << STR_PASSED << "auto my_graph  = xrt::graph(my_device, xclbin_uuid, \"mygraph_top\")" << std::endl;

  auto in_0 = xrt::kernel(my_device, xclbin_uuid, "mm2s:{mm2s_1}");
  std::cout << STR_PASSED << "Create my_mm2s instance" << std::endl;

  auto out_0 = xrt::kernel(my_device, xclbin_uuid, "s2mm:{s2mm_1}");
  std::cout << STR_PASSED << "Create my_s2mm instance" << std::endl;

  // Setup input/output buffer objects
  auto in_0_bo = xrt::bo(my_device, bsize, XCL_BO_FLAGS_NONE, in_0.group_id(0));
  std::cout << STR_PASSED << "Setup input buffer objects" << std::endl;
  auto out_0_bo = xrt::bo(my_device, bsize, XCL_BO_FLAGS_NONE, out_0.group_id(0));
  std::cout << STR_PASSED << "Setup output buffer objects" << std::endl;

  // Cast buffers to type using map method.
  auto in_0_bo_mapped = in_0_bo.map<TYPE_DATA*>();
  std::cout << STR_PASSED << "Mapping input buffer objects" << std::endl;
  auto out_0_bo_mapped = out_0_bo.map<TYPE_DATA*>();
  std::cout << STR_PASSED << "auto out_0_bo_mapped = out_0_bo.map<TYPE_DATAO*>()" << std::endl;

  // Generate input stimuli
  std::vector<TYPE_DATA> in_ref(2*samples);
  std::cout << STR_PASSED << "Declare in_ref with " << in_ref.size() << " samples" << std::endl;
  std::ifstream in_file(in_filename, std::ifstream::in);
  if (!in_file.is_open()) std::cout << STR_INFO << "Missing input data file, will generate default data" << std::endl;

  if (in_file.is_open()){
    TYPE_DATA input_tmp = 0;
    for(int j =0; j<=samples; j++){
      if (in_file.peek() !=EOF){
        in_file >> std::dec >> input_tmp;
        in_ref[j] = input_tmp;
      } else {
        in_ref[j] = (TYPE_DATA) 0;
      }
    }
    std::cout << STR_PASSED << "File read completed" << std::endl;
    in_file.close();
    std::cout << STR_PASSED << "File closed" << std::endl;
  } else {
    std::cout << STR_PASSED << "Generating input stimuli as ramp" << std::endl;
    for(int j =0; j<samples; j++){
      in_ref[j*2]   = j; // Real part
      in_ref[j*2+1] = 0; // Imag part
    }
  }


//  for(int k =0; k=samples; k++){
//    //setting input data
//    //in_ref[k] = (TYPE_DATA) (std::rand());
//    //in_ref[k] = (TYPE_DATA) k;
//    in_ref[k] = (TYPE_DATA) 0;
//    if (k == imp_time) {
//      in_ref[k] = (TYPE_DATA) 1 << SHIFT;
//    }
//  }

  // Prepare input signal for DMA transfer
  for(int j =0; j<=samples; j++){
    in_0_bo_mapped[2*j]   = (TYPE_DATA) in_ref[2*j];     // Real data in
    in_0_bo_mapped[2*j+1] = (TYPE_DATA) in_ref[2*j+1];   // Imag data in
    // Print first 20 input samples
    if (j < 20) {
      std::cout << "in0 = " << (int) in_0_bo_mapped[j] << "+i" << (int) in_0_bo_mapped[j+1] <<  std::endl;
    }
  }

  // Setup filter coefficients
  TYPE_COE coe0[NUM_COE]  = {8192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  TYPE_COE coe1[NUM_COE]  = {0, 0, 0, 4096, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


//  // Reference model - apply filter signals
//  int64_t out_ref[samples];  // allow for 64 bit accumulation
//  TYPE_DATA out_ref_rnd[samples];
//  for(int k =0; k<=samples; k++){
//    out_ref[k] = 0;
//    for (int i = 0; i < NUM_COE; i++){
//      int lag = (int64_t) k-NUM_COE-1+i+ref_mod_offs;
//      if (lag >= 0) {
//        out_ref[k] += in_ref[lag]*coe1[NUM_COE-1-i];
//      }
//    }
//    out_ref_rnd[k] = (TYPE_DATA) round(out_ref[k] >> SHIFT);  // Compensate filter coe scaling
//    // Debug // std::cout << "out_ref[" << k << "] = " << out_ref[k] << std::endl;
//  }

  // Send input buffer to DMA
  //in_0_bo.write(in_0_bo_mapped);
  in_0_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE);
  std::cout << STR_PASSED << "in_0_bo.sync(XCL_BO_SYNC_BO_TO_DEVICE)" << std::endl;
  // Run the DMA
  auto in_0_run = in_0(in_0_bo, nullptr, samples);
  std::cout << STR_PASSED << "auto in_0_run = in_0(in_0_bo, nullptr, " << samples << ")" << std::endl;

  // Starting the output sample s2mm kernel
  auto out_0_run = out_0(out_0_bo, nullptr, samples);
  std::cout << STR_PASSED << "auto out_run = out(out_0_bo, nullptr, " << samples << ")" << std::endl;


  // Reset and run the AIE graph
  my_graph.reset();
  std::cout << STR_PASSED << "my_graph.reset()" << std::endl;


  // Setup filter coefficients
  // Locate AIE folder "Work/pc/c_rts/aie_control_config.json" and search for "alias_name" or "port_name"
  // using alias syntax as using in AIE graph testbench:
  my_graph.update("mygraph_top.coeff[0]", coe0);
  // Setup filter coefficients using port syntax
  my_graph.update("mygraph_top.my_fir_graph[1].fir16_i.in[0]", coe1);

  my_graph.run(-1);
  std::cout << STR_PASSED << "my_graph.run(-1)" << std::endl;

  for (int i = 0; i < 10; i++) {
    sleep(1);
    std::cout << "Poll subtractor register" << std::endl;
    std::cout << "  Value Reg0:  " << std::hex << ( my_rtl_ip.read_register(0x00) ) << std::endl;
    std::cout << "  Value Reg1:  " << std::hex << ( my_rtl_ip.read_register(0x04) ) << std::endl;
    std::cout << "  Value Reg2:  " << std::hex << ( my_rtl_ip.read_register(0x08) ) << std::endl;
    std::cout << "  Value Reg3:  " << std::hex << ( my_rtl_ip.read_register(0x0c) ) << std::endl;
  }

  // Waiting for DMA to end
  std::cout << std::endl << STR_INFO << "Waiting for DMA end..." << std::endl << std::endl;

  in_0_run.wait();
  std::cout << STR_PASSED << "in_0_run.wait()" << std::endl;

  out_0_run.wait();
  std::cout << STR_PASSED << "out_0_run.wait()" << std::endl;

  // Retrieving the results
  out_0_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
  //out_0_bo.read(out_0_bo_mapped);
  std::cout << STR_PASSED << "out_0_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE)" << std::endl;

  // Print first 20 output results
  for(int k =0; k<20; k++){
    //  if (lag >= 0) {
    //    out_ref[k] += in_ref[lag]*coe[i];
    //  }
    std::cout << "out_0_bo_mapped[" << k << "] = " << out_0_bo_mapped[2*k] << "+i" << out_0_bo_mapped[2*k+1];
    //std::cout << " out_ref_rnd[" << k << "] = " << out_ref_rnd[k] << std::endl;
  }


  my_graph.end(1000);
  std::cout << STR_PASSED << "my_graph.end()" << std::endl;
  std::cout << std::endl << STR_PASSED << argv[0] << std::endl << std::endl;
}
