/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#pragma once

#include <adf.h>
#include <fft_ifft_dit_1ch_graph.hpp>

using namespace adf;

using namespace xf::dsp::aie::fft::dit_1ch;


// ------------------------------------------------------------
// FFT Namespace
// ------------------------------------------------------------

namespace fft2 {
  static constexpr int  WIN_SIZE = 2048;
  typedef cint16                              TT_TYPE;
  typedef cint16                              TT_TWIDDLE;
  static constexpr int  TP_POINT_SIZE         = 256;
  static constexpr int  TP_FFT_NIFFT          = 1;
  static constexpr int  TP_SHIFT              = 6;             // Excludes twiddle shift
  static constexpr int  TP_CASC_LEN           = 3;
  static constexpr int  TP_DYN_PT_SIZE        = 0;
  static constexpr int  TP_WINDOW_SIZE        = WIN_SIZE;
  static constexpr int  TP_API                = 1;             // Streams
  static constexpr int  TP_PARALLEL_POWER     = 1;             // Use 2 sub-frame processors with 2 inputs each
  static constexpr int  TP_USE_WIDGETS 	      = 1;             // Use widgets kernel to get the FFT performance (2023.2 onwards)
  static constexpr int NPORTS_O = 1 << (TP_PARALLEL_POWER+1);
};

// ------------------------------------------------------------
// Top Level Graph
// ------------------------------------------------------------

template<int FFT_X, int FFT_Y>
class fft2_graph : public graph {
private:

  // FFT class:
  using TT_FFT = fft_ifft_dit_1ch_graph<fft2::TT_TYPE,fft2::TT_TWIDDLE,fft2::TP_POINT_SIZE,fft2::TP_FFT_NIFFT,
                                        fft2::TP_SHIFT,fft2::TP_CASC_LEN,fft2::TP_DYN_PT_SIZE,
                                        fft2::TP_WINDOW_SIZE,fft2::TP_API,fft2::TP_PARALLEL_POWER,fft2::TP_USE_WIDGETS>;

public:
  std::array<port<input>, fft2::NPORTS_O> fft_i;
  std::array<port<output>,fft2::NPORTS_O> fft_o;

  TT_FFT fft_dut;

  fft2_graph(void) : fft_dut()
  {
    for (int ii=0; ii < fft2::NPORTS_O; ii++) {
      connect<stream>( fft_i[ii],        fft_dut.in[ii] );
      connect<stream>( fft_dut.out[ii],  fft_o[ii]      );
    }

    // My placement:
    //    Give each kernel its own tile, this gives maximum software pipelined performance:
    location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_inWidgetKernel)  = tile(FFT_X + 8, FFT_Y + 0 );
    location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_fftKernels[0])   = tile(FFT_X + 7, FFT_Y + 0 );
    location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_fftKernels[1])   = tile(FFT_X + 6, FFT_Y + 0 );
    location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_fftKernels[2])   = tile(FFT_X + 5, FFT_Y + 0 );
    location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_outWidgetKernel) = tile(FFT_X + 4, FFT_Y + 0 );
    location<kernel>(fft_dut.m_combInKernel[0])                         = tile(FFT_X + 3, FFT_Y + 0 );
    location<kernel>(fft_dut.m_r2Comb[0])                               = tile(FFT_X + 2, FFT_Y + 0 );
    location<kernel>(fft_dut.m_combOutKernel[0])                        = tile(FFT_X + 1, FFT_Y + 0 );
    location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_inWidgetKernel)  = tile(FFT_X + 0, FFT_Y + 1 );
    location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_fftKernels[0])   = tile(FFT_X + 1, FFT_Y + 1 );
    location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_fftKernels[1])   = tile(FFT_X + 2, FFT_Y + 1 );
    location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_fftKernels[2])   = tile(FFT_X + 3, FFT_Y + 1 );
    location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_outWidgetKernel) = tile(FFT_X + 4, FFT_Y + 1 );
    location<kernel>(fft_dut.m_combInKernel[1])                         = tile(FFT_X + 5, FFT_Y + 1 );
    location<kernel>(fft_dut.m_r2Comb[1])                               = tile(FFT_X + 6, FFT_Y + 1 );
    location<kernel>(fft_dut.m_combOutKernel[1])                        = tile(FFT_X + 7, FFT_Y + 1 );


    // My placement:
    //    Give each kernel its own tile, this gives maximum software pipelined performance:
