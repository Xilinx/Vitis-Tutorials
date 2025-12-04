//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include <fstream>
#include <iostream>
#include "radioml_top_graph.h"
#include "read_file.h"

class dut_graph : public graph {
public:
  radioml_top_graph   dut;
  input_gmio          data_iq;
  output_gmio         data_o;
  port<direction::in> weights_w1;
  port<direction::in> biases_w1;
  port<direction::in> weights_w3;
  port<direction::in> biases_w3;
  port<direction::in> weights_w5;
  port<direction::in> biases_w5;
  port<direction::in> weights_w7;
  port<direction::in> biases_w7;
  port<direction::in> weights_w9;
  port<direction::in> biases_w9;
  port<direction::in> weights_w11;
  port<direction::in> biases_w11;
  port<direction::in> weights_w13;
  port<direction::in> biases_w13;
  port<direction::in> weights_0_w16;
  port<direction::in> weights_1_w16;
  port<direction::in> biases_w16;
  port<direction::in> weights_w17;
  port<direction::in> biases_w17;
  port<direction::in> weights_w18;
  port<direction::in> biases_w18;

  dut_graph( void )
  {
    /* Setup the GMIO to connect the AIE to global memory */
    // logical name of the gmio, burst length 64, 128 or 256 bytes), required bandwidth in MB/s
    data_iq =  input_gmio::create("gmioIn",256,100); // Based on input bandwidth when simulating conv1d_w1 standalone
    connect<>(data_iq.out[0],dut.data_iq);
    data_o = output_gmio::create("gmioOut",256,10);  // Based on ratio of # input samples of conv1d_w1 to # output samples of dense_w18
    connect<>(dut.data_o,data_o.in[0]);

    // layer_w1 RTPs
    connect<>(weights_w1,dut.weights_w1);
    connect<>(biases_w1,dut.biases_w1);

    // layer_w3 RTPs
    connect<>(weights_w3,dut.weights_w3);
    connect<>(biases_w3,dut.biases_w3);
    
    // layer_w5 RTPs
    connect<>(weights_w5,dut.weights_w5);
    connect<>(biases_w5,dut.biases_w5);
    
    // layer_w7 RTPs
    connect<>(weights_w7,dut.weights_w7);
    connect<>(biases_w7,dut.biases_w7);
    
    // layer_w9 RTPs
    connect<>(weights_w9,dut.weights_w9);
    connect<>(biases_w9,dut.biases_w9);
    
    // layer_w11 RTPs
    connect<>(weights_w11,dut.weights_w11);
    connect<>(biases_w11,dut.biases_w11);
    
    // layer_w13 RTPs
    connect<>(weights_w13,dut.weights_w13);
    connect<>(biases_w13,dut.biases_w13);
    
    // layer_w16 RTPs
    connect<>(weights_0_w16,dut.weights_0_w16);
    connect<>(weights_1_w16,dut.weights_1_w16);
    connect<>(biases_w16,dut.biases_w16);
    
    // layer_w17 RTPs
    connect<>(weights_w17,dut.weights_w17);
    connect<>(biases_w17,dut.biases_w17);
    
    // layer_w18 RTPs
    connect<>(weights_w18,dut.weights_w18);
    connect<>(biases_w18,dut.biases_w18);
  }
};

// Instantiate AIE graph:
dut_graph aie_dut;

static constexpr unsigned  ITERATIONS = 8;
static constexpr unsigned  NSAMP_I = 1024*2;
static constexpr unsigned  NSAMP_O = 24;
static constexpr unsigned  BYTES_PER_SAMPLE = 2;
static constexpr unsigned  SIG_I_BYTES  = NSAMP_I    * BYTES_PER_SAMPLE * ITERATIONS;
static constexpr unsigned  SIG_O_BYTES  = NSAMP_O    * BYTES_PER_SAMPLE * ITERATIONS;

