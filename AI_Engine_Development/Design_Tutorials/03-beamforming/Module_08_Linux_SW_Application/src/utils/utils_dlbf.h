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

#ifndef __UTILS_DLBF_H__
#define __UTILS_DLBF_H__

#include "params.h"

extern int dlbf_gold0[15360];
extern int dlbf_gold1[15360];
extern int dlbf_din0[20];
extern int dlbf_din1[20];
extern int dlbf_din2[20];
extern int dlbf_din3[20];
extern int dlbf_cin0[20];
extern int dlbf_cin1[20];
extern int dlbf_cin2[20];
extern int dlbf_cin3[20];
extern u32 * dlbf_data_mem_ptr[NUM_DLBF_DATA]; 
extern u32 * dlbf_coeff_mem_ptr[NUM_DLBF_COEFFS]; 
extern u32 * dlbf_slave_mem_ptr[NUM_DLBF_SLAVES];

int dlbf_assign_addr();

void dlbf_data_config_ip(unsigned int i, u32 * ipBaseAddr, unsigned int niter, int debugPrint);
void dlbf_data_config_ips(unsigned int niter, int debugPrint);
void dlbf_data_get_ip_status(unsigned int i, u32 * ipBaseAddr);
void dlbf_data_get_all_ip_status();
void dlbf_coeffs_config_ip(unsigned int i, u32 * ipBaseAddr, unsigned int niter, int debugPrint);
void dlbf_coeffs_config_ips(unsigned int niter, int debugPrint) ;
void dlbf_coeffs_get_ip_status(unsigned int i, u32 * ipBaseAddr);
void dlbf_coeffs_get_all_ip_status();
void dlbf_slave_config_ip(unsigned int i, u32 * ipBaseAddr, unsigned int niter, int debugPrint);
void dlbf_slave_config_ips(unsigned int niter, int debugPrint);
void dlbf_slave_get_ip_status(unsigned int i, u32 * ipBaseAddr);
void dlbf_slave_get_all_ip_status();
void dlbf_data_reset(int debugPrint);
void dlbf_coeffs_reset(int debugPrint); 
void dlbf_slave_reset(int debugPrint);
void dlbf_reset(int debugPrint);
void dlbf_data_check_ram(int ipNum, int * arrPtr0, int * arrPtr1, int * arrPtr2, int * arrPtr3);
void dlbf_coeffs_check_ram(int ipNum, int * arrPtr0, int * arrPtr1, int * arrPtr2, int * arrPtr3);
void dlbf_coeffs_write(unsigned int coeffId, unsigned int niter);
void dlbf_start(int debugPrint);
void dlbf_stop(int debugPrint);
uint32_t dlbf_data_wait_for_done(int debugPrint);
uint32_t dlbf_coeffs_wait_for_done(int debugPrint);
uint32_t dlbf_slave_wait_for_done(unsigned int niter, unsigned int block_size, int debugPrint);
int dlbf_slave_read_and_verify(int * arrPtr, unsigned int id, unsigned int niter, int output_block_size, int debugPrint) ;

#endif // __UTILS_DLBF_H_
