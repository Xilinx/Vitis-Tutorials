// Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT

#include "graph.h"
#include <fstream>
#include <iomanip>
using namespace std;

Graph1<10> G1;

#if defined(__X86SIM__) || defined(__AIESIM__)
int main(int argc, char ** argv) {

    // How many GMIOs in the graph
    int Ngraphs = 1;
    int IterationLength = Dim0*Dim1*Dim2*Dim3;
    int TotalLength = NIterations*IterationLength;
    
    // Allocate memory for input and output GMIOs
    int **GMin,**GMout;
    
    GMin = new int*[Ngraphs];
    for (int i = 0; i < Ngraphs; ++i)
    {
        GMin[i] = (int32*)adf::GMIO::malloc(TotalLength*sizeof(int32));
    }
    GMout = new int*[Ngraphs];
    for (int i = 0; i < Ngraphs; ++i)
    {
        GMout[i] = (int32 *)adf::GMIO::malloc(TotalLength * sizeof(int32));
    }

    // Initialize memory
    for(int i = 0; i < Ngraphs; ++i) {
        for (int j = 0; j < TotalLength; ++j)
        {
            GMin[i][j] = (i+1)*(j+1);
        }
    }
    for (int i = 0; i < Ngraphs; ++i)
    {
        for (int j = 0; j < TotalLength; ++j)
        {
            GMout[i][j] = -999;
        }
    }

    G1.init();
    
    for(int iter=0;iter<NIterations;iter++)
    {
        G1.ddrin.setAddress(GMin[0]+IterationLength*iter);
        G1.ddrout.setAddress(GMout[0] + IterationLength * iter);

        G1.ddrin.gm2aie_nb(G1.ddrin.out[0]);
        G1.ddrout.aie2gm_nb(G1.ddrout.in[0]);

        G1.run(1);
        G1.ddrin.wait(G1.ddrin.out[0]);
        G1.ddrout.wait(G1.ddrout.in[0]);
    }

    G1.end();

    std::ofstream ofs,ofsc;

#ifndef __X86SIM__
    system("mkdir -p extbuf_aiesimulator_output/data");
    ofs.open("extbuf_aiesimulator_output/data/Output_1.txt", std::ofstream::out | std::ofstream::trunc);
    ofsc.open("extbuf_aiesimulator_output/Output_Compare.txt", std::ofstream::out | std::ofstream::trunc);
#else
    system("mkdir -p extbuf_x86simulator_output/data");
    ofs.open("extbuf_x86simulator_output/data/Output_1.txt", std::ofstream::out | std::ofstream::trunc);
    ofsc.open("extbuf_x86simulator_output/Output_Compare.txt", std::ofstream::out | std::ofstream::trunc);
#endif

    int L = 8;
    for (int i = 0; i < TotalLength; i+=L)
    {
        for(int j=0;j<L;j++)
            ofsc << setw(5) << GMin[0][i+j];
        ofsc << "          ";
        for (int j = 0; j < L; j++)
            ofsc << setw(5) << GMout[0][i + j];

        ofsc << std::endl;

        for (int j = 0; j < L; j++)
            ofs << setw(5) << GMout[0][i + j];

        ofs << std::endl;

        if (i % (Dim0*Dim1) == (Dim0*Dim1)-L)
        {
            ofsc << std::endl;
        }
    }

    ofs.close();
    ofsc.close();

    adf::GMIO::free(GMin[0]);
    adf::GMIO::free(GMout[0]);
    delete GMin;
    delete GMout;

    return 0;
}


#endif
