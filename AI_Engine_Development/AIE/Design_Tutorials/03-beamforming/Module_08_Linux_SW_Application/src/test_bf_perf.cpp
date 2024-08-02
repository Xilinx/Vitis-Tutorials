
/*
   Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
   SPDX-License-Identifier: MIT
   */

#include "adf.h"

#include "test_bf_app.h"
#include "utils_linux.h"
#include "params.h"
#include "utils_dlbf.h"
#include "utils_ulbf.h"
#include "xrt/xrt_kernel.h"
#include "xrt/xrt_device.h"
#include "xrt/xrt_aie.h"
#include "xrt/experimental/xrt_ip.h"


int test_perf(std::string str, int samples, std::string inp, int d, long long throughput_exp,xrt::device &device_name ){

    auto event = xrt::aie::profiling(device_name);
    int flag = 0;
    long long throughput_min = 900000000; // initial value to some high number (>800000000) as 800MSps is expected throughput of inputs and outputs
    long long throughput_max = 0;
    int iter=0;
    while(iter < samples) {
        long long count_start_in_0, count_end_in_0;
        long long throughput;
        auto handle = event.start(xrt::aie::profiling::profiling_option::io_stream_running_event_count, inp,"",0);
        count_start_in_0   = event.read();
        //precision of usleep is dependent on linux system call
        usleep(1000000); //1s
        count_end_in_0     = event.read();
        event.stop();
        if (count_end_in_0 > count_start_in_0)    throughput = (count_end_in_0-count_start_in_0);
        else                                      throughput = (count_end_in_0-count_start_in_0+0x100000000);	//roll over correction for 32b performance counter
        if (throughput<throughput_min)  throughput_min = throughput;
        if (throughput>throughput_max)  throughput_max = throughput;
        iter++;
    }
    std::cout << "[throughput] "<<str<<" " << std::dec << d << "   Min:" << throughput_min << "   Max:" << throughput_max << "    Range:" << throughput_max-throughput_min << std::endl;
    log_plnx << "[throughput] "<<str<<" " << std::dec << d << "   Min:" << throughput_min << "   Max:" << throughput_max << "    Range:" << throughput_max-throughput_min << std::endl;
    if ((throughput_max < throughput_exp)||((throughput_min == 900000000)&&((throughput_max-throughput_min) > 60000))) flag = 1;

    return flag;
}

int dlbf_perf(int samples, int flag_all, int mid, int mic, int sid, xrt::device &device_name){

    unsigned int sleepTime = 1000;
    usleep(sleepTime);

    long long throughput_out_exp = 800000000;
    int flag = 0;

    if (!flag_all){
        test_perf("DLBF-data", samples, "dut.dldin["+std::to_string(mid)+"]", mid, 0, device_name);
        test_perf("DLBF-coeffs", samples, "dut.dlcin["+std::to_string(mic)+"]", mic, 0, device_name);
        flag = test_perf("DLBF-slaves", samples, "dut.dlout["+std::to_string(sid)+"]", sid, throughput_out_exp, device_name);
    } else {
        for(int d =0; d < NUM_DLBF_DATA; d++)
            test_perf("DLBF-data", samples, "dut.dldin["+std::to_string(d)+"]", d, 0, device_name);
        for(int d =0; d < NUM_DLBF_COEFFS; d++)
            test_perf("DLBF-coeffs", samples, "dut.dlcin["+std::to_string(d)+"]", d, 0, device_name);
        for(int d =0; d < NUM_DLBF_SLAVES; d++)
            flag = flag + test_perf("DLBF-slaves", samples, "dut.dlout["+std::to_string(d)+"]", d, throughput_out_exp, device_name);
    }

    if (flag>0){
        printf("[DLBF-PERF] PERFORMANCE TEST FAILED\n");
        log_plnx << "[DLBF-PERF] PERFORMANCE TEST FAILED" << std::endl;
    } else {
        printf("[DLBF-PERF] PERFORMANCE TEST PASSED\n");
        log_plnx << "DLBF-PERF] PERFORMANCE TEST PASSED" << std::endl;
    } 
    return !flag;
}

