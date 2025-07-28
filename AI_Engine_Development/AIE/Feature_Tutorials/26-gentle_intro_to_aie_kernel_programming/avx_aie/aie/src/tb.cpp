/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#include "graph.h"

TheGraph mygraph;

int main() {
    
    mygraph.init();		// initialize the graph
    mygraph.run(1);		// run the graph *once*
    mygraph.end();		// terminate the graph
    
    return (0);

} // end main()
