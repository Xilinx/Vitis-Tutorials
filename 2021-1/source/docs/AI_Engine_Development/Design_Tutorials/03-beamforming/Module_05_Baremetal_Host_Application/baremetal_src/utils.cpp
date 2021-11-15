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

#include "utils.h"

int extractIQ(unsigned int dataHex, int lsb) {
	//xil_printf("[extractIQ]---------------\n");
	int data;
	if (lsb==0)	data = int(dataHex & 0xffff);
	else		data = int((dataHex & 0xffff0000)>>16 );
	//xil_printf("dataHex:0x%x data:0x%x data:%d\n", dataHex, data, data);
	if (data>=0x8000)
		data = -(0x10000 - data);
	return data;
}



