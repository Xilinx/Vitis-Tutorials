//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include <fstream>
#include <iostream>
#include "max_pool1d_graph_template.h"
#include "read_file.h"

class dut_graph : public graph {
  max_pool1d_graph_template<512,64> dut;           // NSAMP=512, NNODES=64
public:
  input_gmio   data_i;
  output_gmio  data_o;

  dut_graph( void )
  {
    /* Setup the GMIO to connect the AIE to global memory */
    // logical name of the gmio, burst length 64, 128 or 256 bytes), required bandwidth in MB/s
    data_i =  input_gmio::create("gmioIn",256,5000);
    connect<>(data_i.out[0],dut.data_i);

    data_o = output_gmio::create("gmioOut",256,2500);
    connect<>(dut.data_o,data_o.in[0]);
  }
};

// Instantiate AIE graph:
dut_graph aie_dut;

static constexpr unsigned  ITERATIONS = 4;
static constexpr unsigned  SAMPLES_PER_ITERATION = 512*64;
static constexpr unsigned  BYTES_PER_SAMPLE = 2;
static constexpr unsigned  SIG_I_BYTES  = SAMPLES_PER_ITERATION   * BYTES_PER_SAMPLE * ITERATIONS;
static constexpr unsigned  SIG_O_BYTES  = SAMPLES_PER_ITERATION/2 * BYTES_PER_SAMPLE * ITERATIONS;

bfloat16 data_i_array[SAMPLES_PER_ITERATION*ITERATIONS];

// Initialize and run the graph:
int main(void)
{
  std::ofstream output_File("sig_o_sim.txt");

  
  aie_dut.init();

  // Read input data
  read_file<bfloat16,SAMPLES_PER_ITERATION*ITERATIONS>("data/data_i.txt",data_i_array);

  bfloat16* sig_i_array=(bfloat16*)GMIO::malloc(SIG_I_BYTES);
  bfloat16* sig_o_array=(bfloat16*)GMIO::malloc(SIG_O_BYTES);

  /* Copy the samples to the buffer */
  memcpy(sig_i_array, data_i_array, SIG_I_BYTES);
  
  aie_dut.data_i.gm2aie_nb(sig_i_array, SIG_I_BYTES);

  aie_dut.run(ITERATIONS);

  aie_dut.data_o.aie2gm_nb(sig_o_array,  SIG_O_BYTES);
  aie_dut.data_o.wait();
  
  /* Post-processing */
  int count=0;
  for(int i=0; i<SAMPLES_PER_ITERATION/2*ITERATIONS; i++)
  {
    output_File << sig_o_array[i];
    output_File << " ";
    count++;
    if (count==4) {
        output_File << "\n";
        count=0;
    }
  }
  output_File.close();

  /* Free the allocated memory*/
  GMIO::free(sig_i_array);
  GMIO::free(sig_o_array);

  aie_dut.end();

  return 0;
}
