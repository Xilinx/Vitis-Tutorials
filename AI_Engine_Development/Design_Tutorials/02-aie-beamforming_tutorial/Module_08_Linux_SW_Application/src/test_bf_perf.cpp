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

#include "test_bf_3x.cpp"

#include "test_bf_app.h"
#include "utils_linux.h"
#include "params.h"
#include "utils_dlbf.h"
#include "utils_ulbf.h"


adf::PLIO dlbf_id[NUM_DLBF_DATA*NUM_DLBF_PORTS] = {*attr_i_dlbfid0, *attr_i_dlbfid1,  *attr_i_dlbfid2, *attr_i_dlbfid3, *attr_i_dlbfid_a0,  *attr_i_dlbfid_a1,  *attr_i_dlbfid_a2, *attr_i_dlbfid_a3, 	*attr_i_dlbfid_b0, *attr_i_dlbfid_b1,  *attr_i_dlbfid_b2, *attr_i_dlbfid_b3};

adf::PLIO dlbf_ic[NUM_DLBF_COEFFS*NUM_DLBF_PORTS] = {*attr_i_dlbfic00,  *attr_i_dlbfic01, *attr_i_dlbfic02,  *attr_i_dlbfic03, *attr_i_dlbfic10,  *attr_i_dlbfic11, *attr_i_dlbfic12,  *attr_i_dlbfic13, *attr_i_dlbfic20,  *attr_i_dlbfic21, *attr_i_dlbfic22,  *attr_i_dlbfic23, *attr_i_dlbfic30,  *attr_i_dlbfic31, *attr_i_dlbfic32,  *attr_i_dlbfic33, *attr_i_dlbfic40,  *attr_i_dlbfic41, *attr_i_dlbfic42,  *attr_i_dlbfic43, *attr_i_dlbfic50,  *attr_i_dlbfic51, *attr_i_dlbfic52,  *attr_i_dlbfic53, *attr_i_dlbfic60,  *attr_i_dlbfic61, *attr_i_dlbfic62,  *attr_i_dlbfic63, *attr_i_dlbfic70,  *attr_i_dlbfic71, *attr_i_dlbfic72,  *attr_i_dlbfic73, *attr_i_dlbfic_a00,  *attr_i_dlbfic_a01, *attr_i_dlbfic_a02,  *attr_i_dlbfic_a03, *attr_i_dlbfic_a10,  *attr_i_dlbfic_a11, *attr_i_dlbfic_a12,  *attr_i_dlbfic_a13, *attr_i_dlbfic_a20,  *attr_i_dlbfic_a21, *attr_i_dlbfic_a22,  *attr_i_dlbfic_a23, *attr_i_dlbfic_a30,  *attr_i_dlbfic_a31, *attr_i_dlbfic_a32,  *attr_i_dlbfic_a33, *attr_i_dlbfic_a40,  *attr_i_dlbfic_a41, *attr_i_dlbfic_a42,  *attr_i_dlbfic_a43, *attr_i_dlbfic_a50,  *attr_i_dlbfic_a51, *attr_i_dlbfic_a52,  *attr_i_dlbfic_a53, *attr_i_dlbfic_a60,  *attr_i_dlbfic_a61, *attr_i_dlbfic_a62,  *attr_i_dlbfic_a63, *attr_i_dlbfic_a70,  *attr_i_dlbfic_a71, *attr_i_dlbfic_a72,  *attr_i_dlbfic_a73, *attr_i_dlbfic_b00,  *attr_i_dlbfic_b01, *attr_i_dlbfic_b02,  *attr_i_dlbfic_b03, *attr_i_dlbfic_b10,  *attr_i_dlbfic_b11, *attr_i_dlbfic_b12,  *attr_i_dlbfic_b13, *attr_i_dlbfic_b20,  *attr_i_dlbfic_b21, *attr_i_dlbfic_b22,  *attr_i_dlbfic_b23, *attr_i_dlbfic_b30,  *attr_i_dlbfic_b31, *attr_i_dlbfic_b32,  *attr_i_dlbfic_b33, *attr_i_dlbfic_b40,  *attr_i_dlbfic_b41, *attr_i_dlbfic_b42,  *attr_i_dlbfic_b43, *attr_i_dlbfic_b50,  *attr_i_dlbfic_b51, *attr_i_dlbfic_b52,  *attr_i_dlbfic_b53, *attr_i_dlbfic_b60,  *attr_i_dlbfic_b61, *attr_i_dlbfic_b62,  *attr_i_dlbfic_b63, *attr_i_dlbfic_b70,  *attr_i_dlbfic_b71, *attr_i_dlbfic_b72,  *attr_i_dlbfic_b73};


