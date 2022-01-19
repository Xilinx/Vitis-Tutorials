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
#include "utils_dlbf.h"

#define DEBUGTEST

u32 * dlbf_data_mem_ptr[NUM_DLBF_DATA]; 
u32 * dlbf_coeff_mem_ptr[NUM_DLBF_COEFFS]; 
u32 * dlbf_slave_mem_ptr[NUM_DLBF_SLAVES];

std::ofstream log_plnx;
std::stringstream logPlnx;
std::string logPlnx_str;

int dlbf_assign_addr() {
    int uio_fd[NUM_DLBF_DATA+NUM_DLBF_COEFFS+NUM_DLBF_SLAVES];
    uint32_t mapAddr[NUM_DLBF_DATA+NUM_DLBF_COEFFS+NUM_DLBF_SLAVES] = {};
    uint32_t mapSize[NUM_DLBF_DATA+NUM_DLBF_COEFFS+NUM_DLBF_SLAVES] = {};
    char uioName[NUM_DLBF_DATA+NUM_DLBF_COEFFS+NUM_DLBF_SLAVES][256];
    int uio_num[NUM_DLBF_DATA+NUM_DLBF_COEFFS+NUM_DLBF_SLAVES];
    int ret;
    char addrFile[256];
    char sizeFile[256];
    char nameFile[256];
    char uioList[256];
    FILE *fp;

    log_plnx << "[dlbf_assign_addr] Begin Loop To Processing UIO..." << std::endl;
    int index = 0;
    for(unsigned int i=0; i < (NUM_STREAMS); i++){
        sprintf(nameFile, "/sys/class/uio/uio%d/name", i);
        fp = fopen(nameFile, "r");
        if(!fp) return -1;
        ret = fscanf(fp, "%s", uioName[index]);
        fclose(fp);
        if(ret < 0) return -2;
        if (strstr(uioName[index],"dlbf_coeffs")!= NULL){
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
        if (strstr(uioName[index],"dlbf_data")!= NULL){
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
        if (strstr(uioName[index],"dlbf_o")!= NULL){
	    uio_num[index] = i;
	    index++;
        }
    }

    for(unsigned int i = 0; i < (NUM_DLBF_DATA+NUM_DLBF_COEFFS+NUM_DLBF_SLAVES); i++) {
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

    log_plnx << "[dlbf_assign_addr]  Processed all Masters/Slaves" << std::endl;
    for(unsigned int i = 0; i < (NUM_DLBF_COEFFS); i++){
        dlbf_coeff_mem_ptr[i]  = (u32  *)mmap((void*)mapAddr[i], mapSize[i], PROT_READ|PROT_WRITE, MAP_SHARED, uio_fd[i], 0);
    }
    for(unsigned int i = 0; i < (NUM_DLBF_DATA); i++){
        dlbf_data_mem_ptr[i]  = (u32  *)mmap((void*)mapAddr[NUM_DLBF_COEFFS+i], mapSize[NUM_DLBF_COEFFS+i], PROT_READ|PROT_WRITE, MAP_SHARED, uio_fd[NUM_DLBF_COEFFS+i], 0);
    }
    for(unsigned int i = 0; i < (NUM_DLBF_SLAVES); i++){
        dlbf_slave_mem_ptr[i] = (u32 *)mmap((void*)mapAddr[NUM_DLBF_DATA+NUM_DLBF_COEFFS+i], mapSize[NUM_DLBF_DATA+NUM_DLBF_COEFFS+i], PROT_READ|PROT_WRITE, MAP_SHARED, uio_fd[NUM_DLBF_DATA+NUM_DLBF_COEFFS+i], 0);
    }
    log_plnx << "[dlbf_assign_addr] Done" << std::endl;
    return 0;

}

void dlbf_data_config_ip(unsigned int i, u32 * ipBaseAddr, unsigned int niter, int debugPrint) {
	u32 * csrAddr;
	csrAddr = ipBaseAddr + DLBF_DATA_CSR_OFFSET  + DLBF_DATA_RAM0_OFFSET; 
	memptr_WriteReg(csrAddr, DLBF_DATA_REG_OFFSET_BLOCK_SIZE, DLBF_DATA_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, DLBF_DATA_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, DLBF_DATA_REG_OFFSET_ROLLOVER,   DLBF_DATA_INPUT_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + DLBF_DATA_CSR_OFFSET  + DLBF_DATA_RAM1_OFFSET; 
	memptr_WriteReg(csrAddr, DLBF_DATA_REG_OFFSET_BLOCK_SIZE, DLBF_DATA_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, DLBF_DATA_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, DLBF_DATA_REG_OFFSET_ROLLOVER,   DLBF_DATA_INPUT_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + DLBF_DATA_CSR_OFFSET  + DLBF_DATA_RAM2_OFFSET; 
	memptr_WriteReg(csrAddr, DLBF_DATA_REG_OFFSET_BLOCK_SIZE, DLBF_DATA_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, DLBF_DATA_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, DLBF_DATA_REG_OFFSET_ROLLOVER,   DLBF_DATA_INPUT_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + DLBF_DATA_CSR_OFFSET  + DLBF_DATA_RAM3_OFFSET; 
	memptr_WriteReg(csrAddr, DLBF_DATA_REG_OFFSET_BLOCK_SIZE, DLBF_DATA_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, DLBF_DATA_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, DLBF_DATA_REG_OFFSET_ROLLOVER,   DLBF_DATA_INPUT_ROLLOVER_ADDR);

	if(debugPrint) log_plnx<<"[dlbf_data_config_ip] IP:"<<i<<"/"<<NUM_DLBF_DATA-1<<" Done "<<std::endl;
}

void dlbf_data_config_ips(unsigned int niter, int debugPrint) {
	for (unsigned int i=0; i<NUM_DLBF_DATA;i++) 
		dlbf_data_config_ip(i, dlbf_data_mem_ptr[i], niter, debugPrint);
}

void dlbf_data_get_ip_status(unsigned int i, u32 * ipBaseAddr) {
	log_plnx<<"---------------------------"<<std::endl;
	u32 * addr;
	addr = ipBaseAddr + DLBF_DATA_RAM0_OFFSET + DLBF_DATA_CSR_OFFSET; 
	log_plnx<<"[dlbf_data-0/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_ID          : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[dlbf_data-0/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_RESET       : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[dlbf_data-0/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[dlbf_data-0/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_NITER       : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[dlbf_data-0/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_ROLLOVER    : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[dlbf_data-0/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_DONE        : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[dlbf_data-0/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_RAM_STATUS)<<std::endl;  

	addr = ipBaseAddr + DLBF_DATA_RAM1_OFFSET + DLBF_DATA_CSR_OFFSET; 
	log_plnx<<"[dlbf_data-1/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_ID          : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[dlbf_data-1/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_RESET       : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[dlbf_data-1/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[dlbf_data-1/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_NITER       : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[dlbf_data-1/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_ROLLOVER    : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[dlbf_data-1/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_DONE        : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[dlbf_data-1/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_RAM_STATUS)<<std::endl;  

	addr = ipBaseAddr + DLBF_DATA_RAM2_OFFSET + DLBF_DATA_CSR_OFFSET; 
	log_plnx<<"[dlbf_data-2/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_ID          : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[dlbf_data-2/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_RESET       : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[dlbf_data-2/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[dlbf_data-2/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_NITER       : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[dlbf_data-2/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_ROLLOVER    : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[dlbf_data-2/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_DONE        : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[dlbf_data-2/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_RAM_STATUS)<<std::endl;  

	addr = ipBaseAddr + DLBF_DATA_RAM3_OFFSET + DLBF_DATA_CSR_OFFSET; 
	log_plnx<<"[dlbf_data-3/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_ID          : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[dlbf_data-3/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_RESET       : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[dlbf_data-3/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[dlbf_data-3/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_NITER       : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[dlbf_data-3/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_ROLLOVER    : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[dlbf_data-3/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_DONE        : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[dlbf_data-3/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_DATA-1<<" DLBF_DATA_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_RAM_STATUS)<<std::endl;  

}

void dlbf_data_get_all_ip_status() {
	for (unsigned int i=0; i<NUM_DLBF_DATA;i++) 
		dlbf_data_get_ip_status(i, dlbf_data_mem_ptr[i] );
}


void dlbf_coeffs_config_ip(unsigned int i, u32 * ipBaseAddr, unsigned int niter, int debugPrint) {
	u32 * csrAddr;
	csrAddr = ipBaseAddr + DLBF_COEFFS_CSR_OFFSET  + DLBF_COEFFS_RAM0_OFFSET; 
	memptr_WriteReg(csrAddr, DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE, DLBF_COEFFS_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, DLBF_COEFFS_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, DLBF_COEFFS_REG_OFFSET_ROLLOVER,   DLBF_COEFFS_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + DLBF_COEFFS_CSR_OFFSET  + DLBF_COEFFS_RAM1_OFFSET; 
	memptr_WriteReg(csrAddr, DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE, DLBF_COEFFS_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, DLBF_COEFFS_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, DLBF_COEFFS_REG_OFFSET_ROLLOVER,   DLBF_COEFFS_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + DLBF_COEFFS_CSR_OFFSET  + DLBF_COEFFS_RAM2_OFFSET; 
	memptr_WriteReg(csrAddr, DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE, DLBF_COEFFS_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, DLBF_COEFFS_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, DLBF_COEFFS_REG_OFFSET_ROLLOVER,   DLBF_COEFFS_ROLLOVER_ADDR);

	csrAddr = ipBaseAddr + DLBF_COEFFS_CSR_OFFSET  + DLBF_COEFFS_RAM3_OFFSET; 
	memptr_WriteReg(csrAddr, DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE, DLBF_COEFFS_INPUT_BLOCK_SIZE);
	memptr_WriteReg(csrAddr, DLBF_COEFFS_REG_OFFSET_NITER,      niter);
	memptr_WriteReg(csrAddr, DLBF_COEFFS_REG_OFFSET_ROLLOVER,   DLBF_COEFFS_ROLLOVER_ADDR);

	if(debugPrint) log_plnx<<"[dlbf_coeffs_config_ip] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" Done "<<std::endl;
}

void dlbf_coeffs_config_ips(unsigned int niter, int debugPrint) {
	for (unsigned int i=0; i<NUM_DLBF_COEFFS;i++) 
		dlbf_coeffs_config_ip(i, dlbf_coeff_mem_ptr[i], niter, debugPrint);
}

void dlbf_coeffs_get_ip_status(unsigned int i, u32 * ipBaseAddr) {
	log_plnx<<"---------------------------"<<std::endl;
	u32 * addr;
	addr = ipBaseAddr + DLBF_COEFFS_RAM0_OFFSET + DLBF_COEFFS_CSR_OFFSET; 
	log_plnx<<"[dlbf_coeffs-0/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_ID          : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-0/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_RESET       : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-0/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-0/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_NITER       : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-0/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_ROLLOVER	: 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-0/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_DONE        : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-0/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_RAM_STATUS)<<std::endl;  

	addr = ipBaseAddr + DLBF_COEFFS_RAM1_OFFSET + DLBF_COEFFS_CSR_OFFSET; 
	log_plnx<<"[dlbf_coeffs-1/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_ID          : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-1/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_RESET       : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-1/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-1/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_NITER       : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-1/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_ROLLOVER	: 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-1/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_DONE        : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-1/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_RAM_STATUS)<<std::endl;  

	addr = ipBaseAddr + DLBF_COEFFS_RAM2_OFFSET + DLBF_COEFFS_CSR_OFFSET; 
	log_plnx<<"[dlbf_coeffs-2/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_ID          : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-2/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_RESET       : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-2/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-2/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_NITER       : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-2/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_ROLLOVER	: 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-2/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_DONE        : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-2/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_RAM_STATUS)<<std::endl;  
		
	addr = ipBaseAddr + DLBF_COEFFS_RAM3_OFFSET + DLBF_COEFFS_CSR_OFFSET; 
	log_plnx<<"[dlbf_coeffs-3/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_ID          : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-3/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_RESET       : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-3/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE  : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_BLOCK_SIZE)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-3/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_NITER       : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-3/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_ROLLOVER	: 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_ROLLOVER)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-3/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_DONE        : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[dlbf_coeffs-3/3] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_COEFFS-1<<" DLBF_COEFFS_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_RAM_STATUS)<<std::endl;  

}

void dlbf_coeffs_get_all_ip_status() {
	for (unsigned int i=0; i<NUM_DLBF_COEFFS;i++) 
		dlbf_coeffs_get_ip_status(i, dlbf_coeff_mem_ptr[i] );
}

void dlbf_slave_config_ip(unsigned int i, u32 * ipBaseAddr, unsigned int niter, int debugPrint) {
	u32 * csrAddr = ipBaseAddr + DLBF_SLAVE_CSR_OFFSET ; 
	memptr_WriteReg(csrAddr, DLBF_SLAVE_REG_OFFSET_NITER,      niter);
	if(debugPrint) log_plnx<<"[dlbf_slave_config_ip] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_SLAVES-1<<" Done "<<std::endl;
}

void dlbf_slave_config_ips(unsigned int niter, int debugPrint) {
	for (unsigned int i=0; i<NUM_DLBF_SLAVES;i++) 
		dlbf_slave_config_ip(i, dlbf_slave_mem_ptr[i], niter, debugPrint);
}

void dlbf_slave_get_ip_status(unsigned int i, u32 * ipBaseAddr) {
	log_plnx<<"---------------------------"<<std::endl;
	u32 * addr;
	addr = ipBaseAddr + DLBF_SLAVE_CSR_OFFSET; 
	log_plnx<<"[dlbf_slave] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_SLAVES-1<<" DLBF_SLAVE_REG_OFFSET_ID          : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_SLAVE_REG_OFFSET_ID)<<std::endl; 
	log_plnx<<"[dlbf_slave] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_SLAVES-1<<" DLBF_SLAVE_REG_OFFSET_RESET       : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_SLAVE_REG_OFFSET_RESET)<<std::endl; 
	log_plnx<<"[dlbf_slave] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_SLAVES-1<<" DLBF_SLAVE_REG_OFFSET_NITER       : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_SLAVE_REG_OFFSET_NITER)<<std::endl; 
	log_plnx<<"[dlbf_slave] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_SLAVES-1<<" DLBF_SLAVE_REG_OFFSET_DONE        : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_SLAVE_REG_OFFSET_DONE)<<std::endl; 
	log_plnx<<"[dlbf_slave] IP:"<<std::dec<<i<<"/"<<NUM_DLBF_SLAVES-1<<" DLBF_SLAVE_REG_OFFSET_RAM_STATUS  : 0x"<<std::hex<< memptr_ReadReg(addr, DLBF_SLAVE_REG_OFFSET_RAM_STATUS)<<std::endl;  
}

void dlbf_slave_get_all_ip_status() {
	for (unsigned int i=0; i<NUM_DLBF_SLAVES;i++) 
		dlbf_slave_get_ip_status(i, dlbf_slave_mem_ptr[i]);
}


void dlbf_data_reset(int debugPrint) {
	unsigned long sleepTime = 2; 
	for (unsigned int i=0; i<NUM_DLBF_DATA; i++ ) {
		memptr_WriteReg(dlbf_data_mem_ptr[i], DLBF_DATA_RAM0_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_RESET, 1);
		memptr_WriteReg(dlbf_data_mem_ptr[i], DLBF_DATA_RAM1_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_RESET, 1);
		memptr_WriteReg(dlbf_data_mem_ptr[i], DLBF_DATA_RAM2_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_RESET, 1);
		memptr_WriteReg(dlbf_data_mem_ptr[i], DLBF_DATA_RAM3_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_RESET, 1);
	}
	usleep(sleepTime); 
	for (unsigned int i=0; i<NUM_DLBF_DATA; i++ ) {
		memptr_WriteReg(dlbf_data_mem_ptr[i], DLBF_DATA_RAM0_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_RESET, 0);
		memptr_WriteReg(dlbf_data_mem_ptr[i], DLBF_DATA_RAM1_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_RESET, 0);
		memptr_WriteReg(dlbf_data_mem_ptr[i], DLBF_DATA_RAM2_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_RESET, 0);
		memptr_WriteReg(dlbf_data_mem_ptr[i], DLBF_DATA_RAM3_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_RESET, 0);
	}
	if(debugPrint) log_plnx<<"[dlbf_data_reset] Done "<<std::endl;  
}

void dlbf_coeffs_reset(int debugPrint) {
	unsigned long sleepTime = 2; 
	for (unsigned int i=0; i<NUM_DLBF_COEFFS; i++ ) {
		memptr_WriteReg(dlbf_coeff_mem_ptr[i], DLBF_COEFFS_RAM0_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_RESET, 1);
		memptr_WriteReg(dlbf_coeff_mem_ptr[i], DLBF_COEFFS_RAM1_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_RESET, 1);
		memptr_WriteReg(dlbf_coeff_mem_ptr[i], DLBF_COEFFS_RAM2_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_RESET, 1);
		memptr_WriteReg(dlbf_coeff_mem_ptr[i], DLBF_COEFFS_RAM3_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_RESET, 1);
	}
	usleep(sleepTime); 
	for (unsigned int i=0; i<NUM_DLBF_COEFFS; i++ ) {
		memptr_WriteReg(dlbf_coeff_mem_ptr[i], DLBF_COEFFS_RAM0_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_RESET, 0);
		memptr_WriteReg(dlbf_coeff_mem_ptr[i], DLBF_COEFFS_RAM1_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_RESET, 0);
		memptr_WriteReg(dlbf_coeff_mem_ptr[i], DLBF_COEFFS_RAM2_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_RESET, 0);
		memptr_WriteReg(dlbf_coeff_mem_ptr[i], DLBF_COEFFS_RAM3_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_RESET, 0);
	}
	if(debugPrint) log_plnx<<"[dlbf_coeffs_reset] Done "<<std::endl;  
}

void dlbf_slave_reset(int debugPrint) {
	unsigned long sleepTime = 2; 
	for (unsigned int i=0; i<NUM_DLBF_SLAVES; i++ ) {
		memptr_WriteReg(dlbf_slave_mem_ptr[i], DLBF_SLAVE_CSR_OFFSET+DLBF_SLAVE_REG_OFFSET_RESET, 1);
	}
	usleep(sleepTime); 
	for (unsigned int i=0; i<NUM_DLBF_SLAVES; i++ ) {
		memptr_WriteReg(dlbf_slave_mem_ptr[i], DLBF_SLAVE_CSR_OFFSET+DLBF_SLAVE_REG_OFFSET_RESET, 0);
	}
	if(debugPrint) log_plnx<<"[dlbf_slave_reset] Done "<<std::endl;  
}

void dlbf_reset(int debugPrint) {
	dlbf_data_reset(debugPrint);
	dlbf_coeffs_reset(debugPrint);
	dlbf_slave_reset(debugPrint);
}

void dlbf_data_check_ram(int ipNum, int * arrPtr0, int * arrPtr1, int * arrPtr2, int * arrPtr3) {
    log_plnx<<"[dlbf_data_check_ram] Reading initialized data from dlbf_data IP RAMs"<<std::endl;
	int errCount = 0;
    for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = dlbf_data_mem_ptr[ipNum] + DLBF_DATA_RAM0_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*2);
		unsigned int data1 = memptr_ReadReg(addr, (i*2)+1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr0+0))  errCount++;
		if (imag0!=*(arrPtr0+1))  errCount++;
		if (real1!=*(arrPtr0+2))  errCount++;
		if (imag1!=*(arrPtr0+3))  errCount++;
		arrPtr0 = arrPtr0+4;
		log_plnx<<"[dlbf_data_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_DLBF_DATA-1<<" RAM0 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = dlbf_data_mem_ptr[ipNum] + DLBF_DATA_RAM1_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*2);
		unsigned int data1 = memptr_ReadReg(addr, (i*2)+1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr1+0))  errCount++;
		if (imag0!=*(arrPtr1+1))  errCount++;
		if (real1!=*(arrPtr1+2))  errCount++;
		if (imag1!=*(arrPtr1+3))  errCount++;
		arrPtr1 = arrPtr1+4;
		log_plnx<<"[dlbf_data_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_DLBF_DATA-1<<" RAM1 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = dlbf_data_mem_ptr[ipNum] + DLBF_DATA_RAM2_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*2);
		unsigned int data1 = memptr_ReadReg(addr, (i*2)+1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr2+0))  errCount++;
		if (imag0!=*(arrPtr2+1))  errCount++;
		if (real1!=*(arrPtr2+2))  errCount++;
		if (imag1!=*(arrPtr2+3))  errCount++;
		arrPtr2 = arrPtr2+4;
		log_plnx<<"[dlbf_data_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_DLBF_DATA-1<<" RAM2 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = dlbf_data_mem_ptr[ipNum] + DLBF_DATA_RAM3_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*2);
		unsigned int data1 = memptr_ReadReg(addr, (i*2)+1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr3+0))  errCount++;
		if (imag0!=*(arrPtr3+1))  errCount++;
		if (real1!=*(arrPtr3+2))  errCount++;
		if (imag1!=*(arrPtr3+3))  errCount++;
		arrPtr3 = arrPtr3+4;
		log_plnx<<"[dlbf_data_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_DLBF_DATA-1<<" RAM3 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	if (errCount!=0)   log_plnx<<"[dlbf_data_check_ram] Reading from RAM FAILED\n"<<std::endl;
	else 	log_plnx<<"[dlbf_data_check_ram] PASSED"<<std::endl;
}

