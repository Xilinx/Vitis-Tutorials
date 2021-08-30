/*
 * Copyright 2020 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* This is Sample file for Csim and cosim */
#ifndef _V_MULTI_SCALER_CONFIG_H_
#define _V_MULTI_SCALER_CONFIG_H_


//define macro for horizontal scaler
#define HSC_BILINEAR 0
#define HSC_BICUBIC 1
#define HSC_POLYPHASE 2

#define MAX_OUTS 3

#define HSC_NR_COMPONENTS 3      // 3 can handle RGB, YUV 444, 422, 420, 2 can handle 422 and 420 only
#define HSC_SAMPLES_PER_CLOCK 2  // 1, 2, 4
#define HSC_MAX_WIDTH 1920       // Determines BRAM usage
#define HSC_MAX_HEIGHT 1080      // No impact on resources
#define HSC_BITS_PER_COMPONENT 8 // 8, 10
#define HSC_BITS_PER_CLOCK (HSC_NR_COMPONENTS * HSC_BITS_PER_COMPONENT * HSC_SAMPLES_PER_CLOCK)
#define HSC_PHASE_SHIFT 6 // Number of bits used for phase
#define HSC_PHASES (1 << HSC_PHASE_SHIFT)
#define HSC_SCALE_MODE 0 // 0 - Bilinear 1 - Bicubic  2 - Polyphase
#define AXIMM_DATA_WIDTH_420 (8 * HSC_SAMPLES_PER_CLOCK)

#if (HSC_SCALE_MODE == HSC_BILINEAR)
#define HSC_ARRAY_SIZE (4 * HSC_SAMPLES_PER_CLOCK)
#else
#define HSC_ARRAY_SIZE (HSC_TAPS + (HSC_SAMPLES_PER_CLOCK * 2)) // if the HSC_ARRAY_SIZE is not a factor of HSC_MAX_SMPLS_PERCLK, then need to make it to be factor of HSC_MAX_SMPLS_PERCLK
#endif

#if (HSC_SCALE_MODE == HSC_BILINEAR)
#define HSC_TAPS 2 // Fixed to 2
#endif

#if (HSC_SCALE_MODE == HSC_BICUBIC)
#define HSC_TAPS 4 // Fixed to 4
#endif

#if (HSC_SCALE_MODE == HSC_POLYPHASE)
#define HSC_TAPS 6 // 6, 8, 10, 12
#endif


//define macro for vertical scaler
#define VSC_BILINEAR 0
#define VSC_BICUBIC 1
#define VSC_POLYPHASE 2

#define VSC_NR_COMPONENTS HSC_NR_COMPONENTS
#define VSC_SAMPLES_PER_CLOCK HSC_SAMPLES_PER_CLOCK
#define VSC_MAX_WIDTH HSC_MAX_WIDTH
#define VSC_MAX_HEIGHT HSC_MAX_HEIGHT
#define VSC_BITS_PER_COMPONENT HSC_BITS_PER_COMPONENT
#define VSC_BITS_PER_CLOCK (VSC_NR_COMPONENTS * VSC_BITS_PER_COMPONENT * VSC_SAMPLES_PER_CLOCK)
#define VSC_PHASE_SHIFT HSC_PHASE_SHIFT
#define VSC_PHASES (1 << VSC_PHASE_SHIFT)
#define VSC_SCALE_MODE HSC_SCALE_MODE
#define AXIMM_DATA_WIDTH_420 (8 * HSC_SAMPLES_PER_CLOCK)

#if (VSC_SCALE_MODE == VSC_BILINEAR)
#define VSC_TAPS 2 // Fixed to 2
#endif

#if (VSC_SCALE_MODE == VSC_BICUBIC)
#define VSC_TAPS 4 // Fixed to 4
#endif

#if (VSC_SCALE_MODE == VSC_POLYPHASE)
#define VSC_TAPS HSC_TAPS
#endif

// Defines for DMA
#define AXIMM_DATA_WIDTH (HSC_SAMPLES_PER_CLOCK * 64)
#define AXIMM_DATA_WIDTH8 (AXIMM_DATA_WIDTH / 8)
#define AXIMM_NUM_OUTSTANDING 4
#define AXIMM_BURST_LENGTH 16

#define MAX_NR_PLANES 2 // 1 for packed format only, 2 for packed and semi-planar formats only, 3 for packed, semi-planar and planar formats

// PLANE0_STREAM_DEPTH is (MAX_COLS + (AXIMM_DATA_WIDTH/8)-1) / (AXIMM_DATA_WIDTH/8)
#define PLANE0_STREAM_DEPTH ((HSC_MAX_WIDTH + AXIMM_DATA_WIDTH8 - 1) / AXIMM_DATA_WIDTH8)

#define HAS_YUYV8 0
#define HAS_RGBA8_YUVA8 0
#define HAS_RGBX10_YUVX10 0
#define HAS_Y_UV8_Y_UV8_420 1
#define HAS_RGB8_YUV8 0
#define HAS_Y_UV10_Y_UV10_420 0
#define HAS_Y8 0
#define HAS_Y10 0
#define HAS_BGRA8 0
#define HAS_BGRX8 0
#define HAS_UYVY8 0
#define HAS_BGR8 1
#define HAS_R_G_B8 0
#define HAS_Y_U_V8_420 0

#define USE_URAM 0
#define dbg 0

#endif // _V_MULTI_SCALER_CONFIG_H_
