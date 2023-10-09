/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "graph.h"

clipped clipgraph;

int main(int argc, char ** argv) {
    clipgraph.init();
    clipgraph.run(4);
    clipgraph.end();
    return 0;
}
