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
#include <CL/cl_ext_xilinx.h>

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <thread>
#include <numeric>
#include <cstring>

std::vector<std::string> kernel_names_ {"vadd:{vadd_1}", 
                                         "vadd:{vadd_2}",
                                         "vadd:{vadd_3}",
                                         "vadd:{vadd_4}",
                                         "vadd:{vadd_5}",
                                         "vadd:{vadd_6}",
                                         "vadd:{vadd_7}",
                                         "vadd:{vadd_8}",
                                         "vadd:{vadd_9}",
                                         "vadd:{vadd_10}",
                                         "vadd:{vadd_11}",
                                         "vadd:{vadd_12}",
                                         "vadd:{vadd_13}",
                                         "vadd:{vadd_14}",
                                         "vadd:{vadd_15}" } ;

static void
throw_if_error(cl_int errcode, const char* msg=nullptr)
{
  if (!errcode)
    return;
  std::string err = "errcode '";
  err.append(std::to_string(errcode)).append("'");
  if (msg)
    err.append(" ").append(msg);
  throw std::runtime_error(err);
}

static bool stop = false;

static void kernel_done(cl_event event, cl_int status, void* data);

struct job_type
{
  int runs = 0;

  int nelements;

  cl_context context = nullptr;
  cl_command_queue queue = nullptr;
  cl_program program = nullptr;
  std::string kernel_name;

  cl_kernel krnl;
  cl_mem in1;
  cl_mem in2;
  cl_mem io;
 
  int* in1_mapped;
  int* in2_mapped;
  int* io_mapped;

  const size_t bytes = sizeof(int)*nelements;


  std::vector<int> input;

  job_type(cl_context c, cl_command_queue q, cl_program p, std::string kname, const int nelements)
    : context(c), queue(q), program(p), kernel_name(kname), nelements(nelements)
  {

    input.reserve(nelements);
    std::iota(input.begin(),input.end(),0);

    cl_int err=CL_SUCCESS;
    krnl = clCreateKernel(program,kernel_name.c_str(),&err);
    throw_if_error(err,"failed to allocate kernel");

    cl_mem_ext_ptr_t host_buffer_ext;
    host_buffer_ext.flags = XCL_MEM_EXT_HOST_ONLY;
    host_buffer_ext.obj = NULL;
    host_buffer_ext.param = 0;

    in1 = clCreateBuffer(context,CL_MEM_READ_ONLY|CL_MEM_EXT_PTR_XILINX,bytes,&host_buffer_ext,&err);
    in2 = clCreateBuffer(context,CL_MEM_READ_ONLY|CL_MEM_EXT_PTR_XILINX,bytes,&host_buffer_ext,&err);
    io = clCreateBuffer(context,CL_MEM_WRITE_ONLY|CL_MEM_EXT_PTR_XILINX,bytes,&host_buffer_ext,&err);

    clSetKernelArg(krnl,2,sizeof(cl_mem),&io);
    clSetKernelArg(krnl,0,sizeof(cl_mem),&in1);
    clSetKernelArg(krnl,1,sizeof(cl_mem),&in2);
    clSetKernelArg(krnl,3,sizeof(decltype(nelements)),&nelements);

    in1_mapped = (int*) clEnqueueMapBuffer(queue,in1,true,CL_MAP_WRITE,0,bytes,0,nullptr,nullptr,&err);
    in2_mapped = (int*) clEnqueueMapBuffer(queue,in2,true,CL_MAP_WRITE,0,bytes,0,nullptr,nullptr,&err);
    io_mapped = (int*) clEnqueueMapBuffer(queue,io,true,CL_MAP_READ,0,bytes,0,nullptr,nullptr,&err);
    clFinish(queue);

    std::memcpy(in1_mapped,input.data(),bytes);
    std::memcpy(in2_mapped,input.data(),bytes);

    cl_mem mems[2] = {in1,in2};


    clEnqueueMigrateMemObjects(queue,2,mems,0,0,nullptr,nullptr);
    clEnqueueMigrateMemObjects(queue,1,&io,0,0,nullptr,nullptr);
    clFinish(queue);
  }

  ~job_type()
  {
    clReleaseKernel(krnl); 
    cl_event release_event;
    clEnqueueUnmapMemObject(queue,in1,in1_mapped,0,nullptr,&release_event);
    clWaitForEvents(1,&release_event);
    clReleaseEvent(release_event);

    clEnqueueUnmapMemObject(queue,in2,in2_mapped,0,nullptr,&release_event);
    clWaitForEvents(1,&release_event);
    clReleaseEvent(release_event);

    clEnqueueUnmapMemObject(queue,io,io_mapped,0,nullptr,&release_event);
    clWaitForEvents(1,&release_event);
    clReleaseEvent(release_event);

    clReleaseMemObject(in1);
    clReleaseMemObject(in2);
    clReleaseMemObject(io);
  }

  void
  done()
  {
    if (!stop)
      run();
  }

