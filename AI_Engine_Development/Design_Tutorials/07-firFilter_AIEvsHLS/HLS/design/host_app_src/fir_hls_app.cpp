/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
//#include <complex>
//
//typedef std::complex<int16_t>  cint16;
//
//#include "system_settings.h"
//#include "input_data.h"
//
//#if (N_FIR_FILTERS == 1) && (N_FIR_TAPS == 15)
//#include "golden_data_1f_15t.h"
//#elif (N_FIR_FILTERS == 10) && (N_FIR_TAPS == 15)
//#include "golden_data_10f_15t.h"
//#elif (N_FIR_FILTERS == 1) && (N_FIR_TAPS == 240)
//#include "golden_data_1f_240t.h"
//#elif (N_FIR_FILTERS == 10) && (N_FIR_TAPS == 240)
//#include "golden_data_10f_240t.h"
//#else
//#include "golden_data_1f_15t.h"
//#endif
//
//#include "adf/adf_api/XRTConfig.h"

//#include "experimental/xrt_aie.h"
#include "experimental/xrt_kernel.h"
#include "experimental/xrt_bo.h"

#define XRT_SUCCESS 0
#define ALL_MESSAGES

#define INP_SIZE 8192
#define INP_SIZE_128b (INP_SIZE / 4)

using namespace std;

/*******************************************************************************
 ** load_xclbin Function
 *******************************************************************************/
static std::vector<char>  load_xclbin(xrtDeviceHandle device, const std::string& fnm)  {
    if (fnm.empty())  {
        cout << "A72-Error: <XCLBIN_FILE> is not specified" << endl;
        throw std::runtime_error("A72-Error: <XCLBIN_FILE> is not specified");
    }

    // load bit stream
    std::ifstream stream(fnm);
    stream.seekg(0,stream.end);
    size_t size = stream.tellg();
    stream.seekg(0,stream.beg);

    std::vector<char> header(size);
    stream.read(header.data(),size);

    auto top = reinterpret_cast<const axlf*>(header.data());

    //Download FPGA image (xclbin) to the device and check error
    if (xrtDeviceLoadXclbin(device, top))  {
        cout << "A72-Error: xclLoadXclBin failed, bitstream download unsuccessful" << endl;    
        throw std::runtime_error("A72-Error: Bitstream download failed");
    }

    return header;
}

/*******************************************************************************
 ** datamover Class
 *******************************************************************************/

class datamover_class {
   public: 
      xrtKernelHandle  datamover_khdl;
      xrtRunHandle     datamover_rhdl;
      uint32_t instance_errCnt;
      
      void init(xrtDeviceHandle dhdl, const axlf *top, int16_t iterCnt) { //, char insts) {
         
         //std::string datamover_obj_str = "datamover:{datamover_" + to_string(insts) + "}";
         std::string datamover_obj_str = "datamover:{datamover_0}";
         const char *datamover_obj = datamover_obj_str.c_str();
         
         //////////////////////////////////////////
         // Data Mover IP Init
         //////////////////////////////////////////
         
         // Open kernel handle exclusively to read the ap_return register later for reporting error...
         datamover_khdl = xrtPLKernelOpenExclusive(dhdl, top->m_header.uuid, datamover_obj);
         datamover_rhdl = xrtRunOpen(datamover_khdl);
         
         int rval = xrtRunSetArg(datamover_rhdl, 2, INP_SIZE_128b);
             rval = xrtRunSetArg(datamover_rhdl, 3, iterCnt);
            
         cout << "A72-Info: Initialized datamover kernel" << endl;
      }

      void run(void) {
         xrtRunStart(datamover_rhdl);
         cout << "A72-Info: Started datamover kernel" << endl;
      }

      void waitTo_complete(void) {
         auto state_datamover = xrtRunWait(datamover_rhdl);
         cout << "A72-Info: datamover kernel completed with status(" << state_datamover << ")" <<  endl;
      }