//    location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_inWidgetKernel)  = tile(FFT_X + 8, FFT_Y + 0 );
//    location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_fftKernels[0])   = tile(FFT_X + 7, FFT_Y + 0 );
//    location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_fftKernels[1])   = tile(FFT_X + 6, FFT_Y + 0 );
//    location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_fftKernels[2])   = tile(FFT_X + 5, FFT_Y + 0 );
/*
    location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_fftKernels[0])   = tile(FFT_X + 5, FFT_Y + 0 );
    location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_fftKernels[1])   = tile(FFT_X + 4, FFT_Y + 0 );
    location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_fftKernels[2])   = tile(FFT_X + 3, FFT_Y + 0 );

//    location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_outWidgetKernel) = tile(FFT_X + 4, FFT_Y + 0 );
//    location<kernel>(fft_dut.m_combInKernel[0])                         = tile(FFT_X + 3, FFT_Y + 0 );
    ////location<kernel>(fft_dut.m_r2Comb[0])                               = tile(FFT_X + 2, FFT_Y + 0 );
	// location<kernel>(fft_dut.m_r2Comb[0])                               = tile(FFT_X + 4, FFT_Y + 0 );

  	// location<kernel>(fft_dut.m_r2Comb[0])                               = tile(26, FFT_Y + 0 );
	location<kernel>(fft_dut.m_r2Comb[0])                               = tile(25, FFT_Y + 0 );
//    location<kernel>(fft_dut.m_combOutKernel[0])                        = tile(FFT_X + 1, FFT_Y + 0 );
//    location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_inWidgetKernel)  = tile(FFT_X + 0, FFT_Y + 1 );
//    location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_fftKernels[0])   = tile(FFT_X + 1, FFT_Y + 1 );
//    location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_fftKernels[1])   = tile(FFT_X + 2, FFT_Y + 1 );
//    location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_fftKernels[2])   = tile(FFT_X + 3, FFT_Y + 1 );

    location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_fftKernels[0])   = tile(FFT_X + 0, FFT_Y + 1 );
    location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_fftKernels[1])   = tile(FFT_X + 1, FFT_Y + 1 );
    location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_fftKernels[2])   = tile(FFT_X + 2, FFT_Y + 1 );


//    location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_outWidgetKernel) = tile(FFT_X + 4, FFT_Y + 1 );
//    location<kernel>(fft_dut.m_combInKernel[1])                         = tile(FFT_X + 5, FFT_Y + 1 );
    ////location<kernel>(fft_dut.m_r2Comb[1])                               = tile(FFT_X + 6, FFT_Y + 1 );
	//location<kernel>(fft_dut.m_r2Comb[1])                               = tile(FFT_X + 4, FFT_Y + 1 );
	
	// location<kernel>(fft_dut.m_r2Comb[1])                               = tile(30, FFT_Y + 1 );
	location<kernel>(fft_dut.m_r2Comb[1])                               = tile(27 , FFT_Y + 1 );

//    location<kernel>(fft_dut.m_combOutKernel[1])                        = tile(FFT_X + 7, FFT_Y + 1 );

*/

/// Working
/*
//  location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_inWidgetKernel)  = tile(FFT_X + 4, FFT_Y + 0 );
    location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_fftKernels[0])   = tile(FFT_X + 3, FFT_Y + 0 );
    location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_fftKernels[1])   = tile(FFT_X + 2, FFT_Y + 0 );
    location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_fftKernels[2])   = tile(FFT_X + 1, FFT_Y + 0 );
//  location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_outWidgetKernel) = tile(FFT_X + 4, FFT_Y + 0 );
//  location<kernel>(fft_dut.m_combInKernel[0])                         = tile(FFT_X + 3, FFT_Y + 0 );
    location<kernel>(fft_dut.m_r2Comb[0])                               = tile(FFT_X + 0, FFT_Y + 0 );
//  location<kernel>(fft_dut.m_combOutKernel[0])                        = tile(FFT_X + 1, FFT_Y + 0 );
//  location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_inWidgetKernel)  = tile(FFT_X + 0, FFT_Y + 1 );
    location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_fftKernels[0])   = tile(FFT_X + 0, FFT_Y + 1 );
    location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_fftKernels[1])   = tile(FFT_X + 1, FFT_Y + 1 );
    location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_fftKernels[2])   = tile(FFT_X + 2, FFT_Y + 1 );
//  location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_outWidgetKernel) = tile(FFT_X + 4, FFT_Y + 1 );
//  location<kernel>(fft_dut.m_combInKernel[1])                         = tile(FFT_X + 5, FFT_Y + 1 );
    location<kernel>(fft_dut.m_r2Comb[1])                               = tile(FFT_X + 3, FFT_Y + 1 );
//  location<kernel>(fft_dut.m_combOutKernel[1])                        = tile(FFT_X + 7, FFT_Y + 1 );
*/


/*
    location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_fftKernels[0])   = tile(29, FFT_Y + 0 );
    location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_fftKernels[1])   = tile(28, FFT_Y + 0 );
    location<kernel>(fft_dut.FFTsubframe0.FFTstrproc.m_fftKernels[2])   = tile(27, FFT_Y + 0 );

    location<kernel>(fft_dut.m_r2Comb[0])                               = tile(25, FFT_Y + 0 );

    location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_fftKernels[0])   = tile(24, FFT_Y + 1 );
    location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_fftKernels[1])   = tile(25, FFT_Y + 1 );
    location<kernel>(fft_dut.FFTsubframe1.FFTstrproc.m_fftKernels[2])   = tile(26, FFT_Y + 1 );

    location<kernel>(fft_dut.m_r2Comb[1])                               = tile(29, FFT_Y + 1 );
*/
  }
};
