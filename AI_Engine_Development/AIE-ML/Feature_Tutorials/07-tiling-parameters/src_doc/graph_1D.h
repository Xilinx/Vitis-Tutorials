// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

// 1D Linear Transferring From Source to Destination
template <int COL>
class Graph1_1D : public adf::graph
{

private:

public:
    adf::kernel k1,k2,k3;
    adf::port<input> din;
    adf::port<output> dout[3];
    
    adf::shared_buffer<int32> mtxin;

    Graph1_1D()
    {

        // Shared Buffers
        mtxin = adf::shared_buffer<int32>::create({256}, 1, 3);
        num_buffers(mtxin) = 2;
        adf::location<adf::buffer>(mtxin) = { adf::address(COL, 0, 0), adf::address(COL, 0, 0x8000)};

        k1 = adf::kernel::create(passthrough_buffer<256>);
        adf::source(k1) = "passthrough.cpp";
        adf::runtime<ratio>(k1) = 0.9;
        adf::location<adf::kernel>(k1) = adf::tile(COL,0);

        k2 = adf::kernel::create(passthrough_buffer<256>);
        adf::source(k2) = "passthrough.cpp";
        adf::runtime<ratio>(k2) = 0.9;
        adf::location<adf::kernel>(k2) = adf::tile(COL,1);

        k3 = adf::kernel::create(passthrough_buffer<256>);
        adf::source(k3) = "passthrough.cpp";
        adf::runtime<ratio>(k3) = 0.9;
        adf::location<adf::kernel>(k3) = adf::tile(COL, 2);

        // Connections
        adf::connect (din,mtxin.in[0]);
        adf::write_access(mtxin.in[0]) = adf::tiling({
            .buffer_dimension = {256},
            .tiling_dimension = {256},
            .offset = {0}});

        adf::connect(mtxin.out[0], k1.in[0]);
        adf::read_access(mtxin.out[0]) = adf::tiling({
            .buffer_dimension = {256},
            .tiling_dimension = {256},
            .offset = {0}});

        adf::connect(mtxin.out[1], k2.in[0]);
        adf::read_access(mtxin.out[1]) = adf::tiling({
            .buffer_dimension = {256},
            .tiling_dimension = {256},
            .offset = {0},
            .tile_traversal = {{.dimension = 0, .stride = 256, .wrap = 1}} });

        adf::connect(mtxin.out[2], k3.in[0]);
        adf::read_access(mtxin.out[2]) = adf::tiling({
            .buffer_dimension = {256},
            .tiling_dimension = {1},
            .offset = {0},
            .tile_traversal = {{.dimension = 0, .stride = 1, .wrap = 256}} });

        adf::connect (k1.out[0], dout[0]);
        adf::connect (k2.out[0], dout[1]);
        adf::connect(k3.out[0], dout[2]);
    };
};

// 1D Linear Zero Pre-padding
template <int COL>
class Graph2_1D : public adf::graph
{

private:
public:
    adf::kernel k1;
    adf::port<input> din;
    adf::port<output> dout[1];

    adf::shared_buffer<int32> mtxin;

    Graph2_1D()
    {

        // Shared Buffers
        mtxin = adf::shared_buffer<int32>::create({256}, 1, 1);

        num_buffers(mtxin) = 1;
        adf::location<adf::buffer>(mtxin) = {adf::address(COL, 0, 0)};

        // Original data with zeros all around
        k1 = adf::kernel::create(passthrough_buffer<256>);
        adf::source(k1) = "passthrough.cpp";
        adf::runtime<ratio>(k1) = 0.9;
        adf::location<adf::kernel>(k1) = adf::tile(COL, 0);

        // Connections
        adf::connect(din, mtxin.in[0]);
        adf::write_access(mtxin.in[0]) = adf::tiling({
            .buffer_dimension = {256},
            .tiling_dimension = {256},
            .offset = {0}
            });

        adf::connect(mtxin.out[0], k1.in[0]);
        adf::read_access(mtxin.out[0]) = adf::tiling(
            {.buffer_dimension = {256},
             .tiling_dimension = {256},
             .offset = {-32},
             .tile_traversal = {{.dimension = 0, .stride = 256, .wrap = 1}}});

        adf::connect (k1.out[0], dout[0]);
    };
};

// 1D Linear Zero Pre-padding and Post Padding
template <int COL>
class Graph3_1D : public adf::graph
{

private:
public:
    adf::kernel k1;
    adf::port<input> din;
    adf::port<output> dout[1];

    adf::shared_buffer<int32> mtxin;

    Graph3_1D()
    {

        // Shared Buffers
        mtxin = adf::shared_buffer<int32>::create({256}, 1, 1);

        num_buffers(mtxin) = 1;
        adf::location<adf::buffer>(mtxin) = {adf::address(COL, 0, 0)};

        // Original data with zeros all around
        k1 = adf::kernel::create(passthrough_buffer<288>);
        adf::source(k1) = "passthrough.cpp";
        adf::runtime<ratio>(k1) = 0.9;
        adf::location<adf::kernel>(k1) = adf::tile(COL, 0);

        // Connections
        adf::connect(din, mtxin.in[0]);
        adf::write_access(mtxin.in[0]) = adf::tiling({.buffer_dimension = {256},
                                                      .tiling_dimension = {256},
                                                      .offset = {0}});

        adf::connect(mtxin.out[0], k1.in[0]);
        adf::read_access(mtxin.out[0]) = adf::tiling(
            {.buffer_dimension = {256},
             .tiling_dimension = {288},
             .offset = {-16},
             .tile_traversal = {{.dimension = 0, .stride = 256, .wrap = 1}},
             .boundary_dimension = {256}});

        adf::connect(k1.out[0], dout[0]);
    };
    };

    // Truncated 1D Linear Zero Pre-padding and Post Padding
    template <int COL>
    class Graph4_1D : public adf::graph
    {

    private:
    public:
        adf::kernel k1;
        adf::port<input> din;
        adf::port<output> dout[1];

        adf::shared_buffer<int32> mtxin;

        Graph4_1D()
        {

            // Shared Buffers
            mtxin = adf::shared_buffer<int32>::create({256}, 1, 1);

            num_buffers(mtxin) = 1;
            adf::location<adf::buffer>(mtxin) = {adf::address(COL, 0, 0)};

            // Original data with zeros all around
        k1 = adf::kernel::create(passthrough_buffer<128>);
        adf::source(k1) = "passthrough.cpp";
        adf::runtime<ratio>(k1) = 0.9;
        adf::location<adf::kernel>(k1) = adf::tile(COL, 0);

        // Connections
        adf::connect(din, mtxin.in[0]);
        adf::write_access(mtxin.in[0]) = adf::tiling({
        .buffer_dimension = {256},
        .tiling_dimension = {256},
        .offset = {0}});

        adf::connect(mtxin.out[0], k1.in[0]);
        adf::read_access(mtxin.out[0]) = adf::tiling(
            {.buffer_dimension = {256},
             .tiling_dimension = {128},
             .offset = {-16},
             .tile_traversal = {{.dimension = 0, .stride = 144, .wrap = 2}},
             .boundary_dimension = {96}});

        adf::connect(k1.out[0], dout[0]);
        };
    };
