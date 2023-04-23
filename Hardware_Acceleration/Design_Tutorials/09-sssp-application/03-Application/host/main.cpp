/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/
   
    #ifndef HLS_TEST
    #include "xcl2.hpp"
    #else
    #include "shortestPath_top.hpp"
    #endif
    #include "xf_utils_sw/logger.hpp"
    #include "ap_int.h"
    #include "utils.hpp"
    #include <cstring>
    #include <fstream>
    #include <iostream>
    #include <sstream>
    #include <sys/time.h>
    #include <vector>
    #include <limits>

    
    #define XCL_BANK(n) (((unsigned int)(n)) | XCL_MEM_TOPOLOGY)
    #define XCL_BANK0 XCL_BANK(0)
    #define XCL_BANK1 XCL_BANK(1)
    #define XCL_BANK2 XCL_BANK(2)
    #define XCL_BANK3 XCL_BANK(3)
    #define XCL_BANK4 XCL_BANK(4)
    #define XCL_BANK5 XCL_BANK(5)
    #define XCL_BANK6 XCL_BANK(6)
    #define XCL_BANK7 XCL_BANK(7)

    #define XCL_BANK8 XCL_BANK(8)
    #define XCL_BANK9 XCL_BANK(9)
    #define XCL_BANK10 XCL_BANK(10)
    #define XCL_BANK11 XCL_BANK(11)
    #define XCL_BANK12 XCL_BANK(12)
    #define XCL_BANK13 XCL_BANK(13)
    #define XCL_BANK14 XCL_BANK(14)
    #define XCL_BANK15 XCL_BANK(15)

    #define XCL_BANK16 XCL_BANK(16)
    #define XCL_BANK17 XCL_BANK(17)
    #define XCL_BANK18 XCL_BANK(18)
    #define XCL_BANK19 XCL_BANK(19)
    #define XCL_BANK20 XCL_BANK(20)
    #define XCL_BANK21 XCL_BANK(21)
    #define XCL_BANK22 XCL_BANK(22)
    #define XCL_BANK23 XCL_BANK(23)





    class ArgParser {
    public:
        ArgParser(int& argc, const char** argv) {
            for (int i = 1; i < argc; ++i) mTokens.push_back(std::string(argv[i]));
        }
        bool getCmdOption(const std::string option, std::string& value) const {
            std::vector<std::string>::const_iterator itr;
            itr = std::find(this->mTokens.begin(), this->mTokens.end(), option);
            if (itr != this->mTokens.end() && ++itr != this->mTokens.end()) {
                value = *itr;
                return true;
            }
            return false;
        }

    private:
        std::vector<std::string> mTokens;
    };


    auto constexpr num = 3;

   int main(int argc, const char* argv[]) {
        std::cout << "\n---------------------Short path compute----------------\n";
        xf::common::utils_sw::Logger logger(std::cout, std::cerr);
        cl_int fail;

        // cmd parser
        ArgParser parser(argc, argv);

        std::string xclbin_path;
        #ifndef HLS_TEST
        if (!parser.getCmdOption("-xclbin", xclbin_path)) {
            std::cout << "ERROR:xclbin path is not set!\n";
            return 1;
        }
         #endif

        #ifndef HLS_TEST

        
        std::string offsetfile;
        std::string indices_weightsfile;
        std::string nodefile;

        if (!parser.getCmdOption("-n", nodefile)) { // Node
            std::cout << "ERROR: offset file path is not set!\n";
            return -1;
        }
        if (!parser.getCmdOption("-o", offsetfile)) { // offset
            std::cout << "ERROR: offset file path is not set!\n";
            return -1;
        }
        if (!parser.getCmdOption("-c", indices_weightsfile)) { // column
            std::cout << "ERROR: indices_weights file path is not set!\n";
            return -1;
        }
        #else
        std::string offsetfile = "../csr_data/offset.mtx";
        std::string indice_weightsfile = "../csr_data/indices_weights.mtx";
        std::string nodefile = "../csr_data/node.mtx"
        #endif

        char line[1024] = {0};
        int numVertices;
        int numEdges;
        unsigned int sourceID = 0;
	std::string city;

        std::fstream offsetfstream(offsetfile.c_str(), std::ios::in);
        if (!offsetfstream) {
            std::cout << "Error : " << offsetfile << " file doesn't exist !" << std::endl;
            exit(1);
        }

        offsetfstream.getline(line, sizeof(line));
        std::stringstream numOdata(line);
        numOdata >> numVertices;
	    numOdata >> city;

        std::cout << "numVertices: "<<numVertices << std::endl;
        
        int index = 0;

        ap_uint<32>* offset32 = aligned_alloc< ap_uint<32> >(numVertices + 1);
        while (offsetfstream.getline(line, sizeof(line))) {
            std::stringstream data(line);
            data >> offset32[index];
            index++;
        }

        ap_uint<512>* offset = reinterpret_cast<ap_uint<512>*>(offset32);
  
    // read node information

        std::fstream nodefstream(nodefile.c_str(), std::ios::in);
        if (!nodefstream) {
            std::cout << "Error : " << nodefile << " file doesn't exist !" << std::endl;
            exit(1);
        }
        
        index =0;
        float* longitude = aligned_alloc<float>(numVertices);
        float* latitude = aligned_alloc<float>(numVertices);
        while (nodefstream.getline(line, sizeof(line))) {
            std::stringstream data(line);
            data >> longitude[index];
            data >> latitude[index];
            index++;
        }

    // read indices and weights
        std::fstream indicesfstream(indices_weightsfile.c_str(), std::ios::in);
        if (!indicesfstream) {
            std::cout << "Error : " << indices_weightsfile << " file doesn't exist !" << std::endl;
            exit(1);
        }


        indicesfstream.getline(line, sizeof(line));
        std::stringstream numIdata(line);
        numIdata >> numEdges;

        std::cout << "numEdages: " <<numEdges << std::endl;

        ap_uint<32>* column32 = aligned_alloc< ap_uint<32> >(numEdges);
        
        float* cost = aligned_alloc<float>(numEdges);
        float* flow_free_time = aligned_alloc<float>(numEdges);
        float* Volume = aligned_alloc<float>(numEdges);
        float* capacity = aligned_alloc<float>(numEdges); 
        float* weight = aligned_alloc<float>(numEdges);

        index = 0;

        while (indicesfstream.getline(line, sizeof(line))) {
            std::stringstream data(line);
            data >> column32[index];
            data >> cost[index];
            data >> flow_free_time[index];
            data >> Volume[index];
            data >> capacity[index];
            index++;
        }



    // generate  column512

        ap_uint<512>* column = reinterpret_cast<ap_uint<512>*>(column32);
    
    // Info

        ap_uint<8>* info = aligned_alloc<ap_uint<8> >(4);
        memset(info, 0, 4 * sizeof(ap_uint<8>));

    // Result

        float* result;
        result = aligned_alloc<float>(((numVertices + 1023) / 1024) * 1024);

    // Outdata

        float* out;
        out = aligned_alloc<float>(((3*numVertices + 1023) / 1024) * 1024);

    // Parent node

        ap_uint<32>* pred;

        pred = aligned_alloc<ap_uint<32> >(((numVertices + 1023) / 1024) * 1024);

    // Queue

        ap_uint<32>* ddrQue = aligned_alloc<ap_uint<32> >(10 * 300 * 4096);

    // Config

        ap_uint<32>* config = aligned_alloc<ap_uint<32> >(6);
        config[0] = numVertices;
        union f_cast {
            float f;
            unsigned int i;
        };
        f_cast tmp;
        tmp.f = std::numeric_limits<float>::infinity();
        config[1] =  tmp.i; //-1;
        config[2] = 0;
        config[3] = 10 * 300 * 4096;
        ap_uint<32> cmd;
        cmd.set_bit(0, 1); // enable weight?
        cmd.set_bit(1, 1); // enable predecessor?
        cmd.set_bit(2, 0); // float or fixed? 0 for float, 1 for fixed
        config[4] = cmd;
        config[5] = sourceID;

    // Kernels Setting

        #ifndef HLS_TEST

        cl::Kernel krnls_wa;
        cl::Kernel krnls_sssp;
        cl::Kernel krnls_search;
        auto devices = xcl::get_xil_devices();
        std::vector<cl_mem_ext_ptr_t> mext_o(15);
        cl::Buffer offset_buf, column_buf, weight_buf, info_buf, ddrQue_buf, result_buf, config_buf, pred_buf;
        cl::Buffer cost_buf,flow_free_time_buf,Volume_buf,capacity_buf,out_buf,longitude_buf,latitude_buf;   
        std::vector<cl::Memory> init_wa,init_sssp,init_search;
   
        auto device = devices[0];
       
        cl::Context context(device, NULL, NULL, NULL, &fail);
        logger.logCreateContext(fail);

        cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE,  &fail);
        logger.logCreateCommandQueue(fail);
        cl::Program::Binaries bins = xcl::import_binary_file(xclbin_path);
        devices.resize(1);

        cl::Program program(context, devices, bins, NULL, &fail);
        logger.logCreateProgram(fail);

        krnls_wa = cl::Kernel(program,"weight_average", &fail);
        logger.logCreateKernel(fail);

        krnls_sssp = cl::Kernel(program, "shortestPath_top", &fail);
        logger.logCreateKernel(fail);

        krnls_search = cl::Kernel(program, "search", &fail);
        logger.logCreateKernel(fail);
        
        

        std::cout <<"kernels has been created" << std::endl;


         #ifdef USE_HBM
        mext_o[0] = {(unsigned int)(0) | XCL_MEM_TOPOLOGY, offset, 0};
        mext_o[1] = {(unsigned int)(0) | XCL_MEM_TOPOLOGY, column, 0};
        mext_o[2] = {(unsigned int)(0) | XCL_MEM_TOPOLOGY, weight, 0};
        mext_o[3] = {(unsigned int)(0) | XCL_MEM_TOPOLOGY, info, 0};
        mext_o[4] = {(unsigned int)(0) | XCL_MEM_TOPOLOGY, config, 0};
        mext_o[5] = {(unsigned int)(0) | XCL_MEM_TOPOLOGY, ddrQue, 0};
        mext_o[6] = {(unsigned int)(0) | XCL_MEM_TOPOLOGY, result, 0};
        mext_o[7] = {(unsigned int)(0) | XCL_MEM_TOPOLOGY, pred, 0};
        mext_o[8] = {(unsigned int)(0) | XCL_MEM_TOPOLOGY, cost, 0};
        mext_o[9] = {(unsigned int)(0) | XCL_MEM_TOPOLOGY, flow_free_time, 0};
        mext_o[10] = {(unsigned int)(0) | XCL_MEM_TOPOLOGY, Volume, 0};
        mext_o[11] = {(unsigned int)(0) | XCL_MEM_TOPOLOGY, capacity, 0};
        mext_o[12] = {(unsigned int)(0) | XCL_MEM_TOPOLOGY, out, 0};
        mext_o[13] = {(unsigned int)(0) | XCL_MEM_TOPOLOGY, longitude, 0};
        mext_o[14] = {(unsigned int)(0) | XCL_MEM_TOPOLOGY, latitude, 0};

        #else
        mext_o[0] = {XCL_MEM_DDR_BANK0, offset, 0};
        mext_o[1] = {XCL_MEM_DDR_BANK0, column, 0};
        mext_o[2] = {XCL_MEM_DDR_BANK0, weight, 0};
        mext_o[3] = {XCL_MEM_DDR_BANK0, info, 0};
        mext_o[4] = {XCL_MEM_DDR_BANK0, config, 0};
        mext_o[5] = {XCL_MEM_DDR_BANK0, ddrQue, 0};
        mext_o[6] = {XCL_MEM_DDR_BANK0, result, 0};
        mext_o[7] = {XCL_MEM_DDR_BANK0, pred, 0};
        mext_o[8] = {XCL_MEM_DDR_BANK0, cost, 0};
        mext_o[9] = {XCL_MEM_DDR_BANK0, flow_free_time, 0};
        mext_o[10] = {XCL_MEM_DDR_BANK0, Volume, 0};
        mext_o[11] = {XCL_MEM_DDR_BANK0, capacity, 0};
        mext_o[12] = {XCL_MEM_DDR_BANK0, out, 0};
        mext_o[13] = {XCL_MEM_DDR_BANK0, longitude, 0};
        mext_o[14] = {XCL_MEM_DDR_BANK0, latitude, 0};

        #endif


        offset_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                            sizeof(ap_uint<32>) * (numVertices + 1), &mext_o[0]);

        column_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                sizeof(ap_uint<32>) * numEdges, &mext_o[1]);
        
        weight_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                sizeof(float) * numEdges, &mext_o[2]);
        
        info_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                            sizeof(ap_uint<8>) * 4, &mext_o[3]);
        
        config_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                sizeof(ap_uint<32>) * 6, &mext_o[4]);
        
        ddrQue_buf= cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                sizeof(ap_uint<32>) * 10 * 300 * 4096, &mext_o[5]);
        
        result_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                sizeof(float) * ((numVertices + 1023) / 1024) * 1024, &mext_o[6]);
        
        pred_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                            sizeof(ap_uint<32>) * ((numVertices + 1023) / 1024) * 1024, &mext_o[7]);
        
        cost_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                sizeof(float) * numEdges, &mext_o[8]);
        
        flow_free_time_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                            sizeof(float) * numEdges, &mext_o[9]);
            
        Volume_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                sizeof(float) * numEdges, &mext_o[10]);
        
        capacity_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                sizeof(float) * numEdges, &mext_o[11]);
        
        out_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                sizeof(float) * ((3*numVertices + 1023) / 1024) * 1024, &mext_o[12]);
        
        longitude_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                sizeof(float) * numVertices, &mext_o[13]);
        
        latitude_buf = cl::Buffer(context, CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR | CL_MEM_READ_WRITE,
                                sizeof(float) * numVertices , &mext_o[14]);

        
        std::cout << "kernels start------" << std::endl;

        init_wa.push_back(cost_buf);
        init_wa.push_back(flow_free_time_buf);
        init_wa.push_back(Volume_buf);
        init_wa.push_back(capacity_buf);
        init_wa.push_back(weight_buf);
        init_wa.push_back(result_buf);

        q.enqueueMigrateMemObjects(init_wa,0);
           
        int j =0;
        int size = numEdges;
        krnls_wa.setArg(j++,cost_buf);
        krnls_wa.setArg(j++,flow_free_time_buf);
        krnls_wa.setArg(j++,Volume_buf);
        krnls_wa.setArg(j++,capacity_buf);
        krnls_wa.setArg(j++,weight_buf);
        krnls_wa.setArg(j++,size);

        q.enqueueTask(krnls_wa);
        
        init_sssp.push_back(config_buf);
        init_sssp.push_back(offset_buf);
        init_sssp.push_back(column_buf);
        init_sssp.push_back(info_buf);
            
        q.enqueueMigrateMemObjects(init_sssp,0);
        
        j = 0;
        krnls_sssp.setArg(j++, config_buf);
        krnls_sssp.setArg(j++, offset_buf);
        krnls_sssp.setArg(j++, column_buf);
        krnls_sssp.setArg(j++, weight_buf);
        krnls_sssp.setArg(j++, ddrQue_buf);
        krnls_sssp.setArg(j++, ddrQue_buf);
        krnls_sssp.setArg(j++, result_buf);
        krnls_sssp.setArg(j++, result_buf);
        krnls_sssp.setArg(j++, pred_buf);
        krnls_sssp.setArg(j++, pred_buf);
        krnls_sssp.setArg(j++, info_buf);

        q.enqueueTask(krnls_sssp);
                  
        init_search.push_back(longitude_buf);
        init_search.push_back(latitude_buf);
           
        q.enqueueMigrateMemObjects(init_search,0); 
        
        j = 0;
        size = numVertices;
        krnls_search.setArg(j++,result_buf);
        krnls_search.setArg(j++,longitude_buf);
        krnls_search.setArg(j++,latitude_buf);
        krnls_search.setArg(j++,out_buf);
        krnls_search.setArg(j++,size);

        q.enqueueTask(krnls_search);
     
        q.enqueueMigrateMemObjects({out_buf},CL_MIGRATE_MEM_OBJECT_HOST);
    
        q.finish();

        std::cout << "kernels end------" << std::endl;

    /*
    ////// Option 1 - Interactive Mode //////
    
        std::cout << "********************************************************************************************* "<< std::endl;
        std::cout <<"\n---------------------" << city << " City Shortest Path from source 0----------------\n";
        std::cout << "The start point is: 0 " << "longitude: " << out[0] << " latitude: " << out[1] << std::endl;
        std::cout << "Please input your choice: " << std::endl;
        std::cout << "Input 0 : show all the result and exit."<< std::endl;
        std::cout << "Input point range in ( 1 to "<< numVertices-1 << " ): show the path from start point to the enter point."<<std::endl;
        std::cout << "Input y to exit !" << std::endl;
        
        int m;

        while(std::cin >> m){
            if(m==0)
            {
                std::cout << "Destination to point: "<< std::endl;

                    for (int i =1; i< numVertices;i++){

                            std::cout << "point" << i << "(" << "longitude: " << out[3*i] << " "<< "latitude: " <<out[3*i+1] << ") "<< "short weight path is: " <<out[3*i+2]<<std::endl;
                    }

                break;	

            }
            else if (m>numVertices-1)
            {
                std::cout << "Input point is out of range, please re-enter!" << std::endl;
            
            }
            else if (std::cin.get()=='y')
            {
                break;
            }

            else
            {
                std::cout << "point" << m << "(" << "longitude: " << out[3*m] << " "<< "latitude: " <<out[3*m+1] << ") "<< "short weight path is: " <<out[3*m+2]<<std::endl;
                std::cout << "Please continue to enter or input y to exit!" << std::endl;	
            }

        }
    */

    ////// Option 2 - Output Mode //////

        std::cout << "********************************************************************************************* "<< std::endl;
        std::cout << "The results are printing..." << std::endl;

        std::string result_file = "./result.txt";
        std::ofstream resultofstream(result_file.c_str(),std::ios::app);
        resultofstream <<"\n---------------------" << city << " City Shortest Path from source 0----------------\n";
        resultofstream  << "The start point is: 0 " << "longitude: " << out[0] << " latitude: " << out[1] << std::endl;
        resultofstream  << "Destination to point: "<< std::endl;
        for (int i =1; i< numVertices;i++){
            resultofstream  << "point" << i << "(" << "longitude: " << out[3*i] << " "<< "latitude: " <<out[3*i+1] << ") "<< "short weight path is: " <<out[3*i+2]<<std::endl;
        };
        resultofstream.close();

        std::cout << "The writing is done." << std::endl;
        return 0;

        #endif   
}



        
        




