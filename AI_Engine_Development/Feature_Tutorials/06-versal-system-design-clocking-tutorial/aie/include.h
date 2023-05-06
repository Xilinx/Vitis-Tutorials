/**********
© Copyright 2020 Xilinx, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**********/

#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#define INTERPOLATOR27_INPUT_SAMPLES 128
#define INTERPOLATOR27_INPUT_MARGIN (16*4)
#define INTERPOLATOR27_COEFFICIENTS 16
#define INTERPOLATOR27_OUTPUT_SAMPLES (INTERPOLATOR27_INPUT_SAMPLES * 2)
#define INTERPOLATOR27_INPUT_BLOCK_SIZE (INTERPOLATOR27_INPUT_SAMPLES * 4)

#define POLAR_CLIP_INPUT_SAMPLES (INTERPOLATOR27_OUTPUT_SAMPLES)
#define POLAR_CLIP_INPUT_BLOCK_SIZE (POLAR_CLIP_INPUT_SAMPLES * 4)
#define POLAR_CLIP_OUTPUT_SAMPLES (POLAR_CLIP_INPUT_SAMPLES)

#define CLASSIFIER_OUTPUT_SAMPLES (POLAR_CLIP_OUTPUT_SAMPLES)
#define CLASSIFIER_OUTPUT_BLOCK_SIZE (CLASSIFIER_OUTPUT_SAMPLES * 4)

#endif /**********__INCLUDE_H__**********/
