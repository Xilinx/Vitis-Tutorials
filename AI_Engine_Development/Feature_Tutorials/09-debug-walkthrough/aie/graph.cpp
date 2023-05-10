/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "graph.h"

#define COL_OFFSET 6

#define UL_INPUT_BA  (4+32)
#define UL_OUTPUT_BA (8)

TEST_BF<COL_OFFSET> dut;

#if defined  (__AIESIM__) || defined(__X86SIM__)
int main(void) {
    dut.init();
    dut.run(100) ;
    dut.end() ;
    return 0 ;
}
#endif
