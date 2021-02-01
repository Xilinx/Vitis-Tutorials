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

#include "test_bf_app.h"
#include "utils_linux.h"
#include "params.h"
#include "utils_dlbf.h"
#include "utils_ulbf.h"


int dlbf_func(unsigned int niter, int debug){

    int timeout_counter_dlbf_data=0;
    int timeout_counter_dlbf_coeffs=0;
    int timeout_counter_dlbf_slave=0;
    unsigned int sleepTime = 1000;
    int slave_done = 0;

    //precision of usleep is dependent on linux system call
    usleep(sleepTime);		

    //Reset, Setup and Start DLBF PL IPs FOR FUNC TEST
    dlbf_reset(debug);
    dlbf_data_config_ips(niter, debug);
    dlbf_coeffs_config_ips(niter, debug);
    dlbf_slave_config_ips(niter, debug);
    //dlbf_coeffs_write(1, niter);    //writing to dlbf_coeff ip ram
    dlbf_start(debug);
	
    // Display configuration of PL IPs
    if (debug) {
        dlbf_data_get_all_ip_status();
        dlbf_coeffs_get_all_ip_status();
        dlbf_slave_get_all_ip_status();
        dlbf_data_check_ram(1, dlbf_din0, dlbf_din1, dlbf_din2, dlbf_din3);
    	dlbf_coeffs_check_ram(1, dlbf_cin0, dlbf_cin1, dlbf_cin2, dlbf_cin3);
    }

    // Wait for all dlbf data port masters to be done for 200us
    sleepTime = 1;  // 1us 
    while ( (!dlbf_data_wait_for_done(debug)) && (timeout_counter_dlbf_data<200) ) {
        usleep(sleepTime); 
        timeout_counter_dlbf_data++;
    }
    if (timeout_counter_dlbf_data==200) {
        printf("[ERROR] Not all DLBF masters are done ... Application exitted...\n");
        log_plnx << "[ERROR] Not all DLBF masters are done ... Application exitted..." << std::endl;
        if (debug) dlbf_data_get_all_ip_status();
            return 0;     
    }

    sleepTime = 2;  // sleep for 2us

    // Wait for all dlbf coeffs port masters to be done for 400us
    while ( (!dlbf_coeffs_wait_for_done(debug)) && (timeout_counter_dlbf_coeffs<200) ) {
        usleep(sleepTime);
        timeout_counter_dlbf_coeffs++;
    }
    if (timeout_counter_dlbf_coeffs==200) {
        printf("[ERROR] Not all DLBF masters are done ... Application exitted...\n");
        log_plnx << "[ERROR] Not all DLBF masters are done ... Application exitted..." << std::endl;
        if (debug) dlbf_coeffs_get_all_ip_status();
        return 0;
    }

    sleepTime = 2;  // sleep for 2us

    // Wait for all dlbf slaves to be done for 2000us
    while ( (!dlbf_slave_wait_for_done(niter, DLBF_OUTPUT_BLOCK, debug)) && (timeout_counter_dlbf_slave<1000) ) {
        usleep(sleepTime); 
        timeout_counter_dlbf_slave++;
    }
    if (timeout_counter_dlbf_slave==1000) {
        printf("[ERROR] Not all DLBF slaves are done Application exitted...\n");
        log_plnx << "[ERROR] Not all DLBF slaves are done Application exitted..." << std::endl ;
        if (debug) dlbf_data_get_all_ip_status();
        if (debug) dlbf_coeffs_get_all_ip_status();
        if (debug) dlbf_slave_get_all_ip_status();
    }
    else {
        if (debug) log_plnx << "[DLBF-FUNC] It took " << std::dec << timeout_counter_dlbf_slave*2 << "us for all DLBFs to be done..!" << std::endl ;
        slave_done = 1;
    }
    //dlbf master IPs set niter to 1 to break the HW while loop.
    dlbf_data_config_ips(1, debug);
    dlbf_coeffs_config_ips(1, debug);
    dlbf_stop(debug); 
    return slave_done;

}

int test_dlbf(int iterFlag, int dataCheck){

    unsigned int niter_loop = 0;
    unsigned int niter_func = DLBF_NITER;
    unsigned int dlbf_done;
    int errCount[NUM_DLBF_SLAVES] = {0};
    int *goldArr[NUM_DLBF_SLAVES] = {dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0, dlbf_gold0};
    int result = 1;
    int debugPrint = !iterFlag;
    
    if (iterFlag) niter_loop = 25000; //iterFlag = 1 => niter = 100k iter = 25000 *DLBF_NITER
    else          niter_loop = 1;     //iterFlag = 0 => niter = DLBF_NITER

    //DLBF Functional test
    for(unsigned int i =0 ; i < niter_loop; i ++) {
        dlbf_done = dlbf_func(niter_func, debugPrint);
        if(dlbf_done && (dataCheck || (i == niter_loop-1))){
            for (int k=0; k<NUM_DLBF_SLAVES; k++) {
                errCount[k] = errCount[k] + dlbf_slave_read_and_verify(goldArr[k], k, niter_func, DLBF_OUTPUT_BLOCK, debugPrint);
                printf("[DLBF] Completed reading %d iterations of Slave-%d errCount:%d...\n", (i+1)*niter_func, k, errCount[k]);
                log_plnx << "[DLBF] Completed reading " << (i+1)*niter_func << " iterations of Slave-" << k << " errCount: " << errCount[k] << std::endl;
                if (errCount[k]!=0) result=0; // FAIL
            }
        }
        else if (dlbf_done==0) result=0; // FAIL
    }

    printf("[DLBF] Functional Test Completed...\n");
    log_plnx << "[DLBF] Functional Test Completed..." << std::endl ; 

    return result;

}


