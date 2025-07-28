/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#include "graph.hpp"

theGraph g; // instantiate the ADF graph

int main() {

    g.init();               // initailize the graph
    g.run(iter_count);      // run the graph for the specified number of iterations
    g.end();                // end the graph

    return (0);

} // end main()