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


#include "baremetal_src/utils.h"
#include "baremetal_src/params.h"
#include "baremetal_src/utils_ulbf.h"
#include "baremetal_src/utils_dlbf.h"

#define TIMEOUT 100

int test_ulbf(int debugPrint) {
	printf("\n[ULBF] Start ...\n");
	unsigned long sleepTime = 5000;  // 5us 
	int timeout_counter_ulbf_data=0;
	int timeout_counter_ulbf_coeffs=0;
	int timeout_counter_ulbf_slave=0;
	int niter = ULBF_NITER;
	ulbf_reset();
	
	ulbf_data_config_ips(niter);
	ulbf_coeffs_config_ips(niter);
	ulbf_slave_config_ips(niter);

	if (debugPrint) {
		ulbf_data_check_ram(0, ulbf_din0, ulbf_din1, ulbf_din2, ulbf_din3, ulbf_din4, ulbf_din5, ulbf_din6, ulbf_din7);
		ulbf_coeffs_check_ram(0, ulbf_cin0, ulbf_cin1, ulbf_cin2, ulbf_cin3, ulbf_cin4, ulbf_cin5, ulbf_cin6, ulbf_cin7);
	}

	ulbf_start();

	// Wait for all ulbf data port masters to be done ...
	sleepTime = 1;  // 1us 
	while ( (!ulbf_data_wait_for_done()) && (timeout_counter_ulbf_data<TIMEOUT) ) {
		usleep(sleepTime); 
		timeout_counter_ulbf_data++;
	}
	if (timeout_counter_ulbf_data==TIMEOUT) {
		if (debugPrint) ulbf_data_get_all_ip_status();
		std::cout << "[ERROR] Not all ULBF Data masters done... Application exit...!!!" << std::endl;
		return 0;       
	}
	// Wait for all ulbf coeffs port masters to be done ...
	sleepTime = 1;  // 1us 
	while ( (!ulbf_coeffs_wait_for_done()) && (timeout_counter_ulbf_coeffs<TIMEOUT) ) {
		usleep(sleepTime); 
		timeout_counter_ulbf_coeffs++;
	}
	if (timeout_counter_ulbf_coeffs==TIMEOUT) {
        if (debugPrint) ulbf_data_get_all_ip_status();
		if (debugPrint) ulbf_coeffs_get_all_ip_status();
		std::cout << "[ERROR] Not all ULBF Coeffs masters done... Application exit...!!!" << std::endl;
		return 0;       
	}
	// Wait for all ulbf slaves to be done ...
	while ( (!ulbf_slave_wait_for_done(niter, ULBF_OUTPUT_BLOCK)) && (timeout_counter_ulbf_slave<TIMEOUT) ) {
		usleep(sleepTime); 
		timeout_counter_ulbf_slave++;
	}
	if (timeout_counter_ulbf_slave==TIMEOUT) {
		printf("[ERROR] Not all ULBF slaves are done ... Application exit...!!\n");
		if (debugPrint) ulbf_data_get_all_ip_status();
		if (debugPrint) ulbf_coeffs_get_all_ip_status();
		if (debugPrint) ulbf_slave_get_all_ip_status();
		return 0;	
	}
	else {
		printf("[ULBF] It took %dus for all ULBF Slaves to be done..!\n", timeout_counter_ulbf_slave);
		int result=1;
		int iter = niter;
		int output_block_size = ULBF_OUTPUT_BLOCK;
		int errCount[NUM_ULBF_SLAVES] = {0};
		int *goldArr[NUM_ULBF_SLAVES] = {ulbf_gold0, ulbf_gold0, ulbf_gold0, ulbf_gold0, ulbf_gold0, ulbf_gold0, ulbf_gold0, ulbf_gold0, ulbf_gold0, ulbf_gold0, ulbf_gold0, ulbf_gold0};
		for (int k=0; k<NUM_ULBF_SLAVES; k++) {
			errCount[k] = ulbf_slave_read_and_verify(goldArr[k], k, iter, output_block_size, debugPrint);
			printf("[ULBF] Read first %d iterations of Slave-%d errCount:%d...\n", iter, k, errCount[k]);
			if (errCount[k]>0) result=0; // FAIL
		}
		return result;
	}
}

