/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#ifndef _XF_GRAPH_SHORTESTPATH_KERNEL_HPP_
#define _XF_GRAPH_SHORTESTPATH_KERNEL_HPP_

#include "xf_graph_L2.hpp"
#include <ap_int.h>
#include <hls_stream.h>

// Vertex number
// Edge number
#ifdef HLS_TEST
#define V 2
#define E 2
#else
#define V 80000000
#define E 80000000
#endif

#define MAXOUTDEGREE (4096 * 10)

extern "C" void shortestPath_top(ap_uint<32>* config,
                                 ap_uint<512>* offset,
                                 ap_uint<512>* column,
                                 ap_uint<512>* weight,

                                 ap_uint<512>* ddrQue512,
                                 ap_uint<32>* ddrQue,

                                 ap_uint<512>* result512,
                                 ap_uint<32>* result,
                                 ap_uint<512>* pred512,
                                 ap_uint<32>* pred,
                                 ap_uint<8>* info);

#endif