void dlbf_coeffs_check_ram(int ipNum, int * arrPtr0, int * arrPtr1, int * arrPtr2, int * arrPtr3) {
	log_plnx<<"[dlbf_coeffs_check_ram] Reading initialized data from dlbf_coeffs IP RAMs\n"<<std::endl;
	int errCount = 0;
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = dlbf_coeff_mem_ptr[ipNum] + DLBF_COEFFS_RAM0_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*2);
		unsigned int data1 = memptr_ReadReg(addr, (i*2)+1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr0+0))  errCount++;
		if (imag0!=*(arrPtr0+1))  errCount++;
		if (real1!=*(arrPtr0+2))  errCount++;
		if (imag1!=*(arrPtr0+3))  errCount++;
		arrPtr0 = arrPtr0+4;
		log_plnx<<"[dlbf_coeffs_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_DLBF_COEFFS-1<<" RAM0 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = dlbf_coeff_mem_ptr[ipNum] + DLBF_COEFFS_RAM1_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*2);
		unsigned int data1 = memptr_ReadReg(addr, (i*2)+1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr1+0))  errCount++;
		if (imag0!=*(arrPtr1+1))  errCount++;
		if (real1!=*(arrPtr1+2))  errCount++;
		if (imag1!=*(arrPtr1+3))  errCount++;
		arrPtr1 = arrPtr1+4;
		log_plnx<<"[dlbf_coeffs_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_DLBF_COEFFS-1<<" RAM1 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = dlbf_coeff_mem_ptr[ipNum] + DLBF_COEFFS_RAM2_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*2);
		unsigned int data1 = memptr_ReadReg(addr, (i*2)+1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr2+0))  errCount++;
		if (imag0!=*(arrPtr2+1))  errCount++;
		if (real1!=*(arrPtr2+2))  errCount++;
		if (imag1!=*(arrPtr2+3))  errCount++;
		arrPtr2 = arrPtr2+4;
		log_plnx<<"[dlbf_coeffs_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_DLBF_COEFFS-1<<" RAM2 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	for (unsigned int i=0; i<5; i++ ) {
		u32 * addr   = dlbf_coeff_mem_ptr[ipNum] + DLBF_COEFFS_RAM3_OFFSET;
		unsigned int data0 = memptr_ReadReg(addr, i*2);
		unsigned int data1 = memptr_ReadReg(addr, (i*2)+1);
		int real0 = extractIQ(data0, 0); // 0 for lower 16b and 1 for upper 16b
		int imag0 = extractIQ(data0, 1);
		int real1 = extractIQ(data1, 0);
		int imag1 = extractIQ(data1, 1);
		if (real0!=*(arrPtr3+0))  errCount++;
		if (imag0!=*(arrPtr3+1))  errCount++;
		if (real1!=*(arrPtr3+2))  errCount++;
		if (imag1!=*(arrPtr3+3))  errCount++;
		arrPtr3 = arrPtr3+4;
		log_plnx<<"[dlbf_coeffs_check_ram] IP:"<<std::dec<<ipNum<<"/"<<NUM_DLBF_COEFFS-1<<" RAM3 DATA "<<i<<": 0x"<<std::hex<<data1<<data0<<" "<<std::dec<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<std::endl;
	}
	if (errCount!=0)   log_plnx<<"[dlbf_coeffs_check_ram] Reading from RAM FAILED\n"<<std::endl;
	else 	log_plnx<<"[dlbf_coeffs_check_ram] PASSED "<<std::endl;
}

