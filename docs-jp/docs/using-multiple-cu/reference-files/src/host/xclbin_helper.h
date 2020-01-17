#pragma once

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <streambuf>
#include <algorithm>
#include <vector>

#include <CL/opencl.h>


// This struct provides an 4k alligned memory allocator. Using this allocator 
// allows data objects to be aligned for efficient data transfer to the kernel.
template <typename T>
struct aligned_allocator
{
  using value_type = T;
  T* allocate(std::size_t num)
  {
    void* ptr = nullptr;
    if (posix_memalign(&ptr,4096,num*sizeof(T)))
      throw std::bad_alloc();
    return reinterpret_cast<T*>(ptr);
  }
  void deallocate(T* p, std::size_t num)
  {
    free(p);
  }
};


void load_xclbin_file(	const char* filename, 
						cl_context &context, 
						cl_device_id &device, 
						cl_program &program   );

