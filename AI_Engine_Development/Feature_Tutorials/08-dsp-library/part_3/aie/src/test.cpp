/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "fir_graph.h"

TopGraph filter ;

int main(void) {
    filter.init() ;
    filter.run(NUM_ITER) ;
    filter.end() ;
    return 0 ;
}