void dlbf_coeffs_write(unsigned int coeffId, unsigned int niter){

    std::ifstream inp_file0("dlbf_cin10_hex.mem");
    if (!inp_file0) {
        std::cout << "[ERROR] Unable to open file";
        exit(1);
    }
    log_plnx << "[dlbf_coeffs_write] ****** DLBF COEFFS IP: " <<coeffId << " Port: 0 ****** " << std::endl;
    for (unsigned int j = 0; j < DLBF_COEFFS_INPUT_BLOCK_SIZE*niter; j++) {
        unsigned long long data;
        std::istringstream strInput;
        std::string strInp;
        getline(inp_file0, strInp);
        strInput.str(strInp);
        strInput >> std::hex >> data;
        memptr_WriteReg(dlbf_coeff_mem_ptr[coeffId]+DLBF_COEFFS_RAM0_OFFSET, 2*j, data&0xffffffff);
        memptr_WriteReg(dlbf_coeff_mem_ptr[coeffId]+DLBF_COEFFS_RAM0_OFFSET, 2*j+1, (data>>32)&0xffffffff);
    }
    
    std::ifstream inp_file1("dlbf_cin11_hex.mem");
    if (!inp_file1) {
        std::cout << "[ERROR] Unable to open file";
        exit(1);
    }
    log_plnx << "[dlbf_coeffs_write] ****** DLBF COEFFS IP: " <<coeffId << " Port: 1 ****** : " << std::endl;
    for (unsigned int j = 0; j < DLBF_COEFFS_INPUT_BLOCK_SIZE*niter; j++) {
        unsigned long long data;
        std::istringstream strInput;
        std::string strInp;
        getline(inp_file1, strInp);
        strInput.str(strInp);
        strInput >> std::hex >> data;
        memptr_WriteReg(dlbf_coeff_mem_ptr[coeffId]+DLBF_COEFFS_RAM1_OFFSET, 2*j, data&0xffffffff);
        memptr_WriteReg(dlbf_coeff_mem_ptr[coeffId]+DLBF_COEFFS_RAM1_OFFSET, 2*j+1, (data>>32)&0xffffffff);
    }
    std::ifstream inp_file2("dlbf_cin12_hex.mem");
    if (!inp_file2) {
        std::cout << "[ERROR] Unable to open file";
        exit(1);
    }
    log_plnx << "[dlbf_coeffs_write] ****** DLBF COEFFS IP: " <<coeffId << " Port: 2 ****** : " << std::endl;
    for (unsigned int j = 0; j < DLBF_COEFFS_INPUT_BLOCK_SIZE*niter; j++) {
        unsigned long long data;
        std::istringstream strInput;
        std::string strInp;
        getline(inp_file2, strInp);
        strInput.str(strInp);
        strInput >> std::hex >> data;
        memptr_WriteReg(dlbf_coeff_mem_ptr[coeffId]+DLBF_COEFFS_RAM2_OFFSET, 2*j, data&0xffffffff);
        memptr_WriteReg(dlbf_coeff_mem_ptr[coeffId]+DLBF_COEFFS_RAM2_OFFSET, 2*j+1, (data>>32)&0xffffffff);
    }
    std::ifstream inp_file3("dlbf_cin13_hex.mem");
    if (!inp_file3) {
        std::cout << "[ERROR] Unable to open file";
        exit(1);
    }
    log_plnx << "[dlbf_coeffs_write] ****** DLBF COEFFS IP: " <<coeffId << " Port: 3 ****** : " << std::endl;
    for (unsigned int j = 0; j < DLBF_COEFFS_INPUT_BLOCK_SIZE*niter; j++) {
        unsigned long long data;
        std::istringstream strInput;
        std::string strInp;
        getline(inp_file3, strInp);
        strInput.str(strInp);
        strInput >> std::hex >> data;
        memptr_WriteReg(dlbf_coeff_mem_ptr[coeffId]+DLBF_COEFFS_RAM3_OFFSET, 2*j, data&0xffffffff);
        memptr_WriteReg(dlbf_coeff_mem_ptr[coeffId]+DLBF_COEFFS_RAM3_OFFSET, 2*j+1, (data>>32)&0xffffffff);
    }
    printf("[dlbf_coeffs_write] Done ...\n");
}

