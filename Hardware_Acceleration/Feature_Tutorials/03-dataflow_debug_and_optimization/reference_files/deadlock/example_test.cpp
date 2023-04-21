/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11 
 */
#include <stdio.h>
#include "hls_stream.h"


#define SIZE 10
extern void example(hls::stream<int>& A, hls::stream<int>& B);

int main()
{
  int i;
  hls::stream<int> A;
  hls::stream<int> B;
  
  int time = 0;
  for   (time = 0 ; time < 4; time ++) {
      for(i=0; i < SIZE; i++){
        A << (i + time);
      }

      example(A,B);

  }
  return 0;
}
