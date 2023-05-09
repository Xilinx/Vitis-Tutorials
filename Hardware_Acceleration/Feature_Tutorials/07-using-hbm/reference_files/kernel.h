/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#ifndef DATA_ELEMENTS
#define DATA_ELEMENTS 16
#endif
const unsigned int VDATA_SIZE = DATA_ELEMENTS;

typedef struct v_datatype { unsigned int data[VDATA_SIZE]; } v_dt;