void dlbf_start(int debugPrint) {
	for (unsigned int i=0; i<NUM_DLBF_DATA; i++ ) {
		memptr_WriteReg( dlbf_data_mem_ptr[i], DLBF_DATA_RAM0_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_START, 0x10);
		memptr_WriteReg( dlbf_data_mem_ptr[i], DLBF_DATA_RAM1_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_START, 0x10);
		memptr_WriteReg( dlbf_data_mem_ptr[i], DLBF_DATA_RAM2_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_START, 0x10);
		memptr_WriteReg( dlbf_data_mem_ptr[i], DLBF_DATA_RAM3_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_START, 0x10);
	}
	for (unsigned int i=0; i<NUM_DLBF_COEFFS; i++ ) {
		memptr_WriteReg( dlbf_coeff_mem_ptr[i], DLBF_COEFFS_RAM0_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_START, 0x10);
		memptr_WriteReg( dlbf_coeff_mem_ptr[i], DLBF_COEFFS_RAM1_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_START, 0x10);
		memptr_WriteReg( dlbf_coeff_mem_ptr[i], DLBF_COEFFS_RAM2_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_START, 0x10);
		memptr_WriteReg( dlbf_coeff_mem_ptr[i], DLBF_COEFFS_RAM3_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_START, 0x10);
	}
	if(debugPrint) log_plnx<<"[dlbf_start] Done"<<std::endl;  
}

