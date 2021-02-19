#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <chrono>

#include "xcl2.hpp" 
#include "cmdlineparser.h" 
#include "opencv2/opencv.hpp"

#include "coefficients.h"
#include "common.h" 

using namespace sda;
using namespace sda::utils;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

static void IplImage2Raw(IplImage* img, uchar* y_buf, int stride_y, uchar* u_buf, int stride_u, uchar* v_buf, int stride_v)
{
  // Assumes RGB or YUV 4:4:4
  for (int y = 0; y < img->height; y++)
  {
    for (int x = 0; x < img->width; x++)
    {
      CvScalar cv_pix = cvGet2D(img, y, x);
      y_buf[y*stride_y+x] = (uchar)cv_pix.val[0];
      u_buf[y*stride_u+x] = (uchar)cv_pix.val[1];
      v_buf[y*stride_v+x] = (uchar)cv_pix.val[2];
    }
  }
}

static void Raw2IplImage(uchar* y_buf, int stride_y, uchar* u_buf, int stride_u, uchar* v_buf, int stride_v, IplImage* img )
{
  // Assumes RGB or YUV 4:4:4
  for (int y = 0; y < img->height; y++)
  {
    for (int x = 0; x < img->width; x++)
    {
      CvScalar cv_pix;
      cv_pix.val[0] = y_buf[y*stride_y+x];
      cv_pix.val[1] = u_buf[y*stride_u+x];
      cv_pix.val[2] = v_buf[y*stride_v+x];
      cvSet2D(img, y, x, cv_pix);
    }
  }
}


static void writeRawImage(
	unsigned width, unsigned height, unsigned stride, unsigned depth, unsigned nchannels, 
	uchar* y_buf, uchar* u_buf, uchar* v_buf, std::string filename) 
{
  IplImage *dst = cvCreateImage(cvSize(width, height), depth, nchannels);

  // Convert processed image from Raw to cvImage
  Raw2IplImage(y_buf, stride, u_buf, stride, v_buf, stride, dst);

  // Conver to cvMat
  cvConvert( dst, cvCreateMat(height, width, CV_32FC3 ) );

  // Write to disk
  cvSaveImage(filename.c_str(), dst);
}


// -------------------------------------------------------------------------------------------
// Class used to manage requests to the Filter2D kernel
// -------------------------------------------------------------------------------------------

class Filter2DRequest 
{
    cl::Kernel             kernel;
    cl::CommandQueue       q;
    cl::Buffer             coef_buffer;
    cl::Buffer             src_buffer;
    cl::Buffer             dst_buffer;
    std::vector<cl::Event> events;

  public:
    Filter2DRequest(cl::Context &context, cl::Program &program, cl::CommandQueue &queue) 
    {
      cl_int err;

      q = queue;

      OCL_CHECK(err, kernel = cl::Kernel(program,"Filter2DKernel", &err));

      // Allocate input and output buffers
      OCL_CHECK(err, coef_buffer = cl::Buffer(context, CL_MEM_READ_ONLY,  (FILTER_V_SIZE*FILTER_V_SIZE)*sizeof(char), nullptr, &err));
      OCL_CHECK(err, src_buffer  = cl::Buffer(context, CL_MEM_READ_ONLY,  (1920*1080)*sizeof(char), nullptr, &err));
      OCL_CHECK(err, dst_buffer  = cl::Buffer(context, CL_MEM_WRITE_ONLY, (1920*1080)*sizeof(char), nullptr, &err));

      // Set kernel arguments - this pins the buffers to specific global memory banks
      OCL_CHECK(err, err = kernel.setArg(0, coef_buffer));
      OCL_CHECK(err, err = kernel.setArg(6, src_buffer));
      OCL_CHECK(err, err = kernel.setArg(7, dst_buffer));

      // Make buffers resident in the device
      // If done after setArg, then buffers are pinned and runtime knows in which bank they should be made resident,
      // removing the need for using the vendor extensions to explicitely map to DDR.
      OCL_CHECK(err, err = q.enqueueMigrateMemObjects({coef_buffer, src_buffer, dst_buffer}, CL_MIGRATE_MEM_OBJECT_CONTENT_UNDEFINED));

      // Make sure buffers are migrated before continuing
      q.finish();
    };
 
