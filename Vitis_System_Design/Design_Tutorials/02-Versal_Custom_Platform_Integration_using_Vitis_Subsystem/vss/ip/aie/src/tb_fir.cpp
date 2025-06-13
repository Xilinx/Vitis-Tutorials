//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//
// Author Derek Hagen

#include <adf.h>

// Include to subgraphs to simulate here!
#include "dm_graph.h"
#include "fir_graph.h"
//#include "XmcRuntimeParamHelper.h"

#define STR_LEN 30
// VFS currently does not support passing template variables via --Xpreproc="-D<var_name>"
// Override the template variables as workaround
#define MY_BUF_SIZE 256
#define MY_MARGIN   16

// Testbench Graph section
template <int IN_SIZE, int IN_MARGIN, int NUM>
class mygraph : public adf::graph {

  private:

  public:
    adf::input_plio   in[NUM];
    adf::input_port   coeff[NUM];
    adf::output_plio  out[NUM];
    char plio_in_name[NUM][STR_LEN];
    char plio_out_name[NUM][STR_LEN];
    char in_filename[NUM][STR_LEN];
    char out_filename[NUM][STR_LEN];

    // Graph to test
    fir_graph<IN_SIZE, IN_MARGIN> my_fir_graph[NUM];

    mygraph()
    {
      // specify PLIO ports
      for(int i=0; i<NUM; i++) {
        sprintf(plio_in_name[i],"fir_sig_in%d",i);
        sprintf(plio_out_name[i],"fir_sig_out%d",i);
        sprintf(in_filename[i],"data/input%d.txt",i);
        sprintf(out_filename[i],"data/output%d.txt",i);
        in[i]  = adf::input_plio::create(plio_in_name[i], adf::plio_32_bits, in_filename[i], 250);
        out[i] = adf::output_plio::create(plio_out_name[i], adf::plio_32_bits, out_filename[i], 250);

        adf::connect (coeff[i], my_fir_graph[i].coeff);
        adf::connect (in[i].out[0], my_fir_graph[i].in);
        adf::connect (my_fir_graph[i].out, out[i].in[0]);
      }
    }
};

mygraph<MY_BUF_SIZE, MY_MARGIN, 2>  mygraph_top;

#if defined (__AIESIM__) || (__X86SIM__)
// initialize and run the dataflow graph
int main(void) 
{

  int16 coe0[16] {
    0, 0, 0, 0, 16384, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
  int16 coe1[16] {
    0, 0, 0, 0, 16384, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
   // {-122,  674,  -2370,  10006,  16384,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//  if (!XMCRuntimeParamHelper<int16>::getInput("./data/coe.txt", coe, 16)) {
//    std::cerr << "main(): Error reading input data file " << "./data/coe.txt" << std::endl;
//  }
  mygraph_top.init();
// failing code   mygraph_top.update(mygraph_top[0].coeff, coe0, 16);
// failing code   mygraph_top.update(mygraph_top[1].coeff, coe1, 16);
/* working code */  mygraph_top.update(mygraph_top.coeff[0], coe0, 16);
/* working code */  mygraph_top.update(mygraph_top.coeff[1], coe1, 16);
  mygraph_top.run();
  mygraph_top.end();
  return 0;
}
#endif