void dlbf_stop(int debugPrint) {
	for (unsigned int i=0; i<NUM_DLBF_DATA; i++ ) {
		memptr_WriteReg( dlbf_data_mem_ptr[i], DLBF_DATA_RAM0_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_START, 0x0);
		memptr_WriteReg( dlbf_data_mem_ptr[i], DLBF_DATA_RAM1_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_START, 0x0);
		memptr_WriteReg( dlbf_data_mem_ptr[i], DLBF_DATA_RAM2_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_START, 0x0);
		memptr_WriteReg( dlbf_data_mem_ptr[i], DLBF_DATA_RAM3_OFFSET+DLBF_DATA_CSR_OFFSET+DLBF_DATA_REG_OFFSET_START, 0x0);
	}
	for (unsigned int i=0; i<NUM_DLBF_COEFFS; i++ ) {
		memptr_WriteReg( dlbf_coeff_mem_ptr[i], DLBF_COEFFS_RAM0_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_START, 0x0);
		memptr_WriteReg( dlbf_coeff_mem_ptr[i], DLBF_COEFFS_RAM1_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_START, 0x0);
		memptr_WriteReg( dlbf_coeff_mem_ptr[i], DLBF_COEFFS_RAM2_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_START, 0x0);
		memptr_WriteReg( dlbf_coeff_mem_ptr[i], DLBF_COEFFS_RAM3_OFFSET+DLBF_COEFFS_CSR_OFFSET+DLBF_COEFFS_REG_OFFSET_START, 0x0);
	}
	if(debugPrint) log_plnx<<"[dlbf_stop] Done"<<std::endl;  
}

