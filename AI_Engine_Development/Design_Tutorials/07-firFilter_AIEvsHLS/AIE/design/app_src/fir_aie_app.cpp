//------------------------------------------------------------------------------
// © Copyright 2021 Xilinx, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//------------------------------------------------------------------------------

#include "fir_aie_graph.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
#include <complex>

#include "input_data.h"

#if (N_FIR_FILTERS == 1) && (N_FIR_TAPS == 15)
#include "golden_data_1f_15t.h"
#elif (N_FIR_FILTERS == 10) && (N_FIR_TAPS == 15)
#include "golden_data_10f_15t.h"
#elif (N_FIR_FILTERS == 1) && (N_FIR_TAPS == 240)
#include "golden_data_1f_240t.h"
#elif (N_FIR_FILTERS == 10) && (N_FIR_TAPS == 240)
#include "golden_data_10f_240t.h"
#else
#include "golden_data_1f_15t.h"
#endif

#include "adf/adf_api/XRTConfig.h"

#include "experimental/xrt_aie.h"
#include "experimental/xrt_kernel.h"
#include "experimental/xrt_bo.h"

#define XRT_SUCCESS 0
#define ALL_MESSAGES

#define SAMPLES_PER_WORD   4

#define REPEAT_OFFSET   4096
#define FLUSH_SAMPLES   4096

#ifdef USE_SMALL_DATASET
#define REPETITIONS        3
#else
#define REPETITIONS      509
#endif


#define MAX_ERRS_REPORTED 10


const char *mm2s_obj[1] = {"mm2s"};
const char *s2mm_obj[1] = {"s2mm"};
const char *fir_chain_obj[1] = {"FilterChain"};

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
 ** mm2s Class
 *******************************************************************************/

class mm2s_class  {
    public: 
        xrtBufferHandle  in_bo_hdl;
        int              *in_bo_mapped;
        xrtKernelHandle  mm2s_k_hdl;
        xrtRunHandle     mm2s_r_hdl;

        void init(xrtDeviceHandle dhdl, const axlf *top, const char *mm2s_obj)  {
            int rval;

            size_t total_samples = REPEAT_OFFSET + (REPETITIONS + 1) * (INPUT_SAMPLES - REPEAT_OFFSET) + FLUSH_SAMPLES;
            size_t data_size_in_bytes = total_samples * sizeof(cint16);

            in_bo_hdl = xrtBOAlloc(dhdl, data_size_in_bytes, 0, 0);
            in_bo_mapped = reinterpret_cast<int *>(xrtBOMap(in_bo_hdl));


            mm2s_k_hdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, mm2s_obj);
            mm2s_r_hdl = xrtRunOpen(mm2s_k_hdl);

            rval = xrtRunSetArg(mm2s_r_hdl, 0, in_bo_hdl);
            rval = xrtRunSetArg(mm2s_r_hdl, 2, total_samples / SAMPLES_PER_WORD);

            cout << "A72-Info: Initialized mm2s kernel" << endl;
        }

        void run(void)  {
            xrtRunStart(mm2s_r_hdl);
            cout << "A72-Info: Started mm2s kernel" << endl;
        }

        void run_wait(void)  {
            auto state_mm2s = xrtRunWait(mm2s_r_hdl);
            cout << "A72-Info: mm2s kernel completed with status(" << state_mm2s << ")" <<  endl;
        }

	void close(void)  {
            xrtBOFree(in_bo_hdl);
            xrtRunClose(mm2s_r_hdl);
            xrtKernelClose(mm2s_k_hdl);
            cout << "A72-Info: Freed input buffer and closed kernel and run handles" << endl;
        }

        void load(void)  {
            cint16 *input_array;
            input_array = reinterpret_cast<cint16 *>(in_bo_mapped);

            for (int ix = 0; ix < REPEAT_OFFSET; ix++)  {
                *input_array++ = input_data[ix];
            }

            for (int reps = 0; reps < (REPETITIONS + 1); reps++)  {
                for (int ix = REPEAT_OFFSET; ix < INPUT_SAMPLES; ix++)  {
                    *input_array++ = input_data[ix];
                }
            }

            for (int ix = 0; ix < FLUSH_SAMPLES; ix++)  {
                *input_array++ = {0, 0};
            }

            size_t total_samples = REPEAT_OFFSET + (REPETITIONS + 1) * (INPUT_SAMPLES - REPEAT_OFFSET)+ FLUSH_SAMPLES;
            cout << "A72-Info: Loaded " << total_samples << " input samples into mm2s kernel input buffer" << endl;
            cout << "A72-Info:        " << total_samples - FLUSH_SAMPLES << " data samples + " << FLUSH_SAMPLES <<
                " zero samples to flush FIR filter" << endl;
        }
};


