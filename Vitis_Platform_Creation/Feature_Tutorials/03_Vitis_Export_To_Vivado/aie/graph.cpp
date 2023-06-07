/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "graph.h"

clipped clipgraph;

int main(int argc, char ** argv) {
    clipgraph.init();
    clipgraph.run(4);	// Note: The number of iterations can be changed if needed based on the input
    clipgraph.end();
    return 0;
}
