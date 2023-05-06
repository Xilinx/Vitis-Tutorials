/*******************************************************************************
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*******************************************************************************/

#ifndef __XLNX__BITMAP__
#define __XLNX__BITMAP__

// Basic color definitions
#define BLACK 0
#define WHITE 255

// Maximum image size
#define MAX_ROWS 1080
#define MAX_COLS 1920

//File Information Header
typedef struct{
  unsigned short FileType;
  unsigned int FileSize;
  unsigned short Reserved1;
  unsigned short Reserved2;
  unsigned short Offset;
}BMPHeader;

typedef struct{
  unsigned int Size;
  unsigned int Width;
  unsigned int Height;
  unsigned short Planes;
  unsigned short BitsPerPixel;
  unsigned int Compression;
  unsigned int SizeOfBitmap;
  unsigned int HorzResolution;
  unsigned int VertResolution;
  unsigned int ColorsUsed;
  unsigned int ColorsImportant;
}BMPImageHeader;

typedef struct{
  BMPHeader *file_header;
  BMPImageHeader *image_header;
  unsigned int *colors;
  unsigned char *data;
  unsigned char R[MAX_ROWS][MAX_COLS];
  unsigned char G[MAX_ROWS][MAX_COLS];
  unsigned char B[MAX_ROWS][MAX_COLS];
  unsigned char Y[MAX_ROWS][MAX_COLS];
  char U[MAX_ROWS][MAX_COLS];
  char V[MAX_ROWS][MAX_COLS];
}BMPImage;

//Read Function
int BMP_Read(char *file, int row, int col, unsigned char *R, unsigned char *G, unsigned char *B);

//Write Function
int BMP_Write(char *file, int row, int col, unsigned char *R, unsigned char *G, unsigned char *B);

#endif
