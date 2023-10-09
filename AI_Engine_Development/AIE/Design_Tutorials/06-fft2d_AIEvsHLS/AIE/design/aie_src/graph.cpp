/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include "graph.h"

uint8_t fftCols_grInsts = 0, fftRows_grInsts = 0;

FFT2D_graph fft2d_graph;

#ifdef __AIESIM__

   int main(int argc, char ** argv)
   {
      fft2d_graph.init();
      
      fft2d_graph.run(ITER_CNT * MAT_ROWS);
      
      fft2d_graph.end();
      
      return 0;
   }

#endif
