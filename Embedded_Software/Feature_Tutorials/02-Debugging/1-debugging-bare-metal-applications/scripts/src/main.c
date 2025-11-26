/******************************************************************************
* Copyright (c) 2025 Advanced Micro Devices, Inc. All rights reserved.
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

/************************** Constant Definitions ******************************/


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
#define SIZE		1024 /**< Size of the data to be transferred */
#else
#define SIZE		1000 /**< Size of the data to be transferred */
#endif

#define TESTVALUE	0x1230 /**< For writing into source buffer */

#define POLL_TIMEOUT_COUNTER		1000000U
#define NUM_OF_EVENTS			1
/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

#ifndef SDT
int XZDma_SimpleExample(INTC *IntcInstPtr, XZDma *ZdmaInstPtr,
			u16 DeviceId, u16 IntrId);
static int SetupInterruptSystem(INTC *IntcInstancePtr,
				XZDma *InstancePtr, u16 IntrId);
#else
int XZDma_SimpleExample(XZDma *ZdmaInstPtr,
			UINTPTR BaseAddress);
#endif
static void DoneHandler(void *CallBackRef);
static void ErrorHandler(void *CallBackRef, u32 Mask);

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


/************************** Variable Definitions *****************************/

#ifndef TESTAPP_GEN
XZDma ZDma;		/**<Instance of the ZDMA Device */
#ifndef SDT
static INTC Intc;	/**< XIntc Instance */
#endif
#endif

#if defined(__ICCARM__)
#pragma data_alignment = 64
u8 ZDmaDstBuf[SIZE]; /**< Destination buffer */
#pragma data_alignment = 64
u8 ZDmaSrcBuf[SIZE]; /**< Source buffer */
#else
u8 ZDmaDstBuf[SIZE] __attribute__ ((aligned (64)));	/**< Destination buffer */
u8 ZDmaSrcBuf[SIZE] __attribute__ ((aligned (64)));	/**< Source buffer */
#endif
static volatile u32 Done = 0;				/**< Done flag */
static volatile u32 ErrorStatus = 0;			/**< Error Status flag*/
#ifndef TESTAPP_GEN

int main(void)
{
	int Status;

	/* Run the simple example */
#ifndef SDT
	Status = XZDma_SimpleExample(&Intc, &ZDma, (u16)ZDMA_DEVICE_ID,
				     ZDMA_INTR_DEVICE_ID);
#else
	Status = XZDma_SimpleExample(&ZDma, XPAR_XZDMA_0_BASEADDR);
#endif

	if (Status != XST_SUCCESS) {
		xil_printf("ZDMA Simple Example Failed\r\n");
		return XST_FAILURE;
	}

	xil_printf("Successfully ran ZDMA Simple Example\r\n");
    
	xil_printf("DMA transfer succeed\r\n");
	xil_printf("Src string: %s\r\n", ZDmaSrcBuf);
	xil_printf("Dst string: %s\r\n", ZDmaDstBuf);
	
	lowercase(ZDmaDstBuf, SIZE); 
	
	xil_printf("Lowercase : %s\r\n", ZDmaDstBuf);

	return XST_SUCCESS;
}
#endif

#ifndef SDT
int XZDma_SimpleExample(INTC *IntcInstPtr, XZDma *ZdmaInstPtr,
			u16 DeviceId, u16 IntrId)
