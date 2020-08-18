/**********
Copyright (c) 2018, Xilinx, Inc.
All rights reserved.
Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its contributors
may be used to endorse or promote products derived from this software
without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********/

#include "host.hpp"

int main(int argc, char** argv)
{
// ------------------------------------------------------------------------------------
// Parse command line arguments
// ------------------------------------------------------------------------------------ 
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
		return EXIT_FAILURE;
	}

// ------------------------------------------------------------------------------------
// Initialize local variables 
// ------------------------------------------------------------------------------------ 
    std::string binaryFile = argv[1];
    unsigned fileBufSize;
    int size = DATA_SIZE;
    size_t vector_size_bytes = sizeof(int) * DATA_SIZE;
    std::vector<int,aligned_allocator<int>> source_in1(DATA_SIZE);
    std::vector<int,aligned_allocator<int>> source_in2(DATA_SIZE);
    std::vector<int,aligned_allocator<int>> source_hw_results(DATA_SIZE);
    std::vector<int,aligned_allocator<int>> source_sw_results(DATA_SIZE);

    // Create the test data 
    for(int i = 0 ; i < DATA_SIZE ; i++){
        source_in1[i] = rand() % DATA_SIZE;
        source_in2[i] = rand() % DATA_SIZE;
        source_sw_results[i] = source_in1[i] + source_in2[i];
        source_hw_results[i] = 0;
    }

	
// ------------------------------------------------------------------------------------
// Initialize the OpenCL environment 
// ------------------------------------------------------------------------------------	
    cl_int err;
    std::vector<cl::Device> devices = get_devices("Xilinx");
    devices.resize(1);
    cl::Device device = devices[0];
    OCL_CHECK(err, cl::Context context(device, NULL, NULL, NULL, &err));
    char* fileBuf = read_binary_file(binaryFile, fileBufSize);
    cl::Program::Binaries bins{{fileBuf, fileBufSize}};
    OCL_CHECK(err, cl::Program program(context, devices, bins, NULL, &err));
    OCL_CHECK(err, cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE, &err));
    OCL_CHECK(err, cl::Kernel krnl_vector_add(program,"vadd", &err));

// ------------------------------------------------------------------------------------
// Create buffers
// ------------------------------------------------------------------------------------
    OCL_CHECK(err, cl::Buffer buffer_in1(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, vector_size_bytes, source_in1.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_in2(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, vector_size_bytes, source_in2.data(), &err));
    OCL_CHECK(err, cl::Buffer buffer_out(context, CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY,vector_size_bytes, source_hw_results.data(), &err));

// ------------------------------------------------------------------------------------
// Run the kernel
// ------------------------------------------------------------------------------------
    OCL_CHECK(err, err = krnl_vector_add.setArg(0, buffer_in1));
    OCL_CHECK(err, err = krnl_vector_add.setArg(1, buffer_in2));
    OCL_CHECK(err, err = krnl_vector_add.setArg(2, buffer_out));
    OCL_CHECK(err, err = krnl_vector_add.setArg(3, size));

    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_in1, buffer_in2},0/* 0 means from host*/));	
    OCL_CHECK(err, err = q.enqueueTask(krnl_vector_add));
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_out},CL_MIGRATE_MEM_OBJECT_HOST));

    // Wait for all scheduled operations to finish
    q.finish();
	

// ------------------------------------------------------------------------------------
// Check Results and Release Allocated Resources
// ------------------------------------------------------------------------------------
    bool match = true;
    for (int i = 0 ; i < DATA_SIZE ; i++){
        if (source_hw_results[i] != source_sw_results[i]){
            std::cout << "Error: Result mismatch" << std::endl;
            std::cout << "i = " << i << " CPU result = " << source_sw_results[i]
                << " Device result = " << source_hw_results[i] << std::endl;
            match = false;
            break;
        }
    }
	

    delete[] fileBuf;

    std::cout << "TEST " << (match ? "PASSED" : "FAILED") << std::endl; 
    return (match ? EXIT_SUCCESS : EXIT_FAILURE);
}