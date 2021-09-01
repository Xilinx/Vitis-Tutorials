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
#include "utils_linux.h"
#include "utils_ulbf.h"

#define DEBUGTEST

u32 * ulbf_data_mem_ptr[NUM_ULBF_DATA];
u32 * ulbf_coeffs_mem_ptr[NUM_ULBF_COEFFS];
u32 * ulbf_slave_mem_ptr[NUM_ULBF_SLAVES];

int ulbf_assign_addr() {
    int uio_fd[NUM_ULBF_DATA+NUM_ULBF_COEFFS+NUM_ULBF_SLAVES];
    uint32_t mapAddr[NUM_ULBF_DATA+NUM_ULBF_COEFFS+NUM_ULBF_SLAVES] = {};
    uint32_t mapSize[NUM_ULBF_DATA+NUM_ULBF_COEFFS+NUM_ULBF_SLAVES] = {};
    char uioName[NUM_ULBF_DATA+NUM_ULBF_COEFFS+NUM_ULBF_SLAVES][256];
    int uio_num[NUM_ULBF_DATA+NUM_ULBF_COEFFS+NUM_ULBF_SLAVES];
    int ret;
    char addrFile[256];
    char sizeFile[256];
    char nameFile[256];
    char uioList[256];
    FILE *fp;

    log_plnx << "[ulbf_assign_addr] Begin Loop To Processing UIO..." << std::endl;
    int index = 0;
    for(unsigned int i=0; i < (NUM_STREAMS); i++){
        sprintf(nameFile, "/sys/class/uio/uio%d/name", i);
        fp = fopen(nameFile, "r");
        if(!fp) return -1;
        ret = fscanf(fp, "%s", uioName[index]);
        fclose(fp);
        if(ret < 0) return -2;
        if (strstr(uioName[index],"ulbf_coeffs")!= NULL){
	    uio_num[index] = i;
	    index++;
        }
    }
    for(unsigned int i=0; i < (NUM_STREAMS); i++){
        sprintf(nameFile, "/sys/class/uio/uio%d/name", i);
        fp = fopen(nameFile, "r");
        if(!fp) return -1;
        ret = fscanf(fp, "%s", uioName[index]);
        fclose(fp);
        if(ret < 0) return -2;
        if (strstr(uioName[index],"ulbf_data")!= NULL){
	    uio_num[index] = i;
	    index++;
        }
    }
    for(unsigned int i=0; i < (NUM_STREAMS); i++){
        sprintf(nameFile, "/sys/class/uio/uio%d/name", i);
        fp = fopen(nameFile, "r");
        if(!fp) return -1;
        ret = fscanf(fp, "%s", uioName[index]);
        fclose(fp);
        if(ret < 0) return -2;
        if (strstr(uioName[index],"ulbf_o")!= NULL){
	    uio_num[index] = i;
	    index++;
        }
    }

    for(unsigned int i = 0; i < (NUM_ULBF_DATA+NUM_ULBF_COEFFS+NUM_ULBF_SLAVES); i++) {
	sprintf(uioList, "/dev/uio%d", uio_num[i]);
	uio_fd[i] = open(uioList, O_RDWR);
	if(uio_fd[i] < 1) {
            log_plnx << "[ERROR] Invalid UIO device: " << uioList << std::endl;
            return -1;
        }
        sprintf(addrFile, "/sys/class/uio/uio%d/maps/map0/addr", uio_num[i]);
        fp = fopen(addrFile, "r");
        if(!fp) return -1;
        ret = fscanf(fp, "0x%x", &mapAddr[i]);
        fclose(fp);
        if(ret < 0) return -2;
        sprintf(sizeFile, "/sys/class/uio/uio%d/maps/map0/size", uio_num[i]);
        fp = fopen(sizeFile, "r");
        if(!fp) return -1;
        ret = fscanf(fp, "0x%x", &mapSize[i]);
        fclose(fp);
        if(ret < 0) return -2;
    }

    log_plnx << "[ulbf_assign_addr]  Processed all Masters/Slaves" << std::endl;
    for(unsigned int i = 0; i < (NUM_ULBF_COEFFS); i++){
        ulbf_coeffs_mem_ptr[i]  = (u32  *)mmap((void*)mapAddr[i], mapSize[i], PROT_READ|PROT_WRITE, MAP_SHARED, uio_fd[i], 0);
    }
    for(unsigned int i = 0; i < (NUM_ULBF_DATA); i++){
        ulbf_data_mem_ptr[i]  = (u32  *)mmap((void*)mapAddr[NUM_ULBF_COEFFS+i], mapSize[NUM_ULBF_COEFFS+i], PROT_READ|PROT_WRITE, MAP_SHARED, uio_fd[NUM_ULBF_COEFFS+i], 0);
    }
    for(unsigned int i = 0; i < (NUM_ULBF_SLAVES); i++){
        ulbf_slave_mem_ptr[i] = (u32 *)mmap((void*)mapAddr[NUM_ULBF_DATA+NUM_ULBF_COEFFS+i], mapSize[NUM_ULBF_DATA+NUM_ULBF_COEFFS+i], PROT_READ|PROT_WRITE, MAP_SHARED, uio_fd[NUM_ULBF_DATA+NUM_ULBF_COEFFS+i], 0);
    }
    log_plnx << "[ulbf_assign_addr] Done" << std::endl;
    return 0;

}

