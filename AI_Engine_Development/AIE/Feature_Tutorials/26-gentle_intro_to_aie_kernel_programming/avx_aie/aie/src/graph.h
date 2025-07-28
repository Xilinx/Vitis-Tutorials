/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#include <adf.h>
#include "vadd_aie.h"

using namespace adf;

class TheGraph : public graph {
private:
    kernel k;                       // declare the kernel
    
public:
    TheGraph() {					// constructor
        k = kernel::create(vadd);	// associate the function with the kernel
        source(k) = "vadd_aie.cpp";	// define the source code for the kernel
        runtime<ratio>(k) = 0.1;    // define the runtime ration of the kernel
    }
    
}; // end class TheGraph
