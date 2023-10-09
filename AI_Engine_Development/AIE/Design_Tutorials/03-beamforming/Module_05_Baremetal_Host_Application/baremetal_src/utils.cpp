
/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

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