void ulbf_data_config_ip(unsigned int i, u32 * ipBaseAddr, unsigned int niter, int debugPrint) {
	u32 * csrAddr;
	csrAddr = ipBaseAddr + ULBF_DATA_CSR_OFFSET  + ULBF_DATA_RAM0_OFFSET; 
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_BLOCK_SIZE, ULBF_DATA_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_ROLLOVER,   ULBF_DATA_INPUT_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_DATA_CSR_OFFSET  + ULBF_DATA_RAM1_OFFSET; 
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_BLOCK_SIZE, ULBF_DATA_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_ROLLOVER,   ULBF_DATA_INPUT_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_DATA_CSR_OFFSET  + ULBF_DATA_RAM2_OFFSET; 
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_BLOCK_SIZE, ULBF_DATA_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_ROLLOVER,   ULBF_DATA_INPUT_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_DATA_CSR_OFFSET  + ULBF_DATA_RAM3_OFFSET; 
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_BLOCK_SIZE, ULBF_DATA_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_ROLLOVER,   ULBF_DATA_INPUT_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_DATA_CSR_OFFSET  + ULBF_DATA_RAM4_OFFSET; 
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_BLOCK_SIZE, ULBF_DATA_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_ROLLOVER,   ULBF_DATA_INPUT_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_DATA_CSR_OFFSET  + ULBF_DATA_RAM5_OFFSET; 
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_BLOCK_SIZE, ULBF_DATA_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_ROLLOVER,   ULBF_DATA_INPUT_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_DATA_CSR_OFFSET  + ULBF_DATA_RAM6_OFFSET; 
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_BLOCK_SIZE, ULBF_DATA_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_ROLLOVER,   ULBF_DATA_INPUT_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_DATA_CSR_OFFSET  + ULBF_DATA_RAM7_OFFSET; 
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_BLOCK_SIZE, ULBF_DATA_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, ULBF_DATA_REG_OFFSET_ROLLOVER,   ULBF_DATA_INPUT_ROLLOVER_ADDR);

	if(debugPrint) log_plnx<<"[ulbf_data_config_ip] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" Done "<<std::endl;
}

void ulbf_data_config_ips(unsigned int niter, int debugPrint) {
	for (unsigned int i=0; i<NUM_ULBF_DATA;i++) 
		ulbf_data_config_ip(i, ulbf_data_mem_ptr[i], niter, debugPrint);
}

