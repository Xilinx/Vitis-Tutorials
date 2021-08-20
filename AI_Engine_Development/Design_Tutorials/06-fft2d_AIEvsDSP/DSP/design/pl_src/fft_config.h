// © Copyright 2021 Xilinx, Inc.
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

#pragma once

#if MAT_ROWS == 32 && MAT_COLS == 64
   const char FFT_ROWS_NFFT_MAX       = 6;
   const char FFT_ROWS_CONFIG_WIDTH   = 8;
   const char FFT_ROWS_STAGES_BLK_RAM = 0;
   const char FFT_ROWS_SCALING        = 0;
   
   const char FFT_COLS_NFFT_MAX       = 5;
   const char FFT_COLS_CONFIG_WIDTH   = 8;
   const char FFT_COLS_STAGES_BLK_RAM = 0;
   const char FFT_COLS_SCALING        = 0;

#elif MAT_ROWS == 64 && MAT_COLS == 128
   const char FFT_ROWS_NFFT_MAX       = 7;
   const char FFT_ROWS_CONFIG_WIDTH   = 16;
   const char FFT_ROWS_STAGES_BLK_RAM = 0;
   const char FFT_ROWS_SCALING        = 0;
   
   const char FFT_COLS_NFFT_MAX       = 6;
   const char FFT_COLS_CONFIG_WIDTH   = 8;
   const char FFT_COLS_STAGES_BLK_RAM = 0;
   const char FFT_COLS_SCALING        = 0;

#elif MAT_ROWS == 128 && MAT_COLS == 256
   const char FFT_ROWS_NFFT_MAX       = 8;
   const char FFT_ROWS_CONFIG_WIDTH   = 16;
   const char FFT_ROWS_STAGES_BLK_RAM = 1;
   const char FFT_ROWS_SCALING        = 0;
   
   const char FFT_COLS_NFFT_MAX       = 7;
   const char FFT_COLS_CONFIG_WIDTH   = 16;
   const char FFT_COLS_STAGES_BLK_RAM = 1;
   const char FFT_COLS_SCALING        = 0;

#elif MAT_ROWS == 256 && MAT_COLS == 512
   const char FFT_ROWS_NFFT_MAX       = 9;
   const char FFT_ROWS_CONFIG_WIDTH   = 16;
   const char FFT_ROWS_STAGES_BLK_RAM = 2;
   const char FFT_ROWS_SCALING        = 0;
   
   const char FFT_COLS_NFFT_MAX       = 8;
   const char FFT_COLS_CONFIG_WIDTH   = 16;
   const char FFT_COLS_STAGES_BLK_RAM = 2;
   const char FFT_COLS_SCALING        = 0;

#elif MAT_ROWS == 1024 && MAT_COLS == 2048
   const char FFT_ROWS_NFFT_MAX       = 11;
   const char FFT_ROWS_CONFIG_WIDTH   = 16;
   const char FFT_ROWS_STAGES_BLK_RAM = 4;
   const char FFT_ROWS_SCALING        = 0;
   
   const char FFT_COLS_NFFT_MAX       = 10;
   const char FFT_COLS_CONFIG_WIDTH   = 16;
   const char FFT_COLS_STAGES_BLK_RAM = 4;
   const char FFT_COLS_SCALING        = 0;

#endif
