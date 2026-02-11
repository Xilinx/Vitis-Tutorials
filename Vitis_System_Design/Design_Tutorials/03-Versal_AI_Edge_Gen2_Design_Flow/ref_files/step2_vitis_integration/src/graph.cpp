/*
Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include "graph.h"





myGraph gr;


 
 

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char ** argv) {


    int32* inputArray = (int32*)GMIO::malloc(256*sizeof(int32));





for (int i=0; i<128; i++)
    {
	inputArray[i] = i+1;
    }


	gr.init();
//	GMIO init removed
	gr.gm0.gm2aie_nb(&inputArray[0], 128*sizeof(int32));
    gr.run(4);
	
    
	gr.end();
	
	GMIO::free(inputArray);
	return 0;
}
#endif
