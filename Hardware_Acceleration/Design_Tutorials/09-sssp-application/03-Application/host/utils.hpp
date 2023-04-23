/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/
#ifndef UTILS_H
#define UTILS_H
#include <sys/time.h>
inline int tvdiff(struct timeval* tv0, struct timeval* tv1) {
    return (tv1->tv_sec - tv0->tv_sec) * 1000000 + (tv1->tv_usec - tv0->tv_usec);
}
//--------------------------------------------------------------

#include <new>

#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <vector>

template <typename T>
T* aligned_alloc(std::size_t num) {
    void* ptr = NULL;

    if (posix_memalign(&ptr, 4096, num * sizeof(T))) throw std::bad_alloc();
    // ptr = (void*)malloc(num * sizeof(T));
    return reinterpret_cast<T*>(ptr);
}
#endif