int test_dlbf(int debugPrint) {
	printf("\n[DLBF] Start ...\n");
	unsigned long sleepTime = 5000;  // 5us 
	int timeout_counter_dlbf_data=0;
	int timeout_counter_dlbf_coeffs=0;
	int timeout_counter_dlbf_slave=0;
	int niter = DLBF_NITER;
	dlbf_reset();

	dlbf_data_config_ips(niter);
	dlbf_coeffs_config_ips(niter);
	dlbf_slave_config_ips(niter);

	if (debugPrint) {
		dlbf_data_check_ram(0, dlbf_din0, dlbf_din1, dlbf_din2, dlbf_din3);
		dlbf_coeffs_check_ram(0, dlbf_cin0, dlbf_cin1, dlbf_cin2, dlbf_cin3);
	}

	dlbf_start();

	// Wait for all dlbf data port masters to be done ...
	sleepTime = 1;  // 1us 
	while ( (!dlbf_data_wait_for_done()) && (timeout_counter_dlbf_data<TIMEOUT) ) {
		usleep(sleepTime); 
		timeout_counter_dlbf_data++;
	}
	if (timeout_counter_dlbf_data==TIMEOUT) {
		if (debugPrint) dlbf_data_get_all_ip_status();
		if (debugPrint) dlbf_coeffs_get_all_ip_status();
		std::cout << "[ERROR] Not all DLBF Data masters done... Application exit...!!!" << std::endl;
		return 0;       
	}
	// Wait for all dlbf coeffs port masters to be done ...
	sleepTime = 1;  // 1us 
	while ( (!dlbf_coeffs_wait_for_done()) && (timeout_counter_dlbf_coeffs<TIMEOUT) ) {
		usleep(sleepTime); 
		timeout_counter_dlbf_coeffs++;
	}
	if (timeout_counter_dlbf_coeffs==TIMEOUT) {
		if (debugPrint) dlbf_data_get_all_ip_status();
		if (debugPrint) dlbf_coeffs_get_all_ip_status();
		printf("[ERROR] Not all DLBF COEFFS masters done... Application exit...!!\n");
		return 0;       
	}
	// Wait for all dlbf slaves to be done ...
	while ( (!dlbf_slave_wait_for_done(niter, DLBF_OUTPUT_BLOCK)) && (timeout_counter_dlbf_slave<TIMEOUT) ) {
		usleep(sleepTime); 
		timeout_counter_dlbf_slave++;
	}
	if (timeout_counter_dlbf_slave==TIMEOUT) {
		printf("[ERROR] Not all DLBF slaves are done ... Application exit...!!\n");
		if (debugPrint) dlbf_data_get_all_ip_status();
		if (debugPrint) dlbf_coeffs_get_all_ip_status();
		if (debugPrint) dlbf_slave_get_all_ip_status();
		return 0;	
	}
	else {
		printf("[DLBF] It took %dus for all DLBF Slaves to be done..!\n", timeout_counter_dlbf_slave);
		int result=1;
		int iter = niter;
		int output_block_size = DLBF_OUTPUT_BLOCK;
		int errCount[NUM_DLBF_SLAVES] = {0};
		int *goldArr[NUM_DLBF_SLAVES] = {dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0};
		for (int k=0; k<NUM_DLBF_SLAVES; k++) {
			errCount[k] = dlbf_slave_read_and_verify(goldArr[k], k, iter, output_block_size, debugPrint);
			printf("[DLBF] Read first %d iterations of Slave-%d errCount:%d...\n", iter, k, errCount[k]);
			if (errCount[k]>0) result=0; // FAIL
		}
		return result;
	}
}

int main()
{
	printf(" ***** BEGIN TEST ***** \n");	
	Xil_DCacheDisable();

	unsigned long sleepTime = 5000;  // 5us 
	usleep(sleepTime); 
	int debugPrint=1;

	int dlbf_result = 1;
	int ulbf_result = 1;

	dlbf_result = test_dlbf(debugPrint);
	ulbf_result = test_ulbf(debugPrint);
	
	if ((ulbf_result==0)||(dlbf_result==0))   
		printf("[DLBF+ULBF] - ***** TEST FAILED ***** \n");	
	else				
		printf("[DLBF+ULBF] - ***** TEST PASSED ***** \n");
	printf(" ***** END TEST ***** \n");	
	return 0;
}   


