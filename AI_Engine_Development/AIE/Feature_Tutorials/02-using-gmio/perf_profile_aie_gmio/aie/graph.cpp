/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <adf.h>
#include "graph.h"
#include <fstream>
using namespace adf;

topgraph gr;

#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char ** argv) {
	const int NUM=4;
	int ITERATION=4;
	std::cout<<"ITERATION:"<<ITERATION<<std::endl;
	const int BLOCK_SIZE_in_Bytes=1024*ITERATION;
	const int BLOCK_SIZE_out_Bytes=1032*ITERATION;
    gr.init();
    int error=0;

    int32* dinArray[NUM];
    int32* doutArray[NUM];

    for(int i=0;i<NUM;i++){
		dinArray[i]=(int32*)GMIO::malloc(BLOCK_SIZE_in_Bytes);
		doutArray[i]=(int32*)GMIO::malloc(BLOCK_SIZE_out_Bytes);
    }
    std::cout<<"GMIO::malloc completed"<<std::endl;
    
    for(int i=0;i<NUM;i++){
		for(int j=0;j<BLOCK_SIZE_in_Bytes/sizeof(int);j++){
				dinArray[i][j]=j;
		}
    }
	gr.run(ITERATION);
	
	for(int i=0;i<NUM;i++){
		gr.gmioIn[i].gm2aie_nb(dinArray[i], BLOCK_SIZE_in_Bytes);
		gr.gmioOut[i].aie2gm_nb(doutArray[i], BLOCK_SIZE_out_Bytes);
	}

	for(int i=0;i<NUM;i++){
		gr.gmioOut[i].wait();
	}
	//check output correctness
	for(int i=0;i<NUM;i++){
	    for(int j=0;j<BLOCK_SIZE_out_Bytes/sizeof(int);j++){
			if(j%258!=256 && j%258!=257 && doutArray[i][j]!=j+1-j/258*2){
	  			std::cout<<"ERROR:dout["<<i<<"]["<<j<<"]="<<doutArray[i][j]<<std::endl;
	  			error++;
				break;
	  		}
		}
	}
	
    if(!error){
		std::cout<<"AIE GMIO PASSED!"<<std::endl;
    }else{
		std::cout<<"AIE GMIO FAILED!"<<std::endl;
    }
    for(int i=0;i<NUM;i++){
    	GMIO::free(dinArray[i]);
    	GMIO::free(doutArray[i]);
    }
    std::cout<<"GMIO::free completed"<<std::endl;

    if(error==0){
  	  std::cout<<"TEST PASSED!"<<std::endl;
    }else{
  	  std::cout<<"TEST FAILED!"<<std::endl;
    }

	gr.end();
  
    return error;
};
#endif
