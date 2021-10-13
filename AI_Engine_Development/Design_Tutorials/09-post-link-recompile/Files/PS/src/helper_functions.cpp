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

#include "helper_functions.h"

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
    // cout << array[i] << endl;
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

std::vector<char> load_xclbin(xrtDeviceHandle device, const std::string& fnm)
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
