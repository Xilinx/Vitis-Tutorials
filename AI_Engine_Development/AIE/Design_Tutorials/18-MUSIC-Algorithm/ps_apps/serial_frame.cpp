//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Vlad Druz
//

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "serial_frame.hpp"

bool serial_frame::is_complete(uint32_t frame_length)
{
    size_t header_len = sizeof(serial_frame);

    return ((start_of_frame == SERIAL_FRAME_SOF) &&
            (frame_length >= header_len + payload_len()));
}

bool serial_frame::is_valid()
{
    return start_of_frame == SERIAL_FRAME_SOF;
}

size_t serial_frame::payload_len()
{
    return num_samples_per_antenna * num_antenna_elems *
        sizeof(float) * 2; // complex float
}

void serial_frame::set_payload_len(uint32_t n_payload_bytes)
{
    num_samples_per_antenna = (float)n_payload_bytes /
        (sizeof(float) * 2) / num_antenna_elems;
}