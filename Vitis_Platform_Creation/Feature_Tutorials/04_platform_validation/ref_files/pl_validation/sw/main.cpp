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
#include "xintc.h"
#include "xscugic.h"


//The base address range obtained from xparameters.h file
//DDR4 address range
#define DDR4_LOW0_MEMORY_BASEADDR XPAR_AXI_NOC_DDR_LOW_0_BASEADDR
#define DDR4_LOW0_MEMORY_HIGHADDR XPAR_AXI_NOC_DDR_LOW_0_HIGHADDR
#define DDR4_LOW1_MEMORY_BASEADDR XPAR_AXI_NOC_DDR_LOW_1_BASEADDR
#define DDR4_LOW1_MEMORY_HIGHADDR XPAR_AXI_NOC_DDR_LOW_1_HIGHADDR
//LPDDR4 address range
#define LPDDR4_MEMORY_BASEADDR XPAR_AXI_NOC_DDR_CH_1_BASEADDR
#define LPDDR4_MEMORY_HIGHADDR XPAR_AXI_NOC_DDR_CH_1_HIGHADDR

#ifndef TESTAPP_GEN
#define INTC_DEVICE_ID2		  XPAR_CIPS_0_PSPMC_0_PSV_ACPU_GIC_DEVICE_ID
#define S2MM_DEVICE_ID		  XPAR_S2MM_1_1_DEVICE_ID
#define MM2S_DEVICE_ID		  XPAR_XMM2S_1_0_DEVICE_ID
#define INTC_DEVICE_ID		  XPAR_INTC_0_DEVICE_ID        
#endif
#define INPUT_SIZE 	32
#define OUTPUT_SIZE 32
#define MM2S_1_BASE  XPAR_XMM2S_1_0_S_AXI_CONTROL_BASEADDR
#define S2MM_1_BASE XPAR_XS2MM_1_0_S_AXI_CONTROL_BASEADDR
#define IRQ_NUMBER 				116U //XPAR_FABRIC_INTC_0_VEC_ID
//For debug
#define DEBUG       0
static XS2mm_1 S2mmInstance;
static XMm2s_1 Mm2sInstance;
static XScuGic GicInstance; /* Instance of the GIC Interrupt Controller */
static XIntc IncInstance;  /* Instance of the Intc Interrupt Controller */
int32_t*  INTOUT_BUFF_ADDR;
// Judge the interrupt is coming or not.
static int int_coming=1;
#ifdef DEBUG
#define INTERRUPT_CONTROLLER_BASE_ADDRESS XPAR_INTC_0_BASEADDR
int  ObtainPLIntContrlIRQStatus(uint64_t interrupt_controller_base_addr)
{
	//if(DEBUG)
		//printf("	INFO:: Interrupt Status register value is %x \n",Xil_In32(interrupt_controller_base_addr));	
	return Xil_In32(interrupt_controller_base_addr);
}
#endif

int MemoryReadWrite(uint64_t address, int32_t value,uint64_t length)
{
       int i=0;
	   if(DEBUG)
        	printf("	INFO:: A72 starts writing 16 consecutive addresses starting from address:%x\n",address);
    	for(i=0;i<length;i++)
        {
            Xil_Out32(address+i, value);

        }
		if(DEBUG)
        	printf("	INFO:: A72 starts reading 16 consecutive addresses starting from address and compare the value\n");
        for(i=0;i<length;i++)
        {
            if(Xil_In32(address+i)!=value)
            {
                printf("	ERROR::the value of address %x is not equal to %x\n", address+i, value+i);
                return 0;
            }
        }
    return 1;
}

int PS2MemAccessCheck()
    {
        if(MemoryReadWrite(DDR4_LOW0_MEMORY_BASEADDR, 0xa6a6a6a6,1024)==0)
        	printf("	ERROR::DDR4 LOW0 address checking is failed\r\n");
        if(MemoryReadWrite(DDR4_LOW1_MEMORY_BASEADDR, 0xa6a6a6a6,1024)==0)
        	printf("	ERROR::DDR4 LOW1 address checking is failed\r\n");
        if(MemoryReadWrite(LPDDR4_MEMORY_BASEADDR, 0xa6a6a6a6,1024)==0)
        	printf("	ERROR::LPDDR4 address checking is failed\r\n");
        printf("	INFO::PS to memory access check: PASS!\r\n"); 
        return 1;  
    }

