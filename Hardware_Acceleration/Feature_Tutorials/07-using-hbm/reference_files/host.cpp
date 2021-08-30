/*
* Copyright 2021 Xilinx, Inc.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*   http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "host.hpp"

static uint64_t get_duration_ns(const std::vector<cl::Event>& events) {
    uint64_t duration = 0;
    for (size_t i=0; i<events.size(); i++) {
        uint64_t start, end;
        events[i].getProfilingInfo<uint64_t>(CL_PROFILING_COMMAND_START, &start);
        events[i].getProfilingInfo<uint64_t>(CL_PROFILING_COMMAND_END, &end);
        duration += end - start;
    }
    return duration;
}

int main(int argc, char** argv)
{
  if (argc != 6) {
    std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
    return EXIT_FAILURE;
  }

  std::string binaryFile = argv[1];
  bool addRandom = atoi (argv[3]);
  std::string _addRandom = std::to_string(addRandom);
  unsigned int krnl_loop = atoi (argv[4]);
  unsigned int txSize = atoi (argv[5]);

  long unsigned int vector_size_bytes = (long unsigned int)atoi (argv[2])<<20; // Converted to MB

  cl_int err;
  unsigned fileBufSize;
  size_t numIter = 1;

  /* Reducing the data size for emulation mode */
  char *xcl_mode = getenv("XCL_EMULATION_MODE");
  if (xcl_mode != NULL) {
      std::cout << "** operating in emulation: we are changing some args values to make the emulation run shorter and faster!"
          << std::endl;
     vector_size_bytes = 64 * 1024;
     if (addRandom == 1 && krnl_loop < 16) {
         /* if random, then need to force a largish number of repeats
          * to make sure all the range have more chances to being updated
          * and not create false mismatches; since this is not guaranteed
          * we will still count the number of mismatches */
         krnl_loop=16;
     }
  }

  long unsigned int total_data_size = vector_size_bytes/sizeof(int) ; // Convert to number of integer words

  printf("\n Total Data of %2.3f Mbytes to be written to global memory from host\n ", (float) vector_size_bytes/1024/1024);
  printf("\n Kernel is invoked %zu time and repeats itself %d times \n\n", numIter, krnl_loop);

  // Allocate Memory in Host Memory
  std::vector<int,aligned_allocator<int>> source_in1(total_data_size);
  std::vector<int,aligned_allocator<int>> source_in2(total_data_size);
  std::vector<int,aligned_allocator<int>> source_hw_results(total_data_size);
//  std::vector<int,aligned_allocator<int>> source_sw_results(total_data_size);

  // Create the test data
  for(long unsigned int i = 0 ; i < total_data_size ; i++){
     source_in1[i] = rand() ;//% total_data_size;
     source_in2[i] = rand() ;//% total_data_size;
//     source_sw_results[i] = source_in1[i] + source_in2[i];
     source_hw_results[i] = 0;
  }


	
// ------------------------------------------------------------------------------------
// Step 1: Get All PLATFORMS, then search for Target_Platform_Vendor (CL_PLATFORM_VENDOR)
//	   Search for Platform: Xilinx
// Check if the current platform matches Target_Platform_Vendor
// ------------------------------------------------------------------------------------	
  std::vector<cl::Device> devices = get_devices("Xilinx");
  devices.resize(1);
  cl::Device device = devices[0];
  std::cout << "DEVICE " << device.getInfo<CL_DEVICE_NAME>() << std::endl;


// ------------------------------------------------------------------------------------
// Step 1: Create Context
// ------------------------------------------------------------------------------------
  OCL_CHECK(err, cl::Context context(device, NULL, NULL, NULL, &err));
	
// ------------------------------------------------------------------------------------
// Step 1: Create Command Queue
// ------------------------------------------------------------------------------------
  OCL_CHECK(err, cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE, &err));

// ------------------------------------------------------------------
// Step 1: Load Binary File from disk
// ------------------------------------------------------------------		
  char* fileBuf = read_binary_file(binaryFile, fileBufSize);
  cl::Program::Binaries bins{{fileBuf, fileBufSize}};
	
// -------------------------------------------------------------
// Step 1: Create the program object from the binary and program the FPGA device with it
// -------------------------------------------------------------	
  OCL_CHECK(err, cl::Program program(context, devices, bins, NULL, &err));
  delete[] fileBuf; // delete now, why wait any longer

// -------------------------------------------------------------
// Step 1: Create Kernels
// -------------------------------------------------------------
  OCL_CHECK(err, cl::Kernel krnl_vector_add(program,"vadd", &err));

  cl::Buffer buffer_in1[numIter];
  cl::Buffer buffer_in2[numIter];
  cl::Buffer buffer_output[numIter];
  std::vector<cl::Event> events_enqueueTask(numIter);

  // Create events for read,compute and write
  std::vector<cl::Event> host_2_device_Wait;
  std::vector<cl::Event> krnl_Wait;
  std::vector<cl::Event> device_2_host_Wait;
  cl::Event host_2_device_Done, krnl_Done, device_2_host_Done;


  double kernel_time_in_sec = 0, result = 0;
  std::chrono::duration<double> kernel_time(0);

