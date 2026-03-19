/*
 * Copyright (C) 2023-2025, Advanced Micro Devices, Inc. All rights reserved.
 * SPDX-License-Identifier: MIT
 *
 * AIE Graph Definition for Dummy Kernel
 * 
 * This file defines the AIE graph structure that instantiates the dummy kernel
 * and connects it to the output PLIO interface. The graph is templated to allow
 * flexible placement of the kernel on different AIE tiles.
 */

#pragma once

#include <adf.h>
using namespace adf;

/**
 * @brief Forward declaration of dummy kernel function
 * @param out Output AXI Stream interface pointer
 */
void dummy_kernel(output_stream_int32 *out);

/**
 * @brief AIE Graph class for dummy kernel
 * 
 * This graph class encapsulates the dummy kernel and its connections.
 * It is templated to allow placement on any AIE tile.
 * 
 * @tparam xoff X-coordinate offset for kernel placement
 * @tparam yoff Y-coordinate offset for kernel placement
 * 
 * Graph Structure:
 *   [dummy_kernel] --> [output port]
 */
template <int xoff, int yoff>
class dummy_kernel_graph: public graph {
public:
    kernel       core;        ///< Kernel instance
    port<output> out;         ///< Output port for AXI Stream

    /**
     * @brief Constructor - Initializes the graph
     * 
     * Configures:
     * - Kernel creation and source file
     * - Runtime ratio (80% of tile capacity)
     * - Initialization function
     * - Kernel and stack placement
     * - Port connections
     */
    dummy_kernel_graph() {
        // Create kernel instance
        core = kernel::create(dummy_kernel);
        
        // Configure kernel runtime (80% of tile capacity)
        runtime<ratio>(core) = 0.8;
        
        // Set initialization function (called once at startup)
        initialization_function(core) = "dummy_kernel_init";
        
        // Specify kernel source file
        source(core) = "dummy_kernel.cpp";
        
        // Connect kernel output to graph output port
        connect(core.out[0], out);
        
        // Place kernel at specified tile coordinates
        location<kernel>(core) = tile(xoff, yoff);
        
        // Place stack at specified address (16384-2080 = 14256 bytes available)
        location<stack>(core) = address(xoff, yoff, 16384 - 2080);
    }
};
