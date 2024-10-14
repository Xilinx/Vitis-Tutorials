//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Chandra Sekar Balakrishnan

#include <fstream>
#include "farrow_graph.h"
#include "input_sig_i.h"
#include "input_del_i_optimized.h"
#include "output_sig_o.h"

using namespace adf;

#if defined(__AIESIM__ ) || defined(__X86SIM__)

// Initialize and run the graph:
int main(void)
{
  int i;
  int ITERATION = 4;
  int SAMPLES_PER_ITERATION = 1024;
  int NUMBER_OF_BYTES = 4;
  /* Each iteration computes 1024 cint16 samples */
  const int BLOCK_SIZE_sig_in_Bytes = SAMPLES_PER_ITERATION * NUMBER_OF_BYTES * ITERATION;  // 1024 * 4  (cint16 - 32 bits - 4bytes) * ITERATION (4)
  const int BLOCK_SIZE_del_in_Bytes = SAMPLES_PER_ITERATION * NUMBER_OF_BYTES * ITERATION;  // 1024 * 4  (int32 - 32 bits)  * ITERATION (4)
  const int BLOCK_SIZE_out_Bytes =  SAMPLES_PER_ITERATION * NUMBER_OF_BYTES * ITERATION;    // 1024 * 4  (cint16 - 32 bits) * ITERATION (4)
  const int INPUT_SIZE = 2048 ;
  std::ofstream Output_File("sig_o_gen.txt");
  /* Initialize the Farrow filter */
  aie_dut.init();

  /* Pre-processing */
  short int* sig_i_Array=(short int*)GMIO::malloc(BLOCK_SIZE_sig_in_Bytes);
  int32* del_i_Array=(int32*)GMIO::malloc(BLOCK_SIZE_del_in_Bytes);
  short int* sig_o_Array=(short int*)GMIO::malloc(BLOCK_SIZE_out_Bytes);

  /* Copy the samples to the buffer */
  memcpy(sig_i_Array, input_sig_i, BLOCK_SIZE_sig_in_Bytes);
  
  /* Copy the delay values to the buffer */
  memcpy(del_i_Array, input_del_i,BLOCK_SIZE_del_in_Bytes);

  aie_dut.sig_i.gm2aie_nb(sig_i_Array, BLOCK_SIZE_sig_in_Bytes);
  aie_dut.del_i.gm2aie_nb(del_i_Array, BLOCK_SIZE_del_in_Bytes);

  /* Execute the graph */
  aie_dut.run(4);
    
  aie_dut.sig_o.aie2gm(sig_o_Array,  BLOCK_SIZE_out_Bytes);

  /* Post-processing */
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

  /* Free thge allocated memory*/
  GMIO::free(sig_i_Array);
  GMIO::free(del_i_Array);
  GMIO::free(sig_o_Array);

  aie_dut.end();

  return 0;
}

#endif
