/*  (c) Copyright 2021 Xilinx, Inc. All rights reserved.
   
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


#ifndef __HELPER_FUNCTIONS_H__
#define __HELPER_FUNCTIONS_H__


#include <fstream>
#include <iostream>
#include <sstream>
using namespace std ;

bool extractFirstInteger(string str, int& num)
{
  stringstream ss;    
  ss << str; // Storing the whole string into string stream
  string temp = "";
  while (!ss.eof()) {  // Running loop till the end of the stream
    ss >> temp;  // Extracting word by word from stream
    if (stringstream(temp) >> num)  // Checking the given word is integer or not
      return true;
    temp = "";  // To save from space at the end of string
  }
  return false;
}

bool readIntegersFromFile(string filename, int32_t* array, int numelts)
{
  ifstream myfile(filename);
  if (!(myfile.is_open())) {
    cerr << "readIntegersFromFile(): Unable to open input data file." << endl;
    return false ;
  }
  string line;
  int i = 0;
  while (getline(myfile, line)) {  // Read file line-by-line
    stringstream ss;
    ss << line;
    if (! extractFirstInteger(line, array[i])) {  // Find first integer in the line
      cerr << "readIntegersFromFile(): Unable to find integer in " << line << endl;
      return false ;
    }
    
    i++;
    if (i == numelts)  break;  // Read only the first <numelts> integers
  }
  if (i < numelts) {
    cerr << "readIntegersFromFile(): Found only " << i << " integers in input data file, expected " << numelts << endl;
    return false ;
  }
  myfile.close();
  return true;
}

#endif //HELPER_FUNCTIONS_H__
