//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Vlad Druz
//

#include <stdint.h>
#include <stdbool.h>
#include <cstddef>

#define SERIAL_FRAME_SOF (0xFFC00000)

class serial_frame
{
public:
    uint32_t start_of_frame;
    float    sequence_num;
    float    snapshot_type;
    float    num_sources;
    float    num_antenna_elems;
    float    antenna_spacing;
    float    num_samples_per_antenna;
    float    noise_ss_selection;
    float    noise_floor;
    float    null_theshold;
    float    calc_avg_ns;
    float    reserved[5];
    uint8_t  payload[];

    bool is_valid();
    bool is_complete(uint32_t len);
    size_t payload_len();
    void set_payload_len(uint32_t n_payload_bytes);
};

