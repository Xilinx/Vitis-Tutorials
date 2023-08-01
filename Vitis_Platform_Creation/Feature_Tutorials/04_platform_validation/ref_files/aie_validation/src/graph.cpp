/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include "graph.h"
#include <fstream>

using namespace adf;

plio_graph pl_graph;
gmio_graph gm_graph;


#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char ** argv) { 

    int32* inputArray0 = (int32*)GMIO::malloc(32*sizeof(int32));
	int32* outputArray0 = (int32*)GMIO::malloc(32*sizeof(int32));

	for (int i=0; i<32; i++) {
		inputArray0[i] = i+1;
	}
    
	std::ofstream ofs0;
#ifndef __PTHREAD_API__
    ofs0.open("aiesimulator_output/data/output_gm0.txt", std::ofstream::out | std::ofstream::trunc);
#else
	system("mkdir x86simulator_output/data");
    ofs0.open("x86simulator_output/data/output_gm0.txt", std::ofstream::out | std::ofstream::trunc);
#endif

	pl_graph.init();
	gm_graph.init();
  
	pl_graph.run(1);
	gm_graph.run(1);
	if constexpr (is_rtp) {
		pl_graph.update (pl_graph.myRtp0, 1);
		gm_graph.update (gm_graph.myRtp0, 1);
	} else {
		pl_graph.update (pl_graph.myRtp0, 0);
		gm_graph.update (gm_graph.myRtp0, 0);
	}
  
    gm_graph.gm_in0.gm2aie_nb(&inputArray0[0], 32*sizeof(int32));
    gm_graph.gm_out0.aie2gm_nb(&outputArray0[0], 32*sizeof(int32));

    gm_graph.gm_out0.wait(); //assuming data from gm1 are processed by the graph and output to gm2
	
	for (int i=0; i<32; i++)
	{
		//std::cout<<outputArray0[i]<<", ";
		ofs0<<outputArray0[i]<<std::endl;
	}

    pl_graph.end();	
    gm_graph.end();	
	
	ofs0.close();    
    GMIO::free(inputArray0);
    GMIO::free(outputArray0);
	  
	return 0;
}
#endif
