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

#ifndef GLOBALS_H
#define GLOBALS_H
#include <adf.h>
#include "include.h"

float particles_i[(NUM_I*7)]={0.0};		// x, y, z, vx, vy, vz, m = 7
//{ {x,x,x,...(NUM_I)},  {y,y,y,...(NUM_I)}, {z,z,z,...(NUM_I)}, {vx,vx,vx,...(NUM_I)},  {vy,vy,vy,...(NUM_I)}, {vz,vz,vz,...(NUM_I)}, {m,m,m,...(NUM_J)}}

float particles_i_new[(NUM_I*3)]={0.0};		// x, y, z, vx, vy, vz, m = 7
//{ {x,x,x,...(NUM_I)},  {y,y,y,...(NUM_I)}, {z,z,z,...(NUM_I)}, {vx,vx,vx,...(NUM_I)},  {vy,vy,vy,...(NUM_I)}, {vz,vz,vz,...(NUM_I)}, {m,m,m,...(NUM_J)}}

float particles_j[(NUM_J*4)]={0.0};		// x, y, z, m = 4
//{ {x,x,x,...(NUM_J)},  {y,y,y,...(NUM_J)}, {z,z,z,...(NUM_J)}, {m,m,m,...(NUM_J)} }

static float timestep = 1.0;
static float softening_factor_2 = 1000.0;
 
#endif // GLOBALS_H


