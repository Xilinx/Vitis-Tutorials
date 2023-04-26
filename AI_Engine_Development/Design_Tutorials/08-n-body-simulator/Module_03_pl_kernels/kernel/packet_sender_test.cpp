/*Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
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
  
  fp=fopen("m2s_i.txt","r");
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

  hls::stream<axis_pkt> tx[NUM_CU];
  
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
		tx[9],
		tx[10],
		tx[11],
		tx[12],
		tx[13],
		tx[14],
		tx[15],
		tx[16],
		tx[17],
		tx[18],
		tx[19],
		tx[20],
		tx[21],
		tx[22],
		tx[23],
		tx[24],
		tx[25],
		tx[26],
		tx[27],
		tx[28],
		tx[29],
		tx[30],
		tx[31],
		tx[32],
		tx[33],
		tx[34],
		tx[35],
		tx[36],
		tx[37],
		tx[38],
		tx[39],
		tx[40],
		tx[41],
		tx[42],
		tx[43],
		tx[44],
		tx[45],
		tx[46],
		tx[47],
		tx[48],
		tx[49],
		tx[50],
		tx[51],
		tx[52],
		tx[53],
		tx[54],
		tx[55],
		tx[56],
		tx[57],
		tx[58],
		tx[59],
		tx[60],
		tx[61],
		tx[62],
		tx[63],
		tx[64],
		tx[65],
		tx[66],
		tx[67],
		tx[68],
		tx[69],
		tx[70],
		tx[71],
		tx[72],
		tx[73],
		tx[74],
		tx[75],
		tx[76],
		tx[77],
		tx[78],
		tx[79],
		tx[80],
		tx[81],
		tx[82],
		tx[83],
		tx[84],
		tx[85],
		tx[86],
		tx[87],
		tx[88],
		tx[89],
		tx[90],
		tx[91],
		tx[92],
		tx[93],
		tx[94],
		tx[95],
		tx[96],
		tx[97],
		tx[98],
		tx[99]
		);
  
  int total_error = 0; 
  int data_packet_num = 0; 
  for(int cu =0; cu < NUM_CU; cu++){
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
    printf(" **** TEST PASSED **** ");
    return 0; 
  }
  else   {
    printf(" **** TEST FAILED **** ");
    return 1; 
  } 
  
}

