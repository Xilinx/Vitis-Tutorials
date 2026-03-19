/*
 * Copyright (C) 2023-2025, Advanced Micro Devices, Inc. All rights reserved.
 * SPDX-License-Identifier: MIT
 *
 * Top-Level AIE Graph for Dummy Kernel Test
 * 
 * This file defines the top-level graph that connects the dummy kernel
 * to the PLIO interface for data output. This graph is used for both
 * simulation and hardware implementation.
 */

#pragma once

#include "graph_dummy.h"

using namespace adf;

/**
 * @brief Top-level test graph for dummy kernel
 * 
 * This graph instantiates:
 * - One dummy kernel at tile (12, 0)
 * - One output PLIO interface for 32-bit data streaming
 * 
 * Graph Structure:
 *   [dummy_kernel @ tile(12,0)] --> [output PLIO: dout]
 * 
 * The output PLIO writes to "data/out.txt" with 32-bit width and
 * 500 samples per second throughput.
 */
class test_dummy: public graph {
public:
    output_plio dout;                    ///< Output PLIO interface
    
    dummy_kernel_graph<12, 0> k0;        ///< Dummy kernel instance at tile (12, 0)

    /**
     * @brief Constructor - Initializes the test graph
     * 
     * Creates:
     * - Output PLIO with 32-bit width, 500 samples/sec
     * - Connects kernel output to PLIO input
     */
    test_dummy() {
        // Create output PLIO interface
        // Parameters: name, data width, output file, samples per second
        dout = adf::output_plio::create("dout", adf::plio_32_bits, "data/out.txt", 500);
        
        // Connect kernel output to PLIO input
        adf::connect(k0.out, dout.in[0]);
    }
};
