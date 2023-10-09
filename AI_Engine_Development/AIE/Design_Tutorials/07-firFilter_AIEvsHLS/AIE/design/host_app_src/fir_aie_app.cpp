/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/


#include "fir_aie_graph.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

#include "adf/adf_api/XRTConfig.h"

#include "experimental/xrt_aie.h"
#include "experimental/xrt_kernel.h"
#include "experimental/xrt_bo.h"

#define XRT_SUCCESS 0
#define ALL_MESSAGES

#define SAMPLES_PER_WORD 4

#define INP_SIZE 2048
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
 ** FIR chain class
 *******************************************************************************/

class fir_chain_class  {
    public:
        xrtGraphHandle fir_g_hdl;

        int init(xrtDeviceHandle dhdl, const axlf *top)  {
      
            fir_g_hdl = xrtGraphOpen(dhdl, top->m_header.uuid, "FilterChain");
            if (fir_g_hdl == NULL)  {
                throw std::runtime_error("A72-Error: Unable to open AIE FIR chain graph");
	        return EXIT_FAILURE;
            }
            else  {
                cout << "A72-Info: Initialized AIE FIR chain graph" << endl;
                return EXIT_SUCCESS;
            }
        }

        int run(void)  {
            int errCode;
            errCode = xrtGraphReset(fir_g_hdl);
            if (errCode != XRT_SUCCESS) {
                throw std::runtime_error("A72-Error: Unable to reset AIE FIR chain graph");
                return EXIT_FAILURE;
            }
            else  {
                //size_t total_windows = (REPEAT_OFFSET + (REPETITIONS + 1) * (OUTPUT_SAMPLES - REPEAT_OFFSET) + FLUSH_SAMPLES) / FIR_WINDOW_SIZE;
                //errCode = xrtGraphRun(fir_g_hdl, total_windows);
                errCode = xrtGraphRun(fir_g_hdl, -1);
            }
            if (errCode != XRT_SUCCESS) {
                throw std::runtime_error("A72-Error: Unable to run AIE FIR chain graph");
                return EXIT_FAILURE;
            }
            cout << "A72-Info: Started AIE FIR chain graph" << endl;
            return EXIT_SUCCESS;
        }
       
        void close(void)  {
            xrtGraphClose(fir_g_hdl);
            cout << "A72-Info: Closed AIE FIR chain graph" << endl;
        }
};

/*******************************************************************************
 ** Main Function
 *******************************************************************************/

int main(int argc, char** argv)  {
    uint32_t errCnt = 0;
    int errCode;

    cout << endl << "A72-Info: FIR Filter Benchmark Test - AIE Version" << endl << endl;

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
   
      adf::registerXRT(dhdl, top->m_header.uuid);


     // ============================================================================
     // Step 3: Create and Initialize Data Mover Kernels and FIR Chain Graph
     // ============================================================================
     //   o) Create Data Mover Kernel Handles (datamover, s2mm) and Initialize them
     //   o) Create FIR Filter Graph Handle and Initialize it
     //   o) Load input data into the datamover buffer
     // ============================================================================
     #ifdef ALL_MESSAGES
         cout << "A72-Info: ============================================================= " << endl;
         cout << "A72-Info: (Step 3) Create and Initialize Data Mover Kernels             " << endl;
         cout << "A72-Info:          and FIR Chain Graph                                  " << endl;
         cout << "A72-Info: ============================================================= " << endl;
         cout << endl; 
     #endif
     
     datamover_class  datamover_krnl;
     datamover_krnl.init(dhdl, top, iterCnt);
     
     fir_chain_class fir_graph;
     
     fir_graph.init(dhdl, top);
     
     // ============================================================================
     // Step 4: Run Data Mover Kernels and FIR Chain Graph
     // ============================================================================
     //   o) Invoke Run Methods
     // ============================================================================
     #ifdef ALL_MESSAGES
         cout << "A72-Info: ============================================================= " << endl;
         cout << "A72-Info: (Step 4) Run Data Mover Kernels and FIR Chain Graph           " << endl;
         cout << "A72-Info: ============================================================= " << endl;
         cout << endl; 
     #endif
     
     errCode = fir_graph.run();
     if (errCode != 0)  {
         throw std::runtime_error("A72-Error: Error encountered running AIE FIR Chain graph");
         return EXIT_FAILURE;
     }
     
     datamover_krnl.run();
     
     
     // ============================================================================
     // Step 5: Wait for Data Mover Kernels to Complete
     // ============================================================================
     //   o) Invoke waitTo_complete for Data Mover Kernels
     // ============================================================================
     #ifdef ALL_MESSAGES
         cout << "A72-Info: ============================================================= " << endl;
         cout << "A72-Info: (Step 5) Wait for Data Mover Kernels to Complete              " << endl;
         cout << "A72-Info: ============================================================= " << endl;
         cout << endl; 
     #endif
     
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
     
     //Closing Graph
     fir_graph.close();
     
     //Closing Device
     cout << "A72-Info: Closing Device...\n";
     xrtDeviceClose(dhdl);
     
     //Report Final Result
     cout << endl << "A72-Info: AIE FIR TEST [" << N_FIR_FILTERS << " Filters; " << N_FIR_TAPS << 
         " Taps] TEST " << ((errCnt > 1500) ? "FAILED" : "PASSED") << endl << endl;
     
     //Exit with result
     return ((errCnt > 1500) ? EXIT_FAILURE :  EXIT_SUCCESS);
  
   }
}