bfloat16 data_iq_array[NSAMP_I*ITERATIONS];
bfloat16 weights_w1_array[aie_dut.dut.layer_w1.NWEIGHTS];
bfloat16 biases_w1_array[aie_dut.dut.layer_w1.NBIASES];
bfloat16 weights_w3_array[aie_dut.dut.layer_w3.NWEIGHTS];
bfloat16 biases_w3_array[aie_dut.dut.layer_w3.NBIASES];
bfloat16 weights_w5_array[aie_dut.dut.layer_w5.NWEIGHTS];
bfloat16 biases_w5_array[aie_dut.dut.layer_w5.NBIASES];
bfloat16 weights_w7_array[aie_dut.dut.layer_w7.NWEIGHTS];
bfloat16 biases_w7_array[aie_dut.dut.layer_w7.NBIASES];
bfloat16 weights_w9_array[aie_dut.dut.layer_w9.NWEIGHTS];
bfloat16 biases_w9_array[aie_dut.dut.layer_w9.NBIASES];
bfloat16 weights_w11_array[aie_dut.dut.layer_w11.NWEIGHTS];
bfloat16 biases_w11_array[aie_dut.dut.layer_w11.NBIASES];
bfloat16 weights_w13_array[aie_dut.dut.layer_w13.NWEIGHTS];
bfloat16 biases_w13_array[aie_dut.dut.layer_w13.NBIASES];
bfloat16 weights_0_w16_array[aie_dut.dut.layer_w16.NWEIGHTS/2];
bfloat16 weights_1_w16_array[aie_dut.dut.layer_w16.NWEIGHTS/2];
bfloat16 biases_w16_array[aie_dut.dut.layer_w16.NBIASES];
bfloat16 weights_w17_array[aie_dut.dut.layer_w17.NWEIGHTS];
bfloat16 biases_w17_array[aie_dut.dut.layer_w17.NBIASES];
bfloat16 weights_w18_array[aie_dut.dut.layer_w18.NWEIGHTS];
bfloat16 biases_w18_array[aie_dut.dut.layer_w18.NBIASES];

