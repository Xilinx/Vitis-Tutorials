
/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/


#ifndef __UTILS_DLBF_H__
#define __UTILS_DLBF_H__

#include "params.h"

extern int dlbf_gold0[15360];
extern int dlbf_din0[20];
extern int dlbf_din1[20];
extern int dlbf_din2[20];
extern int dlbf_din3[20];
extern int dlbf_cin0[20];
extern int dlbf_cin1[20];
extern int dlbf_cin2[20];
extern int dlbf_cin3[20];
extern uint64_t dlbfDataAddr[NUM_DLBF_DATA];
extern uint64_t dlbfCoeffsAddr[NUM_DLBF_COEFFS];
extern uint64_t dlbfSlaveAddr[NUM_DLBF_SLAVES];

void dlbf_data_config_ip(unsigned int i, uint64_t ipBaseAddr, unsigned int niter);
void dlbf_data_config_ips(unsigned int niter);
void dlbf_data_get_ip_status(unsigned int i, uint64_t ipBaseAddr);
void dlbf_data_get_all_ip_status();
void dlbf_coeffs_config_ip(unsigned int i, uint64_t ipBaseAddr, unsigned int niter);
void dlbf_coeffs_config_ips(unsigned int niter) ;
void dlbf_coeffs_get_ip_status(unsigned int i, uint64_t ipBaseAddr);
void dlbf_coeffs_get_all_ip_status();
void dlbf_slave_config_ip(unsigned int i, uint64_t ipBaseAddr, unsigned int niter);
void dlbf_slave_config_ips(unsigned int niter);
void dlbf_slave_get_ip_status(unsigned int i, uint64_t ipBaseAddr);
void dlbf_slave_get_all_ip_status();
void dlbf_data_reset();
void dlbf_coeffs_reset(); 
void dlbf_slave_reset();
void dlbf_reset();
void dlbf_data_check_ram(int ipNum, int * arrPtr0, int * arrPtr1, int * arrPtr2, int * arrPtr3);
void dlbf_coeffs_check_ram(int ipNum, int * arrPtr0, int * arrPtr1, int * arrPtr2, int * arrPtr3);
void dlbf_start();
uint32_t dlbf_data_wait_for_done();
uint32_t dlbf_coeffs_wait_for_done();
uint32_t dlbf_slave_wait_for_done(unsigned int niter, unsigned int block_size);
int dlbf_slave_read_and_verify(int * arrPtr, unsigned int id, unsigned int niter, int output_block_size, int debugPrint) ;

#endif // __UTILS_DLBF_H_
