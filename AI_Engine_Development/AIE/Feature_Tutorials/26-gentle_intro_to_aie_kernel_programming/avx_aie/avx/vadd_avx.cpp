/********************************************************************
* Copyright © 2025 Advanced Micro Devices, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
********************************************************************/

#include <iostream>
#include <immintrin.h>

using namespace std;

int main() {
    constexpr unsigned vsize = 8;

    alignas(16) const float x[] = { 1.0,  2.0,  3.0,  4.0,  5.0,  6.0,  7.0,  8.0 };
    alignas(16) const float y[] = { 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0 };
    alignas(16) float z[vsize];         // z = x + y

    __m256 vx = _mm256_loadu_ps(x);     // transfer data from memory to 256-bit vector registers
    __m256 vy = _mm256_loadu_ps(y);

    __m256 vz = _mm256_add_ps(vx, vy);  // perform SIMD addition

    _mm256_storeu_ps(z, vz);            // transfer data from 256-bit vector register to memory

    cout << endl;
    for (auto i = 0u; i < vsize; i++) {
        if (i == 0) cout << "z = ";
        cout << z[i];
        if (i != (vsize - 1)) cout << ", ";
        else cout << endl;
    } // end for (auto i = 0u; i < vsize; i++)
    cout << endl;

    return (0);

} // end main()