void ulbf_data_get_ip_status(unsigned int i, u32 * ipBaseAddr) {
	log_plnx<<"---------------------------"<<std::endl;
	u32 * addr;
	addr = ipBaseAddr + ULBF_DATA_RAM0_OFFSET + ULBF_DATA_CSR_OFFSET; 
	log_plnx<<"[ulbf_data-0/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_ID          : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[ulbf_data-0/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_RESET       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[ulbf_data-0/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[ulbf_data-0/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_NITER       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[ulbf_data-0/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_ROLLOVER    : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[ulbf_data-0/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_DONE        : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[ulbf_data-0/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_RAM_STATUS)<<std::endl;  

	addr = ipBaseAddr + ULBF_DATA_RAM1_OFFSET + ULBF_DATA_CSR_OFFSET; 
	log_plnx<<"[ulbf_data-1/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_ID          : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[ulbf_data-1/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_RESET       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[ulbf_data-1/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[ulbf_data-1/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_NITER       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[ulbf_data-1/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_ROLLOVER    : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[ulbf_data-1/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_DONE        : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[ulbf_data-1/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_RAM_STATUS)<<std::endl;  

	addr = ipBaseAddr + ULBF_DATA_RAM2_OFFSET + ULBF_DATA_CSR_OFFSET; 
	log_plnx<<"[ulbf_data-2/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_ID          : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[ulbf_data-2/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_RESET       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[ulbf_data-2/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[ulbf_data-2/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_NITER       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[ulbf_data-2/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_ROLLOVER    : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[ulbf_data-2/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_DONE        : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[ulbf_data-2/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_RAM_STATUS)<<std::endl;  

	addr = ipBaseAddr + ULBF_DATA_RAM3_OFFSET + ULBF_DATA_CSR_OFFSET; 
	log_plnx<<"[ulbf_data-3/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_ID          : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[ulbf_data-3/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_RESET       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[ulbf_data-3/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[ulbf_data-3/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_NITER       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[ulbf_data-3/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_ROLLOVER    : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[ulbf_data-3/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_DONE        : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[ulbf_data-3/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_RAM_STATUS)<<std::endl;  

	addr = ipBaseAddr + ULBF_DATA_RAM4_OFFSET + ULBF_DATA_CSR_OFFSET; 
	log_plnx<<"[ulbf_data-4/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_ID          : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[ulbf_data-4/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_RESET       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[ulbf_data-4/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[ulbf_data-4/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_NITER       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[ulbf_data-4/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_ROLLOVER    : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[ulbf_data-4/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_DONE        : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[ulbf_data-4/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_RAM_STATUS)<<std::endl;  

	addr = ipBaseAddr + ULBF_DATA_RAM5_OFFSET + ULBF_DATA_CSR_OFFSET; 
	log_plnx<<"[ulbf_data-5/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_ID          : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[ulbf_data-5/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_RESET       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[ulbf_data-5/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[ulbf_data-5/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_NITER       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[ulbf_data-5/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_ROLLOVER    : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[ulbf_data-5/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_DONE        : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[ulbf_data-5/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_RAM_STATUS)<<std::endl;  

	addr = ipBaseAddr + ULBF_DATA_RAM6_OFFSET + ULBF_DATA_CSR_OFFSET; 
	log_plnx<<"[ulbf_data-6/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_ID          : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[ulbf_data-6/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_RESET       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[ulbf_data-6/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[ulbf_data-6/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_NITER       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[ulbf_data-6/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_ROLLOVER    : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[ulbf_data-6/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_DONE        : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[ulbf_data-6/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_RAM_STATUS)<<std::endl;  

	addr = ipBaseAddr + ULBF_DATA_RAM7_OFFSET + ULBF_DATA_CSR_OFFSET; 
	log_plnx<<"[ulbf_data-7/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_ID          : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[ulbf_data-7/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_RESET       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[ulbf_data-7/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[ulbf_data-7/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_NITER       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[ulbf_data-7/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_ROLLOVER    : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[ulbf_data-7/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_DONE        : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[ulbf_data-7/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_DATA-1<<" ULBF_DATA_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_DATA_REG_OFFSET_RAM_STATUS)<<std::endl;  

}

void ulbf_data_get_all_ip_status() {
	for (unsigned int i=0; i<NUM_ULBF_DATA;i++) 
		ulbf_data_get_ip_status(i, ulbf_data_mem_ptr[i] );
}


void ulbf_coeffs_config_ip(unsigned int i, u32 * ipBaseAddr, unsigned int niter, int debugPrint) {
	u32 * csrAddr;
	csrAddr = ipBaseAddr + ULBF_COEFFS_CSR_OFFSET  + ULBF_COEFFS_RAM0_OFFSET; 
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE, ULBF_COEFFS_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_ROLLOVER,   ULBF_COEFFS_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_COEFFS_CSR_OFFSET  + ULBF_COEFFS_RAM1_OFFSET; 
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE, ULBF_COEFFS_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_ROLLOVER,   ULBF_COEFFS_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_COEFFS_CSR_OFFSET  + ULBF_COEFFS_RAM2_OFFSET; 
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE, ULBF_COEFFS_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_ROLLOVER,   ULBF_COEFFS_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_COEFFS_CSR_OFFSET  + ULBF_COEFFS_RAM3_OFFSET; 
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE, ULBF_COEFFS_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_ROLLOVER,   ULBF_COEFFS_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_COEFFS_CSR_OFFSET  + ULBF_COEFFS_RAM4_OFFSET; 
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE, ULBF_COEFFS_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_ROLLOVER,   ULBF_COEFFS_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_COEFFS_CSR_OFFSET  + ULBF_COEFFS_RAM5_OFFSET; 
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE, ULBF_COEFFS_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_ROLLOVER,   ULBF_COEFFS_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_COEFFS_CSR_OFFSET  + ULBF_COEFFS_RAM6_OFFSET; 
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE, ULBF_COEFFS_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_ROLLOVER,   ULBF_COEFFS_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + ULBF_COEFFS_CSR_OFFSET  + ULBF_COEFFS_RAM7_OFFSET; 
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE, ULBF_COEFFS_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, ULBF_COEFFS_REG_OFFSET_ROLLOVER,   ULBF_COEFFS_ROLLOVER_ADDR);

	if(debugPrint) log_plnx<<"[ulbf_coeffs_config_ip] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" Done "<<std::endl;
}

void ulbf_coeffs_config_ips(unsigned int niter, int debugPrint) {
	for (unsigned int i=0; i<NUM_ULBF_COEFFS;i++) 
		ulbf_coeffs_config_ip(i, ulbf_coeffs_mem_ptr[i], niter, debugPrint);
}