adf::PLIO dlbf_out[NUM_DLBF_SLAVES] = {*attr_o_dlbfo0,  *attr_o_dlbfo1, *attr_o_dlbfo2,  *attr_o_dlbfo3, *attr_o_dlbfo4,  *attr_o_dlbfo5, *attr_o_dlbfo6,  *attr_o_dlbfo7, 	*attr_o_dlbfo_a0,  *attr_o_dlbfo_a1, *attr_o_dlbfo_a2,  *attr_o_dlbfo_a3, *attr_o_dlbfo_a4,  *attr_o_dlbfo_a5, *attr_o_dlbfo_a6,  *attr_o_dlbfo_a7, *attr_o_dlbfo_b0,  *attr_o_dlbfo_b1, *attr_o_dlbfo_b2,  *attr_o_dlbfo_b3, *attr_o_dlbfo_b4,  *attr_o_dlbfo_b5, *attr_o_dlbfo_b6,  *attr_o_dlbfo_b7};

adf::PLIO ulbf_id[NUM_ULBF_DATA*NUM_ULBF_PORTS] = {*attr_i_ulbfid0, *attr_i_ulbfid1,  *attr_i_ulbfid2, *attr_i_ulbfid3, *attr_i_ulbfid4, *attr_i_ulbfid5,  *attr_i_ulbfid6, *attr_i_ulbfid7, *attr_i_ulbfid_a0, *attr_i_ulbfid_a1,  *attr_i_ulbfid_a2, *attr_i_ulbfid_a3, *attr_i_ulbfid_a4, *attr_i_ulbfid_a5,  *attr_i_ulbfid_a6, *attr_i_ulbfid_a7, *attr_i_ulbfid_b0, *attr_i_ulbfid_b1,  *attr_i_ulbfid_b2, *attr_i_ulbfid_b3, *attr_i_ulbfid_b4, *attr_i_ulbfid_b5,  *attr_i_ulbfid_b6, *attr_i_ulbfid_b7};

adf::PLIO ulbf_ic[NUM_ULBF_COEFFS*NUM_ULBF_PORTS] = {*attr_i_ulbfic00,  *attr_i_ulbfic01, *attr_i_ulbfic02,  *attr_i_ulbfic03, *attr_i_ulbfic04,  *attr_i_ulbfic05, *attr_i_ulbfic06,  *attr_i_ulbfic07, *attr_i_ulbfic10,  *attr_i_ulbfic11, *attr_i_ulbfic12,  *attr_i_ulbfic13, *attr_i_ulbfic14,  *attr_i_ulbfic15, *attr_i_ulbfic16,  *attr_i_ulbfic17, *attr_i_ulbfic20,  *attr_i_ulbfic21, *attr_i_ulbfic22,  *attr_i_ulbfic23, *attr_i_ulbfic24,  *attr_i_ulbfic25, *attr_i_ulbfic26,  *attr_i_ulbfic27, *attr_i_ulbfic30,  *attr_i_ulbfic31, *attr_i_ulbfic32,  *attr_i_ulbfic33, *attr_i_ulbfic34,  *attr_i_ulbfic35, *attr_i_ulbfic36,  *attr_i_ulbfic37, *attr_i_ulbfic_a00,  *attr_i_ulbfic_a01, *attr_i_ulbfic_a02,  *attr_i_ulbfic_a03, *attr_i_ulbfic_a04,  *attr_i_ulbfic_a05, *attr_i_ulbfic_a06,  *attr_i_ulbfic_a07, *attr_i_ulbfic_a10,  *attr_i_ulbfic_a11, *attr_i_ulbfic_a12,  *attr_i_ulbfic_a13, *attr_i_ulbfic_a14,  *attr_i_ulbfic_a15, *attr_i_ulbfic_a16,  *attr_i_ulbfic_a17, *attr_i_ulbfic_a20,  *attr_i_ulbfic_a21, *attr_i_ulbfic_a22,  *attr_i_ulbfic_a23, *attr_i_ulbfic_a24,  *attr_i_ulbfic_a25, *attr_i_ulbfic_a26,  *attr_i_ulbfic_a27, *attr_i_ulbfic_a30,  *attr_i_ulbfic_a31, *attr_i_ulbfic_a32,  *attr_i_ulbfic_a33, *attr_i_ulbfic_a34,  *attr_i_ulbfic_a35, *attr_i_ulbfic_a36,  *attr_i_ulbfic_a37, *attr_i_ulbfic_b00,  *attr_i_ulbfic_b01, *attr_i_ulbfic_b02,  *attr_i_ulbfic_b03, *attr_i_ulbfic_b04,  *attr_i_ulbfic_b05, *attr_i_ulbfic_b06,  *attr_i_ulbfic_b07, *attr_i_ulbfic_b10,  *attr_i_ulbfic_b11, *attr_i_ulbfic_b12,  *attr_i_ulbfic_b13, *attr_i_ulbfic_b14,  *attr_i_ulbfic_b15, *attr_i_ulbfic_b16,  *attr_i_ulbfic_b17, *attr_i_ulbfic_b20,  *attr_i_ulbfic_b21, *attr_i_ulbfic_b22,  *attr_i_ulbfic_b23, *attr_i_ulbfic_b24,  *attr_i_ulbfic_b25, *attr_i_ulbfic_b26,  *attr_i_ulbfic_b27, *attr_i_ulbfic_b30,  *attr_i_ulbfic_b31, *attr_i_ulbfic_b32,  *attr_i_ulbfic_b33, *attr_i_ulbfic_b34,  *attr_i_ulbfic_b35, *attr_i_ulbfic_b36,  *attr_i_ulbfic_b37};

