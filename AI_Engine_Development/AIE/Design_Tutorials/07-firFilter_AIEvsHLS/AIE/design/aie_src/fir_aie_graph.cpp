/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/


#include "fir_aie_graph.h"

using namespace adf;

FirGraph FilterChain;

#if defined(__AIESIM__) || defined(__NEW_X86Sim__)

int main(void) {
	FilterChain.init() ;
	FilterChain.run(ITER_CNT);  
	FilterChain.end() ;
    return 0 ;
}

#endif

