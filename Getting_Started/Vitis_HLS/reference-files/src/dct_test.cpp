/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#include "dct.h"

// ********************************************************
int main() {
   short a[N], b[N], b_prime[N], x[10*N];
   int retval = 0, i, j, z;
   FILE *fp;

   fp=fopen("in.dat","r");
   //Read 640 entries from the File
   for (i=0; i<(10*N); i++){
      int tmp;
      fscanf(fp, "%d", &tmp);
      x[i] = tmp;
   }
   fclose(fp);


   //Call the DCT function 10 times for Dataflow
   for (i=0; i<10; i++){
	   printf("Checkpoint: i = %d\n", i);
	   // Iterate through the large x[] array to populate the smaller input array a[]
	   for (j=0; j<N; j++){
	      a[j] = x[j+(N*i)];
		   printf("Checkpoint: j = %d; ", j);
	   }//End j_loop
	   printf("\n");

	   //Call DCT for each iteration of a[]
	   dct(a, b);

	   //For testing purposes, save the first iteration of b[] to b_prime[]
	   //To compare to out.golden.dat
	   if (i==0){
		   printf("Copying array b to b_prime\n");
		   for (z=0; z<N; z++) {
				b_prime[z] = b[z];
		   }//End For
	   }//End IF
   }//End i_loop


	fp=fopen("out.dat","w");
	//printf(" Din Dout \n");
	for (i=0;i<N;i++) {
		fprintf(fp, "%d \n", b_prime[i]);
		//printf("  %d   %d\n", a[i], b[i]);
	}
	fclose(fp);

	// Compare the results file with the golden results
	retval = system("diff --brief -w out.dat out.golden.dat");
	if (retval != 0) {
		printf("Test failed  !!!\n"); 
		retval=1;
	} else {
		printf("Test passed !\n");
  }
   return retval;
}
