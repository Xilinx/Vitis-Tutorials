/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*/

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_defines.h"
#include "ap_bmp.h"


int main(void)
{

  int  x,     y;
  int  width, height;
  char *tempbuf;
  char *tempbuf1;

  FILE *fp;

  int check_results = 0;
  int diff_R, diff_G, diff_B, tot_err;
  int tot_diff_R=0;
  int tot_diff_B=0;
  int tot_diff_G=0;
  int tot_diff_H=0;
  int tot_diff_C=0;

  uint25_t cdf[GRAY_LEVELS]  = {0};
  uint19_t ref_hist[GRAY_LEVELS] = { 0 };
  uint25_t ref_cdf[GRAY_LEVELS] = { 0 };

  tempbuf  = (char *) malloc(MAX_PATH * sizeof(char));
  tempbuf1 = (char *) malloc(MAX_PATH * sizeof(char));

  // Arrays to read/write BMP image file
  uint8_t *R, *G, *B;
  //Arrays to send and receive data from the accelerator
  RGB_t *inp_img;
  RGB_t *ref_img;

  //memory allocation
  R = (uint8_t *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(uint8_t));
  G = (uint8_t *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(uint8_t));
  B = (uint8_t *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(uint8_t));
  inp_img = (RGB_t *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(RGB_t));
  ref_img = (RGB_t *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(RGB_t));
  uint8_t *inp_R = (uint8_t *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(uint8_t));
  uint8_t *inp_G = (uint8_t *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(uint8_t));
  uint8_t *inp_B = (uint8_t *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(uint8_t));
  uint8_t *out_R = (uint8_t *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(uint8_t));
  uint8_t *out_G = (uint8_t *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(uint8_t));
  uint8_t *out_B = (uint8_t *) malloc(MAX_HEIGHT * MAX_WIDTH * sizeof(uint8_t));

  //Get image data from file
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
        inp_R[y*MAX_WIDTH + x]   = R[y*MAX_WIDTH + x];
        inp_G[y*MAX_WIDTH + x]   = G[y*MAX_WIDTH + x];
        inp_B[y*MAX_WIDTH + x]   = B[y*MAX_WIDTH + x];
      inp_img[y*MAX_WIDTH + x].R = R[y*MAX_WIDTH + x];
      inp_img[y*MAX_WIDTH + x].G = G[y*MAX_WIDTH + x];
      inp_img[y*MAX_WIDTH + x].B = B[y*MAX_WIDTH + x];
      }
   }

   // if you want to save simulation time you can decrease both dimensions by 4
   width  = MAX_WIDTH;  //1920;
   height = MAX_HEIGHT; //1080;


   // compute the histogram and equalize the image to create golden results to check vs. the DUT
   ref_img_hist_equaliz( width, height, ref_hist, inp_img, ref_img);

   printf("Running the Design Under Test on image size of W=%4d H=%4d\n", width, height);
   wrapper_histogram_equaliz( width, height,ref_hist, inp_R,inp_G,inp_B, out_R,out_G,out_B);

   check_results = 0;
   for (y = 0;   y < height; y++)
    {
      for (x = 0; x < width; x++)
  	{
   	  diff_R = out_R[y*MAX_WIDTH + x] - ref_img[y*MAX_WIDTH + x].R;
  	  diff_R = ABS(diff_R);
  	  tot_diff_R = + diff_R;
   	  diff_G = out_G[y*MAX_WIDTH + x] - ref_img[y*MAX_WIDTH + x].G;
  	  diff_G = ABS(diff_G);
  	  tot_diff_G = + diff_G;
   	  diff_B = out_B[y*MAX_WIDTH + x] - ref_img[y*MAX_WIDTH + x].B;
  	  diff_B = ABS(diff_B);
  	  tot_diff_B = + diff_B;
      }
    }
    tot_err = tot_diff_R + tot_diff_G + tot_diff_B;
    printf("\ntotal error in images = %d\n", tot_err);


  // Copy Output video stream to Image data structure
  for(y =0; y < height; y++){
    for(x = 0; x < width; x++){
      R[y*MAX_WIDTH + x] = out_R[y*MAX_WIDTH + x];
      G[y*MAX_WIDTH + x] = out_G[y*MAX_WIDTH + x];
      B[y*MAX_WIDTH + x] = out_B[y*MAX_WIDTH + x];
    }
  }
  //Write the output image back to disk
  sprintf(tempbuf1, "%s.bmp", OUTPUT_IMAGE_BASE);
  int write_tmp = BMP_Write(tempbuf1, MAX_HEIGHT, MAX_WIDTH, R, G, B);
  if(write_tmp != 0){
    printf("WriteBMP %s failed\n", tempbuf1);
    exit(1);
  }
  // Copy reference video stream to  Image data structure
  for(y =0; y < height; y++){
    for(x = 0; x < width; x++){
      R[y*MAX_WIDTH + x] = ref_img[y*MAX_WIDTH + x].R;
      G[y*MAX_WIDTH + x] = ref_img[y*MAX_WIDTH + x].G;
      B[y*MAX_WIDTH + x] = ref_img[y*MAX_WIDTH + x].B;
    }
  }
  //Write the reference image back to disk
  sprintf(tempbuf1, "%s.bmp", REFER_IMAGE_BASE);
  write_tmp = BMP_Write(tempbuf1, MAX_HEIGHT, MAX_WIDTH, R, G, B);
  if(write_tmp != 0){
    printf("WriteBMP %s failed\n", tempbuf1);
    exit(1);
  }

  //free memory
  free(R); free(G); free(B);
  free(tempbuf); free(tempbuf1);
  free(inp_R); free(inp_G); free(inp_B);
  free(out_R); free(out_G); free(out_B);
  free(ref_img); free(inp_img);

  if (tot_err > 0)
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
