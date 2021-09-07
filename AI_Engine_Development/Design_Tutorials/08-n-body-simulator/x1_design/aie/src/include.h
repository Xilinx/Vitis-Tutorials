/* (c) Copyright 2021 Xilinx, Inc.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 * 		http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */

#ifndef INCLUDE_H
#define INCLUDE_H
#include <adf.h>

#define ENABLE_KERNEL_DEBUG_PRINTS	0

#define NUM_ENGINES_PER_COL			4

#define NUM_I				        32
#define NUM_I_VECTORS				NUM_I/8
#define WINDOW_SIZE_I				NUM_I*7*4	// x, y, z, m, vx, vy, v

#define NUM_PARTICLES				128
#define NUM_J					32
#define NUM_J_VECTORS				NUM_J/8
#define LOOP_COUNT_J				NUM_PARTICLES/NUM_J 
#define WINDOW_SIZE_J				NUM_J*4*4	// x, y, z, m		



//#define ABS_BUFFER_CONSTRAINTS
//#define KERNEL_BYPASS
 
#endif // INCLUDE_H
