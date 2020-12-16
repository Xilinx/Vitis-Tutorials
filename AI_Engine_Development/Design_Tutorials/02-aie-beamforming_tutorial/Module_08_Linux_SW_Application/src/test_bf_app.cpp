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

#include "xrt.h"
#include "experimental/xrt_aie.h"
#include "xrt/experimental/xrt_kernel.h"

#include "adf/adf_api/XRTConfig.h"


static std::vector<char>
 load_xclbin(xrtDeviceHandle device, const std::string& fnm)
 {
   if (fnm.empty())
     throw std::runtime_error("No xclbin speified");

   //load bit stream
   std::ifstream stream(fnm);
   stream.seekg(0,stream.end);
   size_t size = stream.tellg();
   stream.seekg(0,stream.beg);

   std::vector<char> header(size);
   stream.read(header.data(),size);

   auto top = reinterpret_cast<const axlf*>(header.data());
   char *xcb = (char*)top;
   printf("xcb is %s\n", xcb);
   //if (xclLoadXclBin(device, top))
   if (xrtDeviceLoadXclbin(device, top))
     throw std::runtime_error("Bitstream download failed");
   return header;
}

int main(int argc, char ** argv) {

    std::cout << "[main] Output log directory: ./"  << std::endl ;
    logPlnx << "./beamformer_hw.log" ;
    logPlnx_str = logPlnx.str();
    std::cout << "[main] Petalinux Output log file : " << logPlnx_str << std::endl ;
    log_plnx.open (logPlnx_str);

    int iterFlag, check;
    int arg1;
    int func = 0;
    int perf = 0;
    int flag_all;
    int perf_sample;

    int reset_done = -1;

    arg1 = strtol(argv[1], nullptr, 0);    
    if (arg1 == 0){          //FUNC TEST, PERF TEST
        func = 1;       perf = 1;
        iterFlag = 0;   check = 0;
        flag_all = 0;   perf_sample =  MAX_TP_MEASUREMENT_SAMPLES;
    } else if (arg1 ==1){    //FUNC TEST, PERF TEST
        func = 1;        perf = 1;
	iterFlag = 1;    check = 1; // 100K iters, data check every N iters
	flag_all = 1;    perf_sample =  MAX_TP_MEASUREMENT_SAMPLES;   // All PLIO PERF TEST
    } else if (arg1 == 2){   // All PLIO PERF TEST every 1-2mins
        perf = 1;
        flag_all = 0;    perf_sample =  100;
    }

    printf("[Beamformer] Entered argument is  %d\n", arg1);
    log_plnx << "[Beamformer] Entered argument is " << arg1 << std::endl;

    printf("[main]  Beamformer Init... \n");
    log_plnx << "[main]  Beamformer Init... " << std::endl;
				    
    log_plnx << "[DLBF] dlbf get_addr status: "<< dlbf_assign_addr() << std::endl;
    log_plnx << "[ULBF] ulbf get_addr status: "<< ulbf_assign_addr() << std::endl;

    auto dhdl   = xrtDeviceOpen(0);
    auto xclbin = load_xclbin(dhdl, argv[2]);
    auto top = reinterpret_cast<const axlf*>(xclbin.data());
    adf::registerXRT(dhdl, top->m_header.uuid);

    // Loading xclbin resets and loads AIE array. 
    // Another reset prior to load xclbin is not needed  
    // Unlike 2020.1, reset in 2020.2 needs device handle and hence 
    // xclbin_load needs to be done first 

    printf("[main]  Beamformer Run... \n");
    log_plnx << "[main]  Beamformer Run... " << std::endl;

    int dlbf_resulta = 1;
    int dlbf_resultb = 1;
    int ulbf_resulta = 1;
    int ulbf_resultb = 1;

    int fail = 1;

    if (func){
        dlbf_resulta = test_dlbf(iterFlag, check);
        ulbf_resulta = test_ulbf(iterFlag, check);
        
        if (!iterFlag) {
            // Reset AIE - Reload AIE, run  Beamformer func again
            reset_done = xrtResetAIEArray(dhdl);
            if (reset_done == -1){
                printf("[Beamformer] AIE reset FAILS \n");
                log_plnx << "[Beamformer] AIE reset FAILS" <<std::endl;
                printf("[Beamformer] Exiting the application as it cannot be run\n");
                log_plnx << "[Beamformer] Exiting the application as it cannot be run" <<std::endl;
                return 1;
            } else if(reset_done == 0) {
                printf("[Beamformer] AIE reset done successfully\n");
                log_plnx << "[Beamformer] AIE reset done successfully" << std::endl;
            }
            xclbin = load_xclbin(dhdl, argv[2]);
	    
            dlbf_resultb = test_dlbf(iterFlag, check);
            ulbf_resultb = test_ulbf(iterFlag, check);
        }

        if ((dlbf_resulta==0)||(dlbf_resultb==0)||(ulbf_resulta==0)||(ulbf_resultb==0)){
            fail = 1;
            printf("[Beamformer-FUNC] - ***** TEST FAILED ***** \n");
            log_plnx << "[Beamformer-FUNC] - ***** TEST FAILED ***** " << std::endl;	
            printf("[Beamformer-FUNC] DLBF status :%d\n", dlbf_resulta);
            log_plnx << "[Beamformer-FUNC] DLBF status :"<< dlbf_resulta << std::endl;
            printf("[Beamformer-FUNC] ULBF status :%d\n", ulbf_resulta);
            log_plnx << "[Beamformer-FUNC] ULBF status :"<< ulbf_resulta << std::endl;
        } else {			
            fail = 0;
            printf("[Beamformer-FUNC] - ***** TEST PASSED ***** \n");
            log_plnx << "[Beamformer-FUNC] - ***** TEST PASSED ***** " << std::endl;
        }
    }

    int fail_p = 0;
    if (perf){
        // Reset AIE - Reload AIE, run  Beamformer func again
        reset_done = xrtResetAIEArray(dhdl);
        if (reset_done == -1){
            printf("[Beamformer] AIE reset FAILS \n");
            log_plnx << "[Beamformer] AIE reset FAILS" <<std::endl;
            printf("[Beamformer] Exiting the application as it cannot be run\n");
            log_plnx << "[Beamformer] Exiting the application as it cannot be run" <<std::endl;
            return 1;
        } else if(reset_done == 0) {
            printf("[Beamformer] AIE reset done successfully\n");
            log_plnx << "[Beamformer] AIE reset done successfully" << std::endl;
        }
        xclbin = load_xclbin(dhdl, argv[2]);

        enable_graphs(0, 0);  //niter is set to 0 for infinite loop
        test_perf_all(flag_all, perf_sample);   
        if (arg1 == 2) {
            while(1)   test_perf_all(flag_all, perf_sample);
        }
        stop_run();
    }

    printf("[main] Beamformer End... \n");
    log_plnx << "[main] Beamformer End... " << std::endl;

    return fail|fail_p ;
}