int ulbf_func(unsigned int niter, int debug){

    int timeout_counter_ulbf_data=0;
	int timeout_counter_ulbf_coeffs=0;
    int timeout_counter_ulbf_slave=0;
    unsigned int sleepTime = 1000;
    int slave_done = 0;

    //precision of usleep is dependent on linux system call
    usleep(sleepTime);

    //Reset, Setup and Start ULBF PL IPs FOR FUNC TEST
    ulbf_reset(debug);
    ulbf_data_config_ips(niter, debug);
    ulbf_coeffs_config_ips(niter, debug);
    ulbf_slave_config_ips(niter, debug);
    ulbf_start(debug);

    // Display configuration of PL IPs
    if (debug) {
        ulbf_data_get_all_ip_status();
        ulbf_coeffs_get_all_ip_status();
        ulbf_slave_get_all_ip_status();
    }

    // Wait for all ulbf data port masters to be done for 200us
    sleepTime = 1;  // 1us 
    while ( (!ulbf_data_wait_for_done(debug)) && (timeout_counter_ulbf_data<200) ) {
        usleep(sleepTime);
        timeout_counter_ulbf_data++;
    }
    if (timeout_counter_ulbf_data==200) {
        printf("[ERROR] Not all ULBF masters are done ... Application exitted...\n");
        log_plnx << "[ERROR] Not all ULBF masters are done ... Application exitted..." << std::endl;
        if (debug) ulbf_data_get_all_ip_status();
            return 0;
    }

    sleepTime = 1;  // sleep for 1us

    while ( (!ulbf_coeffs_wait_for_done(debug)) && (timeout_counter_ulbf_coeffs<200) ) {
        usleep(sleepTime);
        timeout_counter_ulbf_coeffs++;
    }
    if (timeout_counter_ulbf_coeffs==200) {
        printf("[ERROR] Not all ULBF coeffs are done ... Application exitted...\n");
        log_plnx << "[ERROR] Not all ULBF coeffs are done ... Application exitted..." << std::endl;
        if (debug) ulbf_data_get_all_ip_status();
        if (debug) ulbf_coeffs_get_all_ip_status();
            return 0;
    }

    sleepTime = 1;  // sleep for 1us

    // Wait for all ulbf slaves to be done for 1000us
    while ( (!ulbf_slave_wait_for_done(niter, ULBF_OUTPUT_BLOCK, debug)) && (timeout_counter_ulbf_slave<1000) ) {
        usleep(sleepTime);
        timeout_counter_ulbf_slave++;
    }
    if (timeout_counter_ulbf_slave==1000) {
        printf("[ERROR] Not all ULBF slaves are done Application exitted...\n");
        log_plnx << "[ERROR] Not all ULBF slaves are done Application exitted..." << std::endl ;
        if (debug) ulbf_data_get_all_ip_status();
        if (debug) ulbf_coeffs_get_all_ip_status();
        if (debug) ulbf_slave_get_all_ip_status();
    }
    else {
        if (debug) log_plnx << "[ULBF-FUNC] It took " << std::dec << timeout_counter_ulbf_slave*2 << "us for all ULBFs to be done..!" << std::endl ;
        slave_done = 1;
    }
    //ulbf master IP sets niter to 1 to break the HW while loop.
    ulbf_data_config_ips(1, debug);
    ulbf_coeffs_config_ips(1, debug);
    ulbf_stop(debug);
    return slave_done;

}

int test_ulbf(int iterFlag, int dataCheck){

    unsigned int niter_loop = 0;
    unsigned int niter_func = ULBF_NITER;
    unsigned int ulbf_done;
    int errCount[NUM_ULBF_SLAVES] = {0};
    int *goldArr[NUM_ULBF_SLAVES] = {ulbf_gold0, ulbf_gold0, ulbf_gold0, ulbf_gold0, ulbf_gold0, ulbf_gold0, ulbf_gold0, ulbf_gold0, ulbf_gold0, ulbf_gold0, ulbf_gold0, ulbf_gold0};
    int result = 1;
    int debugPrint = !iterFlag;

    if (iterFlag) niter_loop = 400;   //iterFlag = 1 => niter = 100k iter ~ 400 *ULBF_NITER
    else          niter_loop = 1;     //iterFlag = 0 => niter = ULBF_NITER

    //ULBF Functional test
    for(unsigned int i =0 ; i < niter_loop; i ++) {
        ulbf_done = ulbf_func(niter_func, debugPrint);
        if(ulbf_done && (dataCheck || (i == niter_loop-1))){
            for (int k=0; k<NUM_ULBF_SLAVES; k++) {
                errCount[k] = errCount[k] + ulbf_slave_read_and_verify(goldArr[k], k, niter_func, ULBF_OUTPUT_BLOCK, debugPrint);
                printf("[ULBF] Completed reading %d iterations of Slave-%d errCount:%d...\n", (i+1)*niter_func, k, errCount[k]);
                log_plnx << "[ULBF] Completed reading " << (i+1)*niter_func << " iterations of Slave-" << k << " errCount: " << errCount[k] << std::endl;
                if (errCount[k]!=0) result=0; // FAIL
            }
        }
        else  if (ulbf_done==0) result=0; // FAIL
    }

    printf("[ULBF] Functional Test Completed...\n");
    log_plnx << "[ULBF] Functional Test Completed..." << std::endl ;
  
    return result;

}

