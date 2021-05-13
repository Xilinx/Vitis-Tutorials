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
#include "graph_pl.h"
#include <unistd.h>
#include <fstream>
#if !defined(__AIESIM__) && !defined(__ADF_FRONTEND__)
    #include "adf/adf_api/XRTConfig.h"
    #include "experimental/xrt_kernel.h"
#endif

using namespace adf;
GMIO gmioInPL("gmioInPL_0", 256, 1000);
GMIO gmioOutPL("gmioOutPL_0", 256, 100);
adf::simulation::platform<1,1> platform(&gmioInPL,&gmioOutPL);

mygraph_pl gr_pl;
adf::connect<> c0(platform.src[0], gr_pl.inMem);
adf::connect<> c1(gr_pl.inoutMem,platform.sink[0]);

const int ITERATION=2;
const int BLOCK_SIZE_in_Bytes=2048;
const int TOTAL_REPEAT=4;

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

    gr_pl.init();

    int error=0;
    adf::return_code ret;

    int32* inMem;
    int32* outMem;
    
    inMem=(int32*)GMIO::malloc(BLOCK_SIZE_in_Bytes);
    outMem=(int32*)GMIO::malloc(BLOCK_SIZE_in_Bytes);

    for(int iter=0;iter<TOTAL_REPEAT;iter++){
    	for(int j=0;j<BLOCK_SIZE_in_Bytes/sizeof(int32);j++){
		inMem[j]=j+iter;
    	}
    	std::cout<<"Graph PL run start"<<std::endl;
	gr_pl.update(gr_pl.k_mm2s.in[1],ITERATION*64);
	gr_pl.update(gr_pl.k_s2mm.in[1],ITERATION*64);
    	ret=gmioInPL.pl_gm(inMem, BLOCK_SIZE_in_Bytes);
    	if(ret!=adf::ok){
		std::cout<<"GMIO pl_gm error"<<std::endl;
		return ret;
    	}
    	ret=gmioOutPL.pl_gm(outMem, BLOCK_SIZE_in_Bytes);
    	std::cout<<"GMIO::pl_gm enqueing completed"<<std::endl;
    	ret=gr_pl.run(ITERATION);
    	ret=gr_pl.wait();
    	std::cout<<"Graph PL run end"<<std::endl;
  
    	for(int j=0;j<BLOCK_SIZE_in_Bytes/sizeof(int32);j++){
		if(outMem[j]!=j+iter+1){
			std::cout<<"ERROR:dout["<<j<<"]="<<outMem[j]<<std::endl;
  			error++;
  		}
    	}
    }
    GMIO::free(inMem);
    GMIO::free(outMem);
    std::cout<<"GMIO::free completed"<<std::endl;
  
#if !defined(__AIESIM__) && !defined(__ADF_FRONTEND__)
    xrtDeviceClose(dhdl);
#endif

    if(error==0){
  	  std::cout<<"PASS!"<<std::endl;
    }else{
  	  std::cout<<"ERROR!"<<std::endl;
    }
  
    return error;
};
