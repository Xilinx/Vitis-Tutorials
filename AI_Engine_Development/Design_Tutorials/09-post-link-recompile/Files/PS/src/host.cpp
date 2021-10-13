/*
 * (c) Copyright 2021 Xilinx, Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fstream>
#include <fcntl.h>
#include <sys/stat.h>

// ********************************************************************************** //
// ---------------------------------------------------------------------------------- //
//                          A 7 2 -  A P P L I C A T I O N                            //
// ---------------------------------------------------------------------------------- //
// ********************************************************************************** //

// ============================================================================
// Step 1: Include the AI Engine Graph Application and XRT and ADF Headers
// ============================================================================
#include "graph.cpp" // This is the Graph testbench from which the main is removed

#include "helper_functions.h"

using namespace std;

#define NUM_INPUT_SAMPLES (NUMBER_OF_ITERATIONS*NUM_SAMPLES)
#define NUM_OUTPUT_SAMPLES (NUMBER_OF_ITERATIONS*NUM_SAMPLES)
#define XRT_SUCCESS 0
#define ALL_MESSAGES


// load_xclbin function is used to read in xclbin file
// TODO: move to function file where helper_functions.h


/*******************************************************************************
 ** Main Function
 *******************************************************************************/
int main(int argc, char** argv) {

  cout << endl;

  // ============================================================================
  // Step 2: Check Command Line Argument
  // ============================================================================
  //    o) argv[1] XCLBIN file
  // ============================================================================


#ifdef ALL_MESSAGES
  cout << "A72-Info: ============================================================= " << endl;
  cout << "A72-Info: (Step 2) Check Command Line Arguments                      " << endl;
  cout << "A72-Info: ============================================================= " << endl;
#endif

  if (argc != 2) {
    cout << "A72-Error: Incorrect command line syntax " << endl;
    cout << "A72-Info:  Usage: " << argv[0] << " <XCLBIN_File> " << endl << endl;
    return EXIT_FAILURE;
  }


  // ============================================================================
  // Step 3: Open XLCBIN and Create Data Mover Kernel Handles
  // ============================================================================
  //   o) Open XCLBIN
  //   o) Create Data Mover Kernel Handles (mm2s0, mm2s1, s2mm)
  // ============================================================================
  cout << endl;
#ifdef ALL_MESSAGES
  cout << "A72-Info: ============================================================= " << endl;
  cout << "A72-Info: (Step 3) Open XLCBIN and Create Data Mover Kernel Handles "     << endl;
  cout << "A72-Info: ============================================================= " << endl;
#endif

  // ------------------------------------------------------------------
  // Step 3.1: Load XCLBIN Binary File from a disk to memory
  // ------------------------------------------------------------------

  //open a device and obtain its handle
  auto dhdl = xrtDeviceOpen(0);

  //Download FPGA image (xclbin) to the device
  auto xclbin = load_xclbin(dhdl, argv[1]);

  //point to top of xclbin data
  auto top = reinterpret_cast<const axlf*>(xclbin.data());

  adf::registerXRT(dhdl, top->m_header.uuid);

  //Reset board and AIE array before running design
  //cout << "A72-Info: Reset the AI Engine Array" << endl;
  //auto errCode = xrtResetAIEArray(dhdl);
  //if (errCode != XRT_SUCCESS) {
  //throw std::runtime_error("A72-Error: Unable to reset AIE Array");
  //cout << "A72-Error: Reset AIE Array" << endl;
  //return EXIT_FAILURE;
  //}


  // ------------------------------------------------------------------
  // Step 3.2: Create PL Kernel Handles that point to the correct location
  //           in XCLBIN file for each data mover kernel
  // ------------------------------------------------------------------

  //open PL kernels and obtain handles
  xrtKernelHandle s2mm_khdl  = xrtPLKernelOpen(dhdl, top->m_header.uuid, "s2mm");
  xrtKernelHandle mm2s_khdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, "mm2s");

  // If there is a specific name
  // xrtKernelHandle mm2s_khdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, "mm2s:{mm2s_0}");


  // ============================================================================
  // Step 4: Allocate buffers for Input data and Results in Global Memory
  // ============================================================================
#ifdef ALL_MESSAGES
  cout << endl;
  cout << "A72-Info: ============================================================= " << endl;
  cout << "A72-Info: (Step 4) Allocate buffers for Input data and Results " << endl;
  cout << "A72-Info:          in Global Memory"              << endl;
  cout << "A72-Info: ============================================================= " << endl;
