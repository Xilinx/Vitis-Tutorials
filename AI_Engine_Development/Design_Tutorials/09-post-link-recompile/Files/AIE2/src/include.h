/*
 * (c) Copyright 2021 Xilinx, Inc. All rights reserved.
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
 *
 */


#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#define NUM_SAMPLES 256
#define NBYTES_DATA 4

#define WEIGHTSUM_INPUT_SAMPLES		(NUM_SAMPLES)
#define WEIGHTSUM_INPUT_MARGIN		(8)
#define WEIGHTSUM_OUTPUT_SAMPLES	(WEIGHTSUM_INPUT_SAMPLES)

#define AVERAGE_INPUT_SAMPLES		(WEIGHTSUM_OUTPUT_SAMPLES)
#define AVERAGE_INPUT_MARGIN		(0)
#define AVERAGE_OUTPUT_SAMPLES		(AVERAGE_INPUT_SAMPLES)

#define POLAR_CLIP_INPUT_SAMPLES	(AVERAGE_OUTPUT_SAMPLES)
#define POLAR_CLIP_OUTPUT_SAMPLES	(POLAR_CLIP_INPUT_SAMPLES)

#define CLASSIFIER_INPUT_SAMPLES	(POLAR_CLIP_OUTPUT_SAMPLES)
#define CLASSIFIER_OUTPUT_SAMPLES	(CLASSIFIER_INPUT_SAMPLES)

#endif /* __INCLUDE_H__ */