uint32_t dlbf_data_wait_for_done(int debugPrint) {
	uint32_t done;
	u32 * addr;
	for (unsigned int i=0; i<NUM_DLBF_DATA;i++) {
		addr = dlbf_data_mem_ptr[i] + DLBF_DATA_CSR_OFFSET;
		done = memptr_ReadReg(addr, DLBF_DATA_REG_OFFSET_DONE);
		if(debugPrint) log_plnx<<"[dlbf_data_wait_for_done] -"<<std::dec<<i<<"- Data: 0x"<<std::hex<<done<<" Expected:0xF"<<std::endl;
		if (done!=15)
			break;
	}
	if (done==15) return 1;
	else return 0;
}

uint32_t dlbf_coeffs_wait_for_done(int debugPrint) {
	uint32_t done;
	u32 * addr;
	for (unsigned int i=0; i<NUM_DLBF_COEFFS;i++) {
		addr = dlbf_coeff_mem_ptr[i] + DLBF_COEFFS_CSR_OFFSET;
		done = memptr_ReadReg(addr, DLBF_COEFFS_REG_OFFSET_DONE);
		if(debugPrint) log_plnx<<"[dlbf_coeffs_wait_for_done] -"<<std::dec<<i<<"- Data0: 0x"<<std::hex<<done<<" Expected:0xF"<<std::endl;
		if (done!=15)
			break;
	}
	if (done==15) return 1;
	else return 0;
}

