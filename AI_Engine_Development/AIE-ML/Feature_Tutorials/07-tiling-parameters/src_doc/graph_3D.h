// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

// 3D Linear Transferring From Source to Destination
template <int COL>
class Graph1_3D : public adf::graph
{

private:

public:
    adf::kernel k1;
    adf::port<input> din;
    adf::port<output> dout[1];
    
    adf::shared_buffer<int32> mtxin;

    Graph1_3D()
    {

        // Shared Buffers
        mtxin = adf::shared_buffer<int32>::create({32,4,2}, 1, 1);
        num_buffers(mtxin) = 2;
        adf::location<adf::buffer>(mtxin) = { adf::address(COL, 0, 0), adf::address(COL, 0, 0x8000)};

        k1 = adf::kernel::create(passthrough_buffer<256>);
        adf::source(k1) = "passthrough.cpp";
        adf::runtime<ratio>(k1) = 0.9;
        adf::location<adf::kernel>(k1) = adf::tile(COL,0);

        // Connections
        adf::connect (din,mtxin.in[0]);
        adf::write_access(mtxin.in[0]) = adf::tiling({
            .buffer_dimension = {32,4,2},
            .tiling_dimension = {32,4,2},
            .offset = {0,0,0}});

        adf::connect(mtxin.out[0], k1.in[0]);
        adf::read_access(mtxin.out[0]) = adf::tiling({
            .buffer_dimension = {32,4,2},
            .tiling_dimension = {32,4,2},
            .offset = {0,0,0}});


        adf::connect (k1.out[0], dout[0]);
    };
};

// 3D Linear Zero padding
template <int COL>
class Graph2_3D : public adf::graph
{

private:
public:
    adf::kernel k1;
    adf::port<input> din;
    adf::port<output> dout[1];

    adf::shared_buffer<int32> mtxin;

    Graph2_3D()
    {

        // Shared Buffers
        mtxin = adf::shared_buffer<int32>::create({32,4,2}, 1, 1);

        num_buffers(mtxin) = 1;
        adf::location<adf::buffer>(mtxin) = {adf::address(COL, 0, 0)};

        // Original data with zeros all around
        k1 = adf::kernel::create(passthrough_buffer<34*6*2>);
        adf::source(k1) = "passthrough.cpp";
        adf::runtime<ratio>(k1) = 0.9;
        adf::location<adf::kernel>(k1) = adf::tile(COL, 0);

        // Connections
        adf::connect(din, mtxin.in[0]);
        adf::write_access(mtxin.in[0]) = adf::tiling({
            .buffer_dimension = {32,4,2},
            .tiling_dimension = {32,4,2},
            .offset = {0,0,0}
            });

        adf::connect(mtxin.out[0], k1.in[0]);
        adf::read_access(mtxin.out[0]) = adf::tiling(
            {.buffer_dimension = {32,4,2},
             .tiling_dimension = {34,6,2},
             .offset = {-1,-1,0},
             .tile_traversal = {}});

        adf::connect (k1.out[0], dout[0]);
    };
};