void ulbf_coeffs_get_ip_status(unsigned int i, u32 * ipBaseAddr) {
	log_plnx<<"---------------------------"<<std::endl;
	u32 * addr;
	addr = ipBaseAddr + ULBF_COEFFS_RAM0_OFFSET + ULBF_COEFFS_CSR_OFFSET; 
	log_plnx<<"[ulbf_coeffs-0/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_ID          : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-0/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_RESET       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-0/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-0/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_NITER       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-0/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_ROLLOVER	  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-0/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_DONE        : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-0/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_RAM_STATUS)<<std::endl;  

	addr = ipBaseAddr + ULBF_COEFFS_RAM1_OFFSET + ULBF_COEFFS_CSR_OFFSET; 
	log_plnx<<"[ulbf_coeffs-1/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_ID          : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-1/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_RESET       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-1/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-1/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_NITER       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-1/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_ROLLOVER	  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-1/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_DONE        : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-1/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_RAM_STATUS)<<std::endl;  

	addr = ipBaseAddr + ULBF_COEFFS_RAM2_OFFSET + ULBF_COEFFS_CSR_OFFSET; 
	log_plnx<<"[ulbf_coeffs-2/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_ID          : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-2/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_RESET       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-2/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-2/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_NITER       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-2/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_ROLLOVER	  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-2/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_DONE        : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-2/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_RAM_STATUS)<<std::endl;  

	addr = ipBaseAddr + ULBF_COEFFS_RAM3_OFFSET + ULBF_COEFFS_CSR_OFFSET; 
	log_plnx<<"[ulbf_coeffs-3/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_ID          : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-3/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_RESET       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-3/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-3/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_NITER       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-3/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_ROLLOVER	  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-3/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_DONE        : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-3/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_RAM_STATUS)<<std::endl;  

	addr = ipBaseAddr + ULBF_COEFFS_RAM4_OFFSET + ULBF_COEFFS_CSR_OFFSET; 
	log_plnx<<"[ulbf_coeffs-4/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_ID          : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-4/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_RESET       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-4/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-4/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_NITER       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-4/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_ROLLOVER	  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-4/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_DONE        : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-4/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_RAM_STATUS)<<std::endl;  

	addr = ipBaseAddr + ULBF_COEFFS_RAM5_OFFSET + ULBF_COEFFS_CSR_OFFSET; 
	log_plnx<<"[ulbf_coeffs-5/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_ID          : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-5/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_RESET       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-5/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-5/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_NITER       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-5/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_ROLLOVER	: 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-5/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_DONE        : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-5/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_RAM_STATUS)<<std::endl;  

	addr = ipBaseAddr + ULBF_COEFFS_RAM6_OFFSET + ULBF_COEFFS_CSR_OFFSET; 
	log_plnx<<"[ulbf_coeffs-6/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_ID          : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-6/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_RESET       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-6/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-6/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_NITER       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-6/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_ROLLOVER	: 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-6/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_DONE        : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-6/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_RAM_STATUS)<<std::endl;  
		
	addr = ipBaseAddr + ULBF_COEFFS_RAM7_OFFSET + ULBF_COEFFS_CSR_OFFSET; 
	log_plnx<<"[ulbf_coeffs-7/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_ID          : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-7/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_RESET       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-7/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-7/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_NITER       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-7/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_ROLLOVER	: 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-7/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_DONE        : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[ulbf_coeffs-7/7] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_COEFFS-1<<" ULBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_COEFFS_REG_OFFSET_RAM_STATUS)<<std::endl;  

}

void ulbf_coeffs_get_all_ip_status() {
	for (unsigned int i=0; i<NUM_ULBF_COEFFS;i++) 
		ulbf_coeffs_get_ip_status(i, ulbf_coeffs_mem_ptr[i] );
}

void ulbf_slave_config_ip(unsigned int i, u32 * ipBaseAddr, unsigned int niter, int debugPrint) {
	u32 * csrAddr = ipBaseAddr + ULBF_SLAVE_CSR_OFFSET ; 
	memptr_WriteReg(csrAddr, ULBF_SLAVE_REG_OFFSET_NITER,      niter);
	if (debugPrint) log_plnx<<"[ulbf_slave_config_ip] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_SLAVES-1<<" Done "<<std::endl;
}

void ulbf_slave_config_ips(unsigned int niter, int debugPrint) {
	for (unsigned int i=0; i<NUM_ULBF_SLAVES;i++) 
		ulbf_slave_config_ip(i, ulbf_slave_mem_ptr[i], niter, debugPrint);
}

