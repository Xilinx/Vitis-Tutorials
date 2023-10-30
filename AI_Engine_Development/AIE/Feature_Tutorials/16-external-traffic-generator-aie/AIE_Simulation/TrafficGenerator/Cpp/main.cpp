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
#include "input_cls.h"
#include "xtlm_ipc.h"

void aie_func()
{
	xtlm_ipc::axis_master in_interpolator("in_interpolator");
	xtlm_ipc::axis_slave out_interpolator("out_interpolator");

	xtlm_ipc::axis_master in_classifier("in_classifier");
	xtlm_ipc::axis_slave out_classifier("out_classifier");

	for (int i=0; i<4; i++){

		for(int i=0; i<512; i+=2) {
				std::vector<int16_t> classifier_data = std::vector<int16_t> (cint16InputCls.begin() + i, cint16InputCls.begin() + (i+2));
				std::vector<char> classifier_byte_array = in_classifier.int16ToByteArray(classifier_data);
				in_classifier.send_data(classifier_byte_array, true);
		}
		std::vector<char> data_out_cls;
		out_classifier.receive_data_with_size(data_out_cls, 1024);


		for(int i=0; i<256; i+=2){
				std::vector<int16_t> in_data = std::vector<int16_t> (cint16Input.begin() + i, cint16Input.begin() + (i+2));
				std::vector<char> interpolator_byte_array = in_interpolator.int16ToByteArray(in_data);
				in_interpolator.send_data(interpolator_byte_array, true);
		}
		std::vector<char> data_out; 
		out_interpolator.receive_data_with_size(data_out, 1024);
	}
}

int main(int argc, char *argv[])
{


			std::cout << "\n\n-----------------------------------\n\n Traffic Generator Start \n\n-------------------------------------\n\n" << std::endl;
			aie_func();

			std::cout << "\n\n-----------------------------------\n\n Traffic Generator end \n\n---------------------------------------\n\n" << std::endl;
	return(0);
}
