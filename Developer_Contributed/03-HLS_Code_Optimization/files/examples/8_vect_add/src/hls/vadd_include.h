/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*/

#ifndef _H_VADD_INCLUDE_H_
#define _H_VADD_INCLUDE_H_

#pragma once

#define DATA_SIZE 4096

//to compile host code with XRT APIs instead of OpenCL APIs
#define DB_USE_XRT

void krnl_vadd(uint32_t* in1, uint32_t* in2, uint32_t* out, int size);



#endif