    void Filter2D(
      const char        coeffs[FILTER_V_SIZE][FILTER_H_SIZE],
      float             factor,
      short             bias,
      unsigned short    width,
      unsigned short    height,
      unsigned short    stride,
      unsigned char    *src,
      unsigned char    *dst ) 
    {

      assert(width  <= 1920);
      assert(height <= 1080);
      assert(stride%64 == 0);

      cl_int    err;
      cl::Event in1_event;
      cl::Event in2_event;
      cl::Event run_event;
      cl::Event out_event;

      size_t offset = 0;
      int nbytes = stride*height*sizeof(char);

      // If a previous transaction is pending, wait until it completes
      finish();

      // Set kernel arguments - since buffers are reused, no need to set these args each time...
      // OCL_CHECK(err, err = kernel.setArg(0, coef_buffer));
      OCL_CHECK(err, err = kernel.setArg(1, factor));
      OCL_CHECK(err, err = kernel.setArg(2, bias));
      OCL_CHECK(err, err = kernel.setArg(3, width));
      OCL_CHECK(err, err = kernel.setArg(4, height));
      OCL_CHECK(err, err = kernel.setArg(5, stride));
      // OCL_CHECK(err, err = kernel.setArg(6, src_buffer));
      // OCL_CHECK(err, err = kernel.setArg(7, dst_buffer));

      // Schedule the writing of the inputs from host to device
      OCL_CHECK(err, err = q.enqueueWriteBuffer(coef_buffer, CL_FALSE, offset, (FILTER_V_SIZE*FILTER_V_SIZE)*sizeof(char), &coeffs[0][0], nullptr, &in1_event) );
      OCL_CHECK(err, err = q.enqueueWriteBuffer(src_buffer,  CL_FALSE, offset, nbytes, src, nullptr, &in2_event) );
      events.push_back(in1_event);
      events.push_back(in2_event);

      // Schedule the execution of the kernel
      OCL_CHECK(err, err = q.enqueueTask(kernel, &events, &run_event));
      events.push_back(run_event);

      // Schedule the reading of the outputs from device back to host
      OCL_CHECK(err, err = q.enqueueReadBuffer(dst_buffer, CL_FALSE, offset, nbytes, dst, &events, &out_event) );
      events.push_back(out_event);       
    }

    void finish()
    {
      if (events.size()>0) {
        events.back().wait();
        events.clear();
        if (getenv("XCL_EMULATION_MODE") != NULL) {
          printf("  finished Filter2DRequest\n");
        }        
      }
    }
};

// -------------------------------------------------------------------------------------------
// Class used to manage multiple simultaneous request to the Filter2D kernel
// Implements SW pipelining and scales with multiple CUs
// -------------------------------------------------------------------------------------------

class Filter2DDispatcher
{
  std::vector<Filter2DRequest> req;
  int max;
  int cnt;

  public:
    Filter2DDispatcher(cl::Context &context, cl::Program &program, cl::CommandQueue &queue, int nreqs) 
    {
      cnt = 0;
      max = nreqs; 
      for(int i=0; i<max; i++) {
        req.push_back( Filter2DRequest(context, program, queue) );
      }     
    };
 
    int operator () (
      const char        coeffs[FILTER_V_SIZE][FILTER_H_SIZE],
      float             factor,
      short             bias,
      unsigned short    width,
      unsigned short    height,
      unsigned short    stride,
      unsigned char    *src,
      unsigned char    *dst ) 
    {
      cnt++;
      req[cnt%max].Filter2D(coeffs, factor, bias, width, height, stride, src, dst);
      return (cnt%max);
    }

    void finish(int id) {
      if (id<max) {
        req[id].finish();
      }
    }

    void finish() {
      for(int i=0; i<max; i++) {
        req[i].finish();
      }      
    }
};


