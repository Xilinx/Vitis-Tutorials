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
#include "utils_dlbf.h"

#define DEBUGTEST

uint64_t dlbfDataAddr[NUM_DLBF_DATA] = {
XPAR_DLBF_DATA_00_BASEADDR,
XPAR_DLBF_DATA_10_BASEADDR,
XPAR_DLBF_DATA_20_BASEADDR,
};

uint64_t dlbfCoeffsAddr[NUM_DLBF_COEFFS] = {
XPAR_DLBF_COEFFS_00_BASEADDR,
XPAR_DLBF_COEFFS_01_BASEADDR,
XPAR_DLBF_COEFFS_02_BASEADDR,
XPAR_DLBF_COEFFS_03_BASEADDR,
XPAR_DLBF_COEFFS_04_BASEADDR,
XPAR_DLBF_COEFFS_05_BASEADDR,
XPAR_DLBF_COEFFS_06_BASEADDR,
XPAR_DLBF_COEFFS_07_BASEADDR,
XPAR_DLBF_COEFFS_10_BASEADDR,
XPAR_DLBF_COEFFS_11_BASEADDR,
XPAR_DLBF_COEFFS_12_BASEADDR,
XPAR_DLBF_COEFFS_13_BASEADDR,
XPAR_DLBF_COEFFS_14_BASEADDR,
XPAR_DLBF_COEFFS_15_BASEADDR,
XPAR_DLBF_COEFFS_16_BASEADDR,
XPAR_DLBF_COEFFS_17_BASEADDR,
XPAR_DLBF_COEFFS_20_BASEADDR,
XPAR_DLBF_COEFFS_21_BASEADDR,
XPAR_DLBF_COEFFS_22_BASEADDR,
XPAR_DLBF_COEFFS_23_BASEADDR,
XPAR_DLBF_COEFFS_24_BASEADDR,
XPAR_DLBF_COEFFS_25_BASEADDR,
XPAR_DLBF_COEFFS_26_BASEADDR,
XPAR_DLBF_COEFFS_27_BASEADDR,
};

uint64_t dlbfSlaveAddr[NUM_DLBF_SLAVES] = {
XPAR_DLBF_O00_BASEADDR,
XPAR_DLBF_O01_BASEADDR,
XPAR_DLBF_O02_BASEADDR,
XPAR_DLBF_O03_BASEADDR,
XPAR_DLBF_O04_BASEADDR,
XPAR_DLBF_O05_BASEADDR,
XPAR_DLBF_O06_BASEADDR,
XPAR_DLBF_O07_BASEADDR,
XPAR_DLBF_O10_BASEADDR,
XPAR_DLBF_O11_BASEADDR,
XPAR_DLBF_O12_BASEADDR,
XPAR_DLBF_O13_BASEADDR,
XPAR_DLBF_O14_BASEADDR,
XPAR_DLBF_O15_BASEADDR,
XPAR_DLBF_O16_BASEADDR,
XPAR_DLBF_O17_BASEADDR,
XPAR_DLBF_O20_BASEADDR,
XPAR_DLBF_O21_BASEADDR,
XPAR_DLBF_O22_BASEADDR,
XPAR_DLBF_O23_BASEADDR,
XPAR_DLBF_O24_BASEADDR,
XPAR_DLBF_O25_BASEADDR,
XPAR_DLBF_O26_BASEADDR,
XPAR_DLBF_O27_BASEADDR,
};

