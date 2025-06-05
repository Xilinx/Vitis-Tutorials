// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once


    // 2D Linear Transferring From Source to Destination
    template <int COL>
    class Graph1_2D : public adf::graph
    {

    private:
    public:
        adf::kernel k1, k2;
        adf::port<input> din;
        adf::port<output> dout[2];

        adf::shared_buffer<int32> mtxin;

        Graph1_2D()
        {

            // Shared Buffers
            mtxin = adf::shared_buffer<int32>::create({128,2}, 1, 2);
            // num_buffers(mtxin) = 2;
            adf::location<adf::buffer>(mtxin) = {adf::address(COL, 0, 0)};

            k1 = adf::kernel::create(passthrough_buffer<128*2>);
            adf::source(k1) = "passthrough.cpp";
            adf::runtime<ratio>(k1) = 0.9;
            adf::location<adf::kernel>(k1) = adf::tile(COL, 0);

            k2 = adf::kernel::create(passthrough_buffer<128*2>);
            adf::source(k2) = "passthrough.cpp";
            adf::runtime<ratio>(k2) = 0.9;
            adf::location<adf::kernel>(k2) = adf::tile(COL, 1);


            // Connections
            adf::connect(din, mtxin.in[0]);
            adf::write_access(mtxin.in[0]) = adf::tiling({.buffer_dimension = {128, 2},
                                                          .tiling_dimension = {128, 2},
                                                          .offset = {0,0}});

            adf::connect(mtxin.out[0], k1.in[0]);
            adf::read_access(mtxin.out[0]) = adf::tiling({.buffer_dimension = {128, 2},
                                                          .tiling_dimension = {128, 2},
                                                          .offset = {0,0}});

            adf::connect(mtxin.out[1], k2.in[0]);
            adf::read_access(mtxin.out[1]) = adf::tiling({
                .buffer_dimension = {128, 2},
                .tiling_dimension = {128, 1},
                .offset = {0, 0},
                .tile_traversal = {{.dimension = 0, .stride = 128, .wrap = 1},
                {.dimension = 1, .stride = 1, .wrap = 2}}});


            adf::connect(k1.out[0], dout[0]);
            adf::connect(k2.out[0], dout[1]);
        };

    };

    // 2D Linear with zero padding 
    template <int COL>
    class Graph2_2D : public adf::graph
    {

    private:
    public:
        adf::kernel k1;
        adf::port<input> din;
        adf::port<output> dout[1];

        adf::shared_buffer<int32> mtxin;

        Graph2_2D()
        {

            // Shared Buffers
            mtxin = adf::shared_buffer<int32>::create({128, 2}, 1, 1);
            // num_buffers(mtxin) = 2;
            adf::location<adf::buffer>(mtxin) = {adf::address(COL, 0, 0)};

            k1 = adf::kernel::create(passthrough_buffer<160 * 2>);
            adf::source(k1) = "passthrough.cpp";
            adf::runtime<ratio>(k1) = 0.9;
            adf::location<adf::kernel>(k1) = adf::tile(COL, 0);


            // Connections
            adf::connect(din, mtxin.in[0]);
            adf::write_access(mtxin.in[0]) = adf::tiling({.buffer_dimension = {128, 2},
                                                          .tiling_dimension = {128, 2},
                                                          .offset = {0, 0}});

            adf::connect(mtxin.out[0], k1.in[0]);
            adf::read_access(mtxin.out[0]) = adf::tiling({.buffer_dimension = {128, 2},
                                                          .tiling_dimension = {160, 1},
                                                          .offset = {-16, 0},
                                                          .tile_traversal = {{.dimension = 0, .stride = 128, .wrap = 1}, {.dimension = 1, .stride = 1, .wrap = 2}}});


            adf::connect(k1.out[0], dout[0]);
        };
    };

    // 2D Matrix Transferring to 4x2 Sub Matrices
    template <int COL>
    class Graph3_2D : public adf::graph
    {

    private:
    public:
        adf::kernel k1;
        adf::port<input> din;
        adf::port<output> dout[1];

        adf::shared_buffer<int32> mtxin;

        Graph3_2D()
        {

            // Shared Buffers
            mtxin = adf::shared_buffer<int32>::create({16, 4}, 1, 1);
            // num_buffers(mtxin) = 2;
            adf::location<adf::buffer>(mtxin) = {adf::address(COL, 0, 0)};

            k1 = adf::kernel::create(passthrough_buffer<16*4>);
            adf::source(k1) = "passthrough.cpp";
            adf::runtime<ratio>(k1) = 0.9;
            adf::location<adf::kernel>(k1) = adf::tile(COL, 0);

            // Connections
            adf::connect(din, mtxin.in[0]);
            adf::write_access(mtxin.in[0]) = adf::tiling({.buffer_dimension = {16, 4},
                                                          .tiling_dimension = {16, 4},
                                                          .offset = {0, 0}});

            adf::connect(mtxin.out[0], k1.in[0]);
            adf::read_access(mtxin.out[0]) = adf::tiling({.buffer_dimension = {16, 4},
                                                          .tiling_dimension = {4,2},
                                                          .offset = {0, 0},
                                                          .tile_traversal = {{.dimension = 0, .stride = 4, .wrap = 4}, {.dimension = 1, .stride = 2, .wrap = 2}}});

            adf::connect(k1.out[0], dout[0]);
        };
    };

    // 2D Shuffle from Higher Address to Lower Address
    template <int COL>
    class Graph4_2D : public adf::graph
    {

    private:
    public:
        adf::kernel k1,k2;
        adf::port<input> din;
        adf::port<output> dout[2];

        adf::shared_buffer<int32> mtxin1,mtxin2;

        Graph4_2D()
        {

            // Shared Buffers
            mtxin1 = adf::shared_buffer<int32>::create({32,1}, 1, 1);
            // num_buffers(mtxin1) = 2;
            adf::location<adf::buffer>(mtxin1) = {adf::address(COL, 0, 0)};
            
            mtxin2 = adf::shared_buffer<int32>::create({32, 8}, 1, 1);
            // num_buffers(mtxin2) = 2;
            adf::location<adf::buffer>(mtxin2) = {adf::address(COL, 1, 0)};

            k1 = adf::kernel::create(passthrough_buffer<32*8>);
            adf::source(k1) = "passthrough.cpp";
            adf::runtime<ratio>(k1) = 0.9;
            adf::location<adf::kernel>(k1) = adf::tile(COL, 0);

            k2 = adf::kernel::create(passthrough_buffer<32 * 8>);
            adf::source(k2) = "passthrough.cpp";
            adf::runtime<ratio>(k2) = 0.9;
            adf::location<adf::kernel>(k2) = adf::tile(COL, 1);

            // k1 Connections
            adf::connect(din, mtxin1.in[0]);
            adf::write_access(mtxin1.in[0]) = adf::tiling({.buffer_dimension = {32,1},
                                                          .tiling_dimension = {32,1},
                                                          .offset = {0, 0}});

            adf::connect(mtxin1.out[0], k1.in[0]);
            adf::read_access(mtxin1.out[0]) = {
                adf::tiling({.buffer_dimension = {32, 1}, .tiling_dimension = {16, 1}, .offset = {16, 0},
                .tile_traversal = {{.dimension = 0, .stride = 1, .wrap = 1}}}),
                adf::tiling({.buffer_dimension = {32, 1}, .tiling_dimension = {16, 1}, .offset = {0, 0},
                .tile_traversal = {{.dimension = 0, .stride = 1, .wrap = 1}}})
            };
            adf::connect(k1.out[0], dout[0]);

            // k2 Connections
            adf::connect(din, mtxin2.in[0]);
            adf::write_access(mtxin2.in[0]) = adf::tiling({.buffer_dimension = {32, 8},
                                                          .tiling_dimension = {32, 8},
                                                          .offset = {0, 0}});

            adf::connect(mtxin2.out[0], k2.in[0]);
            adf::read_access(mtxin2.out[0]) = {
                adf::tiling({.buffer_dimension = {32, 8}, .tiling_dimension = {16, 8}, .offset = {16, 0}, .tile_traversal = {{.dimension = 0, .stride = 1, .wrap = 1}}}),
                adf::tiling({.buffer_dimension = {32, 8}, .tiling_dimension = {16, 8}, .offset = {0, 0}, .tile_traversal = {{.dimension = 0, .stride = 1, .wrap = 1}}})};
            adf::connect(k2.out[0], dout[1]);
        };
    };