      void golden_check(uint32_t *errCnt) //, char insts)
      {
         //////////////////////////////////////////
         // Compare results
         //////////////////////////////////////////

         // Reading the error count for the ap_return reg of the hls kernel...
         xrtKernelReadRegister(datamover_khdl, 0x10, &instance_errCnt);
         
         //std::cout << "FIR execution " << (instance_errCnt ? "Failed! " : "Passed! ") << "With error count " << instance_errCnt << ".\n" << std::endl;

         // Adding instance error to the total error count...
         *errCnt += instance_errCnt;
      }

	   void close(void) {
         xrtRunClose(datamover_rhdl);
         xrtKernelClose(datamover_khdl);
         cout << "A72-Info: Closed kernel and run handles" << endl;
      }
};

/*******************************************************************************
 ** fir Class
 *******************************************************************************/

class fir_class {
   public: 
      xrtKernelHandle  fir_khdl;
      xrtRunHandle     fir_rhdl;
      uint32_t instance_errCnt;
      
      void init(xrtDeviceHandle dhdl, const axlf *top, int16_t iterCnt) { //, char insts) {
         
         //std::string fir_obj_str = "fir_hls:{fir_hls_" + to_string(insts) + "}";
         std::string fir_obj_str = "fir_hls:{fir_hls_0}";
         const char *fir_obj = fir_obj_str.c_str();
         
         //////////////////////////////////////////
         // FIR IP Init
         //////////////////////////////////////////
         
         fir_khdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, fir_obj);
         fir_rhdl = xrtRunOpen(fir_khdl);
         
         int rval = xrtRunSetArg(fir_rhdl, 2, iterCnt);
         
         cout << "A72-Info: Initialized fir kernel" << endl;
      }

      void run(void) {
         xrtRunStart(fir_rhdl);
         cout << "A72-Info: Started fir kernel" << endl;
      }

      void waitTo_complete(void) {
         auto state_fir = xrtRunWait(fir_rhdl);
         cout << "A72-Info: fir kernel completed with status(" << state_fir << ")" <<  endl;
      }

	   void close(void) {
         xrtRunClose(fir_rhdl);
         xrtKernelClose(fir_khdl);
         cout << "A72-Info: Closed kernel and run handles" << endl;
      }
};

/*******************************************************************************
 ** Main Function
 *******************************************************************************/

