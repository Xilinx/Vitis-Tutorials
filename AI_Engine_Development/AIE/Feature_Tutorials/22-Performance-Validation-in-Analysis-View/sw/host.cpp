/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <complex>
#include <math.h>

#include "experimental/xrt_kernel.h"
#include "experimental/xrt_graph.h"
#include "xrt/xrt_aie.h"
#include "host.h"

// const int Niter = 999;
const int PROFILE_WINDOW_SIZE_in_bytes = 1024; 

template<typename T>
int load_data_file(std::string fname,T * data,int L)
{
    T d;
    int l=0;

    std::ifstream fin(fname);
    if(!fin) return(-1);

    while(fin>>d)
    {
        data[l++] = d;
        if(l==L) break;
    }
    fin.close();
    return(l);
}

// union {
//     struct { short re,im;} cpx;
//     int val;
// } cvt;


// Simulation

#define MEM_SIZE(NITER) ((NITER) * FRAME_LENGTH / PLIO_NDATA )

int run(int argc, char* argv[]){

    int pl_bram_size_in_bytes = PL_MEMORY_SIZE*PLIO_NBYTES;

    //TARGET_DEVICE macro needs to be passed from gcc command line
    if(argc != 2 && argc != 3) {
        std::cout << "Usage: " << argv[0] <<" <xclbin>  [Niterations (default 1000)]" << std::endl;
        return EXIT_FAILURE;
    }

    std::string niter("1000");
    if(argc==3)
    niter = std::string(argv[2]);

    int Niter = std::stoi(niter);
    int profile_output_size_in_bytes = PROFILE_WINDOW_SIZE_in_bytes * Niter;
    
    char* xclbinFilename = argv[1];

        
    std::cout << "The simulation will run " << Niter << " Iterations" << std::endl << std::endl;

    // Open xclbin
    xrt::device  device = xrt::device(0); //device index=0
    if(device == nullptr)
    throw std::runtime_error("No valid device handle found. Make sure using right xclOpen index.");
    xrt::uuid uuid = device.load_xclbin(xclbinFilename);

    // s2mm & mm2s kernel handle
    const int NioStreams = NAntenna;
    const int NplKernels = NioStreams;
    xrt::kernel * s2mm[NplKernels];
    xrt::kernel * gen2s[NplKernels];
    
    for(int i=0;i<NplKernels;i++)
    {
        s2mm[i] = new xrt::kernel(device,uuid,"s2mm_16_128");
        // Could be "s2mm_16_128:{s2mm_16_128_"+std::to_string(i+1)+"}" to specify instance
        gen2s[i] = new xrt::kernel(device,uuid,"gen2s_16_128");
    }


    // Output memories (No Input as I developped generators)
    xrt::bo * out_bo[NioStreams];
    short * host_out[NioStreams];


    for(int i=0;i<NplKernels;i++)
    {
        out_bo[i] = new xrt::bo(device, pl_bram_size_in_bytes, s2mm[i]->group_id(0));
        host_out[i] = out_bo[i]->map<short *>();
    }

    //kernel run
    xrt::run s2mm_run[NplKernels];
    xrt::run gen2s_run[NplKernels];

    // graph run
    auto cghdl = xrt::graph(device,uuid,"G");
    cghdl.reset();
    std::cout << "graph Reset" << std::endl;
    
    // Update RTPs Before Run
    int16_t coef1[] = {1,0};
    // int16_t coef2[] = {2,0};
    // int16_t coef3[] = {4,0};

    std::string AntName[NAntenna];
    for(int i=0;i<NAntenna;i++)
    {
        AntName[i] = "G.G.";
        for(int k = 0;k<NAntenna-i-1;k++)
            AntName[i] = AntName[i] + "RG.";
        AntName[i] = AntName[i] + "G.";
    }

    for(int i=0;i<NAntenna;i++)
    {
        cghdl.update(AntName[i]+"gaink.in[1]",1);
        cghdl.update(AntName[i]+"filt.in[1]",*((int *)coef1));

    }
    // cghdl.update("G.G.RG.RG.RG.G.gaink.in[1]",1);
    // cghdl.update("G.G.RG.RG.RG.G.filt.in[1]",*((int *)coef1));

    // cghdl.update("G.G.RG.RG.RG.G.gaink.in[1]",1);
    // cghdl.update("G.G.RG.RG.RG.G.filt.in[1]",*((int *)coef1));
    // cghdl.update("G.G.RG.RG.G.gaink.in[1]",1);
    // cghdl.update("G.G.RG.RG.G.filt.in[1]",*((int *)coef1));
    // cghdl.update("G.G.RG.G.gaink.in[1]",1);
    // cghdl.update("G.G.RG.G.filt.in[1]",*((int *)coef1));
    // cghdl.update("G.G.G.gaink.in[1]",1);
    // cghdl.update("G.G.G.filt.in[1]",*((int *)coef1));
    std::cout << "Coefficients Updated" << std::endl;

#if USE_XRT_EVENT_API == true
    xrt::aie::profiling handle[]={xrt::aie::profiling(device), xrt::aie::profiling(device)};
    handle[0].start(xrt::aie::profiling::profiling_option::io_stream_start_to_bytes_transferred_cycles, "G.out[0]", "", profile_output_size_in_bytes);
    handle[1].start(xrt::aie::profiling::profiling_option::io_stream_start_to_bytes_transferred_cycles, "G.out[1]", "", profile_output_size_in_bytes); 
    
    for(int i=0;i<NplKernels;i++)
    {
        s2mm_run[i] = (*s2mm[i])(*out_bo[i],nullptr,MEM_SIZE(Niter));
        gen2s_run[i] = (*gen2s[i])( nullptr, MEM_SIZE(Niter)); 
    }
    cghdl.run(Niter);
    
    std::cout << "Wait for Output ports end" << std::endl;
   for (int i = 0; i < NioStreams; i++) // output ports have finished
        s2mm_run[i].wait();

    long long cycle_count0 = handle[0].read();
    double throughput0 = (double)profile_output_size_in_bytes / (cycle_count0 * 0.8 * 1e-3);
    std::cout<<"Throughput of the Output_0: "<<throughput0<<" MB/s"<<std::endl;

    long long cycle_count1 = handle[1].read();
    double throughput1 = (double)profile_output_size_in_bytes / (cycle_count1 * 0.8 * 1e-3);
    std::cout<<"Throughput of the Output_1: "<<throughput1<<" MB/s"<<std::endl;

    handle[0].stop();
    handle[1].stop();

    std::cout << "Wait for Input ports end" << std::endl;
    for (int i = 0; i < NioStreams; i++) // output ports have finished
            gen2s_run[i].wait();

    std::cout << "Wait for graph end" << std::endl;
    cghdl.end();

#else
     for(int i=0;i<NplKernels;i++)
    {
        s2mm_run[i] = (*s2mm[i])(*out_bo[i],nullptr,MEM_SIZE(Niter));
        gen2s_run[i] = (*gen2s[i])( nullptr, MEM_SIZE(Niter)); 
    }

    cghdl.run(Niter);

    std::cout << "Wait for graph end" << std::endl;
    cghdl.end();

    std::cout << "Wait for Output ports end" << std::endl;
    for (int i = 0; i < NioStreams; i++) // output ports have finished
            s2mm_run[i].wait();

    std::cout << "Wait for Input ports end" << std::endl;
    for (int i = 0; i < NioStreams; i++) // output ports have finished
            gen2s_run[i].wait();
#endif


    std::cout << "Syncing buffers" << std::endl;
    for(int i=0;i<NioStreams;i++) out_bo[i]->sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    int match = 0;

    // std::ofstream out("out.txt",std::ofstream::out);
    // for (int i = 0; i < OUTPUT_SIZE; i++) {
    // 	if(host_out1[i]!=0 || host_out2[i]!=0){
    // 		std::cout<<"ERROR: i="<<i<<" gold="<< 0 <<" out1="<<host_out1[i]<<" out2="<<host_out2[i]<<std::endl;
    // 		match=1;
    // 		break;
    // 	}
    // 	out<<host_out1[i]<< "   " <<host_out2[i]<<std::endl;
    // }
    // out.close();

    return match;
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
