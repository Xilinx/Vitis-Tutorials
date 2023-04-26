/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
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


