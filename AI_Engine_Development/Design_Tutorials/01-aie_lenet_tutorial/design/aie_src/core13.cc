/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/


#include <stdlib.h>
#include <stdio.h>

#include "core13.h"

#ifndef INLINE
INLINE_DECL void core13(
        const int A,
        const int B,
        const int C,
        const int D,
        const int E,
        const int F,
        const int G,
        const int H,
        int8_t* A_in,
        int8_t* C_out1,
        int8_t* C_out2,
        int shift
) {      
  int8_t* ptrA = A_in;//+(4*G*H);
  int8_t* ptrC1 = C_out1;
  int8_t* ptrC2 = C_out2;//ptrC1 + (E * F);
  int i,j,k, l, m, n, o, p, q, r;

  for(i=0;i<E/B;i++) {
    for(j=0;j<F/A;j++) {
      for(k=0;k<A*B;k++) {
        if (i > 0) {
          *ptrC1 = 0;
          ptrC1++;
          *ptrC2 = 0;
          ptrC2++;          
        }
        else if ((i == 0) && (k >= A)) {
          *ptrC1 = 0;
          ptrC1++;
          *ptrC2 = 0;
          ptrC2++;
        }
        else if ((i == 0) && (j == (F/A - 1))) {
          if (k == 0) {
            *ptrC1 = 8;
            ptrC1++;
            *ptrC2 = 8;
            ptrC2++;
          }
          else {
            *ptrC1 = 0;
            ptrC1++;
            *ptrC2 = 0;
            ptrC2++;
          }
        }
        else {
          l = (A * j) + k;
          m = 0;
          o = l/16;
          n = l % 16;
          p = n/D;
          q = o/C;
          r = p * (H/C) + q;
          *ptrC1 = *(ptrA + (r * C * D) + (n % D)*C + (o % C));
          ptrC1++;
          *ptrC2 = *(ptrA + (r * C * D) + (n % D)*C + (o % C));;
          ptrC2++;
        }
      }
    }
  }
}
#endif

void core13_top(input_window_int32 *in, output_window_int32 *out1, output_window_int32 *out2){
	const int A = 8;
 	const int B = 4;
 	const int C = 4;
 	const int D = 4;
 	const int E = 8;
 	const int F = 264;
 	const int G = 16;
 	const int H = 16;
   int shift = 7; 
   core13(A, B, C, D, E, F, G, H, (int8_t *) in -> ptr, (int8_t *) out1 -> ptr, (int8_t *) out2 -> ptr, shift);
}
