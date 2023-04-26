/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
 
#include <iostream>
#include <stdio.h>
#include <stdint.h>

#include "packet_sender.hpp"

int main(void)
{
  
  FILE *fp;

  data_t idata[BURST_SIZE]; 
  data_t resulti[BURST_SIZE];
  data_t diff;
  
  fp=fopen("input_i0.dat","r");
  for (int i=0; i<BURST_SIZE; i++){
    data_t tmp;
    fscanf(fp, "%d", &tmp);
    idata[i] = tmp;
  }

  hls::stream<axis_pkt> rx("rx");

  axis_pkt x;

  for (int i =0; i<BURST_SIZE; i++) {
    x.data  = idata[i];
    x.keep = -1;
    x.last = 0;
    rx.write(x);
  }

  int num_cu = 10;
  hls::stream<axis_pkt> tx[num_cu];
  
  packet_sender(rx,
		tx[0],
		tx[1],
		tx[2],
		tx[3],
		tx[4],
		tx[5],
		tx[6],
		tx[7],
		tx[8],
		tx[9]
		);
  
  int total_error = 0; 
  int data_packet_num = 0; 
  for(int cu =0; cu < num_cu; cu++){
    for (int h = 0; h < 4; h++) 
      {
	axis_pkt temp = tx[cu].read();
	data_t header = temp.data;
	data_packet_num+=1;
	
	for (int i = 0; i < PACKET_LEN; i++)
	  {
	    axis_pkt temp = tx[cu].read();
	    resulti[i] = temp.data;
	    diff = idata[i+(h*PACKET_LEN)] ^ resulti[i];
	    bool tlast = temp.last;
	    if (diff!=0) 
	      { 
		total_error += 1;
	      } 
	    std::cout << std::dec << "packet" << ":" << data_packet_num; 
	    std::cout << " COMPUTE UNIT:" << std::dec<< cu; 
	    std::cout << " HEADER:" << std::hex << header; 
	    std::cout << " TLAST:" << std::hex<< tlast; 
	    std::cout << " rx_strm:" << std::hex << idata[i+(h*PACKET_LEN)];
	    std::cout << " tx_strm:" << std::hex << resulti[i];
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