#if defined(__AIESIM__) || defined(__X86SIM__)
// Initialize and run the graph:
int main(void)
{
  std::ofstream output_File("data_o_sim.txt");

  aie_dut.init();

  // Read input data
  read_file<bfloat16,NSAMP_I*ITERATIONS>("data/data_iq.txt",data_iq_array);
  // Read layer_w1 RTPs
  read_file<bfloat16,aie_dut.dut.layer_w1.NWEIGHTS>("data/w1_weights-rtp.txt",weights_w1_array);
  read_file<bfloat16,aie_dut.dut.layer_w1.NBIASES> ("data/w1_biases-rtp.txt",  biases_w1_array);
  // Read layer_w3 RTPs
  read_file<bfloat16,aie_dut.dut.layer_w3.NWEIGHTS>("data/w3_weights-rtp.txt",weights_w3_array);
  read_file<bfloat16,aie_dut.dut.layer_w3.NBIASES> ("data/w3_biases-rtp.txt",  biases_w3_array);
  // Read layer_w5 RTPs
  read_file<bfloat16,aie_dut.dut.layer_w5.NWEIGHTS>("data/w5_weights-rtp.txt",weights_w5_array);
  read_file<bfloat16,aie_dut.dut.layer_w5.NBIASES> ("data/w5_biases-rtp.txt",  biases_w5_array);
  // Read layer_w7 RTPs
  read_file<bfloat16,aie_dut.dut.layer_w7.NWEIGHTS>("data/w7_weights-rtp.txt",weights_w7_array);
  read_file<bfloat16,aie_dut.dut.layer_w7.NBIASES> ("data/w7_biases-rtp.txt",  biases_w7_array);
  // Read layer_w9 RTPs
  read_file<bfloat16,aie_dut.dut.layer_w9.NWEIGHTS>("data/w9_weights-rtp.txt",weights_w9_array);
  read_file<bfloat16,aie_dut.dut.layer_w9.NBIASES> ("data/w9_biases-rtp.txt",  biases_w9_array);
  // Read layer_w11 RTPs
  read_file<bfloat16,aie_dut.dut.layer_w11.NWEIGHTS>("data/w11_weights-rtp.txt",weights_w11_array);
  read_file<bfloat16,aie_dut.dut.layer_w11.NBIASES> ("data/w11_biases-rtp.txt",  biases_w11_array);
  // Read layer_w13 RTPs
  read_file<bfloat16,aie_dut.dut.layer_w13.NWEIGHTS>("data/w13_weights-rtp.txt",weights_w13_array);
  read_file<bfloat16,aie_dut.dut.layer_w13.NBIASES> ("data/w13_biases-rtp.txt",  biases_w13_array);
  // Read layer_w16 RTPs
  read_file<bfloat16,aie_dut.dut.layer_w16.NWEIGHTS/2>("data/w16_0_weights-rtp.txt",weights_0_w16_array);
  read_file<bfloat16,aie_dut.dut.layer_w16.NWEIGHTS/2>("data/w16_1_weights-rtp.txt",weights_1_w16_array);
  read_file<bfloat16,aie_dut.dut.layer_w16.NBIASES> ("data/w16_biases-rtp.txt",  biases_w16_array);
  // Read layer_w17 RTPs
  read_file<bfloat16,aie_dut.dut.layer_w17.NWEIGHTS>("data/w17_weights-rtp.txt",weights_w17_array);
  read_file<bfloat16,aie_dut.dut.layer_w17.NBIASES> ("data/w17_biases-rtp.txt",  biases_w17_array);
  // Read layer_w18 RTPs
  read_file<bfloat16,aie_dut.dut.layer_w18.NWEIGHTS>("data/w18_weights-rtp.txt",weights_w18_array);
  read_file<bfloat16,aie_dut.dut.layer_w18.NBIASES> ("data/w18_biases-rtp.txt",  biases_w18_array);

  aie_dut.update(aie_dut.weights_w1,weights_w1_array,aie_dut.dut.layer_w1.NWEIGHTS);
  aie_dut.update(aie_dut.biases_w1,biases_w1_array,aie_dut.dut.layer_w1.NBIASES);
  aie_dut.update(aie_dut.weights_w3,weights_w3_array,aie_dut.dut.layer_w3.NWEIGHTS);
  aie_dut.update(aie_dut.biases_w3,biases_w3_array,aie_dut.dut.layer_w3.NBIASES);
  aie_dut.update(aie_dut.weights_w5,weights_w5_array,aie_dut.dut.layer_w5.NWEIGHTS);
  aie_dut.update(aie_dut.biases_w5,biases_w5_array,aie_dut.dut.layer_w5.NBIASES);
  aie_dut.update(aie_dut.weights_w7,weights_w7_array,aie_dut.dut.layer_w7.NWEIGHTS);
  aie_dut.update(aie_dut.biases_w7,biases_w7_array,aie_dut.dut.layer_w7.NBIASES);
  aie_dut.update(aie_dut.weights_w9,weights_w9_array,aie_dut.dut.layer_w9.NWEIGHTS);
  aie_dut.update(aie_dut.biases_w9,biases_w9_array,aie_dut.dut.layer_w9.NBIASES);
  aie_dut.update(aie_dut.weights_w11,weights_w11_array,aie_dut.dut.layer_w11.NWEIGHTS);
  aie_dut.update(aie_dut.biases_w11,biases_w11_array,aie_dut.dut.layer_w11.NBIASES);
  aie_dut.update(aie_dut.weights_w13,weights_w13_array,aie_dut.dut.layer_w13.NWEIGHTS);
  aie_dut.update(aie_dut.biases_w13,biases_w13_array,aie_dut.dut.layer_w13.NBIASES);
  aie_dut.update(aie_dut.weights_0_w16,weights_0_w16_array,aie_dut.dut.layer_w16.NWEIGHTS/2);
  aie_dut.update(aie_dut.weights_1_w16,weights_1_w16_array,aie_dut.dut.layer_w16.NWEIGHTS/2);
  aie_dut.update(aie_dut.biases_w16,biases_w16_array,aie_dut.dut.layer_w16.NBIASES);
  aie_dut.update(aie_dut.weights_w17,weights_w17_array,aie_dut.dut.layer_w17.NWEIGHTS);
  aie_dut.update(aie_dut.biases_w17,biases_w17_array,aie_dut.dut.layer_w17.NBIASES);
  aie_dut.update(aie_dut.weights_w18,weights_w18_array,aie_dut.dut.layer_w18.NWEIGHTS);
  aie_dut.update(aie_dut.biases_w18,biases_w18_array,aie_dut.dut.layer_w18.NBIASES);
  
  bfloat16* sig_iq_array=(bfloat16*)GMIO::malloc(SIG_I_BYTES);
  bfloat16* sig_o_array =(bfloat16*)GMIO::malloc(SIG_O_BYTES);

  /* Copy the input samples to the buffer */
  memcpy(sig_iq_array, data_iq_array, SIG_I_BYTES);
  aie_dut.data_iq.gm2aie_nb(sig_iq_array, SIG_I_BYTES);

  aie_dut.run(ITERATIONS);

  aie_dut.data_o.aie2gm_nb(sig_o_array,  SIG_O_BYTES);
  aie_dut.data_o.wait();

  /* Post-processing */
  int count=0;
  for(int i=0; i<NSAMP_O*ITERATIONS; i++)
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
  GMIO::free(sig_iq_array);
  GMIO::free(sig_o_array);

  aie_dut.end();

  return 0;
}
#endif