#endif


  //Allocate BOs (buffer objects) of requested size with appropriate flags
    xrtBufferHandle input_bohdl = xrtBOAlloc(dhdl, sizeof(uint32_t)*NUM_INPUT_SAMPLES,  0, XCL_BO_FLAGS_CACHEABLE);
    xrtBufferHandle output_bohdl = xrtBOAlloc(dhdl, sizeof(uint32_t)*NUM_OUTPUT_SAMPLES, 0, XCL_BO_FLAGS_CACHEABLE);

  //Memory map BOs into user's address space (DDR Memory)
    auto input_mapped = reinterpret_cast<uint32_t*>(xrtBOMap(input_bohdl));
    auto output_mapped = reinterpret_cast<uint32_t*>(xrtBOMap(output_bohdl));

    std::cout << "A72-Info: Processed BOs (input_mapped and output_mapped)\n";


  // ============================================================================
  // Step 5: Prepare VCK190 board and run TX Chain graph
  //   o) Reset board
  //   o) Use the XRT Run Time Graph Control API calls to run the dataflow graph
  //      x) Read the coeffients from text files
  //      x) Open the graph, run the graph
  //      x) Update the coeffients in the DPD filter AI Engine subgraph
  // ============================================================================

#ifdef ALL_MESSAGES
  cout << endl;
  cout << "A72-Info: ============================================================= " << endl;
  cout << "A72-Info: (Step 5) Prepare VCK190 board and run TX Chain graph" << endl;
  cout << "A72-Info: ============================================================= " << endl;
#endif

  // ------------------------------------------------------------------------------------
  // Step 5.1: Reset VCK190 board before running design
  // ------------------------------------------------------------------------------------

  //Wait
 unsigned int sleep = 1000;
  usleep(sleep);

  //Reset board and AIE array before running design
  //  cout << "A72-Info: Reset the AI Engine Array" << endl;
  //auto errCode = xrtResetAIEArray(dhdl);
  //if (errCode != XRT_SUCCESS) {
  //  throw std::runtime_error("A72-Error: Unable to reset AIE Array");
  //  cout << "A72-Error: Reset AIE Array" << endl;
  //  return EXIT_FAILURE;
  // }

  cout << "A72-Info: Reset the AI Engine Array" << endl;
  system("./versal_run/reset-board.sh");

  //Wait
  usleep(1000);

  // ------------------------------------------------------------------------------------
  // Step 5.2: Use the XRT Run Time Graph Control API calls to run the dataflow graph
  //           o) Open the graph, run the graph
  // ------------------------------------------------------------------------------------


  //Open the graph and obtain its handle
  cout << "A72-Info: Initializing the  graph..." << endl;

  auto graphHandle = xrtGraphOpen(dhdl, top->m_header.uuid, "clipgraph");
  if (graphHandle == NULL) {
    throw std::runtime_error("A72-Error: Unable to open graph handle");
    cout << "A72-Error: Unable to open graph handle" << endl;
    return EXIT_FAILURE;
  }

  //Starts the ClipGraph graph execution.
  //Intializes AI Engine cores and routes datapaths between AI Engine cores
  cout << "A72-Info: Running the clipgraph graph..." << endl;

  auto errCode = xrtGraphReset(graphHandle);
  if (errCode != XRT_SUCCESS) {
    throw std::runtime_error("A72-Error: Unable to reset graph");
    cout << "A72-Error: Unable to run graph" << endl;
    return EXIT_FAILURE;
  }

  errCode = xrtGraphRun(graphHandle,-1);
  if (errCode != XRT_SUCCESS) {
    throw std::runtime_error("A72-Error: Unable to run graph");
    cout << "A72-Error: Unable to run graph" << endl;
    return EXIT_FAILURE;
  }

  //Wait
  usleep(sleep);


  // ============================================================================
  // Step 6: Write input user data to DDR (via mapped input buffer handlers)
  //      o) Read data from text files
  //      o) Write data to buffer handle input_mapped which are mapped to DDR
  //      o) Sync BOs with device
  // ============================================================================
#ifdef ALL_MESSAGES
  cout << endl;
  cout << "A72-Info: ============================================================= " << endl;
  cout << "A72-Info: (Step 6) Write input user data to DDR (via mapped " << endl;
  cout << "A72-Info:          input buffer handles)"                                 << endl;
  cout << "A72-Info: ============================================================= " << endl;
#endif

//  here I should read a file containing the input dataflow