int ulbf_perf(int samples, int flag_all, int mid, int mic, int sid, xrt::device &device_name){

    unsigned int sleepTime = 1000;
    usleep(sleepTime);

    long long throughput_out_exp = 800000000;
    int flag = 0;

    if (!flag_all){
        test_perf("ULBF-data", samples, "dut.uldin["+std::to_string(mid)+"]", mid, 0, device_name);
        test_perf("ULBF-coeffs", samples, "dut.ulcin["+std::to_string(mic)+"]", mic, 0, device_name);
        flag = test_perf("ULBF-slaves", samples, "dut.ulout["+std::to_string(sid)+"]", sid, throughput_out_exp, device_name);
    } else {
        for(int d =0; d < NUM_ULBF_DATA; d++)
            test_perf("ULBF-data", samples, "dut.uldin["+std::to_string(d)+"]", d, 0, device_name);
        for(int d =0; d < NUM_ULBF_COEFFS; d++)
            test_perf("ULBF-coeffs", samples, "dut.ulcin["+std::to_string(d)+"]", d, 0, device_name);
        for(int d =0; d < NUM_ULBF_SLAVES; d++)
            flag = flag + test_perf("ULBF-slaves", samples, "dut.ulout["+std::to_string(d)+"]", d, throughput_out_exp, device_name);
    }

    if (flag>0){
        printf("[ULBF-PERF] PERFORMANCE TEST FAILED\n");
        log_plnx << "[ULBF-PERF] PERFORMANCE TEST FAILED" << std::endl;
    } else {
        printf("[ULBF-PERF] PERFORMANCE TEST PASSED\n");
        log_plnx << "[ULBF-PERF] PERFORMANCE TEST PASSED" << std::endl;
    } 
    return !flag;
}


void enable_graphs(int niter, int debug){

    unsigned int sleepTime = 1000;
    usleep(sleepTime);

    printf("[Beamformer-PERF] Restarting masters and slaves for Performace Test\n");
    log_plnx << "[Beamformer-PERF] Restarting masters and slaves for Performace Test"  << std::endl ;

    dlbf_reset(debug);    ulbf_reset(debug);
    dlbf_data_config_ips(niter, debug);    dlbf_coeffs_config_ips(niter, debug);    dlbf_slave_config_ips(niter, debug);
    ulbf_data_config_ips(niter, debug);    ulbf_coeffs_config_ips(niter, debug);    ulbf_slave_config_ips(niter, debug);
    dlbf_start(debug);    ulbf_start(debug);

}

void test_perf_all(int flag_all, int samples, xrt::device &device_name){

    int dlbf_result = 1;
    int ulbf_result = 1;

    int dlbf_mid = rand()% NUM_DLBF_DATA;
    int dlbf_mic = rand()% NUM_DLBF_COEFFS;
    int dlbf_sid = rand()% NUM_DLBF_SLAVES;

    //DLBF Performance test
    dlbf_result = dlbf_perf(samples, flag_all, dlbf_mid, dlbf_mic, dlbf_sid, device_name);
    printf("[DLBF-PERF] DLBF Performance measurements Done ... \n");
    log_plnx << "[DLBF-PERF] DLBF Performance measurements Done ... " << std::endl ;

    int ulbf_mid = rand()% NUM_ULBF_DATA;
    int ulbf_mic = rand()% NUM_ULBF_COEFFS;
    int ulbf_sid = rand()% NUM_ULBF_SLAVES;

    //ULBF Performance test
    ulbf_result = ulbf_perf(samples, flag_all, ulbf_mid, ulbf_mic, ulbf_sid, device_name);
    printf("[ULBF-PERF] ULBF Performance measurements Done ... \n");
    log_plnx << "[ULBF-PERF] ULBF Performance measurements Done ... " << std::endl ;


    if ((dlbf_result==0)||(ulbf_result==0)){
        printf("[Beamformer-PERF] - ***** TEST FAILED ***** \n");
        log_plnx << "[Beamformer-PERF] - ***** TEST FAILED ***** " << std::endl;
    } else {
        printf("[Beamformer-PERF] - ***** TEST PASSED ***** \n");
        log_plnx << "[Beamformer-PERF] - ***** TEST PASSED ***** " << std::endl;
    }
}

void stop_run(){

    int debug = 0;
    int niter = 1;

    dlbf_data_config_ips(niter, debug);
    dlbf_coeffs_config_ips(niter, debug);
    dlbf_stop(debug);
    ulbf_data_config_ips(niter, debug);
    ulbf_coeffs_config_ips(niter, debug);
    ulbf_stop(debug);

}