void ulbf_slave_get_ip_status(unsigned int i, u32 * ipBaseAddr) {
	log_plnx<<"---------------------------"<<std::endl;
	u32 * addr;
	addr = ipBaseAddr + ULBF_SLAVE_CSR_OFFSET; 
	log_plnx<<"[ulbf_slave] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_SLAVES-1<<" ULBF_SLAVE_REG_OFFSET_ID          : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_SLAVE_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[ulbf_slave] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_SLAVES-1<<" ULBF_SLAVE_REG_OFFSET_RESET       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_SLAVE_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[ulbf_slave] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_SLAVES-1<<" ULBF_SLAVE_REG_OFFSET_NITER       : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_SLAVE_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[ulbf_slave] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_SLAVES-1<<" ULBF_SLAVE_REG_OFFSET_DONE        : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_SLAVE_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[ulbf_slave] IP:"<<std::dec<<i<<"/"<<NUM_ULBF_SLAVES-1<<" ULBF_SLAVE_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<<memptr_ReadReg(addr, ULBF_SLAVE_REG_OFFSET_RAM_STATUS)<<std::endl;  
}

void ulbf_slave_get_all_ip_status() {
	for (unsigned int i=0; i<NUM_ULBF_SLAVES;i++) 
		ulbf_slave_get_ip_status(i, ulbf_slave_mem_ptr[i] );
}


void ulbf_data_reset(int debugPrint) {
	unsigned long sleepTime = 2; 
	for (unsigned int i=0; i<NUM_ULBF_DATA; i++ ) {
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM0_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 1);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM1_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 1);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM2_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 1);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM3_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 1);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM4_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 1);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM5_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 1);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM6_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 1);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM7_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 1);
	}
	usleep(sleepTime); 
	for (unsigned int i=0; i<NUM_ULBF_DATA; i++ ) {
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM0_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 0);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM1_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 0);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM2_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 0);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM3_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 0);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM4_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 0);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM5_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 0);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM6_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 0);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM7_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_RESET, 0);
	}
	if(debugPrint) log_plnx<<"[ulbf_data_reset] Done "<<std::endl;  
}

void ulbf_coeffs_reset(int debugPrint) {
	unsigned long sleepTime = 2; 
	for (unsigned int i=0; i<NUM_ULBF_COEFFS; i++ ) {
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM0_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 1);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM1_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 1);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM2_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 1);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM3_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 1);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM4_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 1);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM5_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 1);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM6_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 1);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM7_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 1);
	}
	usleep(sleepTime); 
	for (unsigned int i=0; i<NUM_ULBF_COEFFS; i++ ) {
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM0_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 0);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM1_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 0);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM2_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 0);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM3_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 0);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM4_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 0);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM5_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 0);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM6_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 0);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM7_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_RESET, 0);
	}
	if(debugPrint) log_plnx<<"[ulbf_coeffs_reset] Done "<<std::endl;  
}

void ulbf_slave_reset(int debugPrint) {
	unsigned long sleepTime = 2; 
	for (unsigned int i=0; i<NUM_ULBF_SLAVES; i++ ) {
		memptr_WriteReg(ulbf_slave_mem_ptr[i], ULBF_SLAVE_CSR_OFFSET+ULBF_SLAVE_REG_OFFSET_RESET, 1);
	}
	usleep(sleepTime); 
	for (unsigned int i=0; i<NUM_ULBF_SLAVES; i++ ) {
		memptr_WriteReg(ulbf_slave_mem_ptr[i], ULBF_SLAVE_CSR_OFFSET+ULBF_SLAVE_REG_OFFSET_RESET, 0);
	}
	if(debugPrint) log_plnx<<"[ulbf_slave_reset] Done "<<std::endl;  
}

void ulbf_reset(int debugPrint) {
	ulbf_data_reset(debugPrint);
	ulbf_coeffs_reset(debugPrint);
	ulbf_slave_reset(debugPrint);
}

