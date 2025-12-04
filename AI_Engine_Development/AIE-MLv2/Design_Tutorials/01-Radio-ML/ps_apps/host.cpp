//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <unistd.h>
#include <xrt/xrt_device.h>
#include <xrt/xrt_kernel.h>
#include <xrt/xrt_aie.h>
#include <xrt/xrt_graph.h>
#include <xrt/experimental/xrt_ip.h>

#include <adf.h>

#include "read_file.h"

static const char*    STR_PASSED = "PASSED:  ";
static const char*    STR_USAGE  = "USAGE:   ";

// ------------------------------------------------------------
// Run Parameters
// ------------------------------------------------------------

static constexpr int32_t   NUM_ITER         = 8;
static constexpr unsigned  NUM_SAMPLES_I    = 1024*2;  // 1024 I/Q samples
static constexpr unsigned  NUM_SAMPLES_O    = 24;
static constexpr unsigned  BYTES_PER_SAMPLE = 2;
static constexpr unsigned  SIG_I_BYTES      = NUM_SAMPLES_I * BYTES_PER_SAMPLE * NUM_ITER;
static constexpr unsigned  SIG_O_BYTES      = NUM_SAMPLES_O * BYTES_PER_SAMPLE * NUM_ITER;

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
  // Declare GMIO buffers and RTP arrays
  // ------------------------------------------------------------

  // Declare GMIO buffers:
  auto sig_iq = xrt::aie::bo(my_device,SIG_I_BYTES,xrt::bo::flags::normal,1);
  auto sig_o  = xrt::aie::bo(my_device,SIG_O_BYTES,xrt::bo::flags::normal,1);
  std::cout << STR_PASSED << "Create GMIO buffer objects" << std::endl;

  // Malloc GMIO buffers:
  bfloat16* data_iq = sig_iq.map<bfloat16*>();
  bfloat16* data_o  = sig_o.map<bfloat16*>();

  // Declare arrays for RTPs
  bfloat16 weights_w1[896];
  bfloat16 biases_w1[64];
  bfloat16 weights_w3[28672];
  bfloat16 biases_w3[64];
  bfloat16 weights_w5[28672];
  bfloat16 biases_w5[64];
  bfloat16 weights_w7[28672];
  bfloat16 biases_w7[64];
  bfloat16 weights_w9[28672];
  bfloat16 biases_w9[64];
  bfloat16 weights_w11[28672];
  bfloat16 biases_w11[64];
  bfloat16 weights_w13[28672];
  bfloat16 biases_w13[64];
  bfloat16 weights_0_w16[65536/2];
  bfloat16 weights_1_w16[65536/2];
  bfloat16 biases_w16[128];
  bfloat16 weights_w17[16384];
  bfloat16 biases_w17[128];
  bfloat16 weights_w18[3072];
  bfloat16 biases_w18[24];

  // Declare array for golden output
  bfloat16 golden_o[NUM_SAMPLES_O*NUM_ITER];

  // Declare array of strings for modulation_classes
  std::string modulation_classes[24];

  // ------------------------------------------------------------
  // Read input/RTPs files
  // ------------------------------------------------------------
  read_file<bfloat16,NUM_SAMPLES_I*NUM_ITER>("data_iq.txt",data_iq);
  read_file<bfloat16,896>  ("w1_weights-rtp.txt",weights_w1);
  read_file<bfloat16,64>   ("w1_biases-rtp.txt",  biases_w1);
  read_file<bfloat16,28672>("w3_weights-rtp.txt",weights_w3);
  read_file<bfloat16,64>   ("w3_biases-rtp.txt",  biases_w3);
  read_file<bfloat16,28672>("w5_weights-rtp.txt",weights_w5);
  read_file<bfloat16,64>   ("w5_biases-rtp.txt",  biases_w5);
  read_file<bfloat16,28672>("w7_weights-rtp.txt",weights_w7);
  read_file<bfloat16,64>   ("w7_biases-rtp.txt",  biases_w7);
  read_file<bfloat16,28672>("w9_weights-rtp.txt",weights_w9);
  read_file<bfloat16,64>   ("w9_biases-rtp.txt",  biases_w9);
  read_file<bfloat16,28672>("w11_weights-rtp.txt",weights_w11);
  read_file<bfloat16,64>   ("w11_biases-rtp.txt",  biases_w11);
  read_file<bfloat16,28672>("w13_weights-rtp.txt",weights_w13);
  read_file<bfloat16,64>   ("w13_biases-rtp.txt",  biases_w13);
  read_file<bfloat16,65536/2>("w16_0_weights-rtp.txt",weights_0_w16);
  read_file<bfloat16,65536/2>("w16_1_weights-rtp.txt",weights_1_w16);
  read_file<bfloat16,128>   ("w16_biases-rtp.txt",  biases_w16);
  read_file<bfloat16,16384> ("w17_weights-rtp.txt",weights_w17);
  read_file<bfloat16,128>   ("w17_biases-rtp.txt",  biases_w17);
  read_file<bfloat16,3072> ("w18_weights-rtp.txt",weights_w18);
  read_file<bfloat16,24>   ("w18_biases-rtp.txt",  biases_w18);
  read_file<bfloat16,NUM_SAMPLES_O*NUM_ITER> ("data_o.txt",  golden_o);
  std::cout << STR_PASSED << "Read all IO/RTP files" << std::endl;
  
  // Read modulation_classes
  std::ifstream inputFile;
  inputFile.open("modulation_classes.txt");
  if (!inputFile.is_open()) {
      std::cerr << "Error opening file!" << std::endl;
      return 1; 
  }  
  std::string line;
  int iter = 0;
  while (std::getline(inputFile, line)) {
      modulation_classes[iter] = line;
      iter++;
  }
  std::cout << STR_PASSED << "Read modulation_classes.txt file" << std::endl;
  
  // ------------------------------------------------------------
  // Load and Run AIE graph
  // ------------------------------------------------------------

  auto my_graph  = xrt::graph(my_device, xclbin_uuid, "aie_dut");
  std::cout << STR_PASSED << "auto my_graph  = xrt::graph(my_device, xclbin_uuid, \"aie_dut\")" << std::endl;

  my_graph.reset();
  std::cout << STR_PASSED << "my_graph.reset()" << std::endl;

  // Transfer RTP's only once:
  my_graph.update("aie_dut.dut.layer_w1.kk.in[1]",weights_w1);
  my_graph.update("aie_dut.dut.layer_w1.kk.in[2]",biases_w1);
  my_graph.update("aie_dut.dut.layer_w3.kk.in[1]",weights_w3);
  my_graph.update("aie_dut.dut.layer_w3.kk.in[2]",biases_w3);
  my_graph.update("aie_dut.dut.layer_w5.kk.in[1]",weights_w5);
  my_graph.update("aie_dut.dut.layer_w5.kk.in[2]",biases_w5);
  my_graph.update("aie_dut.dut.layer_w7.kk.in[1]",weights_w7);
  my_graph.update("aie_dut.dut.layer_w7.kk.in[2]",biases_w7);
  my_graph.update("aie_dut.dut.layer_w9.kk.in[1]",weights_w9);
  my_graph.update("aie_dut.dut.layer_w9.kk.in[2]",biases_w9);
  my_graph.update("aie_dut.dut.layer_w11.kk.in[1]",weights_w11);
  my_graph.update("aie_dut.dut.layer_w11.kk.in[2]",biases_w11);
  my_graph.update("aie_dut.dut.layer_w13.kk.in[1]",weights_w13);
  my_graph.update("aie_dut.dut.layer_w13.kk.in[2]",biases_w13);
  my_graph.update("aie_dut.dut.layer_w16.kk.in[1]",weights_0_w16);
  my_graph.update("aie_dut.dut.layer_w16.kk.in[2]",weights_1_w16);
  my_graph.update("aie_dut.dut.layer_w16.kk.in[3]",biases_w16);
  my_graph.update("aie_dut.dut.layer_w17.kk.in[1]",weights_w17);
  my_graph.update("aie_dut.dut.layer_w17.kk.in[2]",biases_w17);
  my_graph.update("aie_dut.dut.layer_w18.kk.in[1]",weights_w18);
  my_graph.update("aie_dut.dut.layer_w18.kk.in[2]",biases_w18);
  std::cout << STR_PASSED << "Transferred design RTP's to AIE graphs" << std::endl;
  
  // Start data transfer
  sig_iq.async("aie_dut.data_iq",XCL_BO_SYNC_BO_GMIO_TO_AIE,SIG_I_BYTES,0);

  // Start throughput profiling
  xrt::aie::profiling handle(my_device);
  std::cout << STR_PASSED << "xrt::aie::profiling handle(my_device);" << std::endl;
  handle.start(xrt::aie::profiling::profiling_option::io_stream_start_to_bytes_transferred_cycles,"aie_dut.data_o","",SIG_O_BYTES);
  
  my_graph.run(NUM_ITER);
  std::cout << STR_PASSED << "my_graph.run( NUM_ITER=" << NUM_ITER << " )" << std::endl;
  auto sig_o_handle=sig_o.async("aie_dut.data_o",XCL_BO_SYNC_BO_AIE_TO_GMIO,SIG_O_BYTES,0);
  sig_o_handle.wait();

  std::cout << "Graph completed execution" << std::endl;

  my_graph.end();
  
  // ------------------------------------------------------------
  // Measure Inference Throughput
  // ------------------------------------------------------------

  long long cycle_count = handle.read();
  handle.stop();
  double throughput = (double) NUM_ITER / ((cycle_count) * 0.8 * 1e-6);

  // ------------------------------------------------------------
  // Regression Test
  // ------------------------------------------------------------

  bool flag = 0;
  double tolerance = 0.05; // Minor functional mismatch expected
  for(unsigned int i=0;i<NUM_ITER*NUM_SAMPLES_O;i++){
    float error = float(data_o[i])-float(golden_o[i]);
    bool this_flag = ( error > tolerance );
    flag |= this_flag;
    if(this_flag){
      std::cout<<"ERROR:data_o["<<i<<"]="<<data_o[i]<<",gold = "<<golden_o[i]<<std::endl;
    }
  }
  if (flag==0)
    std::cout<<"Model accuracy is within tolerance of " << tolerance << std::endl;
  if (flag==1)
    std::cout<<"Model accuracy is outside tolerance of " << tolerance << std::endl;

  // ------------------------------------------------------------
  // Find the index of the maximum probability in each graph iteration and extract modulation prediction
  // ------------------------------------------------------------
  bfloat16 golden_max = 0;
  int golden_max_index = 0;
  bfloat16 model_max = 0;
  int model_max_index = 0;
  std::string golden_classification;
  std::string implementation_classification;
  for (unsigned int i=0;i<NUM_ITER;i++){
    golden_max = 0;
    golden_max_index = 0;
    model_max = 0;
    model_max_index = 0;
    for (unsigned int j=0;j<NUM_SAMPLES_O;j++){
      if (golden_o[j+i*NUM_SAMPLES_O]>golden_max){
        golden_max       = golden_o[j+i*NUM_SAMPLES_O];
        golden_max_index = j;
      }
      if (data_o[j+i*NUM_SAMPLES_O]>model_max){
        model_max       = data_o[j+i*NUM_SAMPLES_O];
        model_max_index = j;
      }
    }
    golden_classification = modulation_classes[golden_max_index];
    implementation_classification = modulation_classes[model_max_index];
    std::cout << "Inference # = " << i << " golden_classification = " << golden_classification <<  " implementation_classification = " << implementation_classification << std::endl;
  }

  std::cout << "=============================="          << std::endl;
  std::cout << "Cycle count: " << cycle_count            << std::endl;
  std::cout << "Approx Inference Throughput: " << throughput << " KHz" << std::endl;
  std::cout << "=============================="          << std::endl;

  if ( flag == 0 )
    std::cout << std::endl << "--- PASSED ---" << std::endl;
  else
    std::cout << std::endl << "*** FAILED ***" << std::endl;

  return(flag);

}
