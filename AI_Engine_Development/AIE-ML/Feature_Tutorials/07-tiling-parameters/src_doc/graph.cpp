// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "graph.h"

// TopGraph<1,1,1,1, 1,1,1,1, 1,1,1> TG;
TopGraph<0,0,0,0,0,0,0,1, 0,0,0> TG;

int main(int argc, char ** argv) {


    TG.init();

    TG.run(NIterations);

    TG.end();
    return 0;
}
