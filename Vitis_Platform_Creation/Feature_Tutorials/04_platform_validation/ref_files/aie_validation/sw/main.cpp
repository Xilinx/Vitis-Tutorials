/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "platform.h"
#include "xparameters.h"
#include "xil_io.h"
#include "xil_cache.h"
#include "xmm2s_1.h"
#include "xs2mm_1.h"
#include "input.h"
#include "golden.h"
//
#include "graph.cpp"

//The base address range obtained from xparameters.h file
//DDR4 address range
#define DDR4_LOW0_MEMORY_BASEADDR XPAR_AXI_NOC_DDR_LOW_0_BASEADDR
#define DDR4_LOW0_MEMORY_HIGHADDR XPAR_AXI_NOC_DDR_LOW_0_HIGHADDR
#define DDR4_LOW1_MEMORY_BASEADDR XPAR_AXI_NOC_DDR_LOW_1_BASEADDR
#define DDR4_LOW1_MEMORY_HIGHADDR XPAR_AXI_NOC_DDR_LOW_1_HIGHADDR
//LPDDR4 address range
#define LPDDR4_MEMORY_BASEADDR XPAR_AXI_NOC_DDR_CH_1_BASEADDR
#define LPDDR4_MEMORY_HIGHADDR XPAR_AXI_NOC_DDR_CH_1_HIGHADDR

static XS2mm_1 S2mmInstance;
static XMm2s_1 Mm2sInstance;

#ifndef TESTAPP_GEN
	#define S2MM_DEVICE_ID		  XPAR_S2MM_1_1_DEVICE_ID
	#define MM2S_DEVICE_ID		  XPAR_XMM2S_1_0_DEVICE_ID
#endif

#define MM2S_1_BASE XPAR_XMM2S_1_0_S_AXI_CONTROL_BASEADDR
#define S2MM_1_BASE XPAR_XS2MM_1_0_S_AXI_CONTROL_BASEADDR

#define GRAPH_ITER  1
#define INPUT_SIZE 	32*GRAPH_ITER
#define OUTPUT_SIZE 32*GRAPH_ITER
//For debug
#define DEBUG       1

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int aie_pll_status_check()
{
	uint32_t AiePllOffset  = 0xF70A010C ; // 0_0 SHIM tile's PLL status reg offset(XCVC1902). Expected 0x5
	
	//AIE-PLL status register read (XCVC1902 AIE NPI Register address)
	uint32_t aie_pll_status = Xil_In32(AiePllOffset);
	if (aie_pll_status == 0x5)
		printf("INFO:: AIE PLL (Clock) Status is Stable and Locked... \n");
	else {
		printf("ERROR:: AIE PLL (Clock) status is Not Locked \n");
		return 1;
	}
	return 0;
}

int aie_dm_wr_rd_check()
{
	uint64_t AieBaseAddr   = 0x20000000000 ;  // AIE Array-1 Base address (64-bit)
	uint32_t AieCoreOffset = 0x04040000 ; // 8_0 core offset
	const uint32_t data    = 0xA5A50F0F ;
	
	//AIE-Data Memory write/read access
	uint64_t AieDmAddr     = AieBaseAddr + uint64_t(AieCoreOffset);
	
	for(int i1=0; i1<32; i1++)
		Xil_Out32(AieDmAddr + (i1*4), data);

	uint32_t aie_datamem_read = 0;
	for(int i2=0; i2<32; i2++) {
		aie_datamem_read = Xil_In32(AieDmAddr + (i2*4));
		if( aie_datamem_read != data) {
			printf("ERROR:: AIE Data Memory access Failed. Data Mismatch at Offset:%d; Written 0x%08x != Read 0x%08x \n", i2, data, aie_datamem_read);
			return 1;
		}
	}
	printf("INFO:: AIE Data Memory access successful... \n");	
	return 0;
}

int PS2AIEAccessCheck()
{
	printf("############################################################################ \n");
	printf("INFO:: Starting AIE Array's Register Access Test \n");
	printf("############################################################################ \n");
	
	//AIE-PLL status register read (XCVC1902 AIE NPI Register address)
	int ret1 = aie_pll_status_check();

	//AIE-Data Memory write/read access
	int ret2 = aie_dm_wr_rd_check();

	return (ret1 + ret2);
}

int  PLKernelInitialize()
{
	int Status;
	static XS2mm_1_Config *S2mmConfig;
	static XMm2s_1_Config *Mm2sConfig;
	S2mmConfig=XS2mm_1_LookupConfig(S2MM_DEVICE_ID);
	if(NULL == S2mmConfig) {
		return XST_FAILURE;
	}
	Status=XS2mm_1_CfgInitialize(&S2mmInstance,S2mmConfig);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	Mm2sConfig=XMm2s_1_LookupConfig(MM2S_DEVICE_ID);
	if(NULL == Mm2sConfig) {
		return XST_FAILURE;
	}
	Status=XMm2s_1_CfgInitialize(&Mm2sInstance,Mm2sConfig);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}

