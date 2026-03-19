/*
 * Copyright (C) 2023-2026, Advanced Micro Devices, Inc. All rights reserved.
 * SPDX-License-Identifier: MIT
 *
 * Dummy AIE Kernel
 * 
 * This kernel demonstrates a simple AI Engine kernel that generates a sequence
 * of integers and outputs them via AXI Stream interface. It serves as a
 * minimal example for VSS->VMA->NPM integration flow.
 * 
 * Features:
 * - Generates 65536 sequential integers (0 to 65535)
 * - Outputs data via AXI Stream interface
 * - Configures saturation and rounding modes
 */

#include "adf.h"
#include "aie_api/aie.hpp"

/**
 * @brief Initialize AIE tile configuration
 * 
 * This function is called once when the kernel is initialized.
 * It configures the AIE tile with:
 * - Saturation mode: enabled (values clamp at min/max)
 * - Rounding mode: symmetric infinity (rounds to nearest, ties to infinity)
 */
void dummy_kernel_init()
{
    aie::tile::current().set_saturation(aie::saturation_mode::saturate);
    aie::tile::current().set_rounding(aie::rounding_mode::symmetric_inf);
}

/**
 * @brief Dummy kernel main function
 * 
 * Generates a sequence of integers from 0 to 65535 and outputs them
 * via the AXI Stream interface.
 * 
 * @param out Pointer to output AXI Stream interface (int32)
 * 
 * Behavior:
 * - Writes 65536 values (0 to 65535) to the output stream
 * - Sets TLAST flag on the last sample (i == 65535)
 * - Uses writeincr() for efficient stream writes
 */
void dummy_kernel(
    output_stream_int32 *out
)
{
    // Generate and output 65536 sequential integers
    // TLAST is set to true only for the last sample (i == 65535)
    for(int i = 0; i < 65536; i++) {
        writeincr(out, i, i == 65535);
    }
}
