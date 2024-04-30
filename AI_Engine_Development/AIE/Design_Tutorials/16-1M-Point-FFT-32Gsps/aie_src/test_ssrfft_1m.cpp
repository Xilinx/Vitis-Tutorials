//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "test_ssrfft_1m.h"
#include "test_cfg.h"

dut dut;

int main(void) {
	dut.init();
    dut.run(NUM_ITERATION) ;
	dut.end() ;
    return 0 ;
}
