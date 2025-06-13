//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//

#define NUM_COEFFS    8
#define MOVER_SHIFT   15

int16_t chess_storage(%chess_alignof(v8cint16)) delay_vector [NUM_COEFFS] =
        {0, 0, 0, 32767, 0, 0, 0, 0};

int16_t chess_storage(%chess_alignof(v8cint16)) one_vector [NUM_COEFFS] =
        {32767, 0, 0, 0, 0, 0, 0, 0};

int16_t chess_storage(%chess_alignof(v8cint16)) scale_vector [NUM_COEFFS] =
        {12044, 0, 0, 0, 0, 0, 0, 0};
