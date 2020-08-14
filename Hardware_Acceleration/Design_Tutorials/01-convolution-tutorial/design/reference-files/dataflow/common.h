#pragma once
#include <tuple>

#include "constants.h"
#include "types.h"

struct arguments {
  // The path to the input video
  char *input_file;

  // The path to the output video
  char *output_file;

  // The width of the processed image
  int width;

  // The height of the processed image
  int height;

  // The width of the input video
  int in_width;

  // The height of the input video
  int in_height;

  // The number of frames to process
  int nframes;

  // The name of the kernel
  char* kernel_name;

  // The number of compute units on the binary
  int ncompute_units;

  // The path to the xclbin or awsxcbin file
  char* binary_file;

  // If true the output will be converted to grayscale
  bool gray;

  // If true prints extra information about the program
  bool verbose;
};

// Parses the command line arguments
arguments parse_args(int argc, char* argv[]);

// Prints the progress of an operation
void print_progress(int cnt, int total);

// Returns an input and output pipe stream.
std::tuple<FILE*, FILE*> get_streams(arguments &args);

// Method for performing convolution
void convolve(FILE* streamIn, FILE* streamOut,
              float* filter, int filter_size,
              arguments args);
