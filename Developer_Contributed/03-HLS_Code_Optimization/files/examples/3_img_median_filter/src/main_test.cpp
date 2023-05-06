/*******************************************************************************
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*******************************************************************************/

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "median.h"
#include "ap_bmp.h"


//pix_t rgb2y(RGB_t pix)
//{
//	pix_t y;
//
//	// y = ((66 * pix.R + 129 * pix.G + 25 * pix.B + 128) >> 8) + 16;
//	y = ((76 * pix.R + 150 * pix.G + 29 * pix.B + 128) >> 8) ;
//	return y;
//}



int main(void)
{

  int  x,     y;
  int  width, height;
  char *tempbuf;
  char *tempbuf1;
  char *tempbuf2;
  char *tempbuf3;
  char *tempbuf4;

  FILE *fp;

  int check_results = 0;
  int diff_R, diff_G, diff_B;

  tempbuf  = (char *) malloc(MAX_PATH * sizeof(char));
  tempbuf1 = (char *) malloc(MAX_PATH * sizeof(char));

  // Arrays to store image data
  unsigned char *R, *G, *B;

  //Arrays to send and receive data from the accelerator
  pix_t in_pix[MAX_HEIGHT][MAX_WIDTH];
  pix_t out_pix[MAX_HEIGHT][MAX_WIDTH];
  pix_t ref_pix[MAX_HEIGHT][MAX_WIDTH];

  R = (unsigned char *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(unsigned char));
  G = (unsigned char *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(unsigned char));
  B = (unsigned char *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(unsigned char));

  //Get image data from disk
  sprintf(tempbuf,  "%s.bmp", INPUT_IMAGE_BASE);

  // Fill a frame with data
  int read_tmp = BMP_Read(tempbuf, MAX_HEIGHT, MAX_WIDTH, R, G, B);
  if(read_tmp != 0) {
    printf("%s Loading image failed\n", tempbuf);
    exit (1);
  }
  printf("Loaded image file %s of size %4d %4d\n", tempbuf, MAX_HEIGHT, MAX_WIDTH);

  // Copy Input Image to pixel data structure
  // Hardware accelerator works on video pixel streams
   for(y = 0; y < MAX_HEIGHT; y++){
      for(x = 0; x < MAX_WIDTH; x++){
      RGB_t pixel;
      pixel.R = (pix_t) R[y*MAX_WIDTH + x];
      pixel.G = (pix_t) G[y*MAX_WIDTH + x];
      pixel.B = (pix_t) B[y*MAX_WIDTH + x];

	  in_pix[y][x] = pixel.G; //rgb2y(pixel);
      }
   }


#ifdef __SYNTHESIS__
   width  = 1920;
   height = 1080;
#else
   width  = 1920;
   height = 1080;
#endif



   printf("Running the Design Under Test Median[%d][%d] on image size of W=%4d H=%4d\n", KMED, KMED, width, height);
   top_median(in_pix, out_pix, height, width); //, fp);
   ref_median(in_pix, ref_pix, height, width); //, fp);


  // Copy Output video pixel stream to Output Image data structure
  for(y =0; y < height; y++){
    for(x = 0; x < width; x++){
	  pix_t pixel = out_pix[y][x];
      R[y*MAX_WIDTH + x] = (unsigned char) pixel;
      G[y*MAX_WIDTH + x] = (unsigned char) pixel;
      B[y*MAX_WIDTH + x] = (unsigned char) pixel;
    }
  }

  //Write the output median image back to disk
  sprintf(tempbuf1, "%s.bmp", OUTPUT_IMAGE_BASE);
  int write_tmp = BMP_Write(tempbuf1, MAX_HEIGHT, MAX_WIDTH, R, G, B);
  if(write_tmp != 0){
    printf("WriteBMP %s failed\n", tempbuf1);
    exit(1);
  }


  check_results = 0;
  for (y = KMED;   y < height-KMED; y++)
  {
    for (x = KMED; x < width -KMED; x++)
	{
 	  diff_R = ref_pix[y][x] - out_pix[y][x];
	  diff_R = ABS(diff_R);
	  int tot_diff = diff_R;
	  check_results = check_results  + diff_R;
	  //if (tot_diff > 0)
	  //{
		 //printf("x=%3d y=%3d Y=%3d rY=%3d\n", x, y, out_pix[y][x], ref_pix[y][x]);
		 //printf("diffR=%3d\n", diff_R);
		 //break;
	  //}
    }
  }
  printf("median test done\n");


  free(R);
  free(G);
  free(B);
  free(tempbuf);
  free(tempbuf1);

  if (check_results > 0)
  {
    printf("TEST FAILED!: error = %d\n", check_results);
    check_results = 1;
  }
  else
  {
    printf("TEST SUCCESSFUL!\n");
	check_results = 0;
  }

  return check_results;

}