void ulbf_data_check_ram(int ipNum, int * arrPtr0, int * arrPtr1, int * arrPtr2, int * arrPtr3, int * arrPtr4, int * arrPtr5, int * arrPtr6, int * arrPtr7) {
	log_plnx<<"[ulbf_data_check_ram] Reading initialized data from ulbf_data IP RAMs"<<std::endl;
	int errCount = 0;
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = ulbf_data_mem_ptr[ipNum] + ULBF_DATA_RAM0_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*8);
		unsigned int data1 = memptr_ReadReg(addr, (i*8)+4);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr0+0))  errCount++;
		if (imag0!=*(arrPtr0+1))  errCount++;
		if (real1!=*(arrPtr0+2))  errCount++;
		if (imag1!=*(arrPtr0+3))  errCount++;
		arrPtr0 = arrPtr0+4;
		log_plnx<<"[ulbf_data_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_ULBF_DATA-1<<" RAM0 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = ulbf_data_mem_ptr[ipNum] + ULBF_DATA_RAM1_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*8);
		unsigned int data1 = memptr_ReadReg(addr, (i*8)+4);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr1+0))  errCount++;
		if (imag0!=*(arrPtr1+1))  errCount++;
		if (real1!=*(arrPtr1+2))  errCount++;
		if (imag1!=*(arrPtr1+3))  errCount++;
		arrPtr1 = arrPtr1+4;
		log_plnx<<"[ulbf_data_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_ULBF_DATA-1<<" RAM1 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = ulbf_data_mem_ptr[ipNum] + ULBF_DATA_RAM2_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*8);
		unsigned int data1 = memptr_ReadReg(addr, (i*8)+4);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr2+0))  errCount++;
		if (imag0!=*(arrPtr2+1))  errCount++;
		if (real1!=*(arrPtr2+2))  errCount++;
		if (imag1!=*(arrPtr2+3))  errCount++;
		arrPtr2 = arrPtr2+4;
		log_plnx<<"[ulbf_data_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_ULBF_DATA-1<<" RAM2 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = ulbf_data_mem_ptr[ipNum] + ULBF_DATA_RAM3_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*8);
		unsigned int data1 = memptr_ReadReg(addr, (i*8)+4);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr3+0))  errCount++;
		if (imag0!=*(arrPtr3+1))  errCount++;
		if (real1!=*(arrPtr3+2))  errCount++;
		if (imag1!=*(arrPtr3+3))  errCount++;
		arrPtr3 = arrPtr3+4;
		log_plnx<<"[ulbf_data_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_ULBF_DATA-1<<" RAM3 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = ulbf_data_mem_ptr[ipNum] + ULBF_DATA_RAM4_OFFSET + (i*8);
		unsigned int data0 = memptr_ReadReg(addr, i*8);
		unsigned int data1 = memptr_ReadReg(addr, (i*8)+4);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr4+0))  errCount++;
		if (imag0!=*(arrPtr4+1))  errCount++;
		if (real1!=*(arrPtr4+2))  errCount++;
		if (imag1!=*(arrPtr4+3))  errCount++;
		arrPtr4 = arrPtr4+4;
		log_plnx<<"[ulbf_data_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_ULBF_DATA-1<<" RAM4 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = ulbf_data_mem_ptr[ipNum] + ULBF_DATA_RAM5_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*8);
		unsigned int data1 = memptr_ReadReg(addr, (i*8)+4);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr5+0))  errCount++;
		if (imag0!=*(arrPtr5+1))  errCount++;
		if (real1!=*(arrPtr5+2))  errCount++;
		if (imag1!=*(arrPtr5+3))  errCount++;
		arrPtr5 = arrPtr5+4;
		log_plnx<<"[ulbf_data_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_ULBF_DATA-1<<" RAM5 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = ulbf_data_mem_ptr[ipNum] + ULBF_DATA_RAM6_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*8);
		unsigned int data1 = memptr_ReadReg(addr, (i*8)+4);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr6+0))  errCount++;
		if (imag0!=*(arrPtr6+1))  errCount++;
		if (real1!=*(arrPtr6+2))  errCount++;
		if (imag1!=*(arrPtr6+3))  errCount++;
		arrPtr6 = arrPtr6+4;
		log_plnx<<"[ulbf_data_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_ULBF_DATA-1<<" RAM6 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = ulbf_data_mem_ptr[ipNum] + ULBF_DATA_RAM7_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*8);
		unsigned int data1 = memptr_ReadReg(addr, (i*8)+4);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr7+0))  errCount++;
		if (imag0!=*(arrPtr7+1))  errCount++;
		if (real1!=*(arrPtr7+2))  errCount++;
		if (imag1!=*(arrPtr7+3))  errCount++;
		arrPtr7 = arrPtr7+4;
		log_plnx<<"[ulbf_data_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_ULBF_DATA-1<<" RAM7 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	if (errCount!=0)   log_plnx<<"[ulbf_data_check_ram] Reading from RAM FAILED"<<std::endl;
	else 	log_plnx<<"[ulbf_data_check_ram] PASSED "<<std::endl;
}

