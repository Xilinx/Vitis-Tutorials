/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "globals.h"

int main()
{
  int i;
  int count;

  dint A[4096];
  dint B[4096];
  dint CA[4096];
  dint CB[4096];

  axi_stream  SA;
  axi_stream  SB;

  ap_axis<DW,0,0,0>  d_in;
  ap_axis<DW,0,0,0>  d_out;

  count = 0;

  //Put data into A
  for(i=0; i < 64; i++){
    A[i] = i;
  }

  for(i=0; i < 16; i++){
      d_in.data = i;
      d_in.keep = 255;
      if (i == 15)
    	 d_in.last = 1;
      else
    	 d_in.last = 0;
      SB.write(d_in);
   }

  //Call the hardware function
  dma_hls(A, B, SA, SB, 16, 16);

  //Run a software version of the hardware function to validate results
  //Run a software version of the hardware function to validate results
  for(i=0; i < 16; i++){
    CA[i] = i;
  }

  for(i=0; i < 16; i++){
    CB[i] = i;
  }

  for(i=0; i < 256; i++){
     count = count + 1;
   }

/*
  //Compare results
  if (SA.empty() == false)
  {
	  for(i=0; i < 16; i++)
	  {
		  d_out = SA.read();
		  if(CA[i] != d_out.data)
		  {
				printf("i = %d  stream data= %d CA= %d\n",i,d_out.data,CA[i]);
				printf("ERROR HW and SW results mismatch\n");
				return 1;
		  }
		  else
			  	printf("i = %d  stream data= %d CA= %d\n",i,d_out.data,CA[i]);

	  }


  for(i=0; i < 16; i++)
  {
		if(B[i] != CB[i])
		{
		  printf("i = %d B = %d CB= %d\n",i,B[i],CB[i]);
		  printf("ERROR HW and SW results mismatch\n");
		  return 1;
		}
		else
			 printf("i = %d mem data = %d CB= %d\n",i,B[i],CB[i]);
	}
  }
  */
}

  


