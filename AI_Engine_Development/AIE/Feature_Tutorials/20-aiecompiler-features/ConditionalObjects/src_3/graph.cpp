// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "graph.h"

// Contains Sub0
MyGraph<1> g1;

// Contains Sub1
MyGraph<2> g2;

// Contains chain of Sub0 and Sub1
MyGraph<3> g3;

int main(int argc, char ** argv)
{
	g1.init(); 	g2.init(); 	g3.init();
    g1.run(4);	g2.run(4);	g3.run(4);
    g1.end();   g2.end();   g3.end();



    return 0;
}
