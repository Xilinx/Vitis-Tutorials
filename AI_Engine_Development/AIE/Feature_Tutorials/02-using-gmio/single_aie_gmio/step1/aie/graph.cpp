/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include <adf.h>
#include "graph.h"
#include <unistd.h>
#include <fstream>

using namespace adf;

mygraph gr;

void ref_func(int32* din,int32 c[8],int32* dout,int size){
	static int32 d[8]={0,0,0,0,0,0,0,0};
	for(int i=0;i<size;i++){
		for(int j=0;j<7;j++){
			d[j]=d[j+1];
		}
		d[7]=*(din+i);
		int32 tmp=0;
		for(int j=0;j<8;j++){
			tmp+=d[j]*c[j];
		}
		*(dout+i)=tmp;
	}
}
const int ITERATION=1;
const int BLOCK_SIZE_in_Bytes=ITERATION*1024;
int main(int argc, char ** argv) {
    gr.init();

    int32* dinArray=(int32*)GMIO::malloc(BLOCK_SIZE_in_Bytes);
    int32* doutArray=(int32*)GMIO::malloc(BLOCK_SIZE_in_Bytes);
    int32* doutRef=(int32*)malloc(BLOCK_SIZE_in_Bytes);
    std::cout<<"GMIO::malloc completed"<<std::endl;

    int error=0;
    int32 coeff[8]={1,2,3,4,5,6,7,8};

    for(int i=0;i<4;i++){
    	for(int j=0;j<ITERATION*1024/4;j++){
		dinArray[j]=j+i;
    	}

    	gr.gmioIn.gm2aie(dinArray,BLOCK_SIZE_in_Bytes);
    	gr.run(ITERATION);
    	gr.gmioOut.aie2gm(doutArray,BLOCK_SIZE_in_Bytes);

    	ref_func(dinArray,coeff,doutRef,ITERATION*1024/4);
    	for(int j=0;j<ITERATION*1024/4;j++){
		if(doutArray[j]!=doutRef[j]){
			std::cout<<"ERROR:dout["<<j<<"]="<<doutArray[j]<<",gold="<<doutRef[j]<<std::endl;
			error++;
		}
    	}
    }
    
    std::cout<<"GMIO transactions finished"<<std::endl;

    GMIO::free(dinArray);
    GMIO::free(doutArray);
 
    gr.end();
    if(error==0){
	std::cout<<"PASS!"<<std::endl;
    }else{
	std::cout<<"ERROR!"<<std::endl;
    }

    return error;
};
