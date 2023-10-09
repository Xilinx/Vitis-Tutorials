/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef __DDS_H__
#define __DDS_H__

#include "stdlib.h"
#include <stdio.h>
#include <adf.h>
using namespace adf;
  void init_dds();
  void sine(const int32 phase_increment,output_buffer<cint16,extents<128>> & owin);

#endif /* __DDS_H__ */