int main(int argc, char** argv)
{
  printf("----------------------------------------------------------------------------\n");
  printf("\n"); 
  printf("Xilinx 2D Filter Example Application\n"); 
  printf("\n"); 
        
  // ---------------------------------------------------------------------------------
  // Parse command line
  // ---------------------------------------------------------------------------------

  CmdLineParser parser;
  parser.addSwitch("--nruns",   "-n", "Number of times to image is processed", "1");
  parser.addSwitch("--fpga",    "-x", "FPGA binary (xclbin) file to use");
  parser.addSwitch("--input",   "-i", "Input image file");
  parser.addSwitch("--filter",  "-f", "Filter type (0-6)", "0");
  parser.addSwitch("--maxreqs", "-r", "Maximum number of outstanding requests", "3");
  parser.addSwitch("--compare", "-c", "Compare FPGA and SW performance", "false", true);

  //parse all command line options
  parser.parse(argc, argv);
  string   fpgaBinary  = parser.value("fpga");
  string   inputImage  = parser.value("input");
  int      numRuns     = parser.value_to_int("nruns");
  unsigned filterType  = parser.value_to_int("filter");
  int      maxReqs     = parser.value_to_int("maxreqs");
  bool     comparePerf = parser.value_to_bool("compare");

  if (fpgaBinary.size() == 0) {
    printf("ERROR: FPGA binary file (.xclbin) must be specified with the -x command line switch\n");
    return -1;
  }
  if (inputImage.size() == 0) {
    printf("ERROR: input image file must be specified using -i command line switch\n");
    return -1;
  }
  if (filterType>6) {
    printf("ERROR: Supported filter type values are [0:6]\n");
    return -1;
  }

  printf("FPGA binary       : %s\n", fpgaBinary.c_str());
  printf("Input image       : %s\n", inputImage.c_str());
  printf("Number of runs    : %d\n", numRuns);
  printf("Filter type       : %d\n", filterType);
  printf("Max requests      : %d\n", maxReqs);
  printf("Compare perf.     : %d\n", comparePerf);
  printf("\n"); 
  
  // ---------------------------------------------------------------------------------
  // Read input image and format inputs
  // ---------------------------------------------------------------------------------
  
  std::string srcFileName = inputImage;

  // Read Input image
  IplImage *src;
  src = cvLoadImage(srcFileName.c_str()); //format is BGR
  if(!src) {
    printf("ERROR: Loading image %s failed\n", srcFileName.c_str());
    return -1;
  }
  unsigned width     = src->width;
  unsigned height    = src->height;
  unsigned depth     = src->depth;
  unsigned nchannels = src->nChannels;
  unsigned stride    = ceil(width/64.0)*64;
  unsigned nbytes    = (stride*height);

  // Input and output buffers (Y,U, V)
  unsigned char *y_src = (unsigned char *)malloc(nbytes);
  unsigned char *u_src = (unsigned char *)malloc(nbytes);
  unsigned char *v_src = (unsigned char *)malloc(nbytes);
  unsigned char *y_dst = (unsigned char *)malloc(nbytes);
  unsigned char *u_dst = (unsigned char *)malloc(nbytes);
  unsigned char *v_dst = (unsigned char *)malloc(nbytes);

  // Convert CV Image to AXI video data
  IplImage2Raw(src, y_src, stride, u_src, stride, v_src, stride);

  // Release allocated memory
  cvReleaseImage(&src);

  // Retrieve filter factor and bias
  float factor = filterFactors[filterType];
  short bias   = filterBiases[filterType]; 


  // ---------------------------------------------------------------------------------
  // Load XCLBIN file, create OpenCL context, device and program
  // ---------------------------------------------------------------------------------

  printf("Programming FPGA device\n");    
  cl_int err;
  std::vector<cl::Device> devices = xcl::get_xil_devices();
  devices.resize(1); // (arbitrarily) use the first Xilinx device that is found
  OCL_CHECK(err, cl::Context context(devices[0], NULL, NULL, NULL, &err));
  unsigned fileBufSize;
  char* fileBuf = xcl::read_binary_file(fpgaBinary.c_str(), fileBufSize);
  cl::Program::Binaries bins{{fileBuf, fileBufSize}};
  OCL_CHECK(err, cl::Program program(context, devices, bins, NULL, &err));
  OCL_CHECK(err, cl::CommandQueue queue(context, devices[0], cl::QueueProperties::Profiling | cl::QueueProperties::OutOfOrder, &err));


  // ---------------------------------------------------------------------------------
  // Make requests to kernel(s) 
  // ---------------------------------------------------------------------------------

  printf("Running FPGA accelerator on %d images\n", numRuns);    

  // Dispatcher of requests to the kernel
  // 'maxReqs' controls the maximum number of outstanding requests to the kernel 
  // and equates to the depth of SW pipelining.  
  Filter2DDispatcher Filter2DKernel(context, program, queue, maxReqs);

auto fpga_begin = std::chrono::high_resolution_clock::now();

  for(int n=0; n<numRuns; n++) 
  {
    // Enqueue independent requests to Blur Y, U and V planes
    // Requests will run sequentially if there is a single kernel
    // Requests will run in parallel is there are two or more kernels
    Filter2DKernel(filterCoeffs[filterType], factor, bias, width, height, stride, y_src, y_dst);
    Filter2DKernel(filterCoeffs[filterType], factor, bias, width, height, stride, u_src, u_dst);
    Filter2DKernel(filterCoeffs[filterType], factor, bias, width, height, stride, v_src, v_dst);
  }
  Filter2DKernel.finish();

auto fpga_end = std::chrono::high_resolution_clock::now();

  // Write image to file
  writeRawImage(width, height, stride, depth, nchannels, y_dst, u_dst, v_dst, inputImage.substr(0, inputImage.size()-4)+"_out.bmp");

  // ---------------------------------------------------------------------------------
  // Compute reference results and compare
  // ---------------------------------------------------------------------------------

  if (comparePerf) {
    printf("Running Software version\n");    
  }

  // Create output buffers for reference results
  unsigned char *y_ref = (unsigned char *)malloc(nbytes);
  unsigned char *u_ref = (unsigned char *)malloc(nbytes);
  unsigned char *v_ref = (unsigned char *)malloc(nbytes);

  unsigned int numRunsSW = comparePerf?numRuns:1;

auto cpu_begin = std::chrono::high_resolution_clock::now();

  #pragma omp parallel for num_threads(3)
  for(unsigned int n=0; n<numRunsSW; n++)
  {
    // Compute reference results
    Filter2D(filterCoeffs[filterType], factor, bias, width, height, stride, y_src, y_ref);
    Filter2D(filterCoeffs[filterType], factor, bias, width, height, stride, u_src, u_ref);
    Filter2D(filterCoeffs[filterType], factor, bias, width, height, stride, v_src, v_ref);
  }

auto cpu_end = std::chrono::high_resolution_clock::now();

  // Write image to file
  writeRawImage(width, height, stride, depth, nchannels, y_ref, u_ref, v_ref, inputImage.substr(0, inputImage.size()-4)+"_ref.bmp");

  printf("Comparing results\n");  

  // Compare results
  bool diff = false;
  for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
          if ( y_dst[y*stride+x] != y_ref[y*stride+x] ) diff = true;
          if ( u_dst[y*stride+x] != u_ref[y*stride+x] ) diff = true;
          if ( v_dst[y*stride+x] != v_ref[y*stride+x] ) diff = true;
      }
  }

  if(diff) {
      printf("\n%sTest FAILED: Output has mismatches with reference%s\n", RED, RESET);    
  } else {
      printf("\n%sTest PASSED: Output matches reference%s\n", GREEN, RESET);    
  }

  // Report performance (if not running in emulation mode)
  if (getenv("XCL_EMULATION_MODE") == NULL) {
      std::chrono::duration<double> fpga_duration = fpga_end - fpga_begin;
      std::chrono::duration<double> cpu_duration  = cpu_end  - cpu_begin;
      float fpga_throughput = (double) numRuns*3*nbytes / fpga_duration.count() / (1024.0*1024.0);
      float cpu_throughput  = (double) numRuns*3*nbytes / cpu_duration.count() / (1024.0*1024.0);
      printf("\n");
      printf("FPGA Time         : %10.4f s\n",    fpga_duration.count());
      printf("FPGA Throughput   : %10.4f MB/s\n", fpga_throughput);
      if (comparePerf) {
      printf("CPU  Time         : %10.4f s\n",    cpu_duration.count());
      printf("CPU  Throughput   : %10.4f MB/s\n", cpu_throughput);      
      printf("FPGA Speedup      : %10.4f x\n",    cpu_duration.count() / fpga_duration.count());  
      }    
  }

  printf("----------------------------------------------------------------------------\n");

  return (diff?1:0);
}


