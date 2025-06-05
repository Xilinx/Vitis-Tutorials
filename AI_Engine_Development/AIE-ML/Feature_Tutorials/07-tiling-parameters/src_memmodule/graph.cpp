// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "graph.h"

TopGraph<TC1,TC2,TC3,TC4> TG;

int main(int argc, char ** argv) {


    TG.init();

    TG.run(NIterations);

    TG.end();
    return 0;
}
