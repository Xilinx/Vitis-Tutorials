/******************************************************************************
* Copyright (c) 2018 - 2020 Xilinx, Inc. All rights reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/

#include "xzdma.h"
#include "xparameters.h"
#ifdef SDT
#include "xinterrupt_wrap.h"
#endif
#include "xil_cache.h"
#include "xil_util.h"

#ifndef SDT
#ifdef XPAR_INTC_0_DEVICE_ID
#include "xintc.h"
#else
#include "xscugic.h"
#endif
#endif

#define SIZE	32

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#ifndef SDT
#define ZDMA_DEVICE_ID		XPAR_XZDMA_0_DEVICE_ID /* ZDMA device Id */
#ifdef XPAR_INTC_0_DEVICE_ID
#define INTC		XIntc
#define ZDMA_INTC_DEVICE_ID	XPAR_INTC_0_DEVICE_ID
#define ZDMA_INTR_DEVICE_ID	XPAR_INTC_0_ZDMA_0_VEC_ID /**< ZDMA Interrupt Id */
#else
#define INTC		XScuGic
#define ZDMA_INTC_DEVICE_ID	XPAR_SCUGIC_SINGLE_DEVICE_ID
/**< SCUGIC Device ID */
#define ZDMA_INTR_DEVICE_ID	XPAR_XADMAPS_0_INTR /**< ZDMA Interrupt Id */
#endif
#endif

#ifndef TESTAPP_GEN
XZDma ZDma;		/**<Instance of the ZDMA Device */
#ifndef SDT
static INTC Intc;	/**< XIntc Instance */
#endif
#endif

u8 ZDmaSrcBuf[SIZE] __attribute__ ((aligned (64)));
u8 ZDmaDstBuf[SIZE] __attribute__ ((aligned (64)));

static void lowercase(u8* bufptr, u32 size)
{
	Xil_AssertNonvoid(bufptr != NULL);
	Xil_AssertNonvoid(size < SIZE);

	for(u32 idx=0; idx<size; idx++) {
		if((bufptr[idx] > 65) && (bufptr[idx] < 90)) {
			bufptr[idx] += 32;
		}
	}
}

static volatile int Done = 0;

static void DoneHandler(void *CallBackRef)
{
	Done = 1;
}

int main(void)
{
	int Status;
	XZDma_Config *Config;
	XZDma_DataConfig Configure;
	XZDma_Transfer Data;
	XScuGic_Config *IntcConfig;
	u32 Index;

	xil_printf("String Manipulation test\r\n");

	/* Configure LPD DMA 0 channel */
#ifndef SDT
	Config = XZDma_LookupConfig(XPAR_XZDMA_0_DEVICE_ID );
#else
	Config = XZDma_LookupConfig(XPAR_XZDMA_0_BASEADDR);
#endif
	if (NULL == Config) {
		return XST_FAILURE;
	}

	Status = XZDma_CfgInitialize(&ZDma, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/* Configure GIC */
#ifndef SDT
	IntcConfig = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
#else
	IntcConfig = XScuGic_LookupConfig(XPAR_XSCUGIC_0_BASEADDR);
#endif
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}

#ifndef SDT					   
	Status = XScuGic_CfgInitialize(&ScuGic, IntcConfig, IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/* Enable exception and register GIC interrupt handler */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler) XScuGic_InterruptHandler,	&ScuGic);
	Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);

	/* Register DMA interrupt handler in the GIC */
	Status = XScuGic_Connect(&ScuGic, XPAR_XADMAPS_0_INTR , (Xil_ExceptionHandler) XZDma_IntrHandler, (void *) &ZDma);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#else
	Status = XSetupInterruptSystem(&ZDma, &XZDma_IntrHandler,
				       Config->IntrId, Config->IntrParent,
				       XINTERRUPT_DEFAULT_PRIORITY);
#endif
	
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
					   

	/* Set DMA interrupt handler callback and enable interrupt */
	XZDma_SetCallBack(&ZDma, XZDMA_HANDLER_DONE, (void *)DoneHandler, &ZDma);
	XZDma_EnableIntr(&ZDma, (XZDMA_IXR_ALL_INTR_MASK));

	/* Configure DMA mode */
	Status = XZDma_SetMode(&ZDma, FALSE, XZDMA_NORMAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/* Configuration settings */
	Configure.OverFetch = 1;
	Configure.SrcIssue = 0x1F;
	Configure.SrcBurstType = XZDMA_INCR_BURST;
	Configure.SrcBurstLen = 0xF;
	Configure.DstBurstType = XZDMA_INCR_BURST;
	Configure.DstBurstLen = 0xF;
	XZDma_SetChDataConfig(&ZDma, &Configure);

	/* Transfer elements */
	Data.DstAddr = (UINTPTR)ZDmaDstBuf;
	Data.Pause = 0;
	Data.SrcAddr = (UINTPTR)ZDmaSrcBuf;
	Data.Size = SIZE;

	/* Initialize the source buffer */
	sprintf(ZDmaSrcBuf, "asdBqSet kWsdfiQZzlkgiwEd\r\n");
	Xil_DCacheFlush();

	/* Perform DMA transfer */
	XZDma_Start(&ZDma, &Data, 1);
	while(0==Done);

	/* Checking the data transferred */
	for (Index = 0; Index < SIZE; Index++) {
		if (ZDmaSrcBuf[Index] != ZDmaDstBuf[Index]) {
			xil_printf("DMA transfer failure, 0x%0X != 0x%0X\r\n", ZDmaSrcBuf[Index], ZDmaDstBuf[Index]);
			return XST_FAILURE;
		}
	}

	xil_printf("DMA transfer succeed\r\n");
	xil_printf("Src string: %s", ZDmaSrcBuf);
	xil_printf("Dst string: %s", ZDmaDstBuf);

	lowercase(ZDmaDstBuf, SIZE);
	xil_printf("Lowercase : %s", ZDmaDstBuf);

	return XST_SUCCESS;
}