uint32_t dlbf_slave_wait_for_done(unsigned int niter, unsigned int block_size, int debugPrint) {
	uint32_t done;
	u32 * addr;
	for (unsigned int i=0; i<NUM_DLBF_SLAVES;i++) {
		addr = dlbf_slave_mem_ptr[i] + DLBF_SLAVE_CSR_OFFSET;
		done = memptr_ReadReg(addr, DLBF_SLAVE_REG_OFFSET_RAM_STATUS);
		if(debugPrint) log_plnx<<"[dlbf_slave_wait_for_done] -"<<std::dec<<i<<"- Read: 0x"<<std::hex<<done<<" Expected: 0x"<<niter*block_size<<std::endl;
		if (done!=(niter*block_size))
			break;
	}
	if (done==(niter*block_size)) return 1;
	else	return 0;
}


int dlbf_slave_read_and_verify(int * arrPtr, unsigned int id, unsigned int niter, int output_block_size, int debugPrint) {
	int errCount = 0;
	for(unsigned int i=0; i<niter*output_block_size; i++) {
		unsigned int data0 = memptr_ReadReg(dlbf_slave_mem_ptr[id], i*2); 
		unsigned int data1 = memptr_ReadReg(dlbf_slave_mem_ptr[id], (i*2)+1); 
		int real0 = extractIQ(data0, 0); 
		int imag0 = extractIQ(data0,1 ); 
		int real1 = extractIQ(data1, 0); 
		int imag1 = extractIQ(data1, 1); 
		if (real0!=*(arrPtr+0))  errCount++;
		if (imag0!=*(arrPtr+1))  errCount++;
		if (real1!=*(arrPtr+2))  errCount++;
		if (imag1!=*(arrPtr+3))  errCount++;
		if (debugPrint) log_plnx<<"DLBF-"<<std::dec<<id<<"-tdata: "<<i<<": "<<real0<<" "<<imag0<<" "<<real1<<" "<<imag1<<"  Expected: "<<*(arrPtr+0)<<" "<<*(arrPtr+1)<<" "<<*(arrPtr+2)<<" "<<*(arrPtr+3)<<std::endl;
		arrPtr = arrPtr+4;
	}
	return errCount;
}
