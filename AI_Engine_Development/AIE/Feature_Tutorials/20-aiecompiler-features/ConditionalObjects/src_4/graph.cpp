// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "graph.h"
#include <fstream>

// Does not have RTPs.
MyGraph<false> g1;
// Doe have RTPs.
MyGraph<true> g2;

int main(int argc, char ** argv)
{
    g1.init();
    g1.run(2);
    g1.end();

    g2.init();
    g2.run(2);

    // RTP update through graph port
    g2.update(g2._sub._in1, 10);
    // RTP update through kernel port
    g2.update(g2._sub._k.in[1], 20);


    int32_t vals[2];
    // RTP read through graph port
    g2.read(g2._sub._inout0, vals[0]);
    // RTP read through kernel port
    g2.read(g2._sub._k.inout[0], vals[1]);

    g2.end();

    std::ofstream("data/o2.txt") << vals[0] << "\n" << vals[1] << "\n";

    return 0;
}
