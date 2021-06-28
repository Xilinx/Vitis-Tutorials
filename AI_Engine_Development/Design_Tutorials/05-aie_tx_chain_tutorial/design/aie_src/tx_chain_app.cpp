/*  (c) Copyright 2014 - 2020 Xilinx, Inc. All rights reserved.
   
    This file contains confidential and proprietary information
    of Xilinx, Inc. and is protected under U.S. and
    international copyright and other intellectual property
    laws.
   
    DISCLAIMER
    This disclaimer is not a license and does not grant any
    rights to the materials distributed herewith. Except as
    otherwise provided in a valid license issued to you by
    Xilinx, and to the maximum extent permitted by applicable
    law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
    WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
    AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
    BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
    INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
    (2) Xilinx shall not be liable (whether in contract or tort,
    including negligence, or under any other theory of
    liability) for any loss or damage of any kind or nature
    related to, arising under or in connection with these
    materials, including for any direct, or any indirect,
    special, incidental, or consequential loss or damage
    (including loss of data, profits, goodwill, or any type of
    loss or damage suffered as a result of any action brought
    by a third party) even if such damage or loss was
    reasonably foreseeable or Xilinx had been advised of the
    possibility of the same.
   
    CRITICAL APPLICATIONS
    Xilinx products are not designed or intended to be fail-
    safe, or for use in any application requiring fail-safe
    performance, such as life-support or safety devices or
    systems, Class III medical devices, nuclear facilities,
    applications related to the deployment of airbags, or any
    other applications that could lead to death, personal
    injury, or severe property or environmental damage
    (individually and collectively, "Critical
    Applications"). Customer assumes the sole risk and
    liability of any use of Xilinx products in Critical
    Applications, subject only to applicable laws and
    regulations governing limitations on product liability.
   
    THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
    PART OF THIS FILE AT ALL TIMES.                       */

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
#include "tx_chain.cpp"
#include "helper_functions.h"
#include "xrt/experimental/xrt_aie.h"
#include "xrt/experimental/xrt_bo.h"
#include "xrt/experimental/xrt_kernel.h"
#include "adf/adf_api/XRTConfig.h"

#include <adf.h>

using namespace std;

#define NUM_INPUT_SAMPLES 10624
#define NUM_OUTPUT_SAMPLES 65536
#define XRT_SUCCESS 0
#define ALL_MESSAGES


// load_xclbin function is used to read in xclbin file
// TODO: move to function file where readIntegersFromFile() is defined  
static std::vector<char> load_xclbin(xrtDeviceHandle device, const std::string& fnm)
{
  if (fnm.empty()) {
    cout << "A72-Error: <XCLBIN_FILE> is not specified" << endl;
    throw std::runtime_error("<XCLBIN_FILE> is not specified");
  }
  
  // load bitstream
  std::ifstream stream(fnm);
  stream.seekg(0,stream.end);
  size_t size = stream.tellg();
  stream.seekg(0,stream.beg);
  
  std::vector<char> header(size);
  stream.read(header.data(),size);
  
  auto top = reinterpret_cast<const axlf*>(header.data());
  
  //Download FPGA image (xclbin) to the device and check error
  if (xrtDeviceLoadXclbin(device, top)) {
    cout << "A72-Error: xclLoadXclBin failed, bitstream download unsuccessful" << endl;    
    throw std::runtime_error("A72-Error: Bitstream download failed");
  }
  
  return header;
}

/*******************************************************************************
 ** Main Function
 *******************************************************************************/
