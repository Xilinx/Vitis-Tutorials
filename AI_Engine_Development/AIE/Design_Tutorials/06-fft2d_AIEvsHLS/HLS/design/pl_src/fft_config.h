/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#pragma once

#if MAT_ROWS == 32 && MAT_COLS == 64
   #define FFT_ROWS_NFFT_MAX       6
   #define FFT_ROWS_CONFIG_WIDTH   8
   #define FFT_ROWS_STAGES_BLK_RAM 0
   #define FFT_ROWS_SCALING        0
   
   #define FFT_COLS_NFFT_MAX       5
   #define FFT_COLS_CONFIG_WIDTH   8
   #define FFT_COLS_STAGES_BLK_RAM 0
   #define FFT_COLS_SCALING        0

#elif MAT_ROWS == 64 && MAT_COLS == 128
   #define FFT_ROWS_NFFT_MAX       7
   #define FFT_ROWS_CONFIG_WIDTH   16
   #define FFT_ROWS_STAGES_BLK_RAM 0
   #define FFT_ROWS_SCALING        0
   
   #define FFT_COLS_NFFT_MAX       6
   #define FFT_COLS_CONFIG_WIDTH   8
   #define FFT_COLS_STAGES_BLK_RAM 0
   #define FFT_COLS_SCALING        0

#elif MAT_ROWS == 128 && MAT_COLS == 256
   #define FFT_ROWS_NFFT_MAX       8
   #define FFT_ROWS_CONFIG_WIDTH   16
   #define FFT_ROWS_STAGES_BLK_RAM 1
   #define FFT_ROWS_SCALING        0
   
   #define FFT_COLS_NFFT_MAX       7
   #define FFT_COLS_CONFIG_WIDTH   16
   #define FFT_COLS_STAGES_BLK_RAM 1
   #define FFT_COLS_SCALING        0

#elif MAT_ROWS == 256 && MAT_COLS == 512
   #define FFT_ROWS_NFFT_MAX       9
   #define FFT_ROWS_CONFIG_WIDTH   16
   #define FFT_ROWS_STAGES_BLK_RAM 2
   #define FFT_ROWS_SCALING        0
   
   #define FFT_COLS_NFFT_MAX       8
   #define FFT_COLS_CONFIG_WIDTH   16
   #define FFT_COLS_STAGES_BLK_RAM 2
   #define FFT_COLS_SCALING        0

#elif MAT_ROWS == 1024 && MAT_COLS == 2048
   #define FFT_ROWS_NFFT_MAX       11
   #define FFT_ROWS_CONFIG_WIDTH   16
   #define FFT_ROWS_STAGES_BLK_RAM 4
   #define FFT_ROWS_SCALING        0
   
   #define FFT_COLS_NFFT_MAX       10
   #define FFT_COLS_CONFIG_WIDTH   16
   #define FFT_COLS_STAGES_BLK_RAM 4
   #define FFT_COLS_SCALING        0

#endif