void  PLKernelConfig(int32_t* out_mem_addr,int32_t* in_mem_addr,int output_size, int input_size)
{
	uint64_t OutMemAddr = (uint64_t)out_mem_addr;
	uint64_t InMemAddr = (uint64_t)in_mem_addr;
	XS2mm_1_Set_mem(&S2mmInstance,OutMemAddr);
	XS2mm_1_Set_size(&S2mmInstance,output_size);
	XMm2s_1_Set_mem(&Mm2sInstance,InMemAddr);
	XMm2s_1_Set_size(&Mm2sInstance,input_size);
}

void PLKernelStart()
{
	XS2mm_1_Start(&S2mmInstance);
	XMm2s_1_Start(&Mm2sInstance);
}

void InitData(int32_t** out, int size)
{
	int i;
	*out = (int32_t*)malloc(sizeof(int32_t) * size);

	if(!out) {
		printf("Allocation of memory failed \n");
		exit(-1);
	}

	for(i = 0; i < size; i++) {
		(*out)[i] = 0xABCDEF00;
	}
}

int AIE2PL2MemoryAccessCheck()
{
	if (DEBUG) {
		printf("INFO:: Running AIE-PLIO Test \n");
	}
	
	int32_t  errCount = 0;
	int32_t* OUT_BUFF_ADDR;
	InitData(&OUT_BUFF_ADDR, OUTPUT_SIZE);
	
	if(PLKernelInitialize()==XST_SUCCESS)
		printf("INFO::PL kernels are initialized\r\n");

	PLKernelConfig(OUT_BUFF_ADDR,plio_in,OUTPUT_SIZE,INPUT_SIZE);

	XS2mm_1_DisableAutoRestart(&S2mmInstance);
	XS2mm_1_InterruptDisable(&S2mmInstance,0x2);
	XS2mm_1_InterruptGlobalDisable(&S2mmInstance);
	PLKernelStart();
	while(1) {
		uint32_t v1 = XS2mm_1_ReadReg(S2MM_1_BASE, XS2MM_1_CONTROL_ADDR_AP_CTRL);
		if (DEBUG)
			printf("INFO::S2MM CTRL register is %x \n",XS2mm_1_ReadReg(S2MM_1_BASE ,XS2MM_1_CONTROL_ADDR_AP_CTRL));
		if( (v1 & 6) ) {
			break;
		}
	}

    printf("INFO::Comparing the data S2MM write memory with the data MM2S read from memory\n");
	
	for(int32_t i = 0; i < OUTPUT_SIZE; i++) {
		if((((int32_t*)OUT_BUFF_ADDR)[i] != plio_in[i]) ) {
			printf("ERROR:: the data %d S2MM write to memory !=%d the data %d MM2s read from memory\n", ((int32_t*)OUT_BUFF_ADDR)[i], plio_in[i]);
			errCount++;
		} else {
			if (DEBUG)
				printf("INFO::the data %d S2MM write to memory =%d the data %d MM2s read from memory\n", ((int32_t*)OUT_BUFF_ADDR)[i],plio_in[i]);
		}
	}

	if(errCount == 0)
	{
		printf("INFO::AIE2PL Access check: PASS! \r\n");
	}
	else
	{
		printf("ERROR:: AIE2PL Acess check: FAILED: \r\n");
	}
	return errCount;
}

int AIEGmioTest()
{
	if (DEBUG) {
		printf("INFO:: Running AIE-GMIO Test \n");
	}
    
	int32_t  errCount = 0;
	
	// create GMIO buffers
	int32_t* inputArray0  = (int32*)GMIO::malloc(256*sizeof(int32_t));
	int32_t* outputArray0 = (int32*)GMIO::malloc(256*sizeof(int32_t));
    
	// Initialize input GMIO data
	for (int i=0; i<256; i++) {
		inputArray0[i] = i+1;
	}
	// Initialize SHIM DMA registers to start GMIO access 
    gm_graph.gm_out0.aie2gm_nb(&outputArray0[0], 32*sizeof(int32));
	gm_graph.gm_in0.gm2aie_nb(&inputArray0[0], 32*sizeof(int32));
    
	// Waiting for GMIO OUT 
    if (DEBUG)
		printf("INFO:: Waiting for GMIO OUT DMA completion...  \n");
    gm_graph.gm_out0.wait(); 
	
	//compare GMIO read data with golden
    printf("INFO:: Comparing AIE-GMIO read data \n");
	for(int32_t i = 0; i < OUTPUT_SIZE; i++) {
		if((((int32_t*)outputArray0)[i] != golden[i]) ) {
			printf("ERROR:: GMIO data mismatch found in sample %d value %d != %d golden \n", i+1, ((int32_t*)outputArray0)[i], golden[i]);
			errCount++;
		} else {
			if (DEBUG)
				printf("GMIO read sample-%d = %d matches with Golden data = %d \n", i+1, ((int32_t*)outputArray0)[i], golden[i]);
		}
	}
	
	GMIO::free(inputArray0);
    GMIO::free(outputArray0);
	
	if(errCount == 0)
		printf("INFO:: AIE GMIO Test Passed! \n");
	else
		printf("ERROR:: AIE GMIO Test Failed! Error count: %d \n",errCount);	

	return errCount ;
}

