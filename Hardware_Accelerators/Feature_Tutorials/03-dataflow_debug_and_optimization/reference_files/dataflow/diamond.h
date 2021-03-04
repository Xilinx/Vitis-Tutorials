/*
 * Copyright 2020 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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