/*
Copyright (C) 2025, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/


#include <fstream>
#include <cstring>
#include <iomanip>
using namespace std;

#include "experimental/xrt_kernel.h"
#include "experimental/xrt_graph.h"
#include "experimental/xrt_aie.h"

template<typename T>
int load_data_file(std::string fname,T * data,int L)
{
    int d;
    int l=0;

    std::ifstream fin(fname);
    if(!fin) return(-1);

    while(fin>>d)
    {
        data[l++] = (T)d;
        if(l==L) break;
    }
    fin.close();
    return(l);
}

#ifndef Dim0
#error "Dim0 should be defined"
#endif

#ifndef Dim1
#error "Dim1 should be defined"
#endif

#ifndef Dim2
#error "Dim2 should be defined"
#endif

#ifndef Dim3
#error "Dim3 should be defined"
#endif

int run(int argc, char *argv[])
{

    int NGraphs = 1;
    int NIter = 1;
    
    if(argc != 2 && argc != 3) {
        std::cout << "Usage: " << argv[0] <<" <xclbin> [Number of iterations]" << std::endl;
        return EXIT_FAILURE;
    }
    char* xclbinFilename = argv[1];
    if(argc==3)
        NIter = atoi(argv[2]);

    char *emu_mode = getenv("XCL_EMULATION_MODE");
    if (emu_mode != nullptr && NIter>4)
    {
        NIter = 4;
    }

    std::cout << "The simulation will run " << NIter << " Iterations" << std::endl << std::endl;

    //////////////////////////////////////////
    // Open xclbin
    //////////////////////////////////////////

    auto  device = xrt::device(0); //device index=0
    if(device == nullptr)
    throw std::runtime_error("No valid device handle found. Make sure using right xclOpen index.");
    auto xclbin_uuid = device.load_xclbin(xclbinFilename);


    //////////////////////////////////////////
    // Input and Output memories Allocation
    //////////////////////////////////////////
    
    const int TILE_SIZE = Dim0*Dim1;
    const int REPETITION = Dim2*Dim3;
    const int FRAME_LENGTH = TILE_SIZE*REPETITION;
    
    xrt::aie::bo *InputBufferObject[NGraphs];
    xrt::aie::bo *OutputBufferObject[NGraphs];

    int32_t* InputMem[NGraphs];
    int32_t* OutputMem[NGraphs];

    xrt::aie::buffer *outbuf[NGraphs];
    xrt::aie::buffer *inbuf[NGraphs];

    for(int g=0;g<NGraphs;g++)
    {
        // xrt::bo(device, DATA_SIZE , flag,bank_id)
        // <device>: xrt::device object of the accelerator card.
        // <DATA_SIZE>: Size of the buffer in bytes
        // <flag>: xrt::bo::flags:: normal | cacheable | device_only | host_only | p2p | svm
        // <bank_id>: Defines the memory bank on the device where the buffer should be allocated for IP access. The memory bank specified must match with the corresponding IP port's connection inside the .xclbin file. Otherwise you will get bad_alloc when running the application.
        
        InputBufferObject[g] = new xrt::aie::bo(device, FRAME_LENGTH * sizeof(int), xrt::bo::flags::normal, 0);
        InputMem[g] = InputBufferObject[g]->map<int32_t *>();
        for(int k=0;k<FRAME_LENGTH;k++)
            InputMem[g][k] = (k+1)*(g+1);
        printf("Graph %d Input memory virtual addr 0x%px\n",g, InputMem[g]);

        OutputBufferObject[g] = new xrt::aie::bo(device, FRAME_LENGTH * sizeof(int), 0, 0);
        OutputMem[g] = OutputBufferObject[g]->map<int32_t *>();
        for (int k = 0; k < FRAME_LENGTH; k++)
            OutputMem[g][k] = -999;
        printf("Graph %d Input memory virtual addr 0x%px\n",g, OutputMem[g]);
    }    




    //////////////////////////////////////////
    // graph execution for AIE
    //////////////////////////////////////////

    //Obtains the graph handle from the XCLBIN that is loaded into the device
    auto cghdl = xrt::graph(device,xclbin_uuid,"G1");
    cghdl.reset();

    printf("graph run\n");
    cghdl.run(NIter);

    
    // Start the output buffers DMAs
    for (int g = 0; g < NGraphs; g++)
    {
        outbuf[g] = new xrt::aie::buffer(device, xclbin_uuid, "G" + std::to_string(g+1) + ".ddrout");
        outbuf[g]->async(*OutputBufferObject[g], XCL_BO_SYNC_BO_AIE_TO_GMIO, FRAME_LENGTH * sizeof(int), /*offset*/ 0);
    }

    // Start the input buffers DMAs
    for (int g = 0; g < NGraphs; g++)
    {
        inbuf[g] = new xrt::aie::buffer(device, xclbin_uuid, "G" + std::to_string(g + 1) + ".ddrin");
        inbuf[g]->async(*InputBufferObject[g], XCL_BO_SYNC_BO_GMIO_TO_AIE, FRAME_LENGTH * sizeof(int), /*offset*/ 0);
    }

    // Wait for the end of Output buffer DMAs
    for (int g = 0; g < NGraphs; g++)
    {
        outbuf[g]->wait();
    }
    
        // Graph end
        cghdl.end(0);
        printf("graph end\n");

        int match = 0;
        std::cout << "Compare Input and Output" << std::endl;
        for (int g = 0; g < NGraphs; g++)
        {
            int iter_offset = 0;
            for (int it = 0; it < NIter; it++,iter_offset+=FRAME_LENGTH)
            {
                int rep_offset = 0;
                for(int rep=0;rep<REPETITION;rep++,rep_offset+=TILE_SIZE)
                {
                    for(int k=0;k<TILE_SIZE;k++)
                    {
                        // Extract input (row,col)
                        int inrow = k/Dim0;
                        int incol = k%Dim0;
                        // output is input transposition
                        int outcol = inrow;
                        int outrow = incol;

                        int insample = iter_offset + rep_offset + inrow * Dim0 + incol;
                        int outsample = iter_offset + rep_offset + outrow * Dim1 + outcol;

                        if (OutputMem[g][outsample] != InputMem[g][insample])
                        {
                            if (match <= 10)
                            {
                                cout << "Graph=" << g << "  Iter=" << it << "  Rep=" << rep << "  k=" << k;
                                cout << "  -->  (" << inrow << "," << incol << ")";
                                cout << "  insample=k=" << insample << " [" << InputMem[g][insample] << "]  -->  (" << outrow << "," << outcol << ")" ;
                                cout << "  outsample=" << outsample << "  [" << OutputMem[g][outsample] << "]" << endl;
                            }
                            if(match==11)
                                cout << "..." << endl;
                            match++;
                        }
                    }
                }
                
            }
        }

        std::cout << "\n\nDisplays Input and Output" << std::endl;
        for (int g = 0; g < NGraphs; g++)
        {
            int iter_offset = 0;
            for (int it = 0; it < NIter; it++, iter_offset += FRAME_LENGTH)
            {
                int rep_offset = 0;
                for (int rep = 0; rep < REPETITION; rep++, rep_offset += TILE_SIZE)
                {
                    int L = 8;
                    for (int k = 0; k < TILE_SIZE; k+=L)
                    {
                        for (int j = 0; j < L; j++)
                            cout << setw(5) << InputMem[g][k + j + iter_offset + rep_offset];
                        cout << "          ";
                        for (int j = 0; j < L; j++)
                            cout << setw(5) << OutputMem[g][k + j + iter_offset + rep_offset];

                        cout << std::endl;

                        if (k % TILE_SIZE == TILE_SIZE - L)
                            cout << std::endl;

                    }
                }
            }
        }

        return ((match) != 0);
    }

int main(int argc, char* argv[])
{
    try {
        auto match = run(argc, argv);
        std::cout << "TEST " << (match ? "FAILED" : "PASSED") << "\n";
        return (match ? EXIT_FAILURE :  EXIT_SUCCESS);
    }
    catch (std::exception const& e) {
        std::cout << "Exception: " << e.what() << "\n";
        std::cout << "FAILED TEST\n";
        return 1;
    }
}