#else
int XZDma_SimpleExample(XZDma *ZdmaInstPtr, UINTPTR BaseAddress)
#endif
{
	int Status;
	XZDma_Config *Config;
	XZDma_DataConfig Configure; /* Configuration values */
	XZDma_Transfer Data;
	u32 Index;
	u32 Value;

#ifndef SDT
	Config = XZDma_LookupConfig(DeviceId);
#else
	Config = XZDma_LookupConfig(BaseAddress);
#endif
	if (NULL == Config) {
		return XST_FAILURE;
	}

	Status = XZDma_CfgInitialize(ZdmaInstPtr, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	/*
	 * Performs the self-test to check hardware build.
	 */
	Status = XZDma_SelfTest(ZdmaInstPtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/* Filling the buffer for data transfer */
	const char *message = "asdBqSet kWsdfiQZzlkgiwEd";
    u32 len = strlen(message) + 1; // Include null terminator

    memset(ZDmaSrcBuf, 0, SIZE);
    memcpy(ZDmaSrcBuf, message, len);
    memset(ZDmaDstBuf, 0xAB, SIZE);  

	/*
	 * Invalidating destination address and flushing
	 * source address in cache
	 */
	Xil_DCacheFlushRange((INTPTR)ZDmaSrcBuf, SIZE);
	Xil_DCacheInvalidateRange((INTPTR)ZDmaDstBuf, SIZE);
	
	volatile u8 dummy;
	for (u32 i = 0; i < SIZE; i++) {
		dummy = ZDmaDstBuf[i];  
	}
	(void)dummy;  

	Status = XZDma_SetMode(ZdmaInstPtr, FALSE, XZDMA_NORMAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XZDma_SetCallBack(ZdmaInstPtr, XZDMA_HANDLER_DONE,
			  (void *)DoneHandler, ZdmaInstPtr);
	XZDma_SetCallBack(ZdmaInstPtr, XZDMA_HANDLER_ERROR,
			  (void *)ErrorHandler, ZdmaInstPtr);

#ifndef SDT
	Status = SetupInterruptSystem(IntcInstPtr, ZdmaInstPtr,
				      IntrId);
#else
	
    u32 wrong_interrupt_id = 0;  
	Status = XSetupInterruptSystem(ZdmaInstPtr, &XZDma_IntrHandler,
				       wrong_interrupt_id, Config->IntrParent,
				       XINTERRUPT_DEFAULT_PRIORITY);
	
	/* ERROR 2 FIX - Using correct interrupt ID: */
	/*Status = XSetupInterruptSystem(ZdmaInstPtr, &XZDma_IntrHandler,
				       Config->IntrId, Config->IntrParent,
				       XINTERRUPT_DEFAULT_PRIORITY);*/
#endif
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
	Configure.SrcCache = 0x3;
	Configure.DstCache = 0x3;
	
	Configure.SrcQos = 0;
	Configure.DstQos = 0;

	XZDma_SetChDataConfig(ZdmaInstPtr, &Configure);
	/*
	 * Transfer elements
	 */
	Data.DstAddr = (UINTPTR)ZDmaDstBuf;
	Data.DstCoherent = 0;  
	Data.Pause = 0;
	Data.SrcAddr = (UINTPTR)ZDmaSrcBuf;
	Data.SrcCoherent = 0;  
	Data.Size = len;

	/* Enable required interrupts */
	XZDma_EnableIntr(ZdmaInstPtr, (XZDMA_IXR_ALL_INTR_MASK));

	xil_printf("Debug: Starting DMA transfer...\n\r");
	
	XZDma_Start(ZdmaInstPtr, &Data, 1); 

	while (Done == 0);
	
	xil_printf("Debug: DMA transfer completed successfully (Done flag set by interrupt)\n\r");
	Status = XST_SUCCESS;

	/* Enable required interrupts */
	XZDma_DisableIntr(ZdmaInstPtr, (XZDMA_IXR_ALL_INTR_MASK));

	if (ErrorStatus) {
		if (ErrorStatus & XZDMA_IXR_AXI_WR_DATA_MASK) {
			xil_printf("Error occurred on write data channel\n\r");
		}
		if (ErrorStatus & XZDMA_IXR_AXI_RD_DATA_MASK) {
			xil_printf("Error occurred on read data channel\n\r");
		}
		return XST_FAILURE;
	}

	/*if (!Config->IsCacheCoherent) {
		Xil_DCacheInvalidateRange((INTPTR)ZDmaDstBuf, SIZE);
	} else {
		xil_printf("Debug: System is cache coherent, no manual invalidation needed\n\r");
	}*/

	/* Checking the data transferred */
	for (Index = 0; Index < len; Index++) {
		if (ZDmaSrcBuf[Index] != ZDmaDstBuf[Index]) {
			xil_printf("Data mismatch at index %d: expected 0x%02x, got 0x%02x\n\r", 
					   Index, ZDmaSrcBuf[Index], ZDmaDstBuf[Index]);
			return XST_FAILURE;
		}
	}

	Done = 0;

	/* Reset the DMA to remove all configurations done in this example  */
	XZDma_Reset(ZdmaInstPtr);

	return XST_SUCCESS;
}
#ifndef SDT
/*****************************************************************************/
/**
* This function sets up the interrupt system so interrupts can occur for the
* ZDMA. This function is application-specific. The user should modify this
* function to fit the application.
*
* @param	IntcInstancePtr is a pointer to the instance of the INTC.
* @param	InstancePtr contains a pointer to the instance of the ZDMA
*		driver which is going to be connected to the interrupt
*		controller.
* @param	IntrId is the interrupt Id and is typically
*		XPAR_<ZDMA_instance>_INTR value from xparameters.h.
*
* @return
*		- XST_SUCCESS if successful
*		- XST_FAILURE if failed
*
* @note		None.
*
****************************************************************************/
int SetupInterruptSystem(INTC *IntcInstancePtr,
			 XZDma *InstancePtr,
			 u16 IntrId)
{
	int Status;

#ifdef XPAR_INTC_0_DEVICE_ID
#ifndef TESTAPP_GEN
	/* Initialize the interrupt controller and connect the ISRs */
	Status = XIntc_Initialize(IntcInstancePtr, ZDMA_INTC_DEVICE_ID);
	if (Status != XST_SUCCESS) {

		xil_printf("Failed init intc\r\n");
		return XST_FAILURE;
	}
#endif
	/*
	 * Connect the driver interrupt handler
	 */
	Status = XIntc_Connect(IntcInstancePtr, IntrId,
			       (XInterruptHandler)XZDma_IntrHandler, InstancePtr);
	if (Status != XST_SUCCESS) {

		xil_printf("Failed connect intc\r\n");
		return XST_FAILURE;
	}
#ifndef TESTAPP_GEN
	/*
	 * Start the interrupt controller such that interrupts are enabled for
	 * all devices that cause interrupts.
	 */
	Status = XIntc_Start(IntcInstancePtr, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
#endif

	/*
	 * Enable the interrupt for the ZDMA device.
	 */
	XIntc_Enable(IntcInstancePtr, IntrId);
#ifndef TESTAPP_GEN
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				     (Xil_ExceptionHandler)XIntc_InterruptHandler,
				     (void *)IntcInstancePtr);
#endif
#else
#ifndef TESTAPP_GEN
	XScuGic_Config *IntcConfig; /* Config for interrupt controller */

	/*
	 * Initialize the interrupt controller driver
	 */
	IntcConfig = XScuGic_LookupConfig(ZDMA_INTC_DEVICE_ID);
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}

	Status = XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig,
				       IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Connect the interrupt controller interrupt handler to the
	 * hardware interrupt handling logic in the processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				     (Xil_ExceptionHandler) XScuGic_InterruptHandler,
				     IntcInstancePtr);
#endif

	/*
	 * Connect a device driver handler that will be called when an
	 * interrupt for the device occurs, the device driver handler
	 * performs the specific interrupt processing for the device
	 */
	Status = XScuGic_Connect(IntcInstancePtr, IntrId,
				 (Xil_ExceptionHandler) XZDma_IntrHandler,
				 (void *) InstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Enable the interrupt for the device
	 */
	XScuGic_Enable(IntcInstancePtr, IntrId);
#endif
#ifndef TESTAPP_GEN

	/*
	 * Enable interrupts
	 */
	Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);
#endif


	return XST_SUCCESS;
}
#endif

/*****************************************************************************/
/**
* This static function handles ZDMA Done interrupts.
*
* @param	CallBackRef is the callback reference passed from the interrupt
*		handler, which in our case is a pointer to the driver instance.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
static void DoneHandler(void *CallBackRef)
{
	Xil_AssertVoid(CallBackRef != NULL);
	Done = 1;

}
/*****************************************************************************/
/**
* This static function handles ZDMA error interrupts.
*
* @param	CallBackRef is the callback reference passed from the interrupt
*		handler, which in our case is a pointer to the driver instance.
* @param	Mask specifies which interrupts were occurred.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
static void ErrorHandler(void *CallBackRef, u32 Mask)
{
	Xil_AssertVoid(CallBackRef != NULL);
	ErrorStatus = Mask;
}