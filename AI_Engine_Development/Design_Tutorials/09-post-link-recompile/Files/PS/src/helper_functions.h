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
 */


#pragma once


#include <fstream>
#include <iostream>
#include <sstream>

#include "xrt/experimental/xrt_aie.h"
#include "xrt/experimental/xrt_bo.h"
#include "xrt/experimental/xrt_kernel.h"
#include "adf/adf_api/XRTConfig.h"
#include <adf.h>


bool extractFirstInteger(std::string str, int& num);
bool readIntegersFromFile(std::string filename, int32_t* array, int numelts);

std::vector<char> load_xclbin(xrtDeviceHandle device, const std::string& fnm);
