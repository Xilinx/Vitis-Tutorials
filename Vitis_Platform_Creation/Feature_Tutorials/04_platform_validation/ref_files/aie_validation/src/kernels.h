/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef KERNEL_H

#include <adf.h>

static const int is_rtp = 1;  //1- for Enable 0-for Disable

void loopback(input_window_int32 * , output_window_int32 * , int32 myRtp);

#endif