void ulbf_coeffs_check_ram(int ipNum, int * arrPtr0, int * arrPtr1, int * arrPtr2, int * arrPtr3, int * arrPtr4, int * arrPtr5, int * arrPtr6, int * arrPtr7) {
	log_plnx<<"[ulbf_coeffs_check_ram] Reading initialized data from ulbf_coeffs IP RAMs"<<std::endl;
	int errCount = 0;
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = ulbf_coeffs_mem_ptr[ipNum] + ULBF_COEFFS_RAM0_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*8);
		unsigned int data1 = memptr_ReadReg(addr, (i*8)+4);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr0+0))  errCount++;
		if (imag0!=*(arrPtr0+1))  errCount++;
		if (real1!=*(arrPtr0+2))  errCount++;
		if (imag1!=*(arrPtr0+3))  errCount++;
		arrPtr0 = arrPtr0+4;
		log_plnx<<"[ulbf_coeffs_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_ULBF_COEFFS-1<<" RAM0 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = ulbf_coeffs_mem_ptr[ipNum] + ULBF_COEFFS_RAM1_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*8);
		unsigned int data1 = memptr_ReadReg(addr, (i*8)+4);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr1+0))  errCount++;
		if (imag0!=*(arrPtr1+1))  errCount++;
		if (real1!=*(arrPtr1+2))  errCount++;
		if (imag1!=*(arrPtr1+3))  errCount++;
		arrPtr1 = arrPtr1+4;
		log_plnx<<"[ulbf_coeffs_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_ULBF_COEFFS-1<<" RAM1 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = ulbf_coeffs_mem_ptr[ipNum] + ULBF_COEFFS_RAM2_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*8);
		unsigned int data1 = memptr_ReadReg(addr, (i*8)+4);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr2+0))  errCount++;
		if (imag0!=*(arrPtr2+1))  errCount++;
		if (real1!=*(arrPtr2+2))  errCount++;
		if (imag1!=*(arrPtr2+3))  errCount++;
		arrPtr2 = arrPtr2+4;
		log_plnx<<"[ulbf_coeffs_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_ULBF_COEFFS-1<<" RAM2 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = ulbf_coeffs_mem_ptr[ipNum] + ULBF_COEFFS_RAM3_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*8);
		unsigned int data1 = memptr_ReadReg(addr, (i*8)+4);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr3+0))  errCount++;
		if (imag0!=*(arrPtr3+1))  errCount++;
		if (real1!=*(arrPtr3+2))  errCount++;
		if (imag1!=*(arrPtr3+3))  errCount++;
		arrPtr3 = arrPtr3+4;
		log_plnx<<"[ulbf_coeffs_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_ULBF_COEFFS-1<<" RAM3 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = ulbf_coeffs_mem_ptr[ipNum] + ULBF_COEFFS_RAM4_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*8);
		unsigned int data1 = memptr_ReadReg(addr, (i*8)+4);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr4+0))  errCount++;
		if (imag0!=*(arrPtr4+1))  errCount++;
		if (real1!=*(arrPtr4+2))  errCount++;
		if (imag1!=*(arrPtr4+3))  errCount++;
		arrPtr4 = arrPtr4+4;
		log_plnx<<"[ulbf_coeffs_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_ULBF_COEFFS-1<<" RAM4 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = ulbf_coeffs_mem_ptr[ipNum] + ULBF_COEFFS_RAM5_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*8);
		unsigned int data1 = memptr_ReadReg(addr, (i*8)+4);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr5+0))  errCount++;
		if (imag0!=*(arrPtr5+1))  errCount++;
		if (real1!=*(arrPtr5+2))  errCount++;
		if (imag1!=*(arrPtr5+3))  errCount++;
		arrPtr5 = arrPtr5+4;
		log_plnx<<"[ulbf_coeffs_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_ULBF_COEFFS-1<<" RAM5 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = ulbf_coeffs_mem_ptr[ipNum] + ULBF_COEFFS_RAM6_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*8);
		unsigned int data1 = memptr_ReadReg(addr, (i*8)+4);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr6+0))  errCount++;
		if (imag0!=*(arrPtr6+1))  errCount++;
		if (real1!=*(arrPtr6+2))  errCount++;
		if (imag1!=*(arrPtr6+3))  errCount++;
		arrPtr6 = arrPtr6+4;
		log_plnx<<"[ulbf_coeffs_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_ULBF_COEFFS-1<<" RAM6 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = ulbf_coeffs_mem_ptr[ipNum] + ULBF_COEFFS_RAM7_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*8);
		unsigned int data1 = memptr_ReadReg(addr, (i*8)+4);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr7+0))  errCount++;
		if (imag0!=*(arrPtr7+1))  errCount++;
		if (real1!=*(arrPtr7+2))  errCount++;
		if (imag1!=*(arrPtr7+3))  errCount++;
		arrPtr7 = arrPtr7+4;
		log_plnx<<"[ulbf_coeffs_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_ULBF_COEFFS-1<<" RAM7 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	if (errCount!=0)   log_plnx<<"[ulbf_coeffs_check_ram] Reading from RAM FAILED"<<std::endl;
	else 	log_plnx<<"[ulbf_coeffs_check_ram] PASSED "<<std::endl;
}

