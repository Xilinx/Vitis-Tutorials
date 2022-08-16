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
#if !defined(__AIESIM__) && !defined(__ADF_FRONTEND__)
    #include "adf/adf_api/XRTConfig.h"
    #include "experimental/xrt_kernel.h"
#endif

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
#if !defined(__AIESIM__) && !defined(__ADF_FRONTEND__)
    // Create XRT device handle for ADF API
    char* xclbinFilename = argv[1];
    auto dhdl = xrtDeviceOpen(0);//device index=0
    xrtDeviceLoadXclbinFile(dhdl,xclbinFilename);
    xuid_t uuid;
    xrtDeviceGetXclbinUUID(dhdl, uuid);
    adf::registerXRT(dhdl, uuid);
#endif

    gr.init();

    int32* dinArray=(int32*)GMIO::malloc(BLOCK_SIZE_in_Bytes);
    int32* doutArray=(int32*)GMIO::malloc(BLOCK_SIZE_in_Bytes);
    int32* doutRef=(int32*)malloc(BLOCK_SIZE_in_Bytes);
    std::cout<<"GMIO::malloc completed"<<std::endl;

    int error=0;
    int32 coeff[8]={1,2,3,4,5,6,7,8};

    for(int i=0;i<ITERATION*1024/4;i++){
	dinArray[i]=i;
    }

    gmioIn.gm2aie_nb(dinArray,BLOCK_SIZE_in_Bytes);
    gr.run(ITERATION);
    gmioOut.aie2gm_nb(doutArray,BLOCK_SIZE_in_Bytes);
    //PS can do other tasks here when data is transferring
    gmioOut.wait();

    ref_func(dinArray,coeff,doutRef,ITERATION*1024/4);
    for(int i=0;i<ITERATION*1024/4;i++){
	if(doutArray[i]!=doutRef[i]){
		std::cout<<"ERROR:dout["<<i<<"]="<<doutArray[i]<<",gold="<<doutRef[i]<<std::endl;
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
#if !defined(__AIESIM__) && !defined(__ADF_FRONTEND__)
    xrtDeviceClose(dhdl);
#endif

    return error;
};
