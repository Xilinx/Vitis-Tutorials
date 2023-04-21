/*
 Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
 SPDX-License-Identifier: X11
 */

#define N 100
typedef unsigned char data_t;

// Top function
void diamond(data_t vecIn[N], data_t vecOut[N]);

// Sub functions
void funcA(data_t f1In[N], data_t f1Out[N], data_t f1bisOut[N]);
void funcB(data_t f2In[N], data_t f2Out[N]);
void funcC(data_t f3In[N], data_t f3Out[N]);
void funcD(data_t f4In[N], data_t f4bisIn[N], data_t f4Out[N]);
