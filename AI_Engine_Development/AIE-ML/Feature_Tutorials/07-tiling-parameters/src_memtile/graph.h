// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include <vector>
#include "kernels.h"

// 2x2 tiling  --> Compile error
template <int COL>
class Graph1 : public adf::graph
{

private:
public:
    adf::kernel k1;
    adf::port<input> din;
    adf::port<output> dout[1];

    adf::shared_buffer<int32> mtxin;

    Graph1()
    {

        uint D01 = Dim0 * Dim1; // 64
        uint D23 = Dim2*Dim3; // 16
        uint D0123 = D01 * D23; // 1024

        // Shared Buffers
        mtxin = adf::shared_buffer<int32>::create({D0123}, 1, 1);
        
        // D23 = 12;
        // adf::location<adf::dma>(mtxin.in[0]) = adf::dma_channel(adf::memory_tile,COL,0,0);
        // adf::location<adf::dma>(mtxin.out[0]) = adf::dma_channel(adf::memory_tile,COL,0,1);

        num_buffers(mtxin) = 1;
        adf::location<adf::buffer>(mtxin) = {adf::address(COL, 0, 0)};

        // Image split in 4 tiles without padding
        k1 = adf::kernel::create(passthrough_buffer<(Dim0 / 2) * (Dim1 / 2),8>); // 16 pixels per tile
        adf::source(k1) = "passthrough.cpp";
        adf::runtime<ratio>(k1) = 0.9;
        adf::location<adf::kernel>(k1) = adf::tile(COL, 0);

        // Connections
        adf::connect(din, mtxin.in[0]);
        adf::write_access(mtxin.in[0]) = adf::tiling({.buffer_dimension = {D0123},
                                                      .tiling_dimension = {D0123},
                                                      .offset = {0},
                                                      .tile_traversal = {}});

        // Original data zero padded around in 4 tiles
        adf::connect(mtxin.out[0], k1.in[0]);
        adf::read_access(mtxin.out[0]) = adf::tiling(
            {.buffer_dimension = {Dim0, Dim1, D23},           // 8, 8, 16
            .tiling_dimension = {(Dim0 / 2), (Dim1 / 2), 1}, // 4x4x1
            .offset = {0, 0, 0},
            .tile_traversal = {
                {.dimension = 0, .stride = Dim0 / 2, .wrap = 2},
                {.dimension = 1, .stride = Dim1 / 2, .wrap = 2},
                {.dimension = 2, .stride = 1, .wrap = D23}}}); 

        adf::connect(k1.out[0], dout[0]);
    };
};

// 2x2 tiling  --> No Compile error
template <int COL>
class Graph2 : public adf::graph
{

private:
public:
    adf::kernel k1;
    adf::port<input> din;
    adf::port<output> dout[1];

    adf::shared_buffer<int32> mtxin;

    Graph2()
    {

        uint D01 = Dim0 * Dim1; // 64
        uint D23 = Dim2 * Dim3; // 16
        uint D0123 = D01 * D23; // 1024

        // Shared Buffers
        mtxin = adf::shared_buffer<int32>::create({D0123}, 1, 1);

        num_buffers(mtxin) = 1;
        adf::location<adf::buffer>(mtxin) = {adf::address(COL, 0, 0)};

        // Image split in 4 tiles without padding
        k1 = adf::kernel::create(passthrough_buffer<(Dim0 / 2) * (Dim1 / 2),8>); // 16 pixels per tile
        adf::source(k1) = "passthrough.cpp";
        adf::runtime<ratio>(k1) = 0.9;
        adf::location<adf::kernel>(k1) = adf::tile(COL, 0);

        // Connections
        adf::connect(din, mtxin.in[0]);
        adf::write_access(mtxin.in[0]) = adf::tiling({.buffer_dimension = {D0123},
                                                      .tiling_dimension = {D0123},
                                                      .offset = {0},
                                                      .tile_traversal = {}});

        // Original data  in 4 tiles
        adf::connect(mtxin.out[0], k1.in[0]);
        adf::read_access(mtxin.out[0]) = adf::tiling({
                .buffer_dimension = {Dim0, Dim1*D23},           // 8x128
                .tiling_dimension = {(Dim0 / 2), (Dim1 / 2)}, // 4x4
                .offset = {0, 0},
                .tile_traversal = {
                    {.dimension = 0, .stride = Dim0 / 2, .wrap = 2},
                    {.dimension = 1, .stride = Dim1 / 2, .wrap = 2*D23}
                }});

        adf::connect(k1.out[0], dout[0]);
    };
};



// Graph with zero-padding in 2 steps
template <int COL>
class Graph3 : public adf::graph
{

private:
public:
    adf::kernel k1;
    adf::port<input> din;
    adf::port<output> dout[1];

    adf::shared_buffer<int32> mtxin1,mtxin2;

