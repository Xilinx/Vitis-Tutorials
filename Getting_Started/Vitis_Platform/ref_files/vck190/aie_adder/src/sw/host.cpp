/**
* Copyright (C) 2019-2021 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// XRT APIS are used for both the AIE Graph and PL Kernels
#include "xrt/xrt_kernel.h"
#include "xrt/xrt_graph.h"

#define DATA_SIZE 1024
#define N_ITER DATA_SIZE / 4

int main(int argc, char** argv) 
{
    xuid_t uuid;
    auto dhdl = xrtDeviceOpen(0);
    xrtDeviceLoadXclbinFile(dhdl, "krnl_adder.xclbin");
    xrtDeviceGetXclbinUUID(dhdl, uuid);

    printf("xrtGraphOpen\n"); 
    auto ghdl = xrtGraphOpen(dhdl, uuid, "addergraph"); 

    printf("xrtPLKernelOpen\n"); 
    auto mm2s_khdl1 = xrtPLKernelOpen(dhdl, uuid, "mm2s:{mm2s_1}");
    auto mm2s_khdl2 = xrtPLKernelOpen(dhdl, uuid, "mm2s:{mm2s_2}");
    auto s2mm_khdl  = xrtPLKernelOpen(dhdl, uuid, "s2mm:{s2mm}");

    int sizeIn  = DATA_SIZE;
    int sizeOut = DATA_SIZE;

    int* DataInput0 = new int[sizeIn];
    int* DataInput1 = new int[sizeIn];
    int* golden     = new int[sizeOut];

    for (int i = 0; i < sizeIn; i++) {
        DataInput0[i] = rand() % 100;
        DataInput1[i] = rand() % 100;
    }

    printf("xrtBOAlloc\n");
    xrtBufferHandle in_bohdl0 = xrtBOAlloc(dhdl, sizeIn  * sizeof(int), 0, 0);
    xrtBufferHandle in_bohdl1 = xrtBOAlloc(dhdl, sizeIn  * sizeof(int), 0, 0);
    xrtBufferHandle out_bohdl = xrtBOAlloc(dhdl, sizeOut * sizeof(int), 0, 0);

    printf("xrtBOMap\n");
    auto in_bomapped0 = reinterpret_cast<uint32_t*>(xrtBOMap(in_bohdl0));
    memcpy(in_bomapped0, DataInput0, sizeIn * sizeof(int));
    printf("Input memory virtual addr 0x%px\n", in_bomapped0);

    auto in_bomapped1 = reinterpret_cast<uint32_t*>(xrtBOMap(in_bohdl1));
    memcpy(in_bomapped1, DataInput1, sizeIn * sizeof(int));
    printf("Input memory virtual addr 0x%px\n", in_bomapped1);

    auto out_bomapped = reinterpret_cast<uint32_t*>(xrtBOMap(out_bohdl));
    memset(out_bomapped, 0xABCDEF00, sizeOut * sizeof(int));
    printf("Output memory virtual addr 0x%px\n", out_bomapped);

    printf("xrtBOSync\n");
    xrtBOSync(in_bohdl0, XCL_BO_SYNC_BO_TO_DEVICE, sizeIn * sizeof(int), 0);
    xrtBOSync(in_bohdl1, XCL_BO_SYNC_BO_TO_DEVICE, sizeIn * sizeof(int), 0);

    printf("xrtKernelRun\n");
    xrtRunHandle mm2s_rhdl1 = xrtKernelRun(mm2s_khdl1, in_bohdl0, nullptr, sizeIn);
    xrtRunHandle mm2s_rhdl2 = xrtKernelRun(mm2s_khdl2, in_bohdl1, nullptr, sizeIn);
    xrtRunHandle s2mm_rhdl  = xrtKernelRun(s2mm_khdl,  out_bohdl, nullptr, sizeOut);
  
    printf("xrtGraphRun\n"); 
    xrtGraphRun(ghdl, N_ITER);

    printf("xrtGraphEnd\n"); 
    xrtGraphEnd(ghdl, 0);

    printf("xrtRunWait\n");
    xrtRunWait(mm2s_rhdl1);
    xrtRunWait(mm2s_rhdl2);
    xrtRunWait(s2mm_rhdl);

    printf("xrtRunClose\n");
    xrtRunClose(mm2s_rhdl1);
    xrtRunClose(mm2s_rhdl2);
    xrtRunClose(s2mm_rhdl);

    printf("xrtBOSync\n");
    xrtBOSync(out_bohdl, XCL_BO_SYNC_BO_FROM_DEVICE, sizeOut * sizeof(int), 0);

    // Compare results
    int errorCount = 0;
    for (int i = 0; i < sizeOut; i++) {
        golden[i] = DataInput0[i] + DataInput1[i];
        if ((signed)out_bomapped[i] != golden[i]) {
            // printf("Error found @ %d, %d != %d\n", i, out_bomapped[i], golden[i]);
            errorCount++;
        }
    }

    // Clean up
    printf("Releasing objects\n"); 
    delete[] DataInput0;
    delete[] DataInput1;
    delete[] golden;
    xrtGraphClose(ghdl);
    xrtKernelClose(mm2s_khdl1);
    xrtKernelClose(mm2s_khdl2);
    xrtKernelClose(s2mm_khdl);
    xrtBOFree(in_bohdl0);
    xrtBOFree(in_bohdl1);
    xrtBOFree(out_bohdl);
    xrtDeviceClose(dhdl);

    if (errorCount)
        printf("Test failed with %d errors\n", errorCount);
    else
        printf("TEST PASSED\n");

    return errorCount;
}
