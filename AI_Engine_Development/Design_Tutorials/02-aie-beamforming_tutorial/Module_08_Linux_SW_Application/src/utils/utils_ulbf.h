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

#ifndef __UTILS_ULBF_H__
#define __UTILS_ULBF_H__

#include "params.h"

extern int ulbf_gold0[15360];
extern int ulbf_din0[20];
extern int ulbf_din1[20];
extern int ulbf_din2[20];
extern int ulbf_din3[20];
extern int ulbf_din4[20];
extern int ulbf_din5[20];
extern int ulbf_din6[20];
extern int ulbf_din7[20];
extern int ulbf_cin0[20];
extern int ulbf_cin1[20];
extern int ulbf_cin2[20];
extern int ulbf_cin3[20];
extern int ulbf_cin4[20];
extern int ulbf_cin5[20];
extern int ulbf_cin6[20];
extern int ulbf_cin7[20];
extern u32 * ulbf_data_mem_ptr[NUM_ULBF_DATA];
extern u32 * ulbf_coeffs_mem_ptr[NUM_ULBF_COEFFS];
extern u32 * ulbf_slave_mem_ptr[NUM_ULBF_SLAVES];

int ulbf_assign_addr();

void ulbf_data_config_ip(unsigned int i, u32 *  ipBaseAddr, unsigned int niter, int debugPrint);
void ulbf_data_config_ips(unsigned int niter, int debugPrint);
void ulbf_data_get_ip_status(unsigned int i, u32 *  ipBaseAddr);
void ulbf_data_get_all_ip_status();
void ulbf_coeffs_config_ip(unsigned int i, u32 *  ipBaseAddr, unsigned int niter, int debugPrint);
void ulbf_coeffs_config_ips(unsigned int niter, int debugPrint) ;
void ulbf_coeffs_get_ip_status(unsigned int i, u32 *  ipBaseAddr);
void ulbf_coeffs_get_all_ip_status();
void ulbf_slave_config_ip(unsigned int i, u32 *  ipBaseAddr, unsigned int niter, int debugPrint) ;
void ulbf_slave_config_ips(unsigned int niter, int debugPrint);
void ulbf_slave_get_ip_status(unsigned int i, u32 *  ipBaseAddr);
void ulbf_slave_get_all_ip_status();
void ulbf_data_reset(int debugPrint);
void ulbf_coeffs_reset(int debugPrint); 
void ulbf_slave_reset(int debugPrint);
void ulbf_reset(int debugPrint);
void ulbf_data_check_ram(int ipNum, int * arrPtr0, int * arrPtr1, int * arrPtr2, int * arrPtr3, int * arrPtr4, int * arrPtr5, int * arrPtr6, int * arrPtr7) ;
void ulbf_coeffs_check_ram(int ipNum, int * arrPtr0, int * arrPtr1, int * arrPtr2, int * arrPtr3, int * arrPtr4, int * arrPtr5, int * arrPtr6, int * arrPtr7);
void ulbf_start(int debugPrint);
void ulbf_stop(int debugPrint);
uint32_t ulbf_data_wait_for_done(int debugPrint);
uint32_t ulbf_coeffs_wait_for_done(int debugPrint);
uint32_t ulbf_slave_wait_for_done(unsigned int niter, unsigned int block_size, int debugPrint);
int ulbf_slave_read_and_verify(int * arrPtr, unsigned int id, unsigned int niter, int output_block_size, int debugPrint) ;

#endif // __UTILS_ULBF_H_
