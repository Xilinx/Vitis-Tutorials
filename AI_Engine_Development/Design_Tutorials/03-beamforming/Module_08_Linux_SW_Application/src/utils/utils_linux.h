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

#ifndef __PLNX_UTILS_H__
#define __PLNX_UTILS_H__

/********************* Include Files *************/
#include <iostream>
#include <iomanip>

#include <fstream>
#include <sstream>

extern "C"
  {
	#include <errno.h>
	#include <fcntl.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <sys/mman.h>
	#include <unistd.h>
	#include <stdint.h>
	#include <xaiengine.h>
  }

/***************** Macros (Inline Functions) Definitions *********************/

#define memptr_WriteReg(BaseAddress, RegOffset, Data) \
    *((BaseAddress) + (RegOffset)) = (u32)(Data)
#define memptr_ReadReg(BaseAddress, RegOffset) \
    *((BaseAddress) + (RegOffset))

/**************************** Type Definitions ******************************/

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

extern std::ofstream log_plnx;
extern std::stringstream logPlnx;
extern std::string logPlnx_str;

int extractIQ(unsigned int dataHex, int lsb) ;

#endif //  __PLNX_UTILS_H__
