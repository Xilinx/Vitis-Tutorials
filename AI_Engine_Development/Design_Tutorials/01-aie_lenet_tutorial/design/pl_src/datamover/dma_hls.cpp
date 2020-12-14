/* (c) Copyright 2020 Xilinx, Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */
				      
#include "globals.h"

#define keepsize(n)   (1<<n)
#define keepval(n)   (keepsize(n) - 1)

void dma_mm2s(volatile dint *mem_rd, axi_stream &strm_out, int size)
{
 ap_axis<DW,0,0,0>   	  s;
 dint					  mm_data;

 s.last = 0;
 s.keep = 0;

  	 for(int i = 0; i < size; i++)
 	 {
#pragma HLS PIPELINE II=1
		mm_data = mem_rd[i];
  		s.data = mm_data;
		s.keep = keepval(KW);
		if (i == size-1)
			s.last = 1;
		else
			s.last = 0;
        std::cout << "sdata = " << s.data << " skeep = " << s.keep << " slast = " << s.last << std::endl;
		strm_out.write(s);
	 }

}

void dma_s2mm(volatile dint *mem_wr, axi_stream &strm_in, int size)
{
	 ap_axis<DW,0,0,0>	   s;
	 for(int i = 0; i < size; i++)
	 {
#pragma HLS PIPELINE II=1
		s = strm_in.read();
		mem_wr[i] = s.data;
		std::cout << "mdata = " << s.data << std::endl;
		//if(s.last)
		//	break;
	 }
}

extern "C" {

void dma_hls(volatile dint *mem_rd,  volatile dint *mem_wr, axi_stream &strm_out, axi_stream &strm_in, int mem_rd_size, int mem_wr_size)
{
  
#pragma HLS INTERFACE m_axi port=mem_rd depth=4096 offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=mem_wr depth=4096 offset=slave bundle=gmem1

#pragma HLS INTERFACE axis port=strm_in
#pragma HLS INTERFACE axis port=strm_out

#pragma HLS INTERFACE s_axilite port=mem_rd bundle=control
#pragma HLS INTERFACE s_axilite port=mem_wr bundle=control
#pragma HLS INTERFACE s_axilite port=mem_rd_size bundle=control
#pragma HLS INTERFACE s_axilite port=mem_wr_size bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS DATAFLOW

  
  dma_mm2s (mem_rd, strm_out, mem_rd_size);
  dma_s2mm (mem_wr, strm_in, mem_wr_size);

}
}

  