// PS try to access kernel by reading or writing the register
int PS2KernelAccessCheck()
{
	uint32_t a=0xafaf;
	printf("	INFO::Try to wite MM2S kernel offset is %x, Value is %x \r\n",XMM2S_1_CONTROL_ADDR_MEM_DATA, 0xa );
	Xil_Out32(MM2S_1_BASE + XMM2S_1_CONTROL_ADDR_MEM_DATA, a);
	if(Xil_In32(MM2S_1_BASE + XMM2S_1_CONTROL_ADDR_MEM_DATA)!=a)
	{
		if(DEBUG)
			printf("	ERROR::Kernel register read write failed\r\n");
			printf("	ERROR::kernel register  is not accessible\r\n");
			printf("	ERROR::Please check the register is right or not\r\n");
	}
	else
	{
		printf("	INFO::kernel register  is accessible\r\n");
		printf("	INFO::Kernel register access check: PASS!\r\n");
	}
		
	return 1;
}
void InitData(int32_t** out, int size)
{
	int i;
	*out = (int32_t*)malloc(sizeof(int32_t) * size);

	if(!out) {
		printf("	ERROR::Allocation of memory failed \n");
		exit(-1);
	}

	for(i = 0; i < size; i++) {
		(*out)[i] = 0xABCDEF00;
	}
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
void PL2MemoryAccessCheck()
{
	int32_t  errCount = 0;
	int32_t* OUT_BUFF_ADDR;
	InitData(&OUT_BUFF_ADDR, OUTPUT_SIZE);
	if(PLKernelInitialize()==XST_SUCCESS)
		printf("	INFO::PL kernels are initialized\r\n");

	PLKernelConfig(OUT_BUFF_ADDR,plio_in,OUTPUT_SIZE,INPUT_SIZE);

	XS2mm_1_DisableAutoRestart(&S2mmInstance);
	XS2mm_1_InterruptDisable(&S2mmInstance,0x2);
	XS2mm_1_InterruptGlobalDisable(&S2mmInstance);
	PLKernelStart();
	while(1) {
		uint32_t v1 = XS2mm_1_ReadReg(S2MM_1_BASE, XS2MM_1_CONTROL_ADDR_AP_CTRL);
		if (DEBUG)
			printf("	INFO::S2MM CTRL register is %x \n",XS2mm_1_ReadReg(S2MM_1_BASE ,XS2MM_1_CONTROL_ADDR_AP_CTRL));
		if( (v1 & 6) ) {
			break;
		}
	}

    printf("	INFO::Comparing the data S2MM write memory with the data MM2S read from memory\n");

	
	for(int32_t i = 0; i < OUTPUT_SIZE; i++) {
		if((((int32_t*)OUT_BUFF_ADDR)[i] != plio_in[i]) ) {
			printf("	ERROR:: the data %d S2MM write to memory != the data %d MM2s read from memory\n", ((int32_t*)OUT_BUFF_ADDR)[i], plio_in[i]);
			errCount++;
		} else {
			if (DEBUG)
				printf("	INFO::the data %d S2MM write to memory = the data %d MM2s read from memory\n", ((int32_t*)OUT_BUFF_ADDR)[i],plio_in[i]);
		}
	}

	if(errCount == 0)
	{
		printf("	INFO::All the data MM2S read from memory equal to the data S2MM write to memory\r\n");
		printf("	INFO::Kernel to memory access check: PASS! \r\n");
	}
	else
	{
		printf("	ERROR::Kernel to memory access check failed: \r\n");
		printf("	ERROR::Please check the memory address and memory setting in Vivado: \r\n");
	}

	
}


int SetUpInterruptSystem(XScuGic *XScuGicInstancePtr)
{

	/*
	 * Connect the interrupt controller interrupt handler to the hardware
	 * interrupt handling logic in the ARM processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler) XScuGic_InterruptHandler,
			XScuGicInstancePtr);

	/*
	 * Enable interrupts in the ARM
	 */
	Xil_ExceptionEnable();

	return XST_SUCCESS;
}
//Xparameter.h 1393
void *_irqHandler(XScuGic *InstancePtr)
{
	int i, errCount=0;
	printf("	INFO::Interrupt is detected in PS side\r\n");
	//Stop interrupt
	XScuGic_Stop(InstancePtr);
	
	
	//XIntc_Acknowledge(&IncInstance,0x0);	
	for(i = 0; i < OUTPUT_SIZE; i++) {
		if((((int32_t*) INTOUT_BUFF_ADDR)[i] != plio_in[i]) ) {
			printf("	ERROR:: the data %d S2MM write to memory != the data %d MM2s read from memory\n", ((int32_t*) INTOUT_BUFF_ADDR)[i], plio_in[i]);
			errCount++;
		} else {
			if (DEBUG)
				printf("INFO::the data %d S2MM write to memory = the data %d MM2s read from memory\n", ((int32_t*) INTOUT_BUFF_ADDR)[i],plio_in[i]);
		}
	}

	if(errCount == 0)
	{
		printf("	INFO::Succeed to get data from memory in interrupt mode! \r\n");
		printf("	INFO::Interrupt connection check: PASS! \r\n");
	}
	else
	{
		printf("	ERROR:Fail to get data from memory in interrupt mode! \r\n");
		printf("	ERROR:Please check the setting of the S2MM and MM2S! \r\n");
	}
	int_coming=0;
	
}

int InterruptInitialize()
{
	int Status;
	static XScuGic_Config *GicConfig;
	static XIntc_Config *IntcConfig;

	//Find the Gic config 
	GicConfig = XScuGic_LookupConfig(INTC_DEVICE_ID2);
	if (NULL == GicConfig) {
		return XST_FAILURE;
	}
	Status = XScuGic_CfgInitialize(&GicInstance, GicConfig,
				GicConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	Status = SetUpInterruptSystem(&GicInstance);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	Status=XScuGic_Connect(&GicInstance,IRQ_NUMBER , (Xil_InterruptHandler)_irqHandler,&GicInstance);
	if (Status != XST_SUCCESS) 
	{
		printf("	ERROR::Can not connect the handler to the irq source\r\n");
		printf("	ERROR::Please check the IRQ_NUMBER is right or not\r\n");
		return XST_FAILURE;
	}
	
		
	XScuGic_SetPriorityTriggerType(&GicInstance, IRQ_NUMBER ,(u8)0x8, (u8)0x01);
	XScuGic_InterruptHandler(&GicInstance);

	
	Status=XIntc_Initialize(&IncInstance,INTC_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	Status=XIntc_Start(&IncInstance, XIN_REAL_MODE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	
	return XST_SUCCESS;

}
int InterruptEnable()
{
	
	//Clear all the possible interrupts
	int CurrentISR = XIntc_In32(IncInstance.BaseAddress + XIN_ISR_OFFSET);
	
	/*
	 * ISR should be zero after all interrupts are acknowledged
	 */
	if (CurrentISR != 0) {
		return 	XST_FAILURE;
	}
	//Enable S2MM, Interrupt controller to generate interrupt
	XScuGic_Enable(&GicInstance,IRQ_NUMBER );
	//ID is start from 0
	XIntc_Enable(&IncInstance,0);
	if(DEBUG)
		printf("	INTC IRQ enable register and IRQ output enable register are:%x, %x\r\n",Xil_In32(0xa4000000+0x8),Xil_In32(0xa4000000+0x1c));
	//Change the S2MM and Mm2s Ap_done status
	XS2mm_1_Continue(&S2mmInstance);
	XMm2s_1_Continue(&Mm2sInstance);
	
	//Enbale the S2mm interrupt register
	XS2mm_1_InterruptGlobalEnable(&S2mmInstance);
	if(DEBUG)
	{
		printf("	INFO::s2mm IRQ status register is %x, ready=%x\r\n",XS2mm_1_ReadReg(S2mmInstance.Control_BaseAddress, XS2MM_1_CONTROL_ADDR_ISR),XS2mm_1_IsReady(&S2mmInstance));
		printf("	INFO::s2mm IRQ status register is %x, ready=%x, idle=%x, IRQstau=%x\r\n",XS2mm_1_ReadReg(S2mmInstance.Control_BaseAddress, XS2MM_1_CONTROL_ADDR_ISR),XS2mm_1_IsReady(&S2mmInstance),XS2mm_1_IsIdle(&S2mmInstance),XS2mm_1_InterruptGetStatus(&S2mmInstance));
	}
	XS2mm_1_InterruptEnable(&S2mmInstance,0x3);
	return XST_SUCCESS;
	
}
void PL2PSInterruptCheck()
{
	printf("	INFO::We are trying to move data in interrupt mode\r\n");
	//As PL kernels have already been initialized so omit this step
	//if(PLKernelInitialize()==XST_SUCCESS)
		//printf("	PL kernels are initialized\r\n");
	InitData(&INTOUT_BUFF_ADDR, OUTPUT_SIZE);
	PLKernelConfig(INTOUT_BUFF_ADDR,plio_in,OUTPUT_SIZE,INPUT_SIZE);
	if(InterruptInitialize()==XST_SUCCESS)
		printf("	INFO::GIC and INC are initialized\r\n");
	if(InterruptEnable()==XST_SUCCESS)
		printf("	INFO::Interrupt is enabled\r\n");
	PLKernelStart();
	if(DEBUG)
	{
		printf("	INFO::S2MM CTRL register is %x \n",XS2mm_1_ReadReg(S2MM_1_BASE ,XS2MM_1_CONTROL_ADDR_AP_CTRL));
		printf("	INFO::MM2S CTRL register is %x \n",XMm2s_1_ReadReg(MM2S_1_BASE ,XMM2S_1_CONTROL_ADDR_AP_CTRL));
	}
	
}
int main()
{

	Xil_DCacheDisable();
    sleep(1);
	printf("- \n");
	printf("#Beginning test\n");
	printf("--\r\n");
	printf("#Beginning PS to Memory test\r\n");
    PS2MemAccessCheck();
	printf("#Beginning PS to Kernel (register) test\r\n");
	PS2KernelAccessCheck();
	printf("#Beginning PL Kernel to Memory test\r\n");
	PL2MemoryAccessCheck();
	printf("#Beginning to do interrupt check\r\n");
	PL2PSInterruptCheck();
	
	while(1)
	{
		if(int_coming==0)
			break;
		else 
		{
			//Especially in emulation mode, As PL is a little slow. leave enough time for PL to complete the transmission.
			while(1)
			{
				uint32_t v1 = XS2mm_1_ReadReg(S2MM_1_BASE, XS2MM_1_CONTROL_ADDR_AP_CTRL);
				if( (v1 & 6) ) {
					if(DEBUG)
						{
							printf("	INFO::s2mm IRQ status register is %x, ready=%x\r\n",XS2mm_1_ReadReg(S2mmInstance.Control_BaseAddress, XS2MM_1_CONTROL_ADDR_ISR),XS2mm_1_IsReady(&S2mmInstance));
							printf("	INFO::s2mm IRQ status register is %x, ready=%x, idle=%x, IRQstau=%x\r\n",XS2mm_1_ReadReg(S2mmInstance.Control_BaseAddress, XS2MM_1_CONTROL_ADDR_ISR),XS2mm_1_IsReady(&S2mmInstance),XS2mm_1_IsIdle(&S2mmInstance),XS2mm_1_InterruptGetStatus(&S2mmInstance));
						}
					break;
				}
			}
				
		}		
		
		
		if(int_coming==1)
		{
			printf("	ERROR::Interrupt is not detected in PS side\r\n");
			printf("	ERROR::Please check whether the interrupt is exported in your platform and the interrupt connection in Vivado block Design！\r\n");
			break;
		}
	}
	printf("#End\r\n");
	return 1;
}
