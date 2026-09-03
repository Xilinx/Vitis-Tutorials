//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved. Portions of this file consist of AI-generated content.
// SPDX-License-Identifier: MIT
//
// Author: Chandra Sekar Balakrishnan

#include "graph.h"

LinearEquSolverGraph linearEquSolver;

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char** argv) {
    linearEquSolver.init();
    linearEquSolver.run(4);
    linearEquSolver.end();
    return 0;
}
#endif
