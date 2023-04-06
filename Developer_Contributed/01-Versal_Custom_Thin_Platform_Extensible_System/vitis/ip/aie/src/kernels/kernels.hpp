// /**********
// © Copyright 2023 Advanced Micro Devices (AMD), Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// **********/

#pragma once

#include <adf.h>
#include <aie_api/aie.hpp>

template <typename TT_DATA = cint16, size_t BUF_SIZE = 256>
class my_scalar_dm {
  private:

  public:

    // Constructor
    my_scalar_dm();
    // Run:
    void __attribute__ ((noinline)) run(adf::input_buffer<TT_DATA, adf::extents<BUF_SIZE>> & __restrict sig_i, adf::output_buffer<TT_DATA, adf::extents<BUF_SIZE>> & __restrict sig_o );

    static void registerKernelClass()
    {
      REGISTER_FUNCTION(my_scalar_dm::run);
      //REGISTER_PARAMETER(coeffs);
    }
};

template <typename TT_DATA = cint16, size_t BUF_SIZE = 256>
class my_vector_dm {
  private:

  public:

    // Constructor
    my_vector_dm();
    // Run:
    void __attribute__ ((noinline)) run(adf::input_buffer<TT_DATA, adf::extents<BUF_SIZE>> & __restrict sig_i, adf::output_buffer<TT_DATA, adf::extents<BUF_SIZE>> & __restrict sig_o );

    static void registerKernelClass()
    {
      REGISTER_FUNCTION(my_vector_dm::run);
      //REGISTER_PARAMETER(coeffs);
    }
};

template <typename TT_DATA = cint16, size_t BUF_SIZE = 256>
class my_mul_dm {
  private:

  public:

    // Constructor
    my_mul_dm();
    // Run:
    void __attribute__ ((noinline)) run(adf::input_buffer<TT_DATA, adf::extents<BUF_SIZE>> & __restrict sig_i, adf::output_buffer<TT_DATA, adf::extents<BUF_SIZE>> & __restrict sig_o );

    static void registerKernelClass()
    {
      REGISTER_FUNCTION(my_mul_dm::run);
      //REGISTER_PARAMETER(coeffs);
    }
};

template <typename TT_DATA = cint16>
class my_stream_dm {
  private:
    typedef TT_DATA MY_TT_DATA;

  public:

    // constructor
    my_stream_dm();
    // run:
    void __attribute__ ((noinline)) run(input_stream<MY_TT_DATA> * __restrict sig_i, output_stream<MY_TT_DATA> * __restrict sig_o );

    static void registerKernelClass()
    {
      REGISTER_FUNCTION(my_stream_dm::run);
      //register_parameter(coeffs);
    }
};