/*******************************************************************************
 ** s2mm Class
 *******************************************************************************/

class s2mm_class  {
    public: 
        xrtBufferHandle  out_bo_hdl;
        int              *out_bo_mapped;
        xrtKernelHandle  s2mm_k_hdl;
        xrtRunHandle     s2mm_r_hdl;

        void init(xrtDeviceHandle dhdl, const axlf *top, const char *s2mm_obj)  {
            int rval;

            size_t total_samples = REPEAT_OFFSET + (REPETITIONS + 1) * (OUTPUT_SAMPLES - REPEAT_OFFSET) + FLUSH_SAMPLES;
            size_t data_size_in_bytes = total_samples * sizeof(cint16);

            out_bo_hdl = xrtBOAlloc(dhdl, data_size_in_bytes, 0, 0);
            out_bo_mapped = reinterpret_cast<int *>(xrtBOMap(out_bo_hdl));


            s2mm_k_hdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, s2mm_obj);
            s2mm_r_hdl = xrtRunOpen(s2mm_k_hdl);

            rval = xrtRunSetArg(s2mm_r_hdl, 0, out_bo_hdl);
            rval = xrtRunSetArg(s2mm_r_hdl, 2, total_samples / SAMPLES_PER_WORD);

            cout << "A72-Info: Initialized s2mm kernel" << endl;
        }

        void run(void)  {
            xrtRunStart(s2mm_r_hdl);
            cout << "A72-Info: Started s2mm kernel" << endl;
        }
 
       void run_wait(void)  {
            auto state_s2mm = xrtRunWait(s2mm_r_hdl);
            cout << "A72-Info: s2mm kernel completed with status(" << state_s2mm << ")" <<  endl;
        }

	void close(void)  {
            xrtBOFree(out_bo_hdl);
            xrtRunClose(s2mm_r_hdl);
            xrtKernelClose(s2mm_k_hdl);
        }

	int golden_check(void)  {
            cint16 *output_data;
            int errCnt = 0;
            int gix =0;
            
            output_data = reinterpret_cast<cint16 *>(out_bo_mapped);


            if ((GOLDEN_N_FIR_FILTERS == N_FIR_FILTERS) && (GOLDEN_N_FIR_TAPS == N_FIR_TAPS))  {

                cout << "A72-Info: Comparing filter output data to golden reference...." << endl;

                size_t total_samples = REPEAT_OFFSET + (REPETITIONS + 1) * (OUTPUT_SAMPLES - REPEAT_OFFSET);
       
	        for (int ix = 0; ix < total_samples; ix++)  {
                    if ((output_data[ix].real != golden_data[gix].real) || (output_data[ix].imag != golden_data[gix].imag) )  {
                        errCnt += 1;
                        if (errCnt == MAX_ERRS_REPORTED)  {
                            cout << "A72-Error: Maximum Error Report Count Reached; no more compare errors will be reported" << endl;
                        } 
                        else if (errCnt < MAX_ERRS_REPORTED)  {
                            cout << "A72-Error: Filter Output Data word " << ix << " data value: (" << 
                                     output_data[ix].real << " + " << output_data[ix].imag << "i)"  <<
                                     " does not match golden/reference value : ("                   <<
                                     golden_data[gix].real << " + " << golden_data[gix].imag << "i)"  << endl;
                        }
                    }
                    gix++;
                    if (gix == OUTPUT_SAMPLES)  {
                        gix = REPEAT_OFFSET;
                    }
                }
                if (errCnt)  {
                    cout << "A72-Info: COMPARE FAILED: " << errCnt << " mismatches found" << endl;
                }
                else  {
                    cout << "A72-Info: COMPARE PASSED: " << total_samples << " samples compared; all data matched" << endl;
                }
                return(errCnt);
            }
            else  {
                cout << "A72-Info: **** GOLDEN DATA FILE does not match latest FIR parameters - data compare skipped" << endl;
                cout << "               GOLDEN_N_FIR_FILTERS: " << GOLDEN_N_FIR_FILTERS <<
                        " N_FIR_FILTERS: "     << N_FIR_FILTERS <<
                        " GOLDEN_N_FIR_TAPS: " << GOLDEN_N_FIR_TAPS <<
                        " N_FIR_TAPS: "        << N_FIR_TAPS << endl;
                return(0);
            }
        }
};


