/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/



#include "graph.h"

Lab8 clipgraph;

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char ** argv) {
	clipgraph.init();
    clipgraph.run(4);
    clipgraph.end();
    return 0;
}
#endif