    Graph3()
    {

        uint D01 = Dim0 * Dim1; // 64
        uint D23 = Dim2 * Dim3; // 16
        uint D0123 = D01 * D23; // 1024

        // Shared Buffers
        mtxin1 = adf::shared_buffer<int32>::create({D0123}, 1, 1);
        mtxin2 = adf::shared_buffer<int32>::create({D01}, 1, 1);

        num_buffers(mtxin1) = 1;
        num_buffers(mtxin2) = 1;
        adf::location<adf::buffer>(mtxin1) = {adf::address(COL, 0, 0)};
        adf::location<adf::buffer>(mtxin2) = {adf::address(COL, 1, 0)};

        // Image split in 4 tiles without padding
        k1 = adf::kernel::create(passthrough_buffer<(Dim0/2+2)*(Dim1/2+2),4>); // 36 pixels per tile
        adf::source(k1) = "passthrough.cpp";
        adf::runtime<ratio>(k1) = 0.9;
        adf::location<adf::kernel>(k1) = adf::tile(COL, 0);

        // Connections
        adf::connect(din, mtxin1.in[0]);
        adf::write_access(mtxin1.in[0]) = adf::tiling({.buffer_dimension = {D0123},
                                                      .tiling_dimension = {D0123},
                                                      .offset = {0},
                                                      .tile_traversal = {}});

        // Original data sent layer by layer
        adf::connect(mtxin1.out[0], mtxin2.in[0]);
        adf::read_access(mtxin1.out[0]) = adf::tiling({
            .buffer_dimension = {Dim0, Dim1, D23},       // 8x8x16
            .tiling_dimension = {(Dim0 ), (Dim1),1}, // 8x8
            .offset = {0, 0,0},
            .tile_traversal = {
                {.dimension = 2, .stride = 1, .wrap = D23}}});

        adf::write_access(mtxin2.in[0]) = adf::tiling({.buffer_dimension = {D01},
                                                       .tiling_dimension = {D01},
                                                       .offset = {0},
                                                       .tile_traversal = {}});
        // Original data zero padded around, split in 4 tiles
        adf::connect(mtxin2.out[0], k1.in[0]);

        adf::tiling_parameters T = {
            .buffer_dimension = {Dim0, Dim1},                     // 8x8
            .tiling_dimension = {(Dim0 / 2 + 2), (Dim1 / 2 + 2)}, // 6x6
            .offset = {-1, -1},
            .tile_traversal = {
                {.dimension = 0, .stride = Dim0 / 2, .wrap = 2},
                {.dimension = 1, .stride = Dim1 / 2, .wrap = 2}},
            .boundary_dimension = {Dim0,Dim1}
        };
        adf::read_access(mtxin2.out[0]) = adf::tiling(T);

        adf::connect(k1.out[0], dout[0]);
    };
};

// Graph with zero-padding using write and read access
template <int COL>
class Graph4 : public adf::graph
{

private:
public:
    adf::kernel k1;
    adf::port<input> din;
    adf::port<output> dout[1];

    adf::shared_buffer<int32> mtxin;

    Graph4()
    {
        const uint D0 = 64;
        const uint D1 = 512;
        int Pad = 4;
        const uint Tile0 = 8;
        // const uint Tile0 =16;
        const uint Tile1 = 4;

        // Shared Buffers
        mtxin = adf::shared_buffer<int32>::create({D0*D1}, 1, 1);

        num_buffers(mtxin) = 1;
        adf::location<adf::buffer>(mtxin) = {adf::address(COL, 0, 0)};

        // Image split in 4 tiles without padding
        // k1 = adf::kernel::create(passthrough_buffer<32, 4>); // 32 pixels per tile
        k1 = adf::kernel::create(passthrough_buffer<Tile0 * Tile1, 4>);                   // 32 pixels per tile
        adf::source(k1) = "passthrough.cpp";
        adf::runtime<ratio>(k1) = 0.9;
        adf::location<adf::kernel>(k1) = adf::tile(COL, 0);

        // Connections
        adf::connect(din, mtxin.in[0]);
        adf::write_access(mtxin.in[0]) = adf::tiling({
                .buffer_dimension = {D0*D1},
              .tiling_dimension = {},
              .offset = {0, 0},
              .tile_traversal = {}});

        // Original data sent layer by layer
        adf::connect(mtxin.out[0], k1.in[0]);
        adf::read_access(mtxin.out[0]) = adf::tiling({
                .buffer_dimension = {D0,D1}, // 64x512
               .tiling_dimension = {Tile0,Tile1}, // 8x8
               .offset = {0, -Pad},
               .tile_traversal = {
                   {.dimension = 1, .stride = Tile1, .wrap = (D1+2*Pad)/Tile1},
                   {.dimension = 0, .stride = Tile0, .wrap = D0/Tile0}}});

        adf::connect(k1.out[0], dout[0]);
    };
};

template<int setG1, int setG2, int setG3,int setG4>
class TopGraph : public adf::graph
{
public:

    // I do not use index 0 for the sake of simplicity:  1 --> G1, ...
    static constexpr int NGraphs = 4;
    static constexpr  int noutput[NGraphs+1] = {0,1,1,1,1};

    adf::input_plio plin[NGraphs];
    adf::output_plio plout[2*NGraphs];
    

    typename std::conditional<setG1, Graph1<10>, int>::type G1;
    typename std::conditional<setG2, Graph2<11>, int>::type G2;
    typename std::conditional<setG3, Graph3<12>, int>::type G3;
    typename std::conditional<setG4, Graph4<13>, int>::type G4;

    int idxin = 0;
    int idxout = 0;

    template <typename T>
    void Connect(int Gidx, T &G, std::string ext)
    {
        // Input PLIO
        std::string filename = "data/Input" + ext + ".txt";
        plin[idxin] = adf::input_plio::create("input_" + std::to_string(idxin), adf::plio_64_bits, filename, 625);
        adf::connect(plin[idxin].out[0], G.din);
        idxin++;

        // Output PLIO
        for (int i = 0; i < noutput[Gidx]; i++)
        {
            plout[idxout] = adf::output_plio::create("output_" + std::to_string(idxout), adf::plio_64_bits,
                                                     "data/Output_" + std::to_string(idxout) + ".txt", 625);
            adf::connect(G.dout[i], plout[idxout].in[0]);
            idxout++;
        }
    }

    TopGraph()
    {

        // Graph Connections
        
        if constexpr (setG1==1) Connect(1,G1,"");
        if constexpr (setG2==1) Connect(2,G2,"");
        if constexpr (setG3==1) Connect(3,G3,"");
        if constexpr (setG4==1) Connect(4,G4,"_Large");
        
        
    };
};
