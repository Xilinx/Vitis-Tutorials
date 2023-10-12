// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#ifndef BUFFER_vs_STREAM
#error "BUFFER_vs_STREAM should be 1 or 2"
#endif

#if BUFFER_vs_STREAM == 1

#include "graph_iobuf.h"
MultiRateGraph_Buffer<GV> G;

#elif BUFFER_vs_STREAM == 2

#include "graph_stream.h"
MultiRateGraph_Stream<GV> G;

#endif


int main(int argc, char ** argv) {


    G.init();

    G.run(NIterations);

    G.end();
    return 0;
}