//  auto kernel_start = std::chrono::high_resolution_clock::now();

  for (size_t j = 0; j < numIter; j++) {
    std::cout << "- host loop iteration #" << j << " of " << numIter << " total iterations" << std::endl;

    OCL_CHECK(err, buffer_in1[j] = cl::Buffer(context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, vector_size_bytes/numIter, &source_in1[j*total_data_size/numIter], &err));
    OCL_CHECK(err, buffer_output[j] = cl::Buffer(context,CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, vector_size_bytes/numIter, &source_hw_results[j*total_data_size/numIter], &err));
    OCL_CHECK(err, buffer_in2[j] = cl::Buffer(context,CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, vector_size_bytes/numIter, &source_in2[j*total_data_size/numIter], &err));

    int size = total_data_size/numIter;

    OCL_CHECK(err, err = krnl_vector_add.setArg(2, buffer_output[j]));
    OCL_CHECK(err, err = krnl_vector_add.setArg(0, buffer_in1[j]));
    OCL_CHECK(err, err = krnl_vector_add.setArg(1, buffer_in2[j]));
    OCL_CHECK(err, err = krnl_vector_add.setArg(3, size));
    OCL_CHECK(err, err = krnl_vector_add.setArg(4, krnl_loop));
    OCL_CHECK(err, err = krnl_vector_add.setArg(5, addRandom));

    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_in1[j], buffer_in2[j]},0/* 0 means from host*/));	
	
    q.finish();
    auto kernel_start = std::chrono::high_resolution_clock::now();
    OCL_CHECK(err, err = q.enqueueTask(krnl_vector_add,NULL,&events_enqueueTask[j]));
    q.finish();
    auto kernel_end = std::chrono::high_resolution_clock::now();
    kernel_time += std::chrono::duration<double>(kernel_end - kernel_start);
	
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_output[j]},CL_MIGRATE_MEM_OBJECT_HOST));

  }
  q.finish();
//  auto kernel_end = std::chrono::high_resolution_clock::now();
//  kernel_time = std::chrono::duration<double>(kernel_end - kernel_start);
  kernel_time_in_sec = kernel_time.count();
  std::cout << "kernel_time_in_sec = " << kernel_time_in_sec << std::endl;
  auto duration_ns = get_duration_ns(events_enqueueTask);
  std::cout << "Duration using events profiling: " << duration_ns << " ns" << std::endl;


  // Compare the results of the Device to the simulation
  long unsigned int match_count = 0;
  long unsigned int mismatch_count = 0;
  for (size_t i = 0 ; i < total_data_size ; i++){
      // when addRandom is used, the
    int source_sw_results = source_in1[i] + source_in2[i];
    if (source_hw_results[i] != source_sw_results){
        mismatch_count++;
        if(mismatch_count<10) {
      std::cout << "Error: Result mismatch" << std::endl;
      std::cout << "i = " << i << " CPU result = " << source_sw_results
                << " Device result = " << source_hw_results[i] << std::endl;
        }
   } else {
       match_count++;
   }
  }
  std::cout << " match_count = " << match_count << " mismatch_count = " << mismatch_count << " total_data_size = " << total_data_size << std::endl;
// ============================================================================
// Step 3: Release Allocated Resources
// ============================================================================
//  delete[] fileBuf;

  // Multiplying the actual data size by 4 because four buffers are being
  // used.
  std::string _size = std::to_string(vector_size_bytes/(1024*1024));
  result = vector_size_bytes * krnl_loop;
  double bandwidth2 = double(result) / duration_ns;
  result /= 1024;               // to KB
  result /= 1024;               // to MB
  result /= 1024;               // to GB

  result /= kernel_time_in_sec; // to GBps

  std::cout << "Throughput Achieved = " << bandwidth2 << " GB/s" << std::endl; // Kilo units as 1000
  bool match = addRandom ? match_count > (total_data_size / 2) : mismatch_count == 0;
  std::cout << "TEST " << (match ? "PASSED" : "FAILED") << std::endl;

  const char *path="../../makefile/Run.perf";
  std::ofstream outFile;
  outFile.open(path, std::ios_base::app);

  std::string mTopc;
  if(vector_size_bytes/(1024*1024)==256) mTopc = "M0->PC0";
  else if(vector_size_bytes/(1024*1024)==512) mTopc = "M0->PC0_1";
  else if(vector_size_bytes/(1024*1024)==1024) mTopc = "M0->PC0_3";
  else mTopc = "Unknown";

  if (addRandom)  
  outFile << "Random         " <<  vector_size_bytes/(1024*1024) << " " << mTopc << "             " <<  txSize << "                    "  << bandwidth2 << std::endl;
  else
  outFile << "Sequential     " <<  vector_size_bytes/(1024*1024)  << " " << mTopc << "             " <<  txSize << "                    "  << bandwidth2 << std::endl;

  return (match ? EXIT_SUCCESS : EXIT_FAILURE);
}
