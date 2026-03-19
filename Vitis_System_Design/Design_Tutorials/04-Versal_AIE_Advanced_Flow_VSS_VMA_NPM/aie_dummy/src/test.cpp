/*
 * Copyright (C) 2023-2026, Advanced Micro Devices, Inc. All rights reserved.
 * SPDX-License-Identifier: MIT
 *
 * AIE Graph Test Application
 * 
 * This is the main entry point for AIE graph simulation and testing.
 * It initializes, runs, and terminates the dummy kernel graph.
 * 
 * Compilation targets:
 * - __AIESIM__: AIE Simulator
 * - __X86SIM__: x86 Simulator
 */

#include "test.h"

using namespace std;
using namespace adf;

// Global graph instance
test_dummy dut;

#if defined(__AIESIM__) || defined(__X86SIM__)

/**
 * @brief Main function for AIE graph simulation
 * 
 * This function is only compiled for simulation targets (AIESIM or X86SIM).
 * It performs:
 * 1. Graph initialization
 * 2. Graph execution (1 iteration)
 * 3. Graph cleanup
 * 
 * @param argc Number of command-line arguments
 * @param argv Command-line arguments
 * @return 0 on success
 */
int main(int argc, char **argv) {
    // Initialize the graph (sets up AIE tiles, memory, etc.)
    dut.init();
    
    // Run the graph for 1 iteration
    dut.run(1);
    
    // Clean up and finalize
    dut.end();

    return 0;
}

#endif
