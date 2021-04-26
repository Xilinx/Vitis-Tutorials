/*
 * Copyright 2021 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
