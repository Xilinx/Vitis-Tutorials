// 
// Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
// 

#pragma once
#include <adf.h>
#include "aie_api/aie_adf.hpp"
#include "ddc_include.h"

using namespace adf;

class ddc_chain_kernel {
public:
  static constexpr unsigned FIR89_COEFFICIENTS = 48;
  alignas(32) static constexpr int16_t coeffs_fir89[FIR89_COEFFICIENTS] ={
  		-16, -113, -147,    4,   82,  -51,  -48,   92,  -11, -102,   88,   60, -152,   38,  157, -161,  -72,  246,  -95, -225,  280,   63, -381,  207,  301, -471,  -10,  571, -416, -376,  785, -137, -858,  824,  439, -1385,  519, 1420, -1862, -481, 3261, -2195, -4318, 12590, 25792,    0,    0,    0
  };
  static constexpr unsigned FIR199_COEFFICIENTS = 104;
  alignas(32) static constexpr int16_t coeffs_fir199[FIR199_COEFFICIENTS] ={
  		-36,  155,  190,  -48,   16,   20,  -38,   43,  -34,   15,    8,  -29,   41,  -40,   24,    1,  -28,   46,  -50,   35,   -7,  -26,   52,  -61,   49,  -18,  -22,   58,  -75,   66,  -32,  -15,   61,  -89,   85,  -51,   -4,   62, -102,  108,  -75,   12,   59, -115,  133, -104,   35,   51, -125,  160, -139,   67,   35, -132,  188, -181,  107,   10, -133,  217, -229,  159,  -27, -126,  246, -286,  226,  -79, -109,  274, -354,  313, -155,  -75,  299, -437,  429, -264,  -15,  321, -546,  596, -434,   88,  340, -706,  868, -730,  290,  354, -1007, 1435, -1418,  820,  362, -1982, 3772, -5395, 6525, 29546,    0,    0,    0,    0
  };

  static constexpr unsigned DDC199_BUFF_SIZE = 32;
  static constexpr unsigned DDC89_BUFF_SIZE = 16;
  static constexpr unsigned HBFD23_BUFF_SIZE = 3;
  static constexpr unsigned OVLP_SIZE = ((DDC199_BUFF_SIZE)*DDC_NCH_NR+(DDC89_BUFF_SIZE+HBFD23_BUFF_SIZE)*DDC_NCH_LTE)*8;

  alignas(32) cint16 (&ddcchanfir_ovlp)[OVLP_SIZE];
  alignas(32) cint16 (&dmBufA_DDC)[DDC_LTE_SAMPLES_PERCH4D];

  ddc_chain_kernel( cint16 (&ovlp_init)[OVLP_SIZE], cint16 (&dmBufA_init)[DDC_LTE_SAMPLES_PERCH4D] );

  void run(input_buffer<cint16> & __restrict cb_input,output_buffer<cint16> & __restrict cb_output);

  static void registerKernelClass( void )
  {
    REGISTER_FUNCTION( ddc_chain_kernel::run );
    REGISTER_PARAMETER( ddcchanfir_ovlp );
    REGISTER_PARAMETER( dmBufA_DDC );
  }
};
