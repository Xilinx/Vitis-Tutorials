// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include <vector>
#include "kernels.h"

#include "graph_1D.h"
#include "graph_2D.h"
#include "graph_3D.h"
#include "graph_4D.h"

template <int setG1_1, int setG2_1, int setG3_1, int setG4_1, int setG1_2, int setG2_2, int setG3_2, int  setG4_2,int setG1_3,int setG2_3,int setG1_4>
class TopGraph : public adf::graph
{
public:
    // I do not use index 0 for the sake of simplicity:  1 --> G1, ...
    static constexpr int NGraphs = 11;
    static constexpr int noutput[NGraphs + 1] = {0, 3, 1, 1, 1, 2, 1, 1,2,1,1,1};

    adf::input_plio plin[50];
    adf::output_plio plout[50];

    // 1D shared buffer
    typename std::conditional<setG1_1, Graph1_1D<10>, int>::type G1_1;
    typename std::conditional<setG2_1, Graph2_1D<11>, int>::type G2_1;
    typename std::conditional<setG3_1, Graph3_1D<12>, int>::type G3_1;
    typename std::conditional<setG4_1, Graph4_1D<13>, int>::type G4_1;

    // 2D shared buffer
    typename std::conditional<setG1_2, Graph1_2D<14>, int>::type G1_2;
    typename std::conditional<setG2_2, Graph2_2D<15>, int>::type G2_2;
    typename std::conditional<setG3_2, Graph3_2D<16>, int>::type G3_2;
    typename std::conditional<setG4_2, Graph4_2D<17>, int>::type G4_2;

    // 3D shared buffer
    typename std::conditional<setG1_3, Graph1_3D<18>, int>::type G1_3;
    typename std::conditional<setG2_3, Graph2_3D<19>, int>::type G2_3;

    // 4D shared buffer
    typename std::conditional<setG1_4, Graph1_4D<20>, int>::type G1_4;

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
        idxin = 10;
        idxout = 10;
        if constexpr (setG1_1 == 1)
            Connect(1, G1_1, "_1D_256");
        if constexpr (setG2_1 == 1)
            Connect(2, G2_1, "_1D_256");
        if constexpr (setG3_1 == 1)
            Connect(3, G3_1, "_1D_256");
        if constexpr (setG4_1 == 1)
            Connect(4, G4_1, "_1D_256");
            
        idxin = 20;
        idxout = 20;
        if constexpr (setG1_2 == 1)
            Connect(5, G1_2, "_1D_256");
        if constexpr (setG2_2 == 1)
            Connect(6, G2_2, "_1D_256");
        if constexpr (setG3_2 == 1)
            Connect(7, G3_2, "_1D_64");
        if constexpr (setG4_2 == 1)
            Connect(8, G4_2, "_1D_256");

        idxin = 30;
        idxout = 30;
        if constexpr (setG1_3 == 1)
            Connect(9, G1_3, "_1D_256");
        if constexpr (setG2_3 == 1)
            Connect(10, G2_3, "_1D_256");

        idxin = 40;
        idxout = 40;
        if constexpr (setG1_4 == 1)
            Connect(11, G1_4, "_1D_256");
    };
};
