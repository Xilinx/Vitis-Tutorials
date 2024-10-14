//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Chandra Sekar Balakrishnan

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <cstring>
#include "../../aie_ml/input_sig_i.h"
#include "../../aie_ml/input_del_i_optimized.h"
#include "../../aie_ml/output_sig_o.h"

#include <unistd.h>
#include <xrt/xrt_kernel.h>

#include <experimental/xrt_aie.h>
#include <experimental/xrt_graph.h>
#define PROFILE

static const char*    STR_ERROR  = "ERROR:   ";
static const char*    STR_PASSED = "PASSED:  ";
static const char*    STR_USAGE  = "USAGE:   ";
static const char*    STR_INFO   = "INFO:    ";

typedef int TDATA;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main(int argc, char* argv[])
{
  int ITERATION = 4;
  int SAMPLES_PER_ITERATION = 1024;
  int NUMBER_OF_BYTES = 4;
  /* Each iteration computes 1024 cint16 samples */
  const int BLOCK_SIZE_sig_in_Bytes = SAMPLES_PER_ITERATION * NUMBER_OF_BYTES * ITERATION;  // 1024 * 4  (cint16 - 32 bits - 4bytes) * ITERATION (4)
  const int BLOCK_SIZE_del_in_Bytes = SAMPLES_PER_ITERATION * NUMBER_OF_BYTES * ITERATION;  // 1024 * 4  (int32 - 32 bits)  * ITERATION (4)
  const int BLOCK_SIZE_out_Bytes =  SAMPLES_PER_ITERATION * NUMBER_OF_BYTES * ITERATION;    // 1024 * 4  (cint16 - 32 bits) * ITERATION (4)
  std::ofstream Output_File("sig_o_gen.txt");


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

  // Buffer allocation
  // sig_i  buffer
  auto in_buffer_sig_i = xrt::aie::bo (my_device, BLOCK_SIZE_sig_in_Bytes,xrt::bo::flags::normal, /*memory group*/0); //Only non-cacheable buffer is supported
  
  short int* sig_i_Array = in_buffer_sig_i.map<short int*>();
  
  // del_i buffer
  auto in_buffer_del = xrt::aie::bo (my_device, BLOCK_SIZE_del_in_Bytes,xrt::bo::flags::normal, /*memory group*/0); //Only non-cacheable buffer is supported    

  int* del_i_Array = in_buffer_del.map<int*>();

  // sig_o buffer
  auto out_buffer_sig_o = xrt::aie::bo (my_device, BLOCK_SIZE_out_Bytes,xrt::bo::flags::normal, /*memory group*/0); //Only non-cacheable buffer is supported    

  short int* sig_o_Array = out_buffer_sig_o.map<short int*>();

  std::cout<<"GMIO::malloc completed"<<std::endl;

  /* Copy the samples to the buffer */
  memcpy(sig_i_Array, input_sig_i, BLOCK_SIZE_sig_in_Bytes);
  
  /* Copy the delay values to the buffer */
  memcpy(del_i_Array, input_del_i,BLOCK_SIZE_del_in_Bytes);


  in_buffer_sig_i.async("aie_dut.sig_i",XCL_BO_SYNC_BO_GMIO_TO_AIE,BLOCK_SIZE_sig_in_Bytes,/*offset*/0);
  in_buffer_del.async("aie_dut.del_i",XCL_BO_SYNC_BO_GMIO_TO_AIE,BLOCK_SIZE_del_in_Bytes,/*offset*/0);

#ifdef PROFILE
  // Start throughput profiling
  xrt::aie::profiling handle(my_device);
  std::cout << STR_PASSED << "xrt::aie::profiling handle(my_device);" << std::endl;

  handle.start(xrt::aie::profiling::profiling_option::io_stream_start_to_bytes_transferred_cycles,
               "aie_dut.sig_o","",BLOCK_SIZE_out_Bytes);
  std::cout << std::endl << STR_INFO << "Started profiling timers..." << std::endl << std::endl;
#endif

  my_graph.run(ITERATION);
  std::cout << STR_PASSED << "my_graph.run( ITERATION=" << ITERATION << " )" << std::endl;

  auto dout_buffer_run=out_buffer_sig_o.async("aie_dut.sig_o",XCL_BO_SYNC_BO_AIE_TO_GMIO,BLOCK_SIZE_out_Bytes,/*offset*/0);

  // PS can do other tasks here when data is transferring
  dout_buffer_run.wait();//Wait for gmioOut to complete

  // ------------------------------------------------------------
  // Measure Throughput
  // ------------------------------------------------------------
#ifdef PROFILE
  long long cycle_count = handle.read();
  handle.stop();
  double throughput = (double) BLOCK_SIZE_out_Bytes / ((cycle_count) * 0.8 * 1e-3); // bytes per second
  std::cout<<"Throughput of the graph: "<<throughput<<" MB/s"<<std::endl;
  std::cout<<"Throughput of the graph: "<<throughput/4<<" MSPS"<<std::endl;
#endif
  // ------------------------------------------------------------
  // Post-processing     
  // ------------------------------------------------------------
  int count=0;
  for(int i=0; i<SAMPLES_PER_ITERATION*8; i++)
  {
    if(sig_o_Array[i] != output_sig_o[i]){
      // std::cout<<"ERROR:sig_o_Array["<<i<<"]="<<sig_o_Array[i]<<",output_sig_o="<<output_sig_o[i]<<std::endl;
      Output_File << sig_o_Array[i];
      Output_File << " ";
    } else{
      // std::cout<<"CORRECT:sig_o_Array["<<i<<"]="<<sig_o_Array[i]<<",output_sig_o="<<output_sig_o[i]<<std::endl;
      Output_File << sig_o_Array[i];
      Output_File << " ";
    }
    count++;
    
    if (count==4) {
        Output_File << "\n";
        count=0;
    }
  }

  Output_File.close();

  std::cout<<"GMIO transactions finished"<<std::endl;

	my_graph.end();

  return 0;
}