int main(int argc, char** argv) {
#ifdef ALL_MESSAGES
  cout << "A72-Info: ============================================================= " << endl;
  cout << "A72-Info: (Step 1) Include the AI Engine Graph Application              " << endl; 
  cout << "                   and XRT and ADF Headers                              " << endl;
  cout << "A72-Info: ============================================================= " << endl;
#endif

  constexpr const unsigned int NUM_ELEMS=1024;
  
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
  
  // ------------------------------------------------------------------
  // Step 3.2: Create PL Kernel Handles that point to the correct location
  //           in XCLBIN file for each data mover kernel
  // ------------------------------------------------------------------

  //open PL kernels and obtain handles
  xrtKernelHandle s2mm_khdl  = xrtPLKernelOpen(dhdl, top->m_header.uuid, "s2mm");
  xrtKernelHandle mm2s0_khdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, "mm2s:{mm2s_0}");
  xrtKernelHandle mm2s1_khdl = xrtPLKernelOpen(dhdl, top->m_header.uuid, "mm2s:{mm2s_1}");
  
 
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
    xrtBufferHandle lte0_bohdl = xrtBOAlloc(dhdl, sizeof(uint32_t)*NUM_INPUT_SAMPLES,  0, XCL_BO_FLAGS_CACHEABLE);
    xrtBufferHandle lte1_bohdl = xrtBOAlloc(dhdl, sizeof(uint32_t)*NUM_INPUT_SAMPLES,  0, XCL_BO_FLAGS_CACHEABLE);
    xrtBufferHandle dpd0_bohdl = xrtBOAlloc(dhdl, sizeof(uint64_t)*NUM_OUTPUT_SAMPLES, 0, XCL_BO_FLAGS_CACHEABLE);
 
  //Memory map BOs into user's address space (DDR Memory)
    auto lte0_mapped = reinterpret_cast<uint32_t*>(xrtBOMap(lte0_bohdl));
    auto lte1_mapped = reinterpret_cast<uint32_t*>(xrtBOMap(lte1_bohdl));
    auto dpd0_mapped = reinterpret_cast<uint32_t*>(xrtBOMap(dpd0_bohdl));

    std::cout << "A72-Info: Processed BOs (lte0_mapped, lte1_mapped, and dpd0_mapped)\n";
  

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
  cout << "A72-Info: Reset the AI Engine Array" << endl;
  system("./versal_run/reset-board.sh");
  //xrtResetAIEArray(dhdl);

  //Wait
  usleep(1000);

  // ------------------------------------------------------------------------------------
  // Step 5.2: Use the XRT Run Time Graph Control API calls to run the dataflow graph 
  //           o) Read the coeffients from text files
  //           o) Open the graph, run the graph
  //           o) Update the coeffients in the DPD filter AI Engine subgraph 
  // ------------------------------------------------------------------------------------

  //read coeffcients from data.txt files
  int32_t coeffs1[NUM_ELEMS];
  if (! readIntegersFromFile("data/coefs_4c2l_ps01.txt", coeffs1, NUM_ELEMS)) {
    cerr << "A72-Error: Error reading coefs_4c2l_ps01.txt data file." << endl;
    cout << "A72-Error: Error reading coefs_4c2l_ps01.txt data file." << endl;  
    return EXIT_FAILURE;
  }
  int32_t coeffs2[NUM_ELEMS];
  if (! readIntegersFromFile("data/coefs_4c2l_ps23.txt", coeffs2, NUM_ELEMS)) {
    cerr << "A72-Error: Error reading coefs_4c2l_ps23.txt data file." << endl;    
    cout << "A72-Error: Error reading coefs_4c2l_ps23.txt data file." << endl;  
    return EXIT_FAILURE;
  }
  
  //Open the TX Chain graph and obtain its handle 
  cout << "A72-Info: Initializing the tx_chain0 graph..." << endl;  

  auto graphHandle = xrtGraphOpen(dhdl, top->m_header.uuid, "tx_chain0");
  if (graphHandle == NULL) {
    throw std::runtime_error("A72-Error: Unable to open graph handle");
    cout << "A72-Error: Unable to open graph handle" << endl;  
    return EXIT_FAILURE;
  }
  
  //Starts the TX Chain graph execution. 
  //Intializes AI Engine cores and routes datapaths between AI Engine cores 
  //AI Engine cores are then stalled until coefficents are given from PS 
  cout << "A72-Info: Running the tx_chain0 graph..." << endl; 

  auto errCode = xrtGraphReset(graphHandle);
  if (errCode != XRT_SUCCESS) {
    throw std::runtime_error("A72-Error: Unable to reset graph");
    cout << "A72-Error: Unable to reset graph" << endl;  
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
    
  //Update the coeffients in the DPD filter AI Engine subgraph 
  cout << "A72-Info: Updating the TX Chain graph with coeffs1 and coeffs2..." << endl;
  std::string port_c2l_0;
  std::string port_c2l_1;
  port_c2l_0 = "tx_chain0.dpd_filter.c2l[0].in[0]"; 
  port_c2l_1 = "tx_chain0.dpd_filter.c2l[1].in[0]";	
  
  errCode = xrtGraphUpdateRTP(graphHandle, port_c2l_0.c_str(), reinterpret_cast<const char *>(coeffs1), 4096);
  if (errCode != XRT_SUCCESS) {
    throw std::runtime_error("Unable to update rtp");
    return EXIT_FAILURE;
  }
  
  errCode = xrtGraphUpdateRTP(graphHandle, port_c2l_1.c_str(), reinterpret_cast<const char *>(coeffs2), 4096);
  if (errCode != XRT_SUCCESS) {
    throw std::runtime_error("Unable to update rtp");
    return EXIT_FAILURE;
  }
  
  // ============================================================================
  // Step 6: Write input user data to DDR (via mapped input buffer handlers) 
  //      o) Read data from text files
  //      o) Write data to buffer handles (lte0, lte1) which are mapped to DDR
  //      o) Sync BOs with device 
  // ============================================================================
#ifdef ALL_MESSAGES
  cout << endl;
  cout << "A72-Info: ============================================================= " << endl;
  cout << "A72-Info: (Step 6) Write input user data to DDR (via mapped " << endl;
  cout << "A72-Info:          input buffer handles)"                                 << endl;
  cout << "A72-Info: ============================================================= " << endl;
#endif


  // ------------------------------------------------------------------------------------
  // Step 6.1: Read user lte0 and lte1 data from text files
  // ------------------------------------------------------------------------------------
  std::ifstream lte0_file("./data/input0_hex");
  std::ifstream lte1_file("./data/input1_hex");
  cout << "A72-Info: Done reading input0_hex and input1_hex files" << endl;
  if((!lte0_file) || (!lte1_file)) {
    cout << "A72-Error: Unable to open input#_hex files" << endl; 
    return EXIT_FAILURE;
  }


  // ------------------------------------------------------------------------------------
  // Step 6.2: Write user lte0 and lte1 data to BOs mapped to DDR
  // ------------------------------------------------------------------------------------  
  cout << "A72-Info: Storing lt0 data in DDR ..." << endl;
  for(unsigned int i = 0; i < NUM_INPUT_SAMPLES; i++) {
    unsigned int ltedata;
    std::istringstream strInput;
    std::string strInp;
    getline(lte0_file, strInp);
    strInput.str(strInp);
    strInput >> hex >> ltedata;
    lte0_mapped[i] = ltedata; // Store data in DDR
    strInput.str("");
  }
  
  cout << "A72-Info: Storing lt1 data in DDR ..." << endl;
  for(unsigned int i = 0; i < NUM_INPUT_SAMPLES; i++) {
    unsigned int ltedata;
    std::istringstream strInput;
    std::string strInp;
    getline(lte1_file, strInp);
    strInput.str(strInp);
    strInput >> hex >> ltedata;
    lte1_mapped[i] = ltedata; // Store data in DDR
    strInput.str("");
  }
    

  // ------------------------------------------------------------------------------------
  // Step 6.3: Synchronize buffer contents between BOs and device. This ensures that  
  //           the device has the same data as the BOs.This operation may require
  //           DMA to the device or CPU cache flushing/invalidation.             
  // ------------------------------------------------------------------------------------  

  //Synchronize buffer contents in requested direction (XCL_BO_SYNC_BO_TO_DEVICE)
  errCode=xrtBOSync(lte0_bohdl, XCL_BO_SYNC_BO_TO_DEVICE, sizeof(uint32_t)*NUM_INPUT_SAMPLES, 0);
  if (errCode != XRT_SUCCESS) {
    cout << endl << "A72-ERROR: Failed to synchronize lte0 BO" << endl << endl;
    return EXIT_FAILURE;
  }

  errCode=xrtBOSync(lte1_bohdl, XCL_BO_SYNC_BO_TO_DEVICE, sizeof(uint32_t)*NUM_INPUT_SAMPLES, 0);
  if (errCode != XRT_SUCCESS) {
    cout << endl << "A72-ERROR: Failed to synchronize lte1 BO" << endl << endl;
    return EXIT_FAILURE;
  }
  
  errCode=xrtBOSync(dpd0_bohdl, XCL_BO_SYNC_BO_TO_DEVICE, sizeof(uint64_t)*NUM_OUTPUT_SAMPLES, 0);  
  if (errCode != XRT_SUCCESS) {
    cout << endl << "A72-ERROR: Failed to synchronize dpd0 BO" << endl << endl;
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

  cout << "A72-Info: Start mm2s_0, mm2s_1, and s2mm kernels ..." << endl; 
  
  //Create a kernel run handle to start each data mover pl kernel
  //mm2s kernels moves data from DDR to AI Engine
  //s2mm kernel moves data from AI Engine to DDR 

    int rval;
    xrtRunHandle mm2s0_rhdl = xrtRunOpen(mm2s0_khdl);
    rval = xrtRunSetArg(mm2s0_rhdl, 0, lte0_bohdl);
    rval = xrtRunSetArg(mm2s0_rhdl, 2, NUM_INPUT_SAMPLES);

    xrtRunHandle mm2s1_rhdl = xrtRunOpen(mm2s1_khdl);
    rval = xrtRunSetArg(mm2s1_rhdl, 0, lte1_bohdl);
    rval = xrtRunSetArg(mm2s1_rhdl, 2, NUM_INPUT_SAMPLES);



    xrtRunHandle s2mm_rhdl = xrtRunOpen(s2mm_khdl);
    rval = xrtRunSetArg(s2mm_rhdl, 0, dpd0_bohdl);
    rval = xrtRunSetArg(s2mm_rhdl, 2, NUM_OUTPUT_SAMPLES/4);

    xrtRunStart(mm2s0_rhdl);
    xrtRunStart(mm2s1_rhdl);
    xrtRunStart(s2mm_rhdl);




  cout << "A72-Info: Done with xrtKernelRun handles" << endl; 
  
  //Wait for a mm2s_0, mm2s_1, and s2mm kernels' execution to finish
  //Close the run handles obtained by in xrtKernelRun()  
  //Close opened kernel handles obtained from xrtPLKernelOpen()
  auto state_mm2s_0 = xrtRunWait(mm2s0_rhdl);
  std::cout << "A72-Info: mm2s0 completed with status(" << state_mm2s_0 << ")" <<  endl;
  xrtRunClose(mm2s0_rhdl);
  xrtKernelClose(mm2s0_khdl);

  auto state_mm2s_1 = xrtRunWait(mm2s1_rhdl);
  std::cout << "A72-Info: mm2s1 completed with status(" << state_mm2s_1 << ")" << endl;
  xrtRunClose(mm2s1_rhdl);
  xrtKernelClose(mm2s1_khdl);

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
  // Step 8.1: Write lte0 and lte1 input data from DDR (via the mapped BOs)
  //           to log_lte0.txt and log_lte1.txt
  // ---------------------------------------------------------------------------

  //Instantiate streams to write lte0 and lte input data to text files 
  std::ofstream log_lte0, log_lte1;
  std::stringstream logDataIn0, logDataIn1 ;
  std::string logDataIn0_str, logDataIn1_str;
  
  logDataIn0 << logDataIn0_str << "./log_lte0";
  logDataIn0_str = logDataIn0.str();
  logDataIn1 << logDataIn1_str << "./log_lte1";
  logDataIn1_str = logDataIn1.str();
  
  log_lte0.open (logDataIn0_str);
  log_lte1.open (logDataIn1_str);

  //Write lte0 and lte1 mapped BOs' content to text files    
  for (unsigned int i = 0; i <NUM_INPUT_SAMPLES; i++) {
    log_lte0 << hex << lte0_mapped[i] << "\n";
    log_lte1 << hex << lte1_mapped[i] << "\n";
  }

  //Close text file streams
  log_lte0.close();
  log_lte1.close();
    
  // ---------------------------------------------------------------------------
  // Step 8.2: Write dpd0 output data from DDR (via the mapped BO)
  //           to log_output.txt
  // ---------------------------------------------------------------------------

  //Synchronize dpd0 buffer content in requested direction (XCL_BO_SYNC_BO_FROM_DEVICE)
  errCode=xrtBOSync(dpd0_bohdl, XCL_BO_SYNC_BO_TO_DEVICE, sizeof(uint64_t)*NUM_OUTPUT_SAMPLES, 0);  
  if (errCode != XRT_SUCCESS) {
    cout << endl << "A72-ERROR: Failed to synchronize dpd0 BO" << endl << endl;
    return EXIT_FAILURE;
  }


  //Instantiate stream to write dpd0 output data to text file log_output 
  std::ofstream dpdf;
  std::stringstream logDataOut;
  std::string logDataOut_str;
  logDataOut << logDataOut_str << "./log_output";
  logDataOut_str = logDataOut.str();
    
  dpdf.open(logDataOut_str);
  if(!dpdf) {
    cout << "A72-Error: Unable to open log_output file" << endl;
    return EXIT_FAILURE;
  }

  //Write dpd0 mapped BO content to text file        
  cout << "A72-Info: Store dpd0 output to log_output file" << endl;
  for(unsigned int i = 0; i < NUM_OUTPUT_SAMPLES; i++) {
    dpdf << hex << dpd0_mapped[i] << "\n";
  }
  
  //Close text file stream  
  dpdf.close();

  // ---------------------------------------------------------------------------
  // Step 8.2: Compare DPD output to Golden Reference data
  // ---------------------------------------------------------------------------
    
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

  //Determine if the design passed or failed
  if (ErrorCnt==0) {
    cout << "A72-Info: Test Passed for DPD0" << endl;
  } 
  else {
    cout << "A72-Info: Test Failed for DPD0" << endl;
  }

  //Close text file streams 
  dpd0_golden.close();
  dpdf_comp.close();
  
  // ============================================================================
  // Step 9: Release Allocated Resources
  // ============================================================================
#ifdef ALL_MESSAGES
  cout << endl;
  cout << "A72-Info: ============================================================= "   << endl;
  cout << "A72-Info: (Step 9) Release Allocated Resources " << endl;
  cout << "A72-Info: ============================================================= "   << endl;
#endif

    xrtBOFree(lte0_bohdl);
    xrtBOFree(lte1_bohdl);
    xrtBOFree(dpd0_bohdl);
    xrtGraphClose(graphHandle);
    xrtDeviceClose(dhdl);

  return EXIT_SUCCESS;
}
