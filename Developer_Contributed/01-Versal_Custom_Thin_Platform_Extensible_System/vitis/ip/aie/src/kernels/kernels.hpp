//
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: X11
//

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

