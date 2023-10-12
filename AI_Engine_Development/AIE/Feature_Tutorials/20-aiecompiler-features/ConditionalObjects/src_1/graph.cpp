// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "graph.h"

TestGraph1  tg1;
int main(int argc, char ** argv) {


    tg1.init();

    tg1.run(NITERATIONS);

    tg1.end();
    return 0;
}