//=============================================================================
//=============================================================================
//=============================================================================
//=============================================================================


  // ------------------------------------------------------------------------------------
  // Step 6.1: Includes input and reference output data variable
  // Should be changed to file reading these data
  // ------------------------------------------------------------------------------------
  #include "data.h"     // Defines variable  short int cint16input[]
  #include "golden.h"   // defines variable int golden[]



  // ------------------------------------------------------------------------------------
  // Step 6.2: Write user input data to BOs mapped to DDR
  // ------------------------------------------------------------------------------------
  cout << "A72-Info: Storing input data in DDR ..." << endl;
  typedef union { unsigned int ui; short int si[2];} conv;
  for(unsigned int i = 0,j=0; i < NUM_INPUT_SAMPLES; i++) {
    conv c;
    c.si[0] = cint16input[j++];
    c.si[1] = cint16input[j++];

    input_mapped[i] = c.ui; // Store data in DDR
  }


  // ------------------------------------------------------------------------------------
  // Step 6.3: Synchronize buffer contents between BOs and device. This ensures that
  //           the device has the same data as the BOs.This operation may require
  //           DMA to the device or CPU cache flushing/invalidation.
  // ------------------------------------------------------------------------------------

  //Synchronize buffer contents in requested direction (XCL_BO_SYNC_BO_TO_DEVICE)
  errCode=xrtBOSync(input_bohdl, XCL_BO_SYNC_BO_TO_DEVICE, sizeof(uint32_t)*NUM_INPUT_SAMPLES, 0);
  if (errCode != XRT_SUCCESS) {
    cout << endl << "A72-ERROR: Failed to synchronize input BO" << endl << endl;
    return EXIT_FAILURE;
  }


  errCode=xrtBOSync(output_bohdl, XCL_BO_SYNC_BO_TO_DEVICE, sizeof(uint32_t)*NUM_OUTPUT_SAMPLES, 0);
  if (errCode != XRT_SUCCESS) {
    cout << endl << "A72-ERROR: Failed to synchronize output BO" << endl << endl;
    return EXIT_FAILURE;
  }


  // ============================================================================
  // Step 7: Execute the Data Mover Kernels
  // ============================================================================

#ifdef ALL_MESSAGES
  cout << endl;
  cout << "A72-Info: ============================================================= "   << endl;
  cout << "A72-Info: (Step 7) Execute the Data Mover Kernels " << endl;
  cout << "A72-Info: ============================================================= "   << endl;
#endif

  cout << "A72-Info: Start mm2s_0 and s2mm kernels ..." << endl;

  //Create a kernel run handle to start each data mover pl kernel
  //mm2s kernels moves data from DDR to AI Engine
  //s2mm kernel moves data from AI Engine to DDR

    int rval;
    xrtRunHandle mm2s_rhdl = xrtRunOpen(mm2s_khdl);
    rval = xrtRunSetArg(mm2s_rhdl, 0, input_bohdl);
    rval = xrtRunSetArg(mm2s_rhdl, 2, NUM_INPUT_SAMPLES*2);


    xrtRunHandle s2mm_rhdl = xrtRunOpen(s2mm_khdl);
    rval = xrtRunSetArg(s2mm_rhdl, 0, output_bohdl);
    rval = xrtRunSetArg(s2mm_rhdl, 2, NUM_OUTPUT_SAMPLES);

    xrtRunStart(mm2s_rhdl);
    xrtRunStart(s2mm_rhdl);

  cout << "A72-Info: Done with xrtKernelRun handles" << endl;

  //Wait for a mm2s_0, mm2s_1, and s2mm kernels' execution to finish
  //Close the run handles obtained by in xrtRunOpen()
  //Close opened kernel handles obtained from xrtKernelOpen()
  auto state_mm2s = xrtRunWait(mm2s_rhdl);
  std::cout << "A72-Info: mm2s0 completed with status(" << state_mm2s << ")" <<  endl;
  xrtRunClose(mm2s_rhdl);
  xrtKernelClose(mm2s_khdl);


  auto state_s2mm = xrtRunWait(s2mm_rhdl);
  std::cout << "A72-Info: s2mm completed with status(" << state_s2mm << ")" << endl;
  xrtRunClose(s2mm_rhdl);
  xrtKernelClose(s2mm_khdl);

  cout << "A72-Info: s2mm is complete!" <<  endl;

  //Wait
  usleep(sleep);

  // ============================================================================
  // Step 8: Verify Output Results
  //     o) Store input data (in DDR) to log_lte0.txt and log_lte1.txt
  //     o) Store output result (in DDR) to log_output.txt
  //     o) Check correctness of the output results with golden data
  // ============================================================================