/*******************************************************************************
 ** FIR chain class
 *******************************************************************************/

class fir_chain_class  {
    public:
        xrtGraphHandle fir_g_hdl;

        int init(xrtDeviceHandle dhdl, const axlf *top, const char *fir_chain_obj)  {
      
            fir_g_hdl = xrtGraphOpen(dhdl, top->m_header.uuid, fir_chain_obj);
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
                size_t total_windows = (REPEAT_OFFSET + (REPETITIONS + 1) * (OUTPUT_SAMPLES - REPEAT_OFFSET) + FLUSH_SAMPLES) / FIR_WINDOW_SIZE;
                errCode = xrtGraphRun(fir_g_hdl, total_windows);
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
    int errCnt = 0;
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

    if (argc != 2) {
        cout << "A72-Error: Incorrect command line syntax " << endl;
        cout << "A72-Info:  Usage: " << argv[0] << " <XCLBIN_File> " << endl << endl;
        return EXIT_FAILURE;
    }


    // ============================================================================
    // Step 2: Open XLCBIN
    // ============================================================================
    //   o) Open XCLBIN
    // ============================================================================
#ifdef ALL_MESSAGES
    cout << "A72-Info: ============================================================= " << endl;
    cout << "A72-Info: (Step 2) Open XLCBIN                                          " << endl;
    cout << "A72-Info: ============================================================= " << endl;
    cout << endl; 
#endif

    //open a device and obtain its handle
    auto dhdl = xrtDeviceOpen(0);

    //Download FPGA image (xclbin) to the device
    auto xclbin = load_xclbin(dhdl, argv[1]);
  
    //point to top of xclbin data  
    auto top = reinterpret_cast<const axlf*>(xclbin.data());

    adf::registerXRT(dhdl, top->m_header.uuid);


    // ============================================================================
    // Step 3: Create and Initialize Data Mover Kernels and FIR Chain Graph
    // ============================================================================
    //   o) Create Data Mover Kernel Handles (mm2s, s2mm) and Initialize them
    //   o) Create FIR Filter Graph Handle and Initialize it
    //   o) Load input data into the mm2s buffer
    // ============================================================================
#ifdef ALL_MESSAGES
    cout << "A72-Info: ============================================================= " << endl;
    cout << "A72-Info: (Step 3) Create and Initialize Data Mover Kernels             " << endl;
    cout << "A72-Info:          and FIR Chain Graph                                  " << endl;
    cout << "A72-Info: ============================================================= " << endl;
    cout << endl; 
#endif

    mm2s_class  mm2s_krnl;
    s2mm_class  s2mm_krnl;

    mm2s_krnl.init(dhdl, top, mm2s_obj[0]);
    s2mm_krnl.init(dhdl, top, s2mm_obj[0]);

    fir_chain_class    fir_graph;

    fir_graph.init(dhdl, top, fir_chain_obj[0]);

    mm2s_krnl.load();


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

    s2mm_krnl.run();
    mm2s_krnl.run();


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

    mm2s_krnl.run_wait();
    s2mm_krnl.run_wait();


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

    errCnt = s2mm_krnl.golden_check();


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
    mm2s_krnl.close();
    s2mm_krnl.close();

    //Closing Graph
    fir_graph.close();

    //Closing Device
    cout << "A72-Info: Closing Device...\n";
    xrtDeviceClose(dhdl);

    //Report Final Result
    cout << endl << "A72-Info: AIE FIR [" << N_FIR_FILTERS << " Filters; " << N_FIR_TAPS << 
        " Taps] TEST " << (errCnt ? "FAILED" : "PASSED") << endl << endl;

    //Exit with result
    return (errCnt ? EXIT_FAILURE :  EXIT_SUCCESS);
}
