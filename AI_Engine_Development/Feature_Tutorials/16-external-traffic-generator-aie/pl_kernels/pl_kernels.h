/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/


#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

#define POLAR_CLIP_INPUT_SAMPLES 1024
#define PLIOWIDTH 32
typedef qdma_axis<PLIOWIDTH, 0, 0, 0> pliopkt;

extern "C" {
inline void process_polar_clip(int &in_sample, int &out_sample);
void polar_clip(hls::stream<pliopkt>& sin,hls::stream<pliopkt>& sout);
}
