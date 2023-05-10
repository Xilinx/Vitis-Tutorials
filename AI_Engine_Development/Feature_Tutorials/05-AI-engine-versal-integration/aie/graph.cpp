/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "graph.h"

using namespace adf;

clipped clipgraph; //A graph object 'clipgraph' is declared

//This main() function runs only for AIESIM and X86Sim targets. 
//Emulation uses a different host code
#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char ** argv) {
	clipgraph.init(); //Loads the graph to the AI Engine Array
    clipgraph.run(4); //Starts the graph execution by enabling the processors.
    clipgraph.end(); //Wait for 4 iterations to finish
    return 0;
}
#endif