void ulbf_start(int debugPrint) {
	for (unsigned int i=0; i<NUM_ULBF_DATA; i++ ) {
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM0_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x10);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM1_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x10);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM2_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x10);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM3_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x10);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM4_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x10);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM5_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x10);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM6_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x10);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM7_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x10);
	}
	for (unsigned int i=0; i<NUM_ULBF_COEFFS; i++ ) {
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM0_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x10);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM1_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x10);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM2_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x10);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM3_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x10);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM4_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x10);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM5_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x10);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM6_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x10);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM7_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x10);
	}
	if(debugPrint) log_plnx<<"[ulbf_start] Done "<<std::endl;  
}

void ulbf_stop(int debugPrint) {
	for (unsigned int i=0; i<NUM_ULBF_DATA; i++ ) {
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM0_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x0);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM1_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x0);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM2_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x0);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM3_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x0);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM4_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x0);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM5_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x0);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM6_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x0);
		memptr_WriteReg(ulbf_data_mem_ptr[i], ULBF_DATA_RAM7_OFFSET+ULBF_DATA_CSR_OFFSET+ULBF_DATA_REG_OFFSET_START, 0x0);
	}
	for (unsigned int i=0; i<NUM_ULBF_COEFFS; i++ ) {
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM0_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x0);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM1_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x0);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM2_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x0);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM3_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x0);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM4_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x0);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM5_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x0);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM6_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x0);
		memptr_WriteReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_RAM7_OFFSET+ULBF_COEFFS_CSR_OFFSET+ULBF_COEFFS_REG_OFFSET_START, 0x0);
	}
	if(debugPrint) log_plnx<<"[ulbf_stop] Done "<<std::endl;  
}

uint32_t ulbf_data_wait_for_done(int debugPrint) {
	uint32_t done;
	for (unsigned int i=0; i<NUM_ULBF_DATA;i++) {
		done = memptr_ReadReg(ulbf_data_mem_ptr[i], ULBF_DATA_CSR_OFFSET + ULBF_DATA_REG_OFFSET_DONE);
		if(debugPrint) log_plnx<<"[ulbf_data_wait_for_done] -"<<std::dec<<i<<"- Data:"<<std::hex<<done<<" Expected:0xFF"<<std::endl;
		if (done!=255)
			break;
	}
	if (done==255) return 1;
	else return 0;
}

uint32_t ulbf_coeffs_wait_for_done(int debugPrint) {
	uint32_t done;
	for (unsigned int i=0; i<NUM_ULBF_COEFFS;i++) {
		done = memptr_ReadReg(ulbf_coeffs_mem_ptr[i], ULBF_COEFFS_CSR_OFFSET + ULBF_COEFFS_REG_OFFSET_DONE);
		if(debugPrint) log_plnx<<"[ulbf_coeffs_wait_for_done] -"<<std::dec<<i<<"- Data:"<<std::hex<<done<<" Expected:0xFF"<<std::endl;
		if (done!=255)
			break;
	}
	if (done==255) return 1;
	else return 0;
}

uint32_t ulbf_slave_wait_for_done(unsigned int niter, unsigned int block_size, int debugPrint) {
	uint32_t done;
	for (unsigned int i=0; i<NUM_ULBF_SLAVES;i++) {
		done = memptr_ReadReg(ulbf_slave_mem_ptr[i], ULBF_SLAVE_CSR_OFFSET + ULBF_SLAVE_REG_OFFSET_RAM_STATUS);
		if(debugPrint) log_plnx<<"[ulbf_slave_wait_for_done] -"<<std::dec<<i<<"- Data:"<<std::hex<<done<<" Expected:"<<std::dec<<niter*block_size<<std::endl;
		if (done!=(niter*block_size))
			break;
	}
	if (done==(niter*block_size)) return 1;
	else	return 0;
}


int ulbf_slave_read_and_verify(int * arrPtr, unsigned int id, unsigned int niter, int output_block_size, int debugPrint) {
	int errCount = 0;
	for(unsigned int i=0; i<niter*output_block_size; i++) {
		unsigned int data0 = memptr_ReadReg(ulbf_slave_mem_ptr[id], i*2); 
		unsigned int data1 = memptr_ReadReg(ulbf_slave_mem_ptr[id], (i*2)+1); 
		int real0 = extractIQ(data0, 0); 
		int imag0 = extractIQ(data0,1 ); 
		int real1 = extractIQ(data1, 0); 
		int imag1 = extractIQ(data1, 1); 
		if (real0!=*(arrPtr+0))  errCount++;
		if (imag0!=*(arrPtr+1))  errCount++;
		if (real1!=*(arrPtr+2))  errCount++;
		if (imag1!=*(arrPtr+3))  errCount++;
		if (debugPrint) log_plnx<<"ULBF-"<<std::dec<<id<<"-tdata: "<<i<<": "<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<"  Expected: "<<*(arrPtr+0)<<" "<<*(arrPtr+1)<<" "<<*(arrPtr+2)<<" "<<*(arrPtr+3)<<std::endl;
		arrPtr = arrPtr+4;
	}
	return errCount;
}
