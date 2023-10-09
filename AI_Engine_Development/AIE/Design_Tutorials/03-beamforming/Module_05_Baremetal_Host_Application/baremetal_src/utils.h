
/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/


#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "xil_io.h"
#include "xil_cache.h"
#include <iostream>
#include <fstream>

int extractIQ(unsigned int dataHex, int lsb) ;

#endif //  __UTILS_H__
