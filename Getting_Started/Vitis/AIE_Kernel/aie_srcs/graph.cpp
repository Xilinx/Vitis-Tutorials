/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "graph.h"

Simple_Graph mygraph;

int main(void) {	
	mygraph.init();
	mygraph.run(1);
	mygraph.end();
    return 0;
}
