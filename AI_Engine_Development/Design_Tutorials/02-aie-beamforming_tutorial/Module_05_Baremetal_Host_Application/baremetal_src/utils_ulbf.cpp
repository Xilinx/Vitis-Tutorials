//  © Copyright 2020 Xilinx, Inc.
//  Licensed under the Apache License, Version 2.0 (the 'License');
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//  
//       http://www.apache.org/licenses/LICENSE-2.0
//  
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an 'AS IS' BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.

#include "params.h"
#include "utils.h"
#include "utils_ulbf.h"

#define DEBUGTEST

uint64_t ulbfDataAddr[NUM_ULBF_DATA] = {
XPAR_ULBF_DATA_00_BASEADDR,
XPAR_ULBF_DATA_10_BASEADDR,
XPAR_ULBF_DATA_20_BASEADDR,
};

uint64_t ulbfCoeffsAddr[NUM_ULBF_COEFFS] = {
XPAR_ULBF_COEFFS_00_BASEADDR,
XPAR_ULBF_COEFFS_01_BASEADDR,
XPAR_ULBF_COEFFS_02_BASEADDR,
XPAR_ULBF_COEFFS_03_BASEADDR,
XPAR_ULBF_COEFFS_10_BASEADDR,
XPAR_ULBF_COEFFS_11_BASEADDR,
XPAR_ULBF_COEFFS_12_BASEADDR,
XPAR_ULBF_COEFFS_13_BASEADDR,
XPAR_ULBF_COEFFS_20_BASEADDR,
XPAR_ULBF_COEFFS_21_BASEADDR,
XPAR_ULBF_COEFFS_22_BASEADDR,
XPAR_ULBF_COEFFS_23_BASEADDR,

};

uint64_t ulbfSlaveAddr[NUM_ULBF_SLAVES] = {
XPAR_ULBF_O00_BASEADDR,
XPAR_ULBF_O01_BASEADDR,
XPAR_ULBF_O02_BASEADDR,
XPAR_ULBF_O03_BASEADDR,
XPAR_ULBF_O10_BASEADDR,
XPAR_ULBF_O11_BASEADDR,
XPAR_ULBF_O12_BASEADDR,
XPAR_ULBF_O13_BASEADDR,
XPAR_ULBF_O20_BASEADDR,
XPAR_ULBF_O21_BASEADDR,
XPAR_ULBF_O22_BASEADDR,
XPAR_ULBF_O23_BASEADDR,
};

void ulbf_data_config_ip(unsigned int i, unsigned int ipBaseAddr, unsigned int niter ) {
	unsigned int csrAddr;
	csrAddr = ipBaseAddr + ULBF_DATA_CSR_OFFSET  + ULBF_DATA_RAM0_OFFSET; 
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_BLOCK_SIZE, ULBF_DATA_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_ROLLOVER,   ULBF_DATA_INPUT_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_DATA_CSR_OFFSET  + ULBF_DATA_RAM1_OFFSET; 
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_BLOCK_SIZE, ULBF_DATA_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_ROLLOVER,   ULBF_DATA_INPUT_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_DATA_CSR_OFFSET  + ULBF_DATA_RAM2_OFFSET; 
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_BLOCK_SIZE, ULBF_DATA_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_ROLLOVER,   ULBF_DATA_INPUT_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_DATA_CSR_OFFSET  + ULBF_DATA_RAM3_OFFSET; 
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_BLOCK_SIZE, ULBF_DATA_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_ROLLOVER,   ULBF_DATA_INPUT_ROLLOVER_ADDR);

        csrAddr = ipBaseAddr + ULBF_DATA_CSR_OFFSET  + ULBF_DATA_RAM4_OFFSET; 
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_BLOCK_SIZE, ULBF_DATA_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_ROLLOVER,   ULBF_DATA_INPUT_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_DATA_CSR_OFFSET  + ULBF_DATA_RAM5_OFFSET; 
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_BLOCK_SIZE, ULBF_DATA_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_ROLLOVER,   ULBF_DATA_INPUT_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_DATA_CSR_OFFSET  + ULBF_DATA_RAM6_OFFSET; 
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_BLOCK_SIZE, ULBF_DATA_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_ROLLOVER,   ULBF_DATA_INPUT_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_DATA_CSR_OFFSET  + ULBF_DATA_RAM7_OFFSET; 
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_BLOCK_SIZE, ULBF_DATA_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+ULBF_DATA_REG_OFFSET_ROLLOVER,   ULBF_DATA_INPUT_ROLLOVER_ADDR);

	printf("[ulbf_data_config_ip] IP:%d/%d Done \n", i, NUM_ULBF_DATA-1);
}

void ulbf_data_config_ips(unsigned int niter) {
	for (unsigned int i=0; i<NUM_ULBF_DATA;i++) 
		ulbf_data_config_ip(i, ulbfDataAddr[i], niter );
}

