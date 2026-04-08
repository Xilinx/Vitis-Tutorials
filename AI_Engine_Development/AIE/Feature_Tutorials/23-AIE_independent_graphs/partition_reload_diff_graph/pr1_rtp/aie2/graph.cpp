/*
Copyright (C) Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <adf.h>
#include "graph.h"
#include <unistd.h>

using namespace adf;

rtpgraph gr;

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char ** argv) { 

	int value[16];
	for(int i=0;i<16;i++){
		value[i]=i;
	}
	gr.init();
	gr.run(4);

	gr.update(gr.value,value,16);

	gr.end();
	return 0;
};
#endif