int main(int argc, char** argv)  {

    cout << endl << "A72-Info: FIR Filter Benchmark Test - DSP58 Version" << endl << endl;

    // ============================================================================
    // Step 1: Check Command Line Argument
    // ============================================================================
    //    o) argv[1] XCLBIN file
    // ============================================================================
  
    #ifdef ALL_MESSAGES
        cout << "A72-Info: ============================================================= " << endl;
        cout << "A72-Info: (Step 1) Check Command Line Arguments                         " << endl;
        cout << "A72-Info: ============================================================= " << endl;
        cout << endl; 
    #endif

   if ((argc < 2) || (argc > 3)) {
      cout << "A72-Error: Incorrect command line syntax " << endl;
      cout << "A72-Info:  Usage: " << argv[0] << " <XCLBIN_File> " <<  " iteration count(optional)" << endl << endl;
      return EXIT_FAILURE;
   }

   // ============================================================================
   // Step 2: Open XLCBIN
   // ============================================================================
   //   o) Open XCLBIN
   // ============================================================================

   else {
      #ifdef ALL_MESSAGES
         cout << "A72-Info: ============================================================= " << endl;
         cout << "A72-Info: (Step 2) Open XLCBIN                                          " << endl;
         cout << "A72-Info: ============================================================= " << endl;
         cout << endl; 
      #endif
      //If argc is 2 it loads xclbin(Normal Flow)
      //If argc is 3 and argv[2] is LOAD_XCLBIN ,it loads xclbin(To get POWER values)
      const char* xclbinFilename = argv[1];
      int16_t iterCnt = 0;

      if(argc == 3 ) {
         std::string iter = argv[2];
         iterCnt = stoi(iter);
      }
      else {
         iterCnt = ITER_CNT;
      }
      printf("Iteration : %d...\n", iterCnt);
      
      auto dhdl = xrtDeviceOpen(0);
      auto xclbin = load_xclbin(dhdl, xclbinFilename);
      auto top = reinterpret_cast<const axlf*>(xclbin.data());
   
      // ============================================================================
      // Step 3: Create and Initialize Data Mover Kernels
      // ============================================================================
      //   o) Create Data Mover Kernel Handles (mm2s, s2mm) and Initialize them
      //   o) Allocate their buffers
      //   o) Load input data into the mm2s buffer
      // ============================================================================
      #ifdef ALL_MESSAGES
          cout << "A72-Info: ============================================================= " << endl;
          cout << "A72-Info: (Step 3) Create and Initialize Data Mover Kernels             " << endl;
          cout << "A72-Info:          and FIR Chain Graph                                  " << endl;
          cout << "A72-Info: ============================================================= " << endl;
          cout << endl; 
      #endif

      datamover_class  datamover_krnl;
      fir_class fir_krnl;

      datamover_krnl.init(dhdl, top, iterCnt);
      fir_krnl.init(dhdl, top, iterCnt);

      // ============================================================================
      // Step 4: Run Data Mover Kernels
      // ============================================================================
      //   o) Invoke Run Methods
      // ============================================================================
      #ifdef ALL_MESSAGES
          cout << "A72-Info: ============================================================= " << endl;
          cout << "A72-Info: (Step 4) Run Data Mover Kernels                               " << endl;
          cout << "A72-Info: ============================================================= " << endl;
          cout << endl; 
      #endif

      datamover_krnl.run();
      fir_krnl.run();


      // ============================================================================
      // Step 5: Wait for Data Mover Kernels to Complete
      // ============================================================================
      //   o) Invoke run_wait for Data Mover Kernels
      // ============================================================================
      #ifdef ALL_MESSAGES
          cout << "A72-Info: ============================================================= " << endl;
          cout << "A72-Info: (Step 5) Wait for Data Mover Kernels to Complete              " << endl;
          cout << "A72-Info: ============================================================= " << endl;
          cout << endl; 
      #endif

      fir_krnl.waitTo_complete();
      datamover_krnl.waitTo_complete();

      // ============================================================================
      // Step 6: Verify Output Results
      // ============================================================================
      //     o) Check correctness of the output results with golden data
      // ============================================================================
      #ifdef ALL_MESSAGES
          cout << "A72-Info: ============================================================= " << endl;
          cout << "A72-Info: (Step 6) Verify Output Results                                " << endl;
          cout << "A72-Info: ============================================================= " << endl;
          cout << endl; 
      #endif

      uint32_t errCnt = 0;
      datamover_krnl.golden_check(&errCnt);

      // ============================================================================
      // Step 7: Release Allocated Resources
      // ============================================================================
      #ifdef ALL_MESSAGES
          cout << endl;
          cout << "A72-Info: ============================================================= "   << endl;
          cout << "A72-Info: (Step 7) Release Allocated Resources                          "   << endl;
          cout << "A72-Info: ============================================================= "   << endl;
          cout << endl; 
      #endif

      //Closing PL-Handles
      datamover_krnl.close();
      fir_krnl.close();

      //Closing Device
      cout << "A72-Info: Closing Device...\n";
      xrtDeviceClose(dhdl);

      //Report Final Result
      cout << endl << "A72-Info: DSP FIR TEST [" << N_FIR_FILTERS << " Filters; " << N_FIR_TAPS << 
          " Taps] TEST " << ((errCnt > 1500) ? "FAILED" : "PASSED") << endl << endl;

      //Exit with result
      return ((errCnt > 1500) ? EXIT_FAILURE :  EXIT_SUCCESS);
   }
}
