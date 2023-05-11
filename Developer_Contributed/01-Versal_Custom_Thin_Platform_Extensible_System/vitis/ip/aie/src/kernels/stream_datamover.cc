//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//

#include <adf.h>
#include "coeffs.h"

// This example uses vector register as a circular buffer to pass data using
// stream input and outputs.
// To align with 128 bit vector register, 4 samples are read every 4th cycle
// and 4 samples are written every 4th cycle.
// To avoid read/write access conflicts to vector register, the update of
// each 256 bit vector is one phase (clock cycle) ahead of the extract from
// the vector register.
// The loop is infinite to avoid function call overhead once started

void stream_datamover_init()
{
  // Drop samples before starting kernel to start fill fifos.
  // This is usually needed when using broadcast to several kernels.
  int const smpl_drop = 4;
  for (int i = 0; i < smpl_drop; i++)
  {
    // Drop only on stream port 0
    get_ss(0);
  }
}

// temporary block inline to allow vitis_analyzer to display data
//void stream_datamover
void __attribute__ ((noinline)) stream_datamover
(
 input_stream_cint16 * restrict stream_in,
 output_stream_cint16 * restrict stream_out
)
{

  // Input data temp variable
  v16cint16 inp = null_v16cint16();

  while (true)
  chess_prepare_for_pipelining
  {
    // Note that reading 4 samples from stream takes 4 clock cycles
    // In this example we just pass the data, so there will be
    // Nops inserted to fill the gap of 3 clock cycles.

    // phase 0
    inp = upd_v(inp, 1, readincr_v4(stream_in));
    writeincr_v4(stream_out, ext_v(inp, 0));

    // phase 1
    inp = upd_v(inp, 2, readincr_v4(stream_in));
    writeincr_v4(stream_out, ext_v(inp, 1));

    // phase 2
    inp = upd_v(inp, 3, readincr_v4(stream_in));
    writeincr_v4(stream_out, ext_v(inp, 2));

    // phase 3
    inp = upd_v(inp, 0, readincr_v4(stream_in));
    writeincr_v4(stream_out, ext_v(inp, 3));

  }

}


