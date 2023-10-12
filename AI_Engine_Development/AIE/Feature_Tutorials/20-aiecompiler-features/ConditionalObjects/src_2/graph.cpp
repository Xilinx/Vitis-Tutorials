// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "graph.h"

// Contains Sub0 with std::array< sub-graph, 2>
MyGraph<1> g1;

// Contains Sub1 with std::array< sub-graph, 3>
MyGraph<2> g2;

int main(int argc, char ** argv)
{
    g1.init();
    g1.run(1);
    g1.end();

    g2.init();
    g2.run(2);
    g2.end();
    return 0;
}
