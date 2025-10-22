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
    int IterationLength = Dim0*Dim1*Dim2*Dim3;
    int TotalLength = IterationLength; // *NIterations

    // Allocate memory for input and output GMIOs
    int *GMin,*GMout;
    

    GMin = (int32*)adf::GMIO::malloc(TotalLength*sizeof(int32));
    GMout = (int32 *)adf::GMIO::malloc(TotalLength * sizeof(int32));
    

    std::ofstream ofs;

#ifndef __X86SIM__
    system("mkdir -p extbuf_aiesimulator_output/data");
    ofs.open("extbuf_aiesimulator_output/data/Output_1.txt", std::ofstream::out | std::ofstream::trunc);
#else
    system("mkdir -p extbuf_x86simulator_output/data");
    ofs.open("extbuf_x86simulator_output/data/Output_1.txt", std::ofstream::out | std::ofstream::trunc);
#endif

    G1.init();

    // setAddress has to be used only once.
    // If the address has to change, the memory must be freed and reallocated
    G1.ddrin.setAddress(GMin);
    G1.ddrout.setAddress(GMout);

    for(int iter=0;iter<NIterations;iter++)
    {
        int joffset = IterationLength * iter;
        // Initialize memory
        for (int j = 0; j < IterationLength; ++j)
        {
            GMin[j] = (j + joffset + 1);
            GMout[j] = -999;
        }
        

        G1.ddrin.gm2aie_nb(G1.ddrin.out[0]);
        G1.ddrout.aie2gm_nb(G1.ddrout.in[0]);

        G1.run(1);
        G1.ddrin.wait(G1.ddrin.out[0]);
        G1.ddrout.wait(G1.ddrout.in[0]);

        int L = 8;
        for (int i = 0; i < IterationLength; i += L)
        {
            for (int j = 0; j < L; j++)
                ofs << setw(5) << GMin[i + j];
            ofs << "          ";
            for (int j = 0; j < L; j++)
                ofs << setw(5) << GMout[i + j];

            ofs << std::endl;

            if (i % (Dim0 * Dim1) == (Dim0 * Dim1) - L)
                ofs << std::endl;
        }
    }

    G1.end();



    ofs.close();

    adf::GMIO::free(GMin);
    adf::GMIO::free(GMout);

    return 0;
}


#endif
