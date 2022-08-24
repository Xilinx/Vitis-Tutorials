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
const uint32 pktType=0;

void aie_core3(input_pktstream *in,output_pktstream *out){
	readincr(in);//read header and discard
	uint32 ID=getPacketid(out,0);//for output pktstream
	uint32 header=generateHeader(pktType,ID); //Generate header for output
	writeincr(out,header);

	bool tlast;
	for(int i=0;i<8;i++){
		int32 tmp=readincr(in,tlast);
		tmp+=3;
		writeincr(out,tmp,i==7);//TLAST=1 for last word
	}
}
