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
    adf::kernel k1,k2;
    adf::port<input> din;
    adf::port<output> dout[2];

    Graph1()
    {

        k1 = adf::kernel::create(passthrough_buffer2<Dim0 * Dim1, 8>); // 64 pixels per tile
        adf::source(k1) = "passthrough.cpp";
        adf::runtime<ratio>(k1) = 0.9;
        adf::location<adf::kernel>(k1) = adf::tile(COL, 0);

        k2 = adf::kernel::create(passthrough_buffer<Dim0 * Dim1, 8>); // 64 pixels per tile
        adf::source(k2) = "passthrough.cpp";
        adf::runtime<ratio>(k2) = 0.9;
        adf::location<adf::kernel>(k2) = adf::tile(COL, 1);

        // Connections
        adf::connect(din, k1.in[0]);
        adf::connect(k1.out[0], dout[0]); // Reference output
        
        adf::connect(k1.out[1], k2.in[0]);
        adf::read_access(k1.out[1]) = adf::tiling({.buffer_dimension = {Dim0,Dim1},
                                                      .tiling_dimension = {1,1},
                                                      .offset = {0,0},
                                                      .tile_traversal = {
                                                      {.dimension=1,.stride=1,.wrap=Dim1},
                                                      {.dimension=0,.stride=1,.wrap=Dim0}}});
        adf::connect(k2.out[0],dout[1]);
    };
};

// Tile Manipulation
template <int COL>
class Graph2 : public adf::graph
{

private:
public:
    adf::kernel k1, k2;
    adf::port<input> din;
    adf::port<output> dout[2];

    Graph2()
    {

        k1 = adf::kernel::create(passthrough_buffer2<Dim0 * Dim1, 8>); // 64 pixels per tile
        adf::source(k1) = "passthrough.cpp";
        adf::runtime<ratio>(k1) = 0.9;
        adf::location<adf::kernel>(k1) = adf::tile(COL, 0);

        k2 = adf::kernel::create(passthrough_buffer<Dim0 * Dim1, 8>); // 64 pixels per tile
        adf::source(k2) = "passthrough.cpp";
        adf::runtime<ratio>(k2) = 0.9;
        adf::location<adf::kernel>(k2) = adf::tile(COL, 1);

        // Connections
        adf::connect(din, k1.in[0]);
        adf::connect(k1.out[0], dout[0]); // Reference output

        adf::connect(k1.out[1], k2.in[0]);
        adf::read_access(k1.out[1]) = adf::tiling({.buffer_dimension = {Dim0, Dim1},
                                                   .tiling_dimension = {4,2},
                                                   .offset = {0, 0},
                                                   .tile_traversal = {
                                                       {.dimension = 1, .stride = 2, .wrap = Dim1/2},
                                                       {.dimension = 0, .stride = 4, .wrap = Dim0/4}}});
        adf::write_access(k2.in[0]) = adf::tiling({.buffer_dimension = {Dim0, Dim1},
                                                   .tiling_dimension = {1, 8},
                                                   .offset = {0, 0},
                                                   .tile_traversal = {
                                                       {.dimension = 0, .stride = 1, .wrap = Dim0}}});
        adf::connect(k2.out[0], dout[1]);
    };
};

// Internal Kernel Data Access
template <int COL>
class Graph3 : public adf::graph
{

private:
public:
    adf::kernel k1;
    adf::port<input> din;
    adf::port<output> dout[1];

    Graph3()
    {

        k1 = adf::kernel::create(passthrough_buffer_block<Dim0 * Dim1 * Dim2, Dim0 * Dim1, 8>); 
        adf::source(k1) = "passthrough.cpp";
        adf::runtime<ratio>(k1) = 0.9;
        adf::location<adf::kernel>(k1) = adf::tile(COL, 0);


        // Connections
        adf::connect(din, k1.in[0]);
        adf::connect(k1.out[0], dout[0]); // Reference output

    };
};

// Internal Kernel Data Access
template <int COL>
class Graph4 : public adf::graph
{

private:
public:
    adf::kernel k1;
    adf::port<input> din;
    adf::port<output> dout[1];


    Graph4()
    {

        k1 = adf::kernel::create(passthrough_buffer_block_4d<256,1024, 8>);
        adf::source(k1) = "passthrough.cpp";
        adf::runtime<ratio>(k1) = 0.9;
        adf::location<adf::kernel>(k1) = adf::tile(COL, 0);

        // Connections
        adf::connect(din, k1.in[0]);
        adf::connect(k1.out[0], dout[0]); // Reference output
    };
};

template <int setG1, int setG2, int setG3,int setG4>
class TopGraph : public adf::graph
{
public:

    // I do not use index 0 for the sake of simplicity:  1 --> G1, ...
    static constexpr int NGraphs = 4;
    static constexpr  int noutput[NGraphs+1] = {0,2,2,1,1};

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
        if constexpr (setG3==1) Connect(3,G3,"_1D_1024");
        if constexpr (setG4==1) Connect(4,G4,"_1D_1024");
        
        
    };
};
