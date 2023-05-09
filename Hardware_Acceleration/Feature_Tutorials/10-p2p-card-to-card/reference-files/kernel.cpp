/**
* Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: X11
*/

/*******************************************************************************
Description:
    In this Example, a vector addition implementation is done
*******************************************************************************/

extern "C" {
/*
    Vector Addition Kernel Implementation using dataflow
    Arguments:
        in   (input)  --> Input Vector
        out  (output) --> Output Vector
        inc  (input)  --> Increment
        size (input)  --> Size of Vector in Integer
   */
void increment(unsigned int* in, unsigned int* out, int inc, int size) {
// Auto-pipeline is going to apply pipeline to this loop
incr:
    for (int i = 0; i < size; i++) {
        out[i] = in[i] + inc;
    }
}
}