void ulbf_data_get_ip_status(unsigned int i, unsigned int ipBaseAddr) {
	xil_printf("---------------------------\n");
	unsigned int addr;
	addr = ipBaseAddr + ULBF_DATA_RAM0_OFFSET + ULBF_DATA_CSR_OFFSET; 
	xil_printf("[ulbf_data-0/7] IP:%d/%d ULBF_DATA_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_ID)); 
	xil_printf("[ulbf_data-0/7] IP:%d/%d ULBF_DATA_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_RESET)); 
	xil_printf("[ulbf_data-0/7] IP:%d/%d ULBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[ulbf_data-0/7] IP:%d/%d ULBF_DATA_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_NITER)); 
	xil_printf("[ulbf_data-0/7] IP:%d/%d ULBF_DATA_REG_OFFSET_ROLLOVER    : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_ROLLOVER)); 
	xil_printf("[ulbf_data-0/7] IP:%d/%d ULBF_DATA_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_DONE)); 
	xil_printf("[ulbf_data-0/7] IP:%d/%d ULBF_DATA_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_RAM_STATUS));  

	addr = ipBaseAddr + ULBF_DATA_RAM1_OFFSET + ULBF_DATA_CSR_OFFSET; 
	xil_printf("[ulbf_data-1/7] IP:%d/%d ULBF_DATA_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_ID)); 
	xil_printf("[ulbf_data-1/7] IP:%d/%d ULBF_DATA_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_RESET)); 
	xil_printf("[ulbf_data-1/7] IP:%d/%d ULBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[ulbf_data-1/7] IP:%d/%d ULBF_DATA_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_NITER)); 
	xil_printf("[ulbf_data-1/7] IP:%d/%d ULBF_DATA_REG_OFFSET_ROLLOVER    : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_ROLLOVER)); 
	xil_printf("[ulbf_data-1/7] IP:%d/%d ULBF_DATA_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_DONE)); 
	xil_printf("[ulbf_data-1/7] IP:%d/%d ULBF_DATA_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_RAM_STATUS));  

	addr = ipBaseAddr + ULBF_DATA_RAM2_OFFSET + ULBF_DATA_CSR_OFFSET; 
	xil_printf("[ulbf_data-2/7] IP:%d/%d ULBF_DATA_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_ID)); 
	xil_printf("[ulbf_data-2/7] IP:%d/%d ULBF_DATA_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_RESET)); 
	xil_printf("[ulbf_data-2/7] IP:%d/%d ULBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[ulbf_data-2/7] IP:%d/%d ULBF_DATA_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_NITER)); 
	xil_printf("[ulbf_data-2/7] IP:%d/%d ULBF_DATA_REG_OFFSET_ROLLOVER    : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_ROLLOVER)); 
	xil_printf("[ulbf_data-2/7] IP:%d/%d ULBF_DATA_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_DONE)); 
	xil_printf("[ulbf_data-2/7] IP:%d/%d ULBF_DATA_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_ULBF_DATA, Xil_In32(addr+ULBF_DATA_REG_OFFSET_RAM_STATUS));  

	addr = ipBaseAddr + ULBF_DATA_RAM3_OFFSET + ULBF_DATA_CSR_OFFSET; 
	xil_printf("[ulbf_data-3/7] IP:%d/%d ULBF_DATA_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_ID)); 
	xil_printf("[ulbf_data-3/7] IP:%d/%d ULBF_DATA_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_RESET)); 
	xil_printf("[ulbf_data-3/7] IP:%d/%d ULBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[ulbf_data-3/7] IP:%d/%d ULBF_DATA_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_NITER)); 
	xil_printf("[ulbf_data-3/7] IP:%d/%d ULBF_DATA_REG_OFFSET_ROLLOVER    : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_ROLLOVER)); 
	xil_printf("[ulbf_data-3/7] IP:%d/%d ULBF_DATA_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_DONE)); 
	xil_printf("[ulbf_data-3/7] IP:%d/%d ULBF_DATA_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_RAM_STATUS));  

	addr = ipBaseAddr + ULBF_DATA_RAM4_OFFSET + ULBF_DATA_CSR_OFFSET; 
	xil_printf("[ulbf_data-4/7] IP:%d/%d ULBF_DATA_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_ID)); 
	xil_printf("[ulbf_data-4/7] IP:%d/%d ULBF_DATA_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_RESET)); 
	xil_printf("[ulbf_data-4/7] IP:%d/%d ULBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[ulbf_data-4/7] IP:%d/%d ULBF_DATA_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_NITER)); 
	xil_printf("[ulbf_data-4/7] IP:%d/%d ULBF_DATA_REG_OFFSET_ROLLOVER    : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_ROLLOVER)); 
	xil_printf("[ulbf_data-4/7] IP:%d/%d ULBF_DATA_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_DONE)); 
	xil_printf("[ulbf_data-4/7] IP:%d/%d ULBF_DATA_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_RAM_STATUS));  

	addr = ipBaseAddr + ULBF_DATA_RAM5_OFFSET + ULBF_DATA_CSR_OFFSET; 
	xil_printf("[ulbf_data-5/7] IP:%d/%d ULBF_DATA_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_ID)); 
	xil_printf("[ulbf_data-5/7] IP:%d/%d ULBF_DATA_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_RESET)); 
	xil_printf("[ulbf_data-5/7] IP:%d/%d ULBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[ulbf_data-5/7] IP:%d/%d ULBF_DATA_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_NITER)); 
	xil_printf("[ulbf_data-5/7] IP:%d/%d ULBF_DATA_REG_OFFSET_ROLLOVER    : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_ROLLOVER)); 
	xil_printf("[ulbf_data-5/7] IP:%d/%d ULBF_DATA_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_DONE)); 
	xil_printf("[ulbf_data-5/7] IP:%d/%d ULBF_DATA_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_RAM_STATUS));  

	addr = ipBaseAddr + ULBF_DATA_RAM6_OFFSET + ULBF_DATA_CSR_OFFSET; 
	xil_printf("[ulbf_data-6/7] IP:%d/%d ULBF_DATA_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_ID)); 
	xil_printf("[ulbf_data-6/7] IP:%d/%d ULBF_DATA_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_RESET)); 
	xil_printf("[ulbf_data-6/7] IP:%d/%d ULBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[ulbf_data-6/7] IP:%d/%d ULBF_DATA_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_NITER)); 
	xil_printf("[ulbf_data-6/7] IP:%d/%d ULBF_DATA_REG_OFFSET_ROLLOVER    : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_ROLLOVER)); 
	xil_printf("[ulbf_data-6/7] IP:%d/%d ULBF_DATA_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_DONE)); 
	xil_printf("[ulbf_data-6/7] IP:%d/%d ULBF_DATA_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_RAM_STATUS));  

	addr = ipBaseAddr + ULBF_DATA_RAM7_OFFSET + ULBF_DATA_CSR_OFFSET; 
	xil_printf("[ulbf_data-7/7] IP:%d/%d ULBF_DATA_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_ID)); 
	xil_printf("[ulbf_data-7/7] IP:%d/%d ULBF_DATA_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_RESET)); 
	xil_printf("[ulbf_data-7/7] IP:%d/%d ULBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[ulbf_data-7/7] IP:%d/%d ULBF_DATA_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_NITER)); 
	xil_printf("[ulbf_data-7/7] IP:%d/%d ULBF_DATA_REG_OFFSET_ROLLOVER    : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_ROLLOVER)); 
	xil_printf("[ulbf_data-7/7] IP:%d/%d ULBF_DATA_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_DONE)); 
	xil_printf("[ulbf_data-7/7] IP:%d/%d ULBF_DATA_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_ULBF_DATA-1, Xil_In32(addr+ULBF_DATA_REG_OFFSET_RAM_STATUS));  

}

void ulbf_data_get_all_ip_status() {
	for (unsigned int i=0; i<NUM_ULBF_DATA;i++) 
		ulbf_data_get_ip_status(i, ulbfDataAddr[i] );
}


void ulbf_coeffs_config_ip(unsigned int i, unsigned int ipBaseAddr, unsigned int niter ) {
	unsigned int csrAddr;
	csrAddr = ipBaseAddr + ULBF_COEFFS_CSR_OFFSET  + ULBF_COEFFS_RAM0_OFFSET; 
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE, ULBF_COEFFS_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_ROLLOVER,   ULBF_COEFFS_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_COEFFS_CSR_OFFSET  + ULBF_COEFFS_RAM1_OFFSET; 
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE, ULBF_COEFFS_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_ROLLOVER,   ULBF_COEFFS_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_COEFFS_CSR_OFFSET  + ULBF_COEFFS_RAM2_OFFSET; 
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE, ULBF_COEFFS_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_ROLLOVER,   ULBF_COEFFS_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_COEFFS_CSR_OFFSET  + ULBF_COEFFS_RAM3_OFFSET; 
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE, ULBF_COEFFS_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_ROLLOVER,   ULBF_COEFFS_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_COEFFS_CSR_OFFSET  + ULBF_COEFFS_RAM4_OFFSET; 
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE, ULBF_COEFFS_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_ROLLOVER,   ULBF_COEFFS_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_COEFFS_CSR_OFFSET  + ULBF_COEFFS_RAM5_OFFSET; 
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE, ULBF_COEFFS_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_ROLLOVER,   ULBF_COEFFS_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_COEFFS_CSR_OFFSET  + ULBF_COEFFS_RAM6_OFFSET; 
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE, ULBF_COEFFS_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_ROLLOVER,   ULBF_COEFFS_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_COEFFS_CSR_OFFSET  + ULBF_COEFFS_RAM7_OFFSET; 
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE, ULBF_COEFFS_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+ULBF_COEFFS_REG_OFFSET_ROLLOVER,   ULBF_COEFFS_ROLLOVER_ADDR);

	printf("[ulbf_coeffs_config_ip] IP:%d/%d Done \n", i, NUM_ULBF_COEFFS-1);
}

void ulbf_coeffs_config_ips(unsigned int niter) {
	for (unsigned int i=0; i<NUM_ULBF_COEFFS;i++) 
		ulbf_coeffs_config_ip(i, ulbfCoeffsAddr[i], niter );
}

void ulbf_coeffs_get_ip_status(unsigned int i, unsigned int ipBaseAddr) {
	xil_printf("---------------------------\n");
	unsigned int addr;
	addr = ipBaseAddr + ULBF_COEFFS_RAM0_OFFSET + ULBF_COEFFS_CSR_OFFSET; 
	xil_printf("[ulbf_coeffs-0/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_ID)); 
	xil_printf("[ulbf_coeffs-0/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_RESET)); 
	xil_printf("[ulbf_coeffs-0/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[ulbf_coeffs-0/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_NITER)); 
	xil_printf("[ulbf_coeffs-0/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_ROLLOVER	  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_ROLLOVER)); 
	xil_printf("[ulbf_coeffs-0/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_DONE)); 
	xil_printf("[ulbf_coeffs-0/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_RAM_STATUS));  

	addr = ipBaseAddr + ULBF_COEFFS_RAM1_OFFSET + ULBF_COEFFS_CSR_OFFSET; 
	xil_printf("[ulbf_coeffs-1/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_ID)); 
	xil_printf("[ulbf_coeffs-1/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_RESET)); 
	xil_printf("[ulbf_coeffs-1/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[ulbf_coeffs-1/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_NITER)); 
	xil_printf("[ulbf_coeffs-1/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_ROLLOVER	  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_ROLLOVER)); 
	xil_printf("[ulbf_coeffs-1/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_DONE)); 
	xil_printf("[ulbf_coeffs-1/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_RAM_STATUS));  

	addr = ipBaseAddr + ULBF_COEFFS_RAM2_OFFSET + ULBF_COEFFS_CSR_OFFSET; 
	xil_printf("[ulbf_coeffs-2/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_ID)); 
	xil_printf("[ulbf_coeffs-2/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_RESET)); 
	xil_printf("[ulbf_coeffs-2/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[ulbf_coeffs-2/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_NITER)); 
	xil_printf("[ulbf_coeffs-2/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_ROLLOVER	  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_ROLLOVER)); 
	xil_printf("[ulbf_coeffs-2/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_DONE)); 
	xil_printf("[ulbf_coeffs-2/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_RAM_STATUS));  

	addr = ipBaseAddr + ULBF_COEFFS_RAM3_OFFSET + ULBF_COEFFS_CSR_OFFSET; 
	xil_printf("[ulbf_coeffs-3/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_ID)); 
	xil_printf("[ulbf_coeffs-3/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_RESET)); 
	xil_printf("[ulbf_coeffs-3/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[ulbf_coeffs-3/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_NITER)); 
	xil_printf("[ulbf_coeffs-3/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_ROLLOVER	  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_ROLLOVER)); 
	xil_printf("[ulbf_coeffs-3/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_DONE)); 
	xil_printf("[ulbf_coeffs-3/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_RAM_STATUS));  

	addr = ipBaseAddr + ULBF_COEFFS_RAM4_OFFSET + ULBF_COEFFS_CSR_OFFSET; 
	xil_printf("[ulbf_coeffs-4/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_ID)); 
	xil_printf("[ulbf_coeffs-4/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_RESET)); 
	xil_printf("[ulbf_coeffs-4/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[ulbf_coeffs-4/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_NITER)); 
	xil_printf("[ulbf_coeffs-4/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_ROLLOVER	  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_ROLLOVER)); 
	xil_printf("[ulbf_coeffs-4/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_DONE)); 
	xil_printf("[ulbf_coeffs-4/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_RAM_STATUS));  

	addr = ipBaseAddr + ULBF_COEFFS_RAM5_OFFSET + ULBF_COEFFS_CSR_OFFSET; 
	xil_printf("[ulbf_coeffs-5/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_ID)); 
	xil_printf("[ulbf_coeffs-5/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_RESET)); 
	xil_printf("[ulbf_coeffs-5/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[ulbf_coeffs-5/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_NITER)); 
	xil_printf("[ulbf_coeffs-5/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_ROLLOVER	  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_ROLLOVER)); 
	xil_printf("[ulbf_coeffs-5/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_DONE)); 
	xil_printf("[ulbf_coeffs-5/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_RAM_STATUS));  

	addr = ipBaseAddr + ULBF_COEFFS_RAM6_OFFSET + ULBF_COEFFS_CSR_OFFSET; 
	xil_printf("[ulbf_coeffs-6/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_ID)); 
	xil_printf("[ulbf_coeffs-6/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_RESET)); 
	xil_printf("[ulbf_coeffs-6/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[ulbf_coeffs-6/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_NITER)); 
	xil_printf("[ulbf_coeffs-6/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_ROLLOVER	  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_ROLLOVER)); 
	xil_printf("[ulbf_coeffs-6/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_DONE)); 
	xil_printf("[ulbf_coeffs-6/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_RAM_STATUS));  
		
	addr = ipBaseAddr + ULBF_COEFFS_RAM7_OFFSET + ULBF_COEFFS_CSR_OFFSET; 
	xil_printf("[ulbf_coeffs-7/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_ID)); 
	xil_printf("[ulbf_coeffs-7/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_RESET)); 
	xil_printf("[ulbf_coeffs-7/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[ulbf_coeffs-7/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_NITER)); 
	xil_printf("[ulbf_coeffs-7/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_ROLLOVER	  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_ROLLOVER)); 
	xil_printf("[ulbf_coeffs-7/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_DONE)); 
	xil_printf("[ulbf_coeffs-7/7] IP:%d/%d ULBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_ULBF_COEFFS-1, Xil_In32(addr+ULBF_COEFFS_REG_OFFSET_RAM_STATUS));  

}

void ulbf_coeffs_get_all_ip_status() {
	for (unsigned int i=0; i<NUM_ULBF_COEFFS;i++) 
		ulbf_coeffs_get_ip_status(i, ulbfCoeffsAddr[i] );
}

void ulbf_slave_config_ip(unsigned int i, unsigned int ipBaseAddr, unsigned int niter ) {
	unsigned int csrAddr = ipBaseAddr + ULBF_SLAVE_CSR_OFFSET ; 
	Xil_Out32(csrAddr+ULBF_SLAVE_REG_OFFSET_NITER,      niter);
	printf("[ulbf_slave_config_ip] IP:%d/%d Done \n", i, NUM_ULBF_SLAVES-1);
}

void ulbf_slave_config_ips(unsigned int niter) {
	for (unsigned int i=0; i<NUM_ULBF_SLAVES;i++) 
		ulbf_slave_config_ip(i, ulbfSlaveAddr[i], niter );
}

void ulbf_slave_get_ip_status(unsigned int i, unsigned int ipBaseAddr) {
	xil_printf("---------------------------\n");
	unsigned int addr;
	addr = ipBaseAddr + ULBF_SLAVE_CSR_OFFSET; 
	xil_printf("[ulbf_slave] IP:%d/%d ULBF_SLAVE_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_ULBF_SLAVES-1, Xil_In32(addr+ULBF_SLAVE_REG_OFFSET_ID)); 
	xil_printf("[ulbf_slave] IP:%d/%d ULBF_SLAVE_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_ULBF_SLAVES-1, Xil_In32(addr+ULBF_SLAVE_REG_OFFSET_RESET)); 
	xil_printf("[ulbf_slave] IP:%d/%d ULBF_SLAVE_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_ULBF_SLAVES-1, Xil_In32(addr+ULBF_SLAVE_REG_OFFSET_NITER)); 
	xil_printf("[ulbf_slave] IP:%d/%d ULBF_SLAVE_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_ULBF_SLAVES-1, Xil_In32(addr+ULBF_SLAVE_REG_OFFSET_DONE)); 
	xil_printf("[ulbf_slave] IP:%d/%d ULBF_SLAVE_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_ULBF_SLAVES-1, Xil_In32(addr+ULBF_SLAVE_REG_OFFSET_RAM_STATUS));  
}

void ulbf_slave_get_all_ip_status() {
	for (unsigned int i=0; i<NUM_ULBF_SLAVES;i++) 
		ulbf_slave_get_ip_status(i, ulbfSlaveAddr[i] );
}


void ulbf_data_reset() {
	unsigned long sleepTime = 2; 
	for (unsigned int i=0; i<NUM_ULBF_DATA; i++ ) {
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM0_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 1);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM1_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 1);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM2_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 1);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM3_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 1);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM4_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 1);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM5_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 1);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM6_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 1);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM7_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 1);
	}
	usleep(sleepTime); 
	for (unsigned int i=0; i<NUM_ULBF_DATA; i++ ) {
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM0_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 0);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM1_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 0);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM2_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 0);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM3_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 0);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM4_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 0);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM5_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 0);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM6_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 0);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM7_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 0);
	}
	xil_printf("[ulbf_data_reset] Done \n");  
}

void ulbf_coeffs_reset() {
	unsigned long sleepTime = 2; 
	for (unsigned int i=0; i<NUM_ULBF_COEFFS; i++ ) {
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM0_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 1);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM1_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 1);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM2_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 1);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM3_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 1);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM4_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 1);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM5_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 1);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM6_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 1);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM7_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 1);
	}
	usleep(sleepTime); 
	for (unsigned int i=0; i<NUM_ULBF_COEFFS; i++ ) {
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM0_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 0);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM1_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 0);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM2_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 0);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM3_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 0);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM4_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 0);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM5_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 0);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM6_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 0);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM7_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 0);
	}
	xil_printf("[ulbf_coeffs_reset] Done \n");  
}

void ulbf_slave_reset() {
	unsigned long sleepTime = 2; 
	for (unsigned int i=0; i<NUM_ULBF_SLAVES; i++ ) {
		Xil_Out32( ulbfSlaveAddr[i]+ULBF_SLAVE_CSR_OFFSET+ULBF_SLAVE_REG_OFFSET_RESET, 1);
	}
	usleep(sleepTime); 
	for (unsigned int i=0; i<NUM_ULBF_SLAVES; i++ ) {
		Xil_Out32( ulbfSlaveAddr[i]+ULBF_SLAVE_CSR_OFFSET+ULBF_SLAVE_REG_OFFSET_RESET, 0);
	}
	xil_printf("[ulbf_slave_reset] Done \n");  
}

void ulbf_reset() {
	ulbf_data_reset();
	ulbf_coeffs_reset();
	ulbf_slave_reset();
}

void ulbf_data_check_ram(int ipNum, int * arrPtr0, int * arrPtr1, int * arrPtr2, int * arrPtr3, int * arrPtr4, int * arrPtr5, int * arrPtr6, int * arrPtr7) {
	printf("[ulbf_data_check_ram] Reading initialized data from ulbf_data IP RAMs\n");
	int errCount = 0;
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = ulbfDataAddr[ipNum] + ULBF_DATA_RAM0_OFFSET + (i*8);
		unsigned int addr1   = ulbfDataAddr[ipNum] + ULBF_DATA_RAM0_OFFSET + ((i*8)+4);
		unsigned int data0 = Xil_In32(addr0);
		unsigned int data1 = Xil_In32(addr1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr0+0))  errCount++;
		if (imag0!=*(arrPtr0+1))  errCount++;
		if (real1!=*(arrPtr0+2))  errCount++;
		if (imag1!=*(arrPtr0+3))  errCount++;
		arrPtr0 = arrPtr0+4;
		xil_printf("[ulbf_data_check_ram] IP:%d/%d RAM0 DATA %d: 0x%08lX%08lX %d %d %d %d \n",ipNum, NUM_ULBF_DATA-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = ulbfDataAddr[ipNum] + ULBF_DATA_RAM1_OFFSET + (i*8);
		unsigned int addr1   = ulbfDataAddr[ipNum] + ULBF_DATA_RAM1_OFFSET + ((i*8)+4);
		unsigned int data0 = Xil_In32(addr0);
		unsigned int data1 = Xil_In32(addr1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr1+0))  errCount++;
		if (imag0!=*(arrPtr1+1))  errCount++;
		if (real1!=*(arrPtr1+2))  errCount++;
		if (imag1!=*(arrPtr1+3))  errCount++;
		arrPtr1 = arrPtr1+4;
		xil_printf("[ulbf_data_check_ram] IP:%d/%d RAM1 DATA %d: 0x%08lX%08lX %d %d %d %d \nn",ipNum, NUM_ULBF_DATA-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = ulbfDataAddr[ipNum] + ULBF_DATA_RAM2_OFFSET + (i*8);
		unsigned int addr1   = ulbfDataAddr[ipNum] + ULBF_DATA_RAM2_OFFSET + ((i*8)+4);
		unsigned int data0 = Xil_In32(addr0);
		unsigned int data1 = Xil_In32(addr1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr2+0))  errCount++;
		if (imag0!=*(arrPtr2+1))  errCount++;
		if (real1!=*(arrPtr2+2))  errCount++;
		if (imag1!=*(arrPtr2+3))  errCount++;
		arrPtr2 = arrPtr2+4;
		xil_printf("[ulbf_data_check_ram] IP:%d/%d RAM2 DATA %d: 0x%08lX%08lX %d %d %d %d \nn",ipNum, NUM_ULBF_DATA-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = ulbfDataAddr[ipNum] + ULBF_DATA_RAM3_OFFSET + (i*8);
		unsigned int addr1   = ulbfDataAddr[ipNum] + ULBF_DATA_RAM3_OFFSET + ((i*8)+4);
		unsigned int data0 = Xil_In32(addr0);
		unsigned int data1 = Xil_In32(addr1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr3+0))  errCount++;
		if (imag0!=*(arrPtr3+1))  errCount++;
		if (real1!=*(arrPtr3+2))  errCount++;
		if (imag1!=*(arrPtr3+3))  errCount++;
		arrPtr3 = arrPtr3+4;
		xil_printf("[ulbf_data_check_ram] IP:%d/%d RAM3 DATA %d: 0x%08lX%08lX %d %d %d %d \nn",ipNum, NUM_ULBF_DATA-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = ulbfDataAddr[ipNum] + ULBF_DATA_RAM4_OFFSET + (i*8);
		unsigned int addr1   = ulbfDataAddr[ipNum] + ULBF_DATA_RAM4_OFFSET + ((i*8)+4);
		unsigned int data0 = Xil_In32(addr0);
		unsigned int data1 = Xil_In32(addr1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr4+0))  errCount++;
		if (imag0!=*(arrPtr4+1))  errCount++;
		if (real1!=*(arrPtr4+2))  errCount++;
		if (imag1!=*(arrPtr4+3))  errCount++;
		arrPtr4 = arrPtr4+4;
		xil_printf("[ulbf_data_check_ram] IP:%d/%d RAM4 DATA %d: 0x%08lX%08lX %d %d %d %d \n",ipNum, NUM_ULBF_DATA-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = ulbfDataAddr[ipNum] + ULBF_DATA_RAM5_OFFSET + (i*8);
		unsigned int addr1   = ulbfDataAddr[ipNum] + ULBF_DATA_RAM5_OFFSET + ((i*8)+4);
		unsigned int data0 = Xil_In32(addr0);
		unsigned int data1 = Xil_In32(addr1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr5+0))  errCount++;
		if (imag0!=*(arrPtr5+1))  errCount++;
		if (real1!=*(arrPtr5+2))  errCount++;
		if (imag1!=*(arrPtr5+3))  errCount++;
		arrPtr5 = arrPtr5+4;
		xil_printf("[ulbf_data_check_ram] IP:%d/%d RAM5 DATA %d: 0x%08lX%08lX %d %d %d %d \nn",ipNum, NUM_ULBF_DATA-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = ulbfDataAddr[ipNum] + ULBF_DATA_RAM6_OFFSET + (i*8);
		unsigned int addr1   = ulbfDataAddr[ipNum] + ULBF_DATA_RAM6_OFFSET + ((i*8)+4);
		unsigned int data0 = Xil_In32(addr0);
		unsigned int data1 = Xil_In32(addr1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr6+0))  errCount++;
		if (imag0!=*(arrPtr6+1))  errCount++;
		if (real1!=*(arrPtr6+2))  errCount++;
		if (imag1!=*(arrPtr6+3))  errCount++;
		arrPtr6 = arrPtr6+4;
		xil_printf("[ulbf_data_check_ram] IP:%d/%d RAM6 DATA %d: 0x%08lX%08lX %d %d %d %d \nn",ipNum, NUM_ULBF_DATA-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = ulbfDataAddr[ipNum] + ULBF_DATA_RAM7_OFFSET + (i*8);
		unsigned int addr1   = ulbfDataAddr[ipNum] + ULBF_DATA_RAM7_OFFSET + ((i*8)+4);
		unsigned int data0 = Xil_In32(addr0);
		unsigned int data1 = Xil_In32(addr1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr7+0))  errCount++;
		if (imag0!=*(arrPtr7+1))  errCount++;
		if (real1!=*(arrPtr7+2))  errCount++;
		if (imag1!=*(arrPtr7+3))  errCount++;
		arrPtr7 = arrPtr7+4;
		xil_printf("[ulbf_data_check_ram] IP:%d/%d RAM7 DATA %d: 0x%08lX%08lX %d %d %d %d \nn",ipNum, NUM_ULBF_DATA-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	if (errCount!=0)   xil_printf("[ulbf_data_check_ram] Reading from RAM FAILED\n");
	else 	printf("[ulbf_data_check_ram] PASSED \n");
}

void ulbf_coeffs_check_ram(int ipNum, int * arrPtr0, int * arrPtr1, int * arrPtr2, int * arrPtr3, int * arrPtr4, int * arrPtr5, int * arrPtr6, int * arrPtr7) {
	printf("[ulbf_coeffs_check_ram] Reading initialized data from ulbf_coeffs IP RAMs\n");
	int errCount = 0;
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = ulbfCoeffsAddr[ipNum] + ULBF_COEFFS_RAM0_OFFSET + (i*8);
		unsigned int addr1   = ulbfCoeffsAddr[ipNum] + ULBF_COEFFS_RAM0_OFFSET + ((i*8)+4);
		unsigned int data0 = Xil_In32(addr0);
		unsigned int data1 = Xil_In32(addr1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr0+0))  errCount++;
		if (imag0!=*(arrPtr0+1))  errCount++;
		if (real1!=*(arrPtr0+2))  errCount++;
		if (imag1!=*(arrPtr0+3))  errCount++;
		arrPtr0 = arrPtr0+4;
		xil_printf("[ulbf_coeffs_check_ram] IP:%d/%d RAM0 DATA %d: 0x%08lX%08lX %d %d %d %d \n",ipNum, NUM_ULBF_COEFFS-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = ulbfCoeffsAddr[ipNum] + ULBF_COEFFS_RAM1_OFFSET + (i*8);
		unsigned int addr1   = ulbfCoeffsAddr[ipNum] + ULBF_COEFFS_RAM1_OFFSET + ((i*8)+4);
		unsigned int data0 = Xil_In32(addr0);
		unsigned int data1 = Xil_In32(addr1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr1+0))  errCount++;
		if (imag0!=*(arrPtr1+1))  errCount++;
		if (real1!=*(arrPtr1+2))  errCount++;
		if (imag1!=*(arrPtr1+3))  errCount++;
		arrPtr1 = arrPtr1+4;
		xil_printf("[ulbf_coeffs_check_ram] IP:%d/%d RAM1 DATA %d: 0x%08lX%08lX %d %d %d %d \n",ipNum, NUM_ULBF_COEFFS-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = ulbfCoeffsAddr[ipNum] + ULBF_COEFFS_RAM2_OFFSET + (i*8);
		unsigned int addr1   = ulbfCoeffsAddr[ipNum] + ULBF_COEFFS_RAM2_OFFSET + ((i*8)+4);
		unsigned int data0 = Xil_In32(addr0);
		unsigned int data1 = Xil_In32(addr1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr2+0))  errCount++;
		if (imag0!=*(arrPtr2+1))  errCount++;
		if (real1!=*(arrPtr2+2))  errCount++;
		if (imag1!=*(arrPtr2+3))  errCount++;
		arrPtr2 = arrPtr2+4;
		xil_printf("[ulbf_coeffs_check_ram] IP:%d/%d RAM2 DATA %d: 0x%08lX%08lX %d %d %d %d \n",ipNum, NUM_ULBF_COEFFS-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = ulbfCoeffsAddr[ipNum] + ULBF_COEFFS_RAM3_OFFSET + (i*8);
		unsigned int addr1   = ulbfCoeffsAddr[ipNum] + ULBF_COEFFS_RAM3_OFFSET + ((i*8)+4);
		unsigned int data0 = Xil_In32(addr0);
		unsigned int data1 = Xil_In32(addr1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr3+0))  errCount++;
		if (imag0!=*(arrPtr3+1))  errCount++;
		if (real1!=*(arrPtr3+2))  errCount++;
		if (imag1!=*(arrPtr3+3))  errCount++;
		arrPtr3 = arrPtr3+4;
		xil_printf("[ulbf_coeffs_check_ram] IP:%d/%d RAM3 DATA %d: 0x%08lX%08lX %d %d %d %d \n",ipNum, NUM_ULBF_COEFFS-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = ulbfCoeffsAddr[ipNum] + ULBF_COEFFS_RAM4_OFFSET + (i*8);
		unsigned int addr1   = ulbfCoeffsAddr[ipNum] + ULBF_COEFFS_RAM4_OFFSET + ((i*8)+4);
		unsigned int data0 = Xil_In32(addr0);
		unsigned int data1 = Xil_In32(addr1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr4+0))  errCount++;
		if (imag0!=*(arrPtr4+1))  errCount++;
		if (real1!=*(arrPtr4+2))  errCount++;
		if (imag1!=*(arrPtr4+3))  errCount++;
		arrPtr4 = arrPtr4+4;
		xil_printf("[ulbf_coeffs_check_ram] IP:%d/%d RAM4 DATA %d: 0x%08lX%08lX %d %d %d %d \n",ipNum, NUM_ULBF_COEFFS-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = ulbfCoeffsAddr[ipNum] + ULBF_COEFFS_RAM5_OFFSET + (i*8);
		unsigned int addr1   = ulbfCoeffsAddr[ipNum] + ULBF_COEFFS_RAM5_OFFSET + ((i*8)+4);
		unsigned int data0 = Xil_In32(addr0);
		unsigned int data1 = Xil_In32(addr1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr5+0))  errCount++;
		if (imag0!=*(arrPtr5+1))  errCount++;
		if (real1!=*(arrPtr5+2))  errCount++;
		if (imag1!=*(arrPtr5+3))  errCount++;
		arrPtr5 = arrPtr5+4;
		xil_printf("[ulbf_coeffs_check_ram] IP:%d/%d RAM5 DATA %d: 0x%08lX%08lX %d %d %d %d \n",ipNum, NUM_ULBF_COEFFS-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = ulbfCoeffsAddr[ipNum] + ULBF_COEFFS_RAM6_OFFSET + (i*8);
		unsigned int addr1   = ulbfCoeffsAddr[ipNum] + ULBF_COEFFS_RAM6_OFFSET + ((i*8)+4);
		unsigned int data0 = Xil_In32(addr0);
		unsigned int data1 = Xil_In32(addr1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr6+0))  errCount++;
		if (imag0!=*(arrPtr6+1))  errCount++;
		if (real1!=*(arrPtr6+2))  errCount++;
		if (imag1!=*(arrPtr6+3))  errCount++;
		arrPtr6 = arrPtr6+4;
		xil_printf("[ulbf_coeffs_check_ram] IP:%d/%d RAM6 DATA %d: 0x%08lX%08lX %d %d %d %d \n",ipNum, NUM_ULBF_COEFFS-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = ulbfCoeffsAddr[ipNum] + ULBF_COEFFS_RAM7_OFFSET + (i*8);
		unsigned int addr1   = ulbfCoeffsAddr[ipNum] + ULBF_COEFFS_RAM7_OFFSET + ((i*8)+4);
		unsigned int data0 = Xil_In32(addr0);
		unsigned int data1 = Xil_In32(addr1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr7+0))  errCount++;
		if (imag0!=*(arrPtr7+1))  errCount++;
		if (real1!=*(arrPtr7+2))  errCount++;
		if (imag1!=*(arrPtr7+3))  errCount++;
		arrPtr7 = arrPtr7+4;
		xil_printf("[ulbf_coeffs_check_ram] IP:%d/%d RAM7 DATA %d: 0x%08lX%08lX %d %d %d %d \n",ipNum, NUM_ULBF_COEFFS-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	if (errCount!=0)   xil_printf("[ulbf_coeffs_check_ram] Reading from RAM FAILED\n");
	else 	printf("[ulbf_coeffs_check_ram] PASSED \n");
}

void ulbf_start() {
	for (unsigned int i=0; i<NUM_ULBF_DATA; i++ ) {
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM0_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x10);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM1_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x10);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM2_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x10);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM3_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x10);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM4_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x10);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM5_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x10);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM6_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x10);
		Xil_Out32( ulbfDataAddr[i]+ULBF_DATA_RAM7_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x10);
	}
	for (unsigned int i=0; i<NUM_ULBF_COEFFS; i++ ) {
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM0_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x10);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM1_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x10);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM2_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x10);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM3_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x10);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM4_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x10);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM5_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x10);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM6_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x10);
		Xil_Out32( ulbfCoeffsAddr[i]+ULBF_COEFFS_RAM7_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x10);
	}
	xil_printf("[ulbf_start] Done \n");  
}

uint32_t ulbf_data_wait_for_done() {
	uint32_t done;
	unsigned int addr;
	for (unsigned int i=0; i<NUM_ULBF_DATA;i++) {
		addr = ulbfDataAddr[i] + ULBF_DATA_CSR_OFFSET + ULBF_DATA_REG_OFFSET_DONE;
		done = Xil_In32(addr);
		xil_printf("[ulbf_data_wait_for_done] -%d- Data:%x Expected:0xFF\n", i, done);
		if (done!=255)
			break;
	}
	if (done==255) return 1;
	else return 0;
}

uint32_t ulbf_coeffs_wait_for_done() {
	uint32_t done;
	unsigned int addr;
	for (unsigned int i=0; i<NUM_ULBF_COEFFS;i++) {
		addr = ulbfCoeffsAddr[i] + ULBF_COEFFS_CSR_OFFSET + ULBF_COEFFS_REG_OFFSET_DONE;
		done = Xil_In32(addr);
		xil_printf("[ulbf_coeffs_wait_for_done] -%d- Data:%x Expected:0xFF\n", i, done);
		if (done!=255)
			break;
	}
	if (done==255) return 1;
	else return 0;
}

uint32_t ulbf_slave_wait_for_done(unsigned int niter, unsigned int block_size) {
	uint32_t done;
	unsigned int addr;
	for (unsigned int i=0; i<NUM_ULBF_SLAVES;i++) {
		addr = ulbfSlaveAddr[i] + ULBF_SLAVE_CSR_OFFSET + ULBF_SLAVE_REG_OFFSET_RAM_STATUS;
		done = Xil_In32(addr);
		xil_printf("[ulbf_slave_wait_for_done] -%d- Read:%u Expected:%u\n", i, done, niter*block_size);
		if (done!=(niter*block_size))
			break;
	}
	if (done==(niter*block_size)) return 1;
	else	return 0;
}


int ulbf_slave_read_and_verify(int * arrPtr, unsigned int id, unsigned int niter, int output_block_size, int debugPrint) {
	unsigned int addr0, addr1;
	int errCount = 0;
	for(unsigned int i=0; i<niter*output_block_size; i++) {
		addr0 = ulbfSlaveAddr[id] + i*8;
		addr1 = ulbfSlaveAddr[id] +(i*8)+4;
		unsigned int data0 = Xil_In32(addr0); 
		unsigned int data1 = Xil_In32(addr1); 
		int real0 = extractIQ(data0, 0); 
		int imag0 = extractIQ(data0,1 ); 
		int real1 = extractIQ(data1, 0); 
		int imag1 = extractIQ(data1, 1); 
		if (real0!=*(arrPtr+0))  errCount++;
		if (imag0!=*(arrPtr+1))  errCount++;
		if (real1!=*(arrPtr+2))  errCount++;
		if (imag1!=*(arrPtr+3))  errCount++;
		if (debugPrint) xil_printf("ULBF-%d-tdata: %d: %d %d %d %d  Expected: %d %d %d %d \n",id, i, real0, imag0, real1, imag1, *(arrPtr+0), *(arrPtr+1), *(arrPtr+2), *(arrPtr+3) );
		arrPtr = arrPtr+4;
	}
	return errCount;
}