#ifdef ALL_MESSAGES
  cout << endl;
  cout << "A72-Info: ============================================================= "   << endl;
  cout << "A72-Info: (Step 8) Verify Output Results " << endl;
  cout << "A72-Info: ============================================================= "   << endl;
#endif

  // ---------------------------------------------------------------------------
  // Step 8.1: Write input data from DDR (via the mapped BOs)
  //           to log_input.txt
  // ---------------------------------------------------------------------------

  //Instantiate streams to write lte0 and lte input data to text files
  std::ofstream log_input;
  std::stringstream logDataIn ;
  std::string logDataIn_str;

  logDataIn << logDataIn_str << "./log_input";
  logDataIn_str = logDataIn.str();

  log_input.open (logDataIn_str);

  //Write input mapped BOs' content to text files
  for (unsigned int i = 0; i <NUM_INPUT_SAMPLES; i++) {
      conv c;
      c.ui = input_mapped[i];
    log_input << c.si[0] << "   " << c.si[1] << "\n";
  }

  //Close text file streams
  log_input.close();

  // ---------------------------------------------------------------------------
  // Step 8.2: Write  output data from DDR (via the mapped BO)
  //           to log_output.txt
  // ---------------------------------------------------------------------------
  errCode=xrtBOSync(output_bohdl, XCL_BO_SYNC_BO_TO_DEVICE, sizeof(uint32_t)*NUM_OUTPUT_SAMPLES, 0);
  if (errCode != XRT_SUCCESS) {
    cout << endl << "A72-ERROR: Failed to synchronize output BO" << endl << endl;
    return EXIT_FAILURE;
  }

  //Instantiate stream to write dpd0 output data to text file log_output
  std::ofstream outf;
  std::stringstream logDataOut;
  std::string logDataOut_str;
  logDataOut << logDataOut_str << "./log_output";
  logDataOut_str = logDataOut.str();

  outf.open(logDataOut_str);
  if(!outf) {
    cout << "A72-Error: Unable to open log_output file" << endl;
    return EXIT_FAILURE;
  }

  //Write output mapped BO content to text file
  cout << "A72-Info: Store  output to log_output file" << endl;
  for(unsigned int i = 0; i < NUM_OUTPUT_SAMPLES; i++) {
    outf << output_mapped[i] << "\n";
  }

  //Close text file stream
  outf.close();

  // ---------------------------------------------------------------------------
  // Step 8.2: Compare  output to Golden Reference data
  // ---------------------------------------------------------------------------

/* // This part is important if the output reference is in a file
  // Instantiate streams to read dpd output and golden reference data
  std::ifstream dpd0_golden("./data/dpd0_golden");
  std::ifstream dpdf_comp("./log_output");

  //Compare dpd output and golden reference data and count number of errors
  int ErrorCnt=0 ;
  while ((!dpdf_comp.eof()) && (!dpd0_golden.eof())) {
    std::string line,line2;
    getline(dpdf_comp,line);
    getline(dpd0_golden,line2);
    if(line!=line2){ //are the strings equal? i.e. on the same line and same?
      ErrorCnt++;
      break;
    }
  }
*/
  int ErrorCnt=0 ;
  for(int i=0;i<NUM_OUTPUT_SAMPLES;i++)
      if((int)output_mapped[i] != (int)golden[i])
      {
          cout << "Error Line " << i << endl;
          ErrorCnt++;
      }

  //Determine if the design passed or failed
  if (ErrorCnt==0) {
    cout << "A72-Info: Test Passed " << endl;
  }
  else {
    cout << "A72-Info: Test Failed " << endl;
  }

  //Close text file streams
  //dpd0_golden.close();
  //dpdf_comp.close();

  // ============================================================================
  // Step 9: Release Allocated Resources
  // ============================================================================
#ifdef ALL_MESSAGES
  cout << endl;
  cout << "A72-Info: ============================================================= "   << endl;
  cout << "A72-Info: (Step 9) Release Allocated Resources " << endl;
  cout << "A72-Info: ============================================================= "   << endl;
#endif

    xrtBOFree(input_bohdl);
    xrtBOFree(output_bohdl);
    xrtGraphClose(graphHandle);
    xrtDeviceClose(dhdl);

  return EXIT_SUCCESS;
}
