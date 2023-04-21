/*
 Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
 */

#include "diamond.h"

void diamond(data_t vecIn[N], data_t vecOut[N])
{
  data_t c1[N], c2[N], c3[N], c4[N];
#pragma HLS dataflow
  funcA(vecIn, c1, c2);
  funcB(c1, c3);
  funcC(c2, c4);
  funcD(c3, c4, vecOut);
}

void funcA(data_t *in, data_t *out1, data_t *out2)
{
Loop0:
  for (int i = 0; i < N; i++)
  {
#pragma HLS pipeline rewind
#pragma HLS unroll factor = 2
    data_t t = in[i] * 3;
    out1[i] = t;
    out2[i] = t;
  }
}

void funcB(data_t *in, data_t *out)
{
Loop0:
  for (int i = 0; i < N; i++)
  {
#pragma HLS pipeline rewind
#pragma HLS unroll factor = 2
    out[i] = in[i] + 25;
  }
}

void funcC(data_t *in, data_t *out)
{
Loop0:
  for (data_t i = 0; i < N; i++)
  {
#pragma HLS pipeline rewind
#pragma HLS unroll factor = 2
    out[i] = in[i] * 2;
  }
}

void funcD(data_t *in1, data_t *in2, data_t *out)
{
Loop0:
  for (int i = 0; i < N; i++)
  {
#pragma HLS pipeline rewind
#pragma HLS unroll factor = 2
    out[i] = in1[i] + in2[i] * 2;
  }
}
