/*  (c) Copyright 2014 - 2019 Xilinx, Inc. All rights reserved.
   
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

#include "tx_chain.h"
#include "helper_functions.h"
using namespace adf ;


// ********************************************************************************** //
// ---------------------------------------------------------------------------------- //
//                    A I    E N G I N E    A P P L I C A T I O N                     //
// ---------------------------------------------------------------------------------- //
// ********************************************************************************** //


    // ============================================================================
    // Step 1: Connect graph to simulation platform
    // ============================================================================
    // o) Declare PLIOs and use them to declare the simulation platform 
    // o) Instantiate tx chain graph
    // o) Connect simulation platform to tx chain graph
    // ============================================================================

//PLIO pointers
PLIO *in0  = new PLIO("lte_0", adf::plio_32_bits, "data/input0.txt",153.6 );
PLIO *in1  = new PLIO("lte_1", adf::plio_32_bits, "data/input1.txt", 153.6);
PLIO *out0 = new PLIO("dpd_out0", adf::plio_64_bits, "data/output0.txt", 500);

//platform
simulation::platform<2, 1> platform(in0, in1, out0);

//tx chain graph 
Adf_DFE::tx_chain_200MHz<0, 0, 1> tx_chain0;

//connect platform source and sink ports to tx_chain graph
connect<> net00(platform.src[0], tx_chain0.in[0]);
connect<> net01(platform.src[1], tx_chain0.in[1]);
connect<> net02(tx_chain0.out, platform.sink[0]);

    // ============================================================================
    // Step 2: Use the Run Time Graph Control API calls to run the dataflow graph 
    // ============================================================================
    // o) Read the coeffients from text files
    // o) Initialize the graph, run the graph
    // o) Update the coeffients in the DPD filter subgraph 
    // ============================================================================

#ifdef __AIESIM__
int main(int argc, char ** argv) {
 #define NUM_ELEMS 1024
  int32_t coeffs1[NUM_ELEMS];
  if (! readIntegersFromFile("../design/aie_src/data/coefs_4c2l_ps01.txt", coeffs1, NUM_ELEMS)) {
    cerr << "main(): Error reading input data file." << endl;
    return 1;
  }

  int32_t coeffs2[NUM_ELEMS];
  if (! readIntegersFromFile("../design/aie_src/data/coefs_4c2l_ps23.txt", coeffs2, NUM_ELEMS)) {
    cerr << "main(): Error reading input data file." << endl;
    return 1;
  }

    tx_chain0.init();    
    tx_chain0.run();  
    tx_chain0.update(tx_chain0.dpd_filter.fromPS[0], coeffs1, NUM_ELEMS);
    tx_chain0.update(tx_chain0.dpd_filter.fromPS[1], coeffs2, NUM_ELEMS);
    tx_chain0.end();  
    
    return 0;
}
#endif
