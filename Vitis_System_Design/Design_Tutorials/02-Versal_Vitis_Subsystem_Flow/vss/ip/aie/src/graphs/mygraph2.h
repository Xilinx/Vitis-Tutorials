//
// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//
// Author Derek Hagen

#include <adf.h>
#include "dm_graph.h"
#include "fir_graph.h"

// skip namespace show explicit adf syntax//using namespace adf;

#define STR_LEN 30

template <int IN_SIZE, int IN_MARGIN, int NUM>
class mygraph : public adf::graph {

  private:

  public:
    adf::input_plio   in[NUM+2];
    adf::input_port   coeff[2];
    adf::output_plio  out[NUM+2];
    char plio_in_name[NUM+2][STR_LEN];
    char plio_out_name[NUM+2][STR_LEN];
    char in_filename[NUM+2][STR_LEN];
    char out_filename[NUM+2][STR_LEN];

    // Add NUM data mover examples and two 16-Tap FIR filter
    dm_graph<IN_SIZE, NUM>        my_dm_graph;
    fir_graph<IN_SIZE, IN_MARGIN> my_fir_graph[2];

    mygraph()
    {
      // specify PLIO ports and connect datamovers
      for(int i=0; i<NUM; i++) {
        sprintf(plio_in_name[i],"dm_in%d",i);
        sprintf(plio_out_name[i],"dm_out%d",i);
        sprintf(out_filename[i],"data/output_dm%d.txt",i);
        in[i]  = adf::input_plio::create(plio_in_name[i], adf::plio_64_bits, "data/input_dm.txt", 500);
        out[i] = adf::output_plio::create(plio_out_name[i], adf::plio_64_bits, out_filename[i], 500);
        adf::connect (in[i].out[0], my_dm_graph.in[i]);
        adf::connect (my_dm_graph.out[i], out[i].in[0]);
      }

      for(int i=0; i<2; i++) {
        sprintf(plio_in_name[i+NUM],"fir_sig_in%d",i);
        sprintf(plio_out_name[i+NUM],"fir_sig_out%d",i);
        sprintf(in_filename[i+NUM],"data/input%d.txt",i);
        sprintf(out_filename[i+NUM],"data/output%d.txt",i);
        in[i+NUM]  = adf::input_plio::create(plio_in_name[i+NUM], adf::plio_32_bits, in_filename[i+NUM], 250);
        out[i+NUM] = adf::output_plio::create(plio_out_name[i+NUM], adf::plio_32_bits, out_filename[i+NUM], 250);

        adf::connect (coeff[i], my_fir_graph[i].coeff);
        adf::connect (in[i+NUM].out[0], my_fir_graph[i].in);
        adf::connect (my_fir_graph[i].out, out[i+NUM].in[0]);
      }
    }
};


