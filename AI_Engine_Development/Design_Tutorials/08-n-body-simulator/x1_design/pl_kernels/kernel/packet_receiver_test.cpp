/* (c) Copyright 2021 Xilinx, Inc.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 * 
 * 		http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */

#include <iostream>
#include <stdio.h>
#include <stdint.h>

#include "packet_receiver.hpp"

int main(void)
{
  
  FILE *fp;

  data_t idata[BURST_SIZE]; 
  data_t resulti[BURST_SIZE];
  data_t diff;
  data_t headers[4];
  std::string headers_str[4] = {"8fff0000","0fff0001", "0fff0002","8fff0003"}; 

  for (int i =0; i < 4; i++) {
    std::istringstream converter(headers_str[i]);
    unsigned int value;
    converter >> std::hex >> value;
    headers[i] = value; 
  }  

  fp=fopen("input_i0.dat","r");
  for (int i=0; i<BURST_SIZE; i++){
    data_t tmp;
    fscanf(fp, "%d", &tmp);
    idata[i] = tmp;
  }
  
  
  hls::stream<axis_pkt> rx("rx");
  hls::stream<axis_pkt> tx[4]; 

  axis_pkt x;
  
  for (int h = 0; h < 4; h++)
    {
        x.data  = headers[h];
	std::cout << x.data << std::endl;
        x.keep = -1;
        x.last = 0;
	rx.write(x);

	for (int i =0; i<PACKET_LEN; i++) {
	  x.data  = idata[i+(h*PACKET_LEN)];
	  x.keep = -1;
	  x.last = (i==PACKET_LEN-1);
	  rx.write(x);
	}
    }  
  
  packet_receiver(rx,tx[0],tx[1],tx[2],tx[3]);
  int data_packet_num = 0 ; 
  int total_error = 0;
  for(int h = 0; h <4; h++) {
    for(int i =0; i<PACKET_LEN; i++){     
      axis_pkt temp = tx[h].read();
      resulti[i] = temp.data;
      diff = idata[i+(h*PACKET_LEN)] ^ resulti[i];
      if (diff!=0) 
	{ 
	  total_error += 1;
	} 
      std::cout << std::dec << "packet" << ":" << data_packet_num; 
      std::cout << " rx_strm:" << std::hex << idata[i+(h*PACKET_LEN)];
      std::cout << " tx_strm["<<h<<"]:" << std::hex << resulti[i];
      std::cout << std::dec << " diff:" << diff;
      std::cout << " err:" << total_error << std::endl;
      data_packet_num+=1; 
    }
  }
  if (total_error == 0)   {
    printf(" **** TEST PASSED **** \n");
    return 0; 
  }
  else   {
    printf(" **** TEST FAILED **** \n");
    return 1; 
  } 
}