adf::PLIO ulbf_out[NUM_ULBF_SLAVES] = {*attr_o_ulbfo0,  *attr_o_ulbfo1, *attr_o_ulbfo2,  *attr_o_ulbfo3, *attr_o_ulbfo_a0,  *attr_o_ulbfo_a1, *attr_o_ulbfo_a2,  *attr_o_ulbfo_a3, *attr_o_ulbfo_b0,  *attr_o_ulbfo_b1, *attr_o_ulbfo_b2,  *attr_o_ulbfo_b3};

int test_perf(std::string str, int samples, adf::PLIO inp, int d, long long throughput_exp){

    adf::event::handle handle_in_0;
	int flag = 0;
	long long throughput_min = 900000000; // initial value to some high number (>800000000) as 800MSps is expected throughput of inputs and outputs
        long long throughput_max = 0;
        int iter=0;
        while(iter < samples) {
            long long count_start_in_0, count_end_in_0;
            long long throughput;
            handle_in_0  = adf::event::start_profiling(inp,     adf::event::io_stream_running_event_count);
            count_start_in_0   = adf::event::read_profiling(handle_in_0);
            //precision of usleep is dependent on linux system call
            usleep(1000000); //1s
            count_end_in_0     = adf::event::read_profiling(handle_in_0);
            adf::event::stop_profiling(handle_in_0);
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

int dlbf_perf(int samples, int flag_all, int mid, int mic, int sid){

    unsigned int sleepTime = 1000;
    usleep(sleepTime);

    long long throughput_out_exp = 800000000;
    int flag = 0;

	if (!flag_all){
		test_perf("DLBF-data", samples, dlbf_id[mid], mid, 0);
		test_perf("DLBF-coeffs", samples, dlbf_ic[mic], mic, 0);
		flag = test_perf("DLBF-slaves", samples, dlbf_out[sid], sid, throughput_out_exp);
	} else {
		for(int d =0; d < NUM_DLBF_DATA; d++)
			test_perf("DLBF-data", samples, dlbf_id[d], d, 0);
		for(int d =0; d < NUM_DLBF_COEFFS; d++)
			test_perf("DLBF-coeffs", samples, dlbf_ic[d], d, 0);
		for(int d =0; d < NUM_DLBF_SLAVES; d++)
			flag = flag + test_perf("DLBF-slaves", samples, dlbf_out[d], d, throughput_out_exp);
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

int ulbf_perf(int samples, int flag_all, int mid, int mic, int sid){

    unsigned int sleepTime = 1000;
    usleep(sleepTime);

    long long throughput_out_exp = 800000000;
    int flag = 0;

	if (!flag_all){
		test_perf("ULBF-data", samples, ulbf_id[mid], mid, 0);
		test_perf("ULBF-coeffs", samples, ulbf_ic[mic], mic, 0);
		flag = test_perf("ULBF-slaves", samples, ulbf_out[sid], sid, throughput_out_exp);
	} else {
		for(int d =0; d < NUM_ULBF_DATA; d++)
			test_perf("ULBF-data", samples, ulbf_id[d], d, 0);
		for(int d =0; d < NUM_ULBF_COEFFS; d++)
			test_perf("ULBF-coeffs", samples, ulbf_ic[d], d, 0);
		for(int d =0; d < NUM_ULBF_SLAVES; d++)
			flag = flag + test_perf("ULBF-slaves", samples, ulbf_out[d], d, throughput_out_exp);
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

void test_perf_all(int flag_all, int samples){

    int dlbf_result = 1;
    int ulbf_result = 1;

    int dlbf_mid = rand()% NUM_DLBF_DATA;
    int dlbf_mic = rand()% NUM_DLBF_COEFFS;
    int dlbf_sid = rand()% NUM_DLBF_SLAVES;

    //DLBF Performance test
    dlbf_result = dlbf_perf(samples, flag_all, dlbf_mid, dlbf_mic, dlbf_sid);
    printf("[DLBF-PERF] DLBF Performance measurements Done ... \n");
    log_plnx << "[DLBF-PERF] DLBF Performance measurements Done ... " << std::endl ;

    int ulbf_mid = rand()% NUM_ULBF_DATA;
    int ulbf_mic = rand()% NUM_ULBF_COEFFS;
    int ulbf_sid = rand()% NUM_ULBF_SLAVES;

    //ULBF Performance test
    ulbf_result = ulbf_perf(samples, flag_all, ulbf_mid, ulbf_mic, ulbf_sid);
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


