/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#ifndef __ALIGNEDALLOCATOR_H__
#define __ALIGNEDALLOCATOR_H__


/* *************************************************************************** 

AlignedAllocator

This struct provides an 4k alligned memory allocator. Using this
allocator allows data objects to be aligned for efficient data
transfer to the kernel.

The struct provides an allocate and deallocate function

*************************************************************************** */
template <typename T>
struct AlignedAllocator
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


#endif
