/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "graph.h"

using namespace adf;

//clipped clipgraph; //A graph object 'clipgraph' is declared

clipped<1> clipgraph1;
clipped<2> clipgraph2;
 

//This main() function runs only for AIESIM and X86Sim targets. 
//Emulation uses a different host code
#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char ** argv) {
    clipgraph1.init(); //Loads the graph to the AI Engine Array
    clipgraph2.init(); //Loads the graph to the AI Engine Array
    clipgraph1.run(4); //Starts the graph execution by enabling the processors.
    clipgraph2.run(4); //Starts the graph execution by enabling the processors.
    clipgraph1.wait();
    clipgraph2.wait();
    clipgraph1.end(); //Wait for 4 iterations to finish
    clipgraph2.end(); //Wait for 4 iterations to finish

    return 0;
}
#endif