void dlbf_data_config_ip(unsigned int i, unsigned int ipBaseAddr, unsigned int niter ) {
	unsigned int csrAddr;
	csrAddr = ipBaseAddr + DLBF_DATA_CSR_OFFSET  + DLBF_DATA_RAM0_OFFSET; 
	Xil_Out32(csrAddr+DLBF_DATA_REG_OFFSET_BLOCK_SIZE, DLBF_DATA_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+DLBF_DATA_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+DLBF_DATA_REG_OFFSET_ROLLOVER,   DLBF_DATA_INPUT_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + DLBF_DATA_CSR_OFFSET  + DLBF_DATA_RAM1_OFFSET; 
	Xil_Out32(csrAddr+DLBF_DATA_REG_OFFSET_BLOCK_SIZE, DLBF_DATA_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+DLBF_DATA_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+DLBF_DATA_REG_OFFSET_ROLLOVER,   DLBF_DATA_INPUT_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + DLBF_DATA_CSR_OFFSET  + DLBF_DATA_RAM2_OFFSET; 
	Xil_Out32(csrAddr+DLBF_DATA_REG_OFFSET_BLOCK_SIZE, DLBF_DATA_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+DLBF_DATA_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+DLBF_DATA_REG_OFFSET_ROLLOVER,   DLBF_DATA_INPUT_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + DLBF_DATA_CSR_OFFSET  + DLBF_DATA_RAM3_OFFSET; 
	Xil_Out32(csrAddr+DLBF_DATA_REG_OFFSET_BLOCK_SIZE, DLBF_DATA_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+DLBF_DATA_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+DLBF_DATA_REG_OFFSET_ROLLOVER,   DLBF_DATA_INPUT_ROLLOVER_ADDR);

	printf("[dlbf_data_config_ip] IP:%d/%d Done \n", i, NUM_DLBF_DATA-1);
}

void dlbf_data_config_ips(unsigned int niter) {
	for (unsigned int i=0; i<NUM_DLBF_DATA;i++) 
		dlbf_data_config_ip(i, dlbfDataAddr[i], niter );
}

void dlbf_data_get_ip_status(unsigned int i, unsigned int ipBaseAddr) {
	xil_printf("---------------------------\n");
	unsigned int addr;
	addr = ipBaseAddr + DLBF_DATA_RAM0_OFFSET + DLBF_DATA_CSR_OFFSET; 
	xil_printf("[dlbf_data-0/3] IP:%d/%d DLBF_DATA_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_ID)); 
	xil_printf("[dlbf_data-0/3] IP:%d/%d DLBF_DATA_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_RESET)); 
	xil_printf("[dlbf_data-0/3] IP:%d/%d DLBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[dlbf_data-0/3] IP:%d/%d DLBF_DATA_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_NITER)); 
	xil_printf("[dlbf_data-0/3] IP:%d/%d DLBF_DATA_REG_OFFSET_ROLLOVER    : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_ROLLOVER)); 
	xil_printf("[dlbf_data-0/3] IP:%d/%d DLBF_DATA_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_DONE)); 
	xil_printf("[dlbf_data-0/3] IP:%d/%d DLBF_DATA_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_RAM_STATUS));  

	addr = ipBaseAddr + DLBF_DATA_RAM1_OFFSET + DLBF_DATA_CSR_OFFSET; 
	xil_printf("[dlbf_data-1/3] IP:%d/%d DLBF_DATA_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_ID)); 
	xil_printf("[dlbf_data-1/3] IP:%d/%d DLBF_DATA_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_RESET)); 
	xil_printf("[dlbf_data-1/3] IP:%d/%d DLBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[dlbf_data-1/3] IP:%d/%d DLBF_DATA_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_NITER)); 
	xil_printf("[dlbf_data-1/3] IP:%d/%d DLBF_DATA_REG_OFFSET_ROLLOVER    : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_ROLLOVER)); 
	xil_printf("[dlbf_data-1/3] IP:%d/%d DLBF_DATA_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_DONE)); 
	xil_printf("[dlbf_data-1/3] IP:%d/%d DLBF_DATA_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_RAM_STATUS));  

	addr = ipBaseAddr + DLBF_DATA_RAM2_OFFSET + DLBF_DATA_CSR_OFFSET; 
	xil_printf("[dlbf_data-2/3] IP:%d/%d DLBF_DATA_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_ID)); 
	xil_printf("[dlbf_data-2/3] IP:%d/%d DLBF_DATA_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_RESET)); 
	xil_printf("[dlbf_data-2/3] IP:%d/%d DLBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[dlbf_data-2/3] IP:%d/%d DLBF_DATA_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_NITER)); 
	xil_printf("[dlbf_data-2/3] IP:%d/%d DLBF_DATA_REG_OFFSET_ROLLOVER    : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_ROLLOVER)); 
	xil_printf("[dlbf_data-2/3] IP:%d/%d DLBF_DATA_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_DONE)); 
	xil_printf("[dlbf_data-2/3] IP:%d/%d DLBF_DATA_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_RAM_STATUS));  

	addr = ipBaseAddr + DLBF_DATA_RAM3_OFFSET + DLBF_DATA_CSR_OFFSET; 
	xil_printf("[dlbf_data-3/3] IP:%d/%d DLBF_DATA_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_ID)); 
	xil_printf("[dlbf_data-3/3] IP:%d/%d DLBF_DATA_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_RESET)); 
	xil_printf("[dlbf_data-3/3] IP:%d/%d DLBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[dlbf_data-3/3] IP:%d/%d DLBF_DATA_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_NITER)); 
	xil_printf("[dlbf_data-3/3] IP:%d/%d DLBF_DATA_REG_OFFSET_ROLLOVER    : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_ROLLOVER)); 
	xil_printf("[dlbf_data-3/3] IP:%d/%d DLBF_DATA_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_DONE)); 
	xil_printf("[dlbf_data-3/3] IP:%d/%d DLBF_DATA_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_DLBF_DATA-1, Xil_In32(addr+DLBF_DATA_REG_OFFSET_RAM_STATUS));  

}

void dlbf_data_get_all_ip_status() {
	for (unsigned int i=0; i<NUM_DLBF_DATA;i++) 
		dlbf_data_get_ip_status(i, dlbfDataAddr[i] );
}


void dlbf_coeffs_config_ip(unsigned int i, unsigned int ipBaseAddr, unsigned int niter ) {
	unsigned int csrAddr;
	csrAddr = ipBaseAddr + DLBF_COEFFS_CSR_OFFSET  + DLBF_COEFFS_RAM0_OFFSET; 
	Xil_Out32(csrAddr+DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE, DLBF_COEFFS_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+DLBF_COEFFS_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+DLBF_COEFFS_REG_OFFSET_ROLLOVER,   DLBF_COEFFS_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + DLBF_COEFFS_CSR_OFFSET  + DLBF_COEFFS_RAM1_OFFSET; 
	Xil_Out32(csrAddr+DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE, DLBF_COEFFS_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+DLBF_COEFFS_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+DLBF_COEFFS_REG_OFFSET_ROLLOVER,   DLBF_COEFFS_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + DLBF_COEFFS_CSR_OFFSET  + DLBF_COEFFS_RAM2_OFFSET; 
	Xil_Out32(csrAddr+DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE, DLBF_COEFFS_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+DLBF_COEFFS_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+DLBF_COEFFS_REG_OFFSET_ROLLOVER,   DLBF_COEFFS_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + DLBF_COEFFS_CSR_OFFSET  + DLBF_COEFFS_RAM3_OFFSET; 
	Xil_Out32(csrAddr+DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE, DLBF_COEFFS_INPUT_BLOCK_SIZE);
	Xil_Out32(csrAddr+DLBF_COEFFS_REG_OFFSET_NITER,      niter);
	Xil_Out32(csrAddr+DLBF_COEFFS_REG_OFFSET_ROLLOVER,   DLBF_COEFFS_ROLLOVER_ADDR);

	printf("[dlbf_coeffs_config_ip] IP:%d/%d Done \n", i, NUM_DLBF_COEFFS-1);
}

void dlbf_coeffs_config_ips(unsigned int niter) {
	for (unsigned int i=0; i<NUM_DLBF_COEFFS;i++) 
		dlbf_coeffs_config_ip(i, dlbfCoeffsAddr[i], niter );
}

void dlbf_coeffs_get_ip_status(unsigned int i, unsigned int ipBaseAddr) {
	xil_printf("---------------------------\n");
	unsigned int addr;
	addr = ipBaseAddr + DLBF_COEFFS_RAM0_OFFSET + DLBF_COEFFS_CSR_OFFSET; 
	xil_printf("[dlbf_coeffs-0/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_ID)); 
	xil_printf("[dlbf_coeffs-0/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_RESET)); 
	xil_printf("[dlbf_coeffs-0/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[dlbf_coeffs-0/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_NITER)); 
	xil_printf("[dlbf_coeffs-0/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_ROLLOVER	  : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_ROLLOVER)); 
	xil_printf("[dlbf_coeffs-0/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_DONE)); 
	xil_printf("[dlbf_coeffs-0/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_RAM_STATUS));  

	addr = ipBaseAddr + DLBF_COEFFS_RAM1_OFFSET + DLBF_COEFFS_CSR_OFFSET; 
	xil_printf("[dlbf_coeffs-1/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_ID)); 
	xil_printf("[dlbf_coeffs-1/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_RESET)); 
	xil_printf("[dlbf_coeffs-1/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[dlbf_coeffs-1/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_NITER)); 
	xil_printf("[dlbf_coeffs-1/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_ROLLOVER	  : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_ROLLOVER)); 
	xil_printf("[dlbf_coeffs-1/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_DONE)); 
	xil_printf("[dlbf_coeffs-1/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_RAM_STATUS));  

	addr = ipBaseAddr + DLBF_COEFFS_RAM2_OFFSET + DLBF_COEFFS_CSR_OFFSET; 
	xil_printf("[dlbf_coeffs-2/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_ID)); 
	xil_printf("[dlbf_coeffs-2/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_RESET)); 
	xil_printf("[dlbf_coeffs-2/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[dlbf_coeffs-2/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_NITER)); 
	xil_printf("[dlbf_coeffs-2/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_ROLLOVER	  : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_ROLLOVER)); 
	xil_printf("[dlbf_coeffs-2/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_DONE)); 
	xil_printf("[dlbf_coeffs-2/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_RAM_STATUS));  
		
	addr = ipBaseAddr + DLBF_COEFFS_RAM3_OFFSET + DLBF_COEFFS_CSR_OFFSET; 
	xil_printf("[dlbf_coeffs-3/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_ID)); 
	xil_printf("[dlbf_coeffs-3/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_RESET)); 
	xil_printf("[dlbf_coeffs-3/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE)); 
	xil_printf("[dlbf_coeffs-3/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_NITER)); 
	xil_printf("[dlbf_coeffs-3/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_ROLLOVER	  : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_ROLLOVER)); 
	xil_printf("[dlbf_coeffs-3/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_DONE)); 
	xil_printf("[dlbf_coeffs-3/3] IP:%d/%d DLBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_DLBF_COEFFS-1, Xil_In32(addr+DLBF_COEFFS_REG_OFFSET_RAM_STATUS));  

}

void dlbf_coeffs_get_all_ip_status() {
	for (unsigned int i=0; i<NUM_DLBF_COEFFS;i++) 
		dlbf_coeffs_get_ip_status(i, dlbfCoeffsAddr[i] );
}

void dlbf_slave_config_ip(unsigned int i, unsigned int ipBaseAddr, unsigned int niter ) {
	unsigned int csrAddr = ipBaseAddr + DLBF_SLAVE_CSR_OFFSET ; 
	Xil_Out32(csrAddr+DLBF_SLAVE_REG_OFFSET_NITER,      niter);
	printf("[dlbf_slave_config_ip] IP:%d/%d Done \n", i, NUM_DLBF_SLAVES-1);
}

void dlbf_slave_config_ips(unsigned int niter) {
	for (unsigned int i=0; i<NUM_DLBF_SLAVES;i++) 
		dlbf_slave_config_ip(i, dlbfSlaveAddr[i], niter );
}

void dlbf_slave_get_ip_status(unsigned int i, unsigned int ipBaseAddr) {
	xil_printf("---------------------------\n");
	unsigned int addr;
	addr = ipBaseAddr + DLBF_SLAVE_CSR_OFFSET; 
	xil_printf("[dlbf_slave] IP:%d/%d DLBF_SLAVE_REG_OFFSET_ID          : 0x%08lX\n", i, NUM_DLBF_SLAVES-1, Xil_In32(addr+DLBF_SLAVE_REG_OFFSET_ID)); 
	xil_printf("[dlbf_slave] IP:%d/%d DLBF_SLAVE_REG_OFFSET_RESET       : 0x%08lX\n", i, NUM_DLBF_SLAVES-1, Xil_In32(addr+DLBF_SLAVE_REG_OFFSET_RESET)); 
	xil_printf("[dlbf_slave] IP:%d/%d DLBF_SLAVE_REG_OFFSET_NITER       : 0x%08lX\n", i, NUM_DLBF_SLAVES-1, Xil_In32(addr+DLBF_SLAVE_REG_OFFSET_NITER)); 
	xil_printf("[dlbf_slave] IP:%d/%d DLBF_SLAVE_REG_OFFSET_DONE        : 0x%08lX\n", i, NUM_DLBF_SLAVES-1, Xil_In32(addr+DLBF_SLAVE_REG_OFFSET_DONE)); 
	xil_printf("[dlbf_slave] IP:%d/%d DLBF_SLAVE_REG_OFFSET_RAM_STATUS  : 0x%08lX\n", i, NUM_DLBF_SLAVES-1, Xil_In32(addr+DLBF_SLAVE_REG_OFFSET_RAM_STATUS));  
}

void dlbf_slave_get_all_ip_status() {
	for (unsigned int i=0; i<NUM_DLBF_SLAVES;i++) 
		dlbf_slave_get_ip_status(i, dlbfSlaveAddr[i] );
}


void dlbf_data_reset() {
	unsigned long sleepTime = 2; 
	for (unsigned int i=0; i<NUM_DLBF_DATA; i++ ) {
		Xil_Out32( dlbfDataAddr[i]+DLBF_DATA_RAM0_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_RESET, 1);
		Xil_Out32( dlbfDataAddr[i]+DLBF_DATA_RAM1_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_RESET, 1);
		Xil_Out32( dlbfDataAddr[i]+DLBF_DATA_RAM2_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_RESET, 1);
		Xil_Out32( dlbfDataAddr[i]+DLBF_DATA_RAM3_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_RESET, 1);
	}
	usleep(sleepTime); 
	for (unsigned int i=0; i<NUM_DLBF_DATA; i++ ) {
		Xil_Out32( dlbfDataAddr[i]+DLBF_DATA_RAM0_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_RESET, 0);
		Xil_Out32( dlbfDataAddr[i]+DLBF_DATA_RAM1_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_RESET, 0);
		Xil_Out32( dlbfDataAddr[i]+DLBF_DATA_RAM2_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_RESET, 0);
		Xil_Out32( dlbfDataAddr[i]+DLBF_DATA_RAM3_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_RESET, 0);
	}
	xil_printf("[dlbf_data_reset] Done \n");  
}

void dlbf_coeffs_reset() {
	unsigned long sleepTime = 2; 
	for (unsigned int i=0; i<NUM_DLBF_COEFFS; i++ ) {
		Xil_Out32( dlbfCoeffsAddr[i]+DLBF_COEFFS_RAM0_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_RESET, 1);
		Xil_Out32( dlbfCoeffsAddr[i]+DLBF_COEFFS_RAM1_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_RESET, 1);
		Xil_Out32( dlbfCoeffsAddr[i]+DLBF_COEFFS_RAM2_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_RESET, 1);
		Xil_Out32( dlbfCoeffsAddr[i]+DLBF_COEFFS_RAM3_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_RESET, 1);
	}
	usleep(sleepTime); 
	for (unsigned int i=0; i<NUM_DLBF_COEFFS; i++ ) {
		Xil_Out32( dlbfCoeffsAddr[i]+DLBF_COEFFS_RAM0_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_RESET, 0);
		Xil_Out32( dlbfCoeffsAddr[i]+DLBF_COEFFS_RAM1_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_RESET, 0);
		Xil_Out32( dlbfCoeffsAddr[i]+DLBF_COEFFS_RAM2_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_RESET, 0);
		Xil_Out32( dlbfCoeffsAddr[i]+DLBF_COEFFS_RAM3_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_RESET, 0);
	}
	xil_printf("[dlbf_coeffs_reset] Done \n");  
}

void dlbf_slave_reset() {
	unsigned long sleepTime = 2; 
	for (unsigned int i=0; i<NUM_DLBF_SLAVES; i++ ) {
		Xil_Out32( dlbfSlaveAddr[i]+DLBF_SLAVE_CSR_OFFSET+DLBF_SLAVE_REG_OFFSET_RESET, 1);
	}
	usleep(sleepTime); 
	for (unsigned int i=0; i<NUM_DLBF_SLAVES; i++ ) {
		Xil_Out32( dlbfSlaveAddr[i]+DLBF_SLAVE_CSR_OFFSET+DLBF_SLAVE_REG_OFFSET_RESET, 0);
	}
	xil_printf("[dlbf_slave_reset] Done \n");  
}

void dlbf_reset() {
	dlbf_data_reset();
	dlbf_coeffs_reset();
	dlbf_slave_reset();
}

void dlbf_data_check_ram(int ipNum, int * arrPtr0, int * arrPtr1, int * arrPtr2, int * arrPtr3) {
    printf("[dlbf_data_check_ram] Reading initialized data from dlbf_data IP RAMs\n");
	int errCount = 0;
    for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = dlbfDataAddr[ipNum] + DLBF_DATA_RAM0_OFFSET + (i*8);
		unsigned int addr1   = dlbfDataAddr[ipNum] + DLBF_DATA_RAM0_OFFSET + ((i*8)+4);
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
		xil_printf("[dlbf_data_check_ram] IP:%d/%d RAM0 DATA %d: 0x%08lX%08lX %d %d %d %d \n",ipNum, NUM_DLBF_DATA-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = dlbfDataAddr[ipNum] + DLBF_DATA_RAM1_OFFSET + (i*8);
		unsigned int addr1   = dlbfDataAddr[ipNum] + DLBF_DATA_RAM1_OFFSET + ((i*8)+4);
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
		xil_printf("[dlbf_data_check_ram] IP:%d/%d RAM1 DATA %d: 0x%08lX%08lX %d %d %d %d \nn",ipNum, NUM_DLBF_DATA-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = dlbfDataAddr[ipNum] + DLBF_DATA_RAM2_OFFSET + (i*8);
		unsigned int addr1   = dlbfDataAddr[ipNum] + DLBF_DATA_RAM2_OFFSET + ((i*8)+4);
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
		xil_printf("[dlbf_data_check_ram] IP:%d/%d RAM2 DATA %d: 0x%08lX%08lX %d %d %d %d \nn",ipNum, NUM_DLBF_DATA-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = dlbfDataAddr[ipNum] + DLBF_DATA_RAM3_OFFSET + (i*8);
		unsigned int addr1   = dlbfDataAddr[ipNum] + DLBF_DATA_RAM3_OFFSET + ((i*8)+4);
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
		xil_printf("[dlbf_data_check_ram] IP:%d/%d RAM3 DATA %d: 0x%08lX%08lX %d %d %d %d \nn",ipNum, NUM_DLBF_DATA-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	if (errCount!=0)   xil_printf("[dlbf_data_check_ram] Reading from RAM FAILED\n");
	else 	printf("[dlbf_data_check_ram] PASSED \n");
}

void dlbf_coeffs_check_ram(int ipNum, int * arrPtr0, int * arrPtr1, int * arrPtr2, int * arrPtr3) {
	printf("[dlbf_coeffs_check_ram] Reading initialized data from dlbf_coeffs IP RAMs\n");
	int errCount = 0;
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = dlbfCoeffsAddr[ipNum] + DLBF_COEFFS_RAM0_OFFSET + (i*8);
		unsigned int addr1   = dlbfCoeffsAddr[ipNum] + DLBF_COEFFS_RAM0_OFFSET + ((i*8)+4);
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
		xil_printf("[dlbf_coeffs_check_ram] IP:%d/%d RAM0 DATA %d: 0x%08lX%08lX %d %d %d %d \n",ipNum, NUM_DLBF_COEFFS-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = dlbfCoeffsAddr[ipNum] + DLBF_COEFFS_RAM1_OFFSET + (i*8);
		unsigned int addr1   = dlbfCoeffsAddr[ipNum] + DLBF_COEFFS_RAM1_OFFSET + ((i*8)+4);
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
		xil_printf("[dlbf_coeffs_check_ram] IP:%d/%d RAM1 DATA %d: 0x%08lX%08lX %d %d %d %d \n",ipNum, NUM_DLBF_COEFFS-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = dlbfCoeffsAddr[ipNum] + DLBF_COEFFS_RAM2_OFFSET + (i*8);
		unsigned int addr1   = dlbfCoeffsAddr[ipNum] + DLBF_COEFFS_RAM2_OFFSET + ((i*8)+4);
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
		xil_printf("[dlbf_coeffs_check_ram] IP:%d/%d RAM2 DATA %d: 0x%08lX%08lX %d %d %d %d \n",ipNum, NUM_DLBF_COEFFS-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	for (unsigned int i=0; i<5; i++ ) {
		unsigned int addr0   = dlbfCoeffsAddr[ipNum] + DLBF_COEFFS_RAM3_OFFSET + (i*8);
		unsigned int addr1   = dlbfCoeffsAddr[ipNum] + DLBF_COEFFS_RAM3_OFFSET + ((i*8)+4);
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
		xil_printf("[dlbf_coeffs_check_ram] IP:%d/%d RAM3 DATA %d: 0x%08lX%08lX %d %d %d %d \n",ipNum, NUM_DLBF_COEFFS-1, i, data1, data0, real0, imag0, real1, imag1);
	}
	if (errCount!=0)   xil_printf("[dlbf_coeffs_check_ram] Reading from RAM FAILED\n");
	else 	printf("[dlbf_coeffs_check_ram] PASSED \n");
}

void dlbf_start() {
	for (unsigned int i=0; i<NUM_DLBF_DATA; i++ ) {
		Xil_Out32( dlbfDataAddr[i]+DLBF_DATA_RAM0_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_START, 0x10);
		Xil_Out32( dlbfDataAddr[i]+DLBF_DATA_RAM1_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_START, 0x10);
		Xil_Out32( dlbfDataAddr[i]+DLBF_DATA_RAM2_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_START, 0x10);
		Xil_Out32( dlbfDataAddr[i]+DLBF_DATA_RAM3_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_START, 0x10);
	}
	for (unsigned int i=0; i<NUM_DLBF_COEFFS; i++ ) {
		Xil_Out32( dlbfCoeffsAddr[i]+DLBF_COEFFS_RAM0_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_START, 0x10);
		Xil_Out32( dlbfCoeffsAddr[i]+DLBF_COEFFS_RAM1_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_START, 0x10);
		Xil_Out32( dlbfCoeffsAddr[i]+DLBF_COEFFS_RAM2_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_START, 0x10);
		Xil_Out32( dlbfCoeffsAddr[i]+DLBF_COEFFS_RAM3_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_START, 0x10);
	}
	xil_printf("[dlbf_start] Done \n");  
}

uint32_t dlbf_data_wait_for_done() {
	uint32_t done;
	unsigned int addr;
	for (unsigned int i=0; i<NUM_DLBF_DATA;i++) {
		addr = dlbfDataAddr[i] + DLBF_DATA_CSR_OFFSET + DLBF_DATA_REG_OFFSET_DONE;
		done = Xil_In32(addr);
		xil_printf("[dlbf_data_wait_for_done] -%d- Data:%x Expected:0xF\n", i, done);
		if (done!=15)
			break;
	}
	if (done==15) return 1;
	else return 0;
}

uint32_t dlbf_coeffs_wait_for_done() {
	uint32_t done;
	unsigned int addr;
	for (unsigned int i=0; i<NUM_DLBF_COEFFS;i++) {
		addr = dlbfCoeffsAddr[i] + DLBF_COEFFS_CSR_OFFSET + DLBF_COEFFS_REG_OFFSET_DONE;
		done = Xil_In32(addr);
		xil_printf("[dlbf_coeffs_wait_for_done] -%d- Data0:%x Expected:0xF\n", i, done);
		if (done!=15)
			break;
	}
	if (done==15) return 1;
	else return 0;
}

uint32_t dlbf_slave_wait_for_done(unsigned int niter, unsigned int block_size) {
	uint32_t done;
	unsigned int addr;
	for (unsigned int i=0; i<NUM_DLBF_SLAVES;i++) {
		addr = dlbfSlaveAddr[i] + DLBF_SLAVE_CSR_OFFSET + DLBF_SLAVE_REG_OFFSET_RAM_STATUS;
		done = Xil_In32(addr);
		xil_printf("[dlbf_slave_wait_for_done] -%d- Read:%u Expected:%u\n", i, done, niter*block_size);
		if (done!=(niter*block_size))
			break;
	}
	if (done==(niter*block_size)) return 1;
	else	return 0;
}


int dlbf_slave_read_and_verify(int * arrPtr, unsigned int id, unsigned int niter, int output_block_size, int debugPrint) {
	unsigned int addr0, addr1;
	int errCount = 0;
	for(unsigned int i=0; i<niter*output_block_size; i++) {
		addr0 = dlbfSlaveAddr[id] + i*8;
		addr1 = dlbfSlaveAddr[id] +(i*8)+4;
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
		if (debugPrint) xil_printf("DLBF-%d-tdata: %d: %d %d %d %d  Expected: %d %d %d %d \n",id, i, real0, imag0, real1, imag1, *(arrPtr+0), *(arrPtr+1), *(arrPtr+2), *(arrPtr+3) );
		arrPtr = arrPtr+4;
	}
	return errCount;
}
