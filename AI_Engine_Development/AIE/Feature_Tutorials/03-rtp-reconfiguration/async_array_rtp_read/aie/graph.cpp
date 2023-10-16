/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <adf.h>
#include "graph.h"
#include <unistd.h>

using namespace adf;

rtpgraph gr;

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char ** argv) { 
	const int TOTAL=40;
	int value[256];
	for(int i=0;i<256;i++){
		value[i]=i;
	}
	gr.init();
	gr.run(TOTAL);

	gr.update(gr.value,value,256);
	wait(3,SC_US);
	int current_iter=-1;
	gr.read(gr.iter,current_iter);
	std::cout<<"Current Iteration="<<current_iter<<std::endl;
	gr.end();
	return 0;
};
#endif
