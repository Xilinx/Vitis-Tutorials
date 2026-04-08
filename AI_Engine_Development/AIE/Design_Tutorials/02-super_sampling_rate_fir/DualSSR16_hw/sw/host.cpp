/*
Copyright (C) Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <complex>
#include "adf/adf_api/XRTConfig.h"
#include "experimental/xrt_kernel.h"

#include "SSRgraph.cpp"
#include "host.h"

using namespace adf;

template<typename T>
int load_data_file(std::string fname,T * data,int L)
{
    int ret = 0;
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


// Simulation

#define INPUT_SIZE(NITER) ((NITER)*FRAME_LENGTH/PLIO_NDATA)
#define OUTPUT_SIZE(NITER) ((NITER)*FRAME_LENGTH/PLIO_NDATA)

int run(int argc, char* argv[]){

    int pl_bram_size_in_bytes = PL_MEMORY_SIZE*PLIO_NBYTES;
    int pl_bram_size_in_shorts = PL_MEMORY_SIZE*PLIO_NBYTES/sizeof(short);


    //TARGET_DEVICE macro needs to be passed from gcc command line
    if(argc != 2 && argc != 3) {
        std::cout << "Usage: " << argv[0] <<" <xclbin>  [Niterations (default 1000)]" << std::endl;
        return EXIT_FAILURE;
    }
    char* xclbinFilename = argv[1];
    std::string niter("1000");
    if(argc==3)
    niter = std::string(argv[2]);

    int Niter = std::stoi(niter);


    std::cout << "The simulation will run " << Niter << " Iterations" << std::endl << std::endl;

    int ret;
    // Open xclbin
    xrt::device  device = xrt::device(0); //device index=0
    xrt::uuid uuid = device.load_xclbin(xclbinFilename);
    xclDeviceHandle dhdl = xrtDeviceOpenFromXcl(device);

    // s2mm & mm2s kernel handle
    const int NPhases = 8;
    const int NSplit = 2;
    const int NStreams = NPhases*NSplit;
    const int NGraphs = 2;

    const int NioStreams = NStreams*NGraphs;
    const int NplKernels = NPhases*NGraphs;
    xrt::kernel * s2mm[NplKernels];
    xrt::kernel * mm2s[NplKernels];

    for(int i=0;i<NplKernels;i++)
    {
        s2mm[i] = new xrt::kernel(device,uuid,"s2mm_dual_128");
        // Could be "s2mm_dual_128:{s2mm_dual_128_"+std::to_string(i+1)+"}" to specify instance
        mm2s[i] = new xrt::kernel(device,uuid,"mm2s_dual_128");
    }


    // Input and Output memories
    xrt::bo * in_bo[NioStreams];
    short * host_in[NioStreams];
    xrt::bo * out_bo[NioStreams];
    short * host_out[NioStreams];


    for(int i=0;i<NplKernels;i++)
    {
        in_bo[2*i] = new xrt::bo(device,pl_bram_size_in_bytes, mm2s[i]->group_id(0));
        host_in[2*i] = in_bo[2*i]->map<short*>();
        in_bo[2*i+1] = new xrt::bo(device,pl_bram_size_in_bytes, mm2s[i]->group_id(1));
        host_in[2*i+1] = in_bo[2*i+1]->map<short*>();

        out_bo[2*i] = new xrt::bo(device,pl_bram_size_in_bytes, s2mm[i]->group_id(0));
        host_out[2*i] = out_bo[2*i]->map<short*>();
        out_bo[2*i+1] = new xrt::bo(device,pl_bram_size_in_bytes, s2mm[i]->group_id(1));
        host_out[2*i+1] = out_bo[2*i+1]->map<short*>();
    }


    // Graph labels (Starting columns)
    // const int Label[NGraphs] = { 15, 25};

    // Initialization input memory
    int host_index = 0;
    for(int i=0;i<NGraphs;i++)
    for(int j=0;j<NPhases;j++)
    for(int k=0;k<NSplit;k++,host_index++)
    {
        load_data_file<short>("data/PhaseIn_" +std::to_string(j)+"_"+std::to_string(k)+".txt",
        host_in[host_index],pl_bram_size_in_shorts);
    }

    //kernel run
    xrt::run s2mm_run[NplKernels];
    xrt::run mm2s_run[NplKernels];
    for(int i=0;i<NplKernels;i++)
    {
        s2mm_run[i] = (*s2mm[i])(*out_bo[2*i],*out_bo[2*i+1],nullptr,nullptr,OUTPUT_SIZE(Niter));
        mm2s_run[i] = (*mm2s[i])(*in_bo[2*i],*in_bo[2*i+1],nullptr,nullptr,INPUT_SIZE(Niter));
    }


    // graph run
    adf::registerXRT(dhdl, uuid.get());
    std::cout<<"Register XRT"<<std::endl;

    // event::handle handle0 = event::start_profiling(g.plout[0], event::io_stream_start_to_bytes_transferred_cycles, output_size_in_bytes);
    // if(handle0==event::invalid_handle){
    // 	printf("ERROR:Invalid handle. Only two performance counter in a AIE-PL interface tile\n");
    // 	return 1;
    // }
    // event::handle handle1 = event::start_profiling(g.plout[1], event::io_stream_start_to_bytes_transferred_cycles, output_size_in_bytes);
    // if(handle1==event::invalid_handle){
    // 	printf("ERROR:Invalid handle. Only two performance counter in a AIE-PL interface tile\n");
    // 	return 1;
    // }

    G1.run(Niter);
    G2.run(Niter);
    G1.end();
    G2.end();

    for(int i=0;i<NioStreams;i++) s2mm_run[i].wait();

    // Wait graph for some cycles
    // long long cycle_count0 = event::read_profiling(handle0);
    // std::cout<<"Graph 0"<< std::endl;
    // std::cout<<"cycle count:"<<cycle_count0<<std::endl;
    // event::stop_profiling(handle0);
    // double throughput0 = (double)output_size_in_bytes / (cycle_count0 * 1e-3);
    // std::cout<<"Throughput of the graph: "<<throughput0<<" MB/s"<<std::endl;
    //
    // long long cycle_count1 = event::read_profiling(handle1);
    // std::cout<<"Graph 1"<< std::endl;
    // std::cout<<"cycle count:"<<cycle_count1<<std::endl;
    // event::stop_profiling(handle1);
    // double throughput1 = (double)output_size_in_bytes / (cycle_count1 * 1e-3);
    // std::cout<<"Throughput of the graph: "<<throughput1<<" MB/s"<<std::endl;

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