int AIE2DDRMemoryAccessCheck()
{
	printf("############################################################################ \n");
	printf("INFO:: Starting AIE Kernel(GMIO) <-> Memory(DDR) access test \n");
	printf("############################################################################ \n");
	int32_t  errCount_gmio = 0;
    
	//printf("INFO:: AIE Graph Initialization...\n"); // AIE initialization happens from PMC in default flow
    //gm_graph.init();

    if (DEBUG)
		printf("INFO:: AIE Graph Run with %d - iterations \n", GRAPH_ITER);
    gm_graph.run(GRAPH_ITER);
	
	// Update Async RTP value with "1" to match output with input values(loopback)
	// update Async RTP value with "0" gives output as input multiplied by 2
	if constexpr (is_rtp) {
		gm_graph.update (gm_graph.myRtp0, 1);
	} else {
		gm_graph.update (gm_graph.myRtp0, 0);
	}
    
	if (DEBUG)
		printf("INFO:: AIE Graph RTP value updated successfully \n");

	errCount_gmio = AIEGmioTest();
	
	if (DEBUG)
	    printf("INFO:: Waiting for AIE Graph End...\n");
    
	gm_graph.end();

    if (DEBUG)
	    printf("INFO:: AIE Graph END complete... \n");
	
	return (errCount_gmio);
}

int AIE2PLExample()
{
	
	printf("############################################################################ \n");
	printf("INFO:: Starting AIE Example (AIE+PL) test \n");
	printf("############################################################################ \n");
	int32_t  errCount_plio = 0;
    
	//printf("INFO:: AIE Graph Initialization...\n"); // AIE initialization happens from PMC in default flow
    //pl_graph.init();

    if (DEBUG)
		printf("INFO:: AIE Graph Run with %d - iterations \n", GRAPH_ITER);
    pl_graph.run(GRAPH_ITER);
	
	// Update Async RTP value with "1" to match output with input values(loopback)
	// update Async RTP value with "0" gives output as input multiplied by 2
	if constexpr (is_rtp) {
		pl_graph.update (pl_graph.myRtp0, 1);
	} else {
		pl_graph.update (pl_graph.myRtp0, 0);
	}
    
	if (DEBUG)
		printf("INFO:: AIE Graph RTP value updated successfully \n");

	errCount_plio = AIE2PL2MemoryAccessCheck();
	
	if (DEBUG)
	    printf("INFO:: Waiting for AIE Graph End...\n");
    
	pl_graph.end();

    if (DEBUG)
	    printf("INFO:: AIE Graph END \n");
	return errCount_plio;
}

int main()
{

	Xil_DCacheDisable();
    sleep(1);
	
	printf("- \n");
	printf("#Beginning AIE BareMetal Test\n");
	printf("-\r\n");
	printf("#Beginning PS2AIE Register Access Test\r\n");
	//PS2AIEAccessCheck{}   //PS to AIE Registers  (PLL + AIE DataMemory) Access Check 
	int ret1 = PS2AIEAccessCheck();
	if(ret1)
		printf("ERROR:: PS2AIE Array Register Access Failed! \n");
	printf("\n-\r\n");
	printf("#Beginning AIE2DDR Memory Access Test \r\n");
	//AIE2DDRMemoryAccessCheck{}  // AIE+GMIO example to AIE <-> DDR Memory Access Check(AXI-MM)
	int ret2 = AIE2DDRMemoryAccessCheck();
	if(ret2)
		printf("ERROR:: AIE2DDR Memory Access Failed! \n");
	printf("\n-\r\n");

	printf("#Beginning AIE2PL2DDR Access Test\r\n");
	//AIE2PLExample{}  // AIE+PLIO example to check AIE <-> PL <-> DDR Access paths (AXI-S)
	int ret3 = AIE2PLExample(); 
	if(ret3)
		printf("ERROR:: AIE2PL kernel Acess check Failed! \n");
	printf("-\r\n");

	//cleanup_platform();
	if(ret1 + ret2 + ret3)
		printf("ERROR:: AIE Platform Validation Test Failed! \n");
	else
		printf("INFO:: AIE Platform Validation Test Passed.\nExiting AIE Baremetal Test...\n");	
	return (ret1 + ret2 + ret3);
}
