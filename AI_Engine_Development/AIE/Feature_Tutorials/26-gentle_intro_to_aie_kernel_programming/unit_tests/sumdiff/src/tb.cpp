/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#include "graph.hpp"

theGraph g; // instantiate the ADF graph

int main() {

    g.init();               // initailize the graph

    // run once in addtion mode
    g.update(g.mode, static_cast<int8>(0));     // mode = 0 : addition
    g.run(1);                                   // run once
    g.wait();                                   // wait for it to complete

    // run once in subtration mode
    g.update(g.mode, static_cast<int8>(1));     // mode != 0 : subtraction
    g.run(1);                                   // run once
    g.wait();                                   // wait for it to complete

    g.end();                // end the graph

    return (0);

} // end main()