  void
  run()
  {
    ++runs;

    cl_int err = CL_SUCCESS;
    cl_event events[3]={nullptr};

    cl_event write_event;
    cl_event ev_kernel_done;
    cl_event read_done;
   
    cl_mem mems[2] = {in1,in2};
    clEnqueueMigrateMemObjects(queue,2,mems,0,0,nullptr,&write_event);

    clEnqueueTask(queue,krnl,1,&write_event,&ev_kernel_done);

    clEnqueueMigrateMemObjects(queue,1,&io,CL_MIGRATE_MEM_OBJECT_HOST,1,&ev_kernel_done,&read_done);

    clSetEventCallback(read_done,CL_COMPLETE,&kernel_done,this);

    clReleaseEvent(write_event);
    clReleaseEvent(ev_kernel_done);

  }

  void verify_results()
  {
    std::vector<int> result(nelements);

    for (size_t idx=0; idx<nelements; ++idx) {
      if (io_mapped[idx] != in1_mapped[idx] + in2_mapped[idx]) {
        std::cout << "got result[" << idx << "] = " << io_mapped[idx] << " expected " << in1_mapped[idx]+in2_mapped[idx] << "\n";
        throw std::runtime_error("VERIFY FAILED");
      }
    } 
  }

};

static void
kernel_done(cl_event event, cl_int status, void* data)
{
  clReleaseEvent(event);
  reinterpret_cast<job_type*>(data)->done();
} 

int
run_test(cl_context context, cl_command_queue queue, cl_program program, const int nelements)
{
  int ncu = 15;
  std::vector<job_type> jobs;
  jobs.reserve(ncu);
  for (size_t j=0; j<ncu; ++j)
    jobs.emplace_back(context,queue,program,kernel_names_[j],nelements);

  stop = false; 
  std::for_each(jobs.begin(),jobs.end(),[](job_type& j){j.run();});

  std::this_thread::sleep_for(std::chrono::seconds(20));
  stop=true;

  clFinish(queue);

  auto total_kernel_execution =0;
  for (size_t j=0; j<ncu; ++j) {
    std::cout << "kernel[" << j << "]:" << jobs[j].runs << "\n";
    total_kernel_execution+=jobs[j].runs;
  }

   std::cout<<"Total Kernel execution in 20 seconds:"<<total_kernel_execution<<"\n";
   std::cout<<"\n Data processed in 20 seconds: 4MB*total_kernel_executions:"<<total_kernel_execution*4<<" MB \n";
   double total_data_mb = (double)(total_kernel_execution*4); 
   double total_data_mb_sec = (double)(total_data_mb/20); 
   double total_data_gb_sec = (double)(total_data_mb_sec/1000); 
   std::cout<<"\n Data processed/sec (GBPs)= "<<total_data_gb_sec<<" GBPs \n";


  return 0;
}

int
run(int argc, char** argv)
{
  if (argc < 2)
    throw std::runtime_error("usage: host.exe <xclbin>");

  cl_int err = CL_SUCCESS;
  cl_platform_id platform = nullptr;
  throw_if_error(clGetPlatformIDs(1,&platform,nullptr));

  cl_uint num_devices = 0;
  throw_if_error(clGetDeviceIDs(platform,CL_DEVICE_TYPE_ACCELERATOR,0,nullptr,&num_devices));
  throw_if_error(num_devices==0,"no devices");
  std::vector<cl_device_id> devices(num_devices);
  throw_if_error(clGetDeviceIDs(platform,CL_DEVICE_TYPE_ACCELERATOR,num_devices,devices.data(),nullptr));
  cl_device_id device = devices.front();

  cl_context context = clCreateContext(0,1,&device,nullptr,nullptr,&err);
  throw_if_error(err);

  cl_command_queue queue = clCreateCommandQueue(context,device,CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,&err);
  throw_if_error(err,"failed to create command queue");

  std::string fnm = argv[1];
  std::ifstream stream(fnm);
  stream.seekg(0,stream.end);
  size_t size = stream.tellg();
  stream.seekg(0,stream.beg);
  std::vector<char> xclbin(size);
  stream.read(xclbin.data(),size);
  const unsigned char* data = reinterpret_cast<unsigned char*>(xclbin.data());
  cl_int status = CL_SUCCESS;
  cl_program program = clCreateProgramWithBinary(context,1,&device,&size,&data,&status,&err);
  throw_if_error(err,"failed to create program");

  
  int number_of_elements = 1024*512;

  double dmbytes = (number_of_elements*sizeof(int))/(((double)1024) * ((double)1024));
  std::cout<<"\n Buffer Inputs "<<dmbytes<<" MB \n";

  
  run_test(context,queue,program,number_of_elements);

  clReleaseProgram(program);
  clReleaseCommandQueue(queue);
  clReleaseContext(context);
  clReleaseDevice(device);
  std::for_each(devices.begin(),devices.end(),[](cl_device_id d){clReleaseDevice(d);});

  return 0;
}

int
main(int argc, char* argv[])
{

  try {
    run(argc,argv);
    std::cout << "TEST SUCCESS\n";
    return 0;
  }
  catch (const std::exception& ex) {
    std::cout << "TEST FAILED: " << ex.what() << "\n";
  }
  catch (...) {
    std::cout << "TEST FAILED\n";
  }

  return 1;
}
