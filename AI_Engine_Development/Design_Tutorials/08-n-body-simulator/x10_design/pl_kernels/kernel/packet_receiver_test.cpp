/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
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
  
  int num_cu = 10; 
  hls::stream<axis_pkt> rx[num_cu];
  hls::stream<axis_pkt> tx[4]; 

  axis_pkt x;
  
  for( int cu=0; cu <num_cu; cu++){
    for (int h = 0; h < 4; h++)
      {
        x.data  = headers[h];
	std::cout << x.data << std::endl;
        x.keep = -1;
        x.last = 0;
	rx[cu].write(x);
	
	for (int i =0; i<PACKET_LEN; i++) {
	  x.data  = idata[i+(h*PACKET_LEN)];
	  x.keep = -1;
	  x.last = (i==PACKET_LEN-1);
	  rx[cu].write(x);
	}
    }  
  }
  
  packet_receiver(rx[0],
		  rx[1],
		  rx[2],
		  rx[3],
		  rx[4],
		  rx[5],
		  rx[6],
		  rx[7],
		  rx[8],
		  rx[9],
		  tx[0],
		  tx[1],
		  tx[2],
		  tx[3]);

  int data_packet_num = 0 ; 
  int total_error = 0;

  for(int h = 0; h <4; h++) {
    for( int cu =0; cu<num_cu; cu++){
      for(int i =0; i<PACKET_LEN; i++){     
	axis_pkt temp = tx[h].read();
	resulti[i] = temp.data;
	diff = idata[i+(h*PACKET_LEN)] ^ resulti[i];
	if (diff!=0) 
	  { 
	    total_error += 1;
	  } 
	std::cout << std::dec << "Compute Unit:" << ":" << cu; 
	std::cout << std::dec << "packet" << ":" << data_packet_num; 
	std::cout << " rx_strm:" << std::hex << idata[i+(h*PACKET_LEN)];
	std::cout << " tx_strm["<<h<<"]:" << std::hex << resulti[i];
	std::cout << std::dec << " diff:" << diff;
	std::cout << " err:" << total_error << std::endl;
	data_packet_num+=1; 
      }
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

