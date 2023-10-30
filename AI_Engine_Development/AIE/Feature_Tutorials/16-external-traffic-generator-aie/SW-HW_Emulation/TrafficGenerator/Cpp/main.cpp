/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#include <chrono>
#include <fstream>
#include <stdint.h>
#include <string>
#include <vector>
#include "input.h"
#include "xtlm_ipc.h"

int aie_func()
{
	xtlm_ipc::axis_master in_interpolator("in_interpolator");

	xtlm_ipc::axis_slave out_classifier("out_classifier");

	for (int i=0; i<4; i++){
					for(int i=0;i<256;i+=2){
								std::vector<int16_t> in_data = std::vector<int16_t> (cint16Input.begin() + i, cint16Input.begin() + (i+2));
								std::vector<char> interpolator_byte_array = in_interpolator.int16ToByteArray(in_data);
								in_interpolator.send_data(interpolator_byte_array, true);
					}
	std::vector<char> data_out; 
	out_classifier.receive_data_with_size(data_out, 1024);
	}
}

int main(int argc, char *argv[])
{


			std::cout << "\n\n-----------------------------------\n\n Run Traffic Generator \n\n---------------------------------------\n\n" << std::endl;
			aie_func();

			std::cout << "\n\n-----------------------------------\n\n Traffic Generator end \n\n---------------------------------------\n\n" << std::endl;
	return(0);
}
