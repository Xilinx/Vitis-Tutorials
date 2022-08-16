/**********
© Copyright 2020 Xilinx, Inc.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**********/
#include <adf.h>
#include "graph.h"
#include <unistd.h>
#include <fstream>

using namespace adf;
GMIO gmioIn("gmioIn",64,1000);
GMIO gmioOut("gmioOut",64,1000);
adf::simulation::platform<1,1> platform(&gmioIn,&gmioOut);

mygraph gr;
adf::connect<> net0(gr.out, platform.sink[0]);
adf::connect<> net1(platform.src[0], gr.din);
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
const int ITERATION=4;
const int BLOCK_SIZE_in_Bytes=ITERATION*1024;
int main(int argc, char ** argv) {
    gr.init();

    int32* dinArray=(int32*)GMIO::malloc(BLOCK_SIZE_in_Bytes);
    int32* doutArray=(int32*)GMIO::malloc(BLOCK_SIZE_in_Bytes);
    int32* doutRef=(int32*)malloc(BLOCK_SIZE_in_Bytes);
    std::cout<<"GMIO::malloc completed"<<std::endl;

    int error=0;
    int32 coeff[8]={1,2,3,4,5,6,7,8};

    for(int j=0;j<ITERATION*1024/4;j++){
	dinArray[j]=j;
    }

    gmioIn.gm2aie_nb(dinArray,BLOCK_SIZE_in_Bytes);//Transfer all blocks input data at a time
    gr.run(ITERATION);
    gmioOut.aie2gm(doutArray,BLOCK_SIZE_in_Bytes);//Transfer all blocks output data at a time

    ref_func(dinArray,coeff,doutRef,ITERATION*1024/4);
    for(int j=0;j<ITERATION*1024/4;j++){
	if(doutArray[j]!=doutRef[j]){
		std::cout<<"ERROR:dout["<<j<<"]="<<doutArray[j]<<",gold="<<doutRef[j]<<std::endl;
		error++;
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
