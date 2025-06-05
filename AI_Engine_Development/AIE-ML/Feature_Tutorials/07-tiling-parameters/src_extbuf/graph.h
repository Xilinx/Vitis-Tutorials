// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include <vector>
#include "kernels.h"

// Matrix Transpose
template <int COL>
class Graph1 : public adf::graph
{

private:
public:
    adf::kernel k1;

    adf::external_buffer<int32> ddrin, ddrout;
    
    
    Graph1()
    {

        ddrin = adf::external_buffer<int32>::create({Dim0, Dim1, Dim2 * Dim3}, 0, 1);
        ddrout = adf::external_buffer<int32>::create({Dim1, Dim0, Dim2 * Dim3}, 1, 0);

        adf::read_access(ddrin.out[0]) = adf::tiling(
                {.buffer_dimension = {Dim0, Dim1, Dim2 * Dim3},
                 .tiling_dimension = {Dim0, Dim1, 1},
                 .offset = {0, 0, 0},
                 .tile_traversal = {{.dimension = 2, .stride = 1, .wrap = Dim2*Dim3}}});

        adf::write_access(ddrout.in[0]) = adf::tiling(
            {.buffer_dimension = {Dim1, Dim0, Dim2 * Dim3},
             .tiling_dimension = {1, Dim0, 1},
             .offset = {0, 0, 0},
             .tile_traversal = {
                 {.dimension = 0, .stride = 1, .wrap = Dim1},
                 {.dimension = 2, .stride = 1, .wrap = Dim2 * Dim3}}});

        k1 = adf::kernel::create(passthrough_buffer<Dim0 * Dim1, 8>); // 64 pixels per tile
        adf::source(k1) = "passthrough.cpp";
        adf::runtime<ratio>(k1) = 0.9;
        adf::location<adf::kernel>(k1) = adf::tile(COL, 0);

        // Connections
        adf::connect(ddrin.out[0], k1.in[0]);
        
        adf::connect(k1.out[0],ddrout.in[0]);
    };
};


