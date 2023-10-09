// 
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
// 

#include "ddc_chain.h"

#define COL_OFFSET_DDC 10

using namespace adf;


TEST_32ant<COL_OFFSET_DDC> ddc_inst;


int main(int argc, char ** argv) {

    ddc_inst.init();
    ddc_inst.run(100);
    ddc_inst.end();

    return 0;
}
