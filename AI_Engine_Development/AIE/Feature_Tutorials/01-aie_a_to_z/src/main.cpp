/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

/*
 * main.cpp : AI Engine A-to-Z Application
 *
 *  Created on: Jun 2, 2020
 *      Author: florentw
 */

#include <stdio.h>
#include <stdlib.h>
#include "xil_printf.h"
#include "xil_cache.h"
#include "xil_io.h"
#include "sleep.h"

#include "project.cpp"


#define MM2S_BASE XPAR_XMM2S_0_S_AXI_CONTROL_BASEADDR
#define S2MM_BASE XPAR_XS2MM_0_S_AXI_CONTROL_BASEADDR

#define MEM_OFFSET 0x10
#define SIZE_OFFSET 0x1C
#define CTRL_OFFSET 0x0

#define INPUT_SIZE 128
#define OUTPUT_SIZE 128

int InitInputData(int32_t** in, int size)
{
	int i;
	int data = 0x00010000;
	*in = (int32_t*)malloc(sizeof(int32_t) * size);

	if(!in) {
		printf("Allocation of memory failed \n");
		return -1;
	}

	for(i = 0; i < size; i++) {
		(*in)[i] = data;
		data += 0x00020002;
		usleep(50);
	}

	return 0;
}

int InitOutputData(int32_t** out, int size)
{
	int i;
	*out = (int32_t*)malloc(sizeof(int32_t) * size);

	if(!out) {
		printf("Allocation of memory failed \n");
		return -1;
	}

	for(i = 0; i < size; i++) {
		(*out)[i] = 0xABCDEF00;
		usleep(50);
	}

	return 0;
}

void AIE_systemConfig(int32_t* in, int32_t* out, int input_size, int output_size)
{
	uint64_t memAddr = (uint64_t)in;
	uint64_t mem_outAddr = (uint64_t)out;

	printf("Configuring MM2S IP\n");
	Xil_Out32(MM2S_BASE + MEM_OFFSET, (uint32_t) memAddr);
	Xil_Out32(MM2S_BASE + MEM_OFFSET + 4, 0);
	Xil_Out32(MM2S_BASE + SIZE_OFFSET, input_size);
	Xil_Out32(MM2S_BASE + CTRL_OFFSET, 1);
	printf("Done \n");
	printf("- \n");

	printf("Configuring S2MM IP\n");
	Xil_Out32(S2MM_BASE + MEM_OFFSET, (uint32_t) mem_outAddr);
	Xil_Out32(S2MM_BASE + MEM_OFFSET + 4, 0);
	Xil_Out32(S2MM_BASE + SIZE_OFFSET, output_size);
	Xil_Out32(S2MM_BASE + CTRL_OFFSET, 1);
	printf("Done \n");
	printf("- \n");
}


int main()
{
	int32_t* in;
	int32_t* out;
	int status;

    print("Starting AI Engine A to Z Application\n\r");
    printf("------------------------------------\n\n");
    printf("Beginning test\n");
    printf("- \n");

    printf("Initializing input data in memory\n");
    Xil_DCacheDisable();
    status = InitInputData(&in, INPUT_SIZE);
    if(status != 0)
    {
    	printf("Error Initializing Input data memory \n");
    	return -1;
    }
    printf("Done \n");
    printf("- \n");

    printf("Initializing output memory space\n");
    status = InitOutputData(&out, OUTPUT_SIZE);
    if(status != 0)
    {
    	printf("Error Initializing output memory space\n");
    	return -1;
    }
    printf("Done \n");
    printf("- \n");

    printf("System Configuration\n");
    AIE_systemConfig(in, out, INPUT_SIZE, OUTPUT_SIZE);
    printf("Done \n");
    printf("- \n");

    printf("Starting AIE Graph\n");
    printf("Graph Initialization\n");
    mygraph.init();
    printf("Done \n");
    printf("- \n");

    printf("Running Graph for 4 iterations\n");
    mygraph.run(4);

    int checks = 1;

    while(1) {
    	printf("Checking output, check #%d\n",checks);
    	uint32_t v = Xil_In32(S2MM_BASE + CTRL_OFFSET);
    	checks++;
    	if(v & 6) {
    		break;
    	}
    	sleep(2);
    }
    printf("Done \n");
    printf("- \n");

    printf("Checking Output Data: \n");
    int err = 0;
    int32_t golden, dataIn, realTemp, imTemp;

    for(int i = 0; i < OUTPUT_SIZE; i++) {
    	dataIn = ((int32_t*)in)[i];
    	realTemp = (dataIn & 0xFFFF) + ((dataIn & 0xFFFF0000)>>16);
    	imTemp = (dataIn & 0xFFFF) - ((dataIn & 0xFFFF0000)>>16);
    	golden =  ((realTemp - imTemp) << 16) + realTemp + imTemp;

    	if(golden != ((int32_t*)out)[i])
    	{
    		err++;
    		printf("Output Error: Golden = %x Output = %x\n ",golden,((int32_t*)out)[i]);
    	}
    }

    if(err > 0)
    {
    	printf("The AI Engine A to Z Application failed with %d errors", err);
    }
    else
    {
    	print("Successfully ran AI Engine A to Z Application with no error");
    }

    return 0;
}
