/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <chrono>
#include "logger.h"
#include "cmdlineparser.h" 

#include "xclbin_helper.h" 
#include "opencv2/opencv.hpp"

#include "coefficients.h"
#include "filter2d.h" 

using namespace sda;
using namespace sda::utils;

static void IplImage2Raw(IplImage* img, uchar* y, int stride_y, uchar* u, int stride_u, uchar* v, int stride_v);
static void Raw2IplImage(uchar* y, int stride_y, uchar* u, int stride_u, uchar* v, int stride_v, IplImage* img);

// -------------------------------------------------------------------------------------------
// An event callback function that prints the operations performed by the OpenCL runtime.
// -------------------------------------------------------------------------------------------
void event_cb(cl_event event, cl_int cmd_status, void *id) 
{
	if (getenv("XCL_EMULATION_MODE") != NULL) {
	 	std::cout << "  kernel finished processing request " << *(int *)id << std::endl;
	}
}

// -------------------------------------------------------------------------------------------
// Struct returned by BlurDispatcher() and used to keep track of the request sent to the kernel
// The sync() method waits for completion of the request. After it returns, results are ready
// -------------------------------------------------------------------------------------------
struct Filter2DRequest {

  cl_event mEvent[3];	
  int      mId;

  Filter2DRequest(int id) {
    mId = id;
  }	

  void sync()
  {
  	// Wait until the outputs have been read back
	clWaitForEvents(1, &mEvent[2]);
	clReleaseEvent(mEvent[0]);
   	clReleaseEvent(mEvent[1]);
   	clReleaseEvent(mEvent[2]);	
  }

};


// -------------------------------------------------------------------------------------------
// Class used to dispatch requests to the kernel
// The BlurDispatcher() method schedules the necessary operations (write, kernel, read) and
// returns a BlurRequest* struct which can be used to track the completion of the request. 
// The dispatcher has its own OOO command queue allowing multiple requests to be scheduled
// and executed independently by the OpenCL runtime.
// -------------------------------------------------------------------------------------------
class Filter2DDispatcher {

public:

  Filter2DDispatcher(
  	cl_device_id     &Device,
    cl_context       &Context,
  	cl_program       &Program )	
  {
	mKernel  = clCreateKernel(Program, "Filter2DKernel", &mErr);
	mQueue   = clCreateCommandQueue(Context, Device, CL_QUEUE_PROFILING_ENABLE, &mErr);
	mContext = Context;
	mCounter = 0;
  }
  
  Filter2DRequest* operator() (
  	short            *coeffs,
	unsigned char    *src,
	unsigned int      width,
	unsigned int      height,
	unsigned int      stride,
	unsigned char    *dst ) 
  { 
  
  	Filter2DRequest* req = new Filter2DRequest(mCounter++);

	unsigned nbytes = (stride*height);

	// Create input buffers for coefficients (host to device)
  	mSrcBuf[0] = clCreateBuffer(mContext, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,  (FILTER2D_KERNEL_V_SIZE*FILTER2D_KERNEL_V_SIZE)*sizeof(short), coeffs, &mErr);

  	// Create input buffer for src (host to device)
	mSrcBuf[1] = clCreateBuffer(mContext, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY,  nbytes, src, &mErr);

	// Create output buffer for dst (device to host)
	mDstBuf[0] = clCreateBuffer(mContext,CL_MEM_USE_HOST_PTR | CL_MEM_WRITE_ONLY, nbytes, dst, &mErr);
  
  	// Set the kernel arguments
  	clSetKernelArg(mKernel, 0, sizeof(cl_mem),       &mSrcBuf[0]);
  	clSetKernelArg(mKernel, 1, sizeof(cl_mem),       &mSrcBuf[1]);
  	clSetKernelArg(mKernel, 2, sizeof(unsigned int), &width);
  	clSetKernelArg(mKernel, 3, sizeof(unsigned int), &height);
  	clSetKernelArg(mKernel, 4, sizeof(unsigned int), &stride);
  	clSetKernelArg(mKernel, 5, sizeof(cl_mem),       &mDstBuf[0]);

	// Schedule the writing of the inputs
	clEnqueueMigrateMemObjects(mQueue, 2, mSrcBuf, 0, 0, nullptr,  &req->mEvent[0]);	

	// Schedule the execution of the kernel
	clEnqueueTask(mQueue, mKernel, 1,  &req->mEvent[0], &req->mEvent[1]);	
	
	// Schedule the reading of the outputs
  	clEnqueueMigrateMemObjects(mQueue, 1, mDstBuf, CL_MIGRATE_MEM_OBJECT_HOST, 1, &req->mEvent[1], &req->mEvent[2]);

	// Register call back to notify of kernel completion
	clSetEventCallback(req->mEvent[1], CL_COMPLETE, event_cb, &req->mId); 
	
	return req;
  }; 
 
  ~Filter2DDispatcher()
  {  
	clReleaseCommandQueue(mQueue);
	clReleaseKernel(mKernel);
  };  
  
private:
  cl_kernel         mKernel;
  cl_command_queue  mQueue;	
  cl_context        mContext;  
  cl_mem            mSrcBuf[2];
  cl_mem            mDstBuf[1]; 
  cl_int            mErr;
  int               mCounter; 
};



int main(int argc, char** argv)
{
	std::cout << std::endl;	
	std::cout << "Xilinx 2D Filter Example Application\n";
		
	// ---------------------------------------------------------------------------------
	// Parse command line
	// ---------------------------------------------------------------------------------

	CmdLineParser parser;
	parser.addSwitch("--nruns", "-n", "Number of times to image is processed", "1");
	parser.addSwitch("--fpga", "-x", "FPGA binary (xclbin) file to use", "xclbin/fpga.hw.xilinx_aws-vu9p-f1_4ddr-xpr-2pr_4_0.awsxclbin");
	parser.addSwitch("--input", "-i", "Input image file");
	parser.addSwitch("--filter", "-f", "Filter type (0-3)", "0");

	//parse all command line options
	parser.parse(argc, argv);
	string inputImage = parser.value("input");
	string fpgaBinary = parser.value("fpga");
	int    numRuns    = parser.value_to_int("nruns");
	int    coeffs     = parser.value_to_int("filter");

	if (inputImage.size() == 0) {
		std::cout << std::endl;	
		std::cout << "ERROR: input image file must be specified using -i command line switch" << std::endl;
		exit(1);
	}
	if ((coeffs<0) || (coeffs>3)) {
		std::cout << std::endl;	
		std::cout << "ERROR: Supported filter type values are [0:3]" << std::endl;
		exit(1);
	}

	std::cout << std::endl;	
	std::cout << "FPGA binary    : " << fpgaBinary << std::endl;
	std::cout << "Input image    : " << inputImage << std::endl;
	std::cout << "Number of runs : " << numRuns    << std::endl;
	std::cout << "Filter type    : " << coeffs     << std::endl;
	std::cout << std::endl;	
	
	
	// ---------------------------------------------------------------------------------
	// Load XCLBIN file, create OpenCL context, device and program
	// ---------------------------------------------------------------------------------

	std::cout << "Programming FPGA" << std::endl;
	cl_context    	context;
	cl_program    	program;
	cl_device_id	device;
	load_xclbin_file(fpgaBinary.c_str(), context, device, program);


	// ---------------------------------------------------------------------------------
	// Read input image and format inputs
	// ---------------------------------------------------------------------------------
	
	// Create filenames for input and ouput images
	std::string srcFileName  = inputImage;
	std::string dstFileName  = inputImage.substr(0, inputImage.size()-4)+"_out.bmp";

	// Read Input image
	IplImage *src, *dst;
	src = cvLoadImage(srcFileName.c_str()); //format is BGR
	if(!src) {
		std::cout << "ERROR: Loading image " << srcFileName << " failed" << std::endl;
		exit(1);
	}
	unsigned width  = src->width;
	unsigned height = src->height;
	unsigned stride = width;
	unsigned nbytes = (stride*height);

	// 4k aligned buffers for efficient data transfer to the kernel
	std::vector<uchar, aligned_allocator<uchar>> y_src(nbytes);
	std::vector<uchar, aligned_allocator<uchar>> u_src(nbytes);
	std::vector<uchar, aligned_allocator<uchar>> v_src(nbytes);
	std::vector<uchar, aligned_allocator<uchar>> y_dst(nbytes);
	std::vector<uchar, aligned_allocator<uchar>> u_dst(nbytes);
	std::vector<uchar, aligned_allocator<uchar>> v_dst(nbytes);
	std::vector<short, aligned_allocator<short>> coeff(FILTER2D_KERNEL_V_SIZE*FILTER2D_KERNEL_V_SIZE);


	// Create destination image
	dst = cvCreateImage(cvSize(width, height), src->depth, src->nChannels);

	// Convert CV Image to AXI video data
	IplImage2Raw(src, y_src.data(), stride, u_src.data(), stride, v_src.data(), stride);

	// Copy coefficients to 4k aligned vector
	memcpy(coeff.data() , &filterCoeffs[coeffs][0][0], coeff.size()*sizeof(short) );

	// ---------------------------------------------------------------------------------
	// Make requests to kernel(s) 
	// ---------------------------------------------------------------------------------
	// Note: change the number of kernels in the device, or reorder the sync() methods
	// to see the impact on performance and how requests are scheduled.
	// ---------------------------------------------------------------------------------

	std::cout << std::endl;	
	std::cout << "Running FPGA version" << std::endl;	

	// Create a dispatcher of requests to the Blur kernel(s) 
	Filter2DDispatcher Filter(device, context, program);

  auto fpga_begin = std::chrono::high_resolution_clock::now();

	Filter2DRequest* request[numRuns*3];
	for(int xx=0; xx<numRuns; xx++) 
	{
		// Make independent requests to Blur Y, U and V planes
		// Requests will run sequentially if there is a single CU
		// Requests will run in parallel if there are two or more CUs
		request[xx*3+0] = Filter(coeff.data(), y_src.data(), width, height, stride, y_dst.data());
		request[xx*3+1] = Filter(coeff.data(), u_src.data(), width, height, stride, u_dst.data());
		request[xx*3+2] = Filter(coeff.data(), v_src.data(), width, height, stride, v_dst.data());

		// Wait for completion of the outstanding requests
		request[xx*3+0]->sync();
		request[xx*3+1]->sync();
		request[xx*3+2]->sync();
	}


  auto fpga_end = std::chrono::high_resolution_clock::now();

	// ---------------------------------------------------------------------------------
	// Format output and write image out 
	// ---------------------------------------------------------------------------------

	// Convert processed image back to CV Image
	Raw2IplImage(y_dst.data(), stride, u_dst.data(), stride, v_dst.data(), stride, dst);

	// Convert image to cvMat and write it to disk
	cvConvert( dst, cvCreateMat(height, width, CV_32FC3 ) );
	cvSaveImage(dstFileName.c_str(), dst);


	// ---------------------------------------------------------------------------------
	// Compute reference results and compare 
	// ---------------------------------------------------------------------------------

	std::cout << std::endl;
   	std::cout << "Running Software version" << std::endl;

	// Create output buffers for reference results
	std::vector<uchar, aligned_allocator<uchar>> y_ref(nbytes);
	std::vector<uchar, aligned_allocator<uchar>> u_ref(nbytes);
	std::vector<uchar, aligned_allocator<uchar>> v_ref(nbytes);

  auto cpu_begin = std::chrono::high_resolution_clock::now();

	#pragma omp parallel for
	for(int xx=0; xx<numRuns; xx++) 
	{
		// Compute reference results
		Filter2D(filterCoeffs[coeffs], y_src.data(), width, height, stride, y_ref.data());
		Filter2D(filterCoeffs[coeffs], u_src.data(), width, height, stride, u_ref.data());
		Filter2D(filterCoeffs[coeffs], v_src.data(), width, height, stride, v_ref.data());
	}

  auto cpu_end = std::chrono::high_resolution_clock::now();

	std::string refFileName  = inputImage.substr(0, inputImage.size()-4)+"_ref.bmp";
	Raw2IplImage(y_ref.data(), stride, u_ref.data(), stride, v_ref.data(), stride, dst);
	cvConvert( dst, cvCreateMat(height, width, CV_32FC3 ) );
	cvSaveImage(refFileName.c_str(), dst);

	// Compare results
	bool diff = false;
	for (int y = 0; y < height; y++) {
    	for (int x = 0; x < width; x++) {
			if ( y_dst[y*stride+x] != y_ref[y*stride+x] ) diff = true;
			if ( u_dst[y*stride+x] != u_ref[y*stride+x] ) diff = true;
			if ( v_dst[y*stride+x] != v_ref[y*stride+x] ) diff = true;
    	}
	}

	std::cout << std::endl;	
	std::cout << "*******************************************************" << std::endl;	
	if(diff) {
		std::cout << "MATCH FAILED: Output has mismatches with reference" << std::endl;
	} else {
		std::cout << "MATCH PASS: Output matches reference" << std::endl;
	}
	std::cout << "*******************************************************" << std::endl;	


	// Report performance (if not running in emulation mode)
	if (getenv("XCL_EMULATION_MODE") == NULL) {

		std::chrono::duration<double> fpga_duration = fpga_end - fpga_begin;
		std::cout << "FPGA Time:       " << fpga_duration.count() << " s" << std::endl;
		std::cout << "FPGA Throughput: " 
		          << (double) numRuns*3*nbytes / fpga_duration.count() / (1024.0*1024.0)
		          << " MB/s" << std::endl;

		std::chrono::duration<double> cpu_duration = cpu_end - cpu_begin;
		std::cout << "CPU Time:        " << cpu_duration.count() << " s" << std::endl;
		std::cout << "CPU Throughput:  " 
		          << (double) numRuns*3*nbytes / cpu_duration.count() / (1024.0*1024.0)
		          << " MB/s" << std::endl;

		std::cout << "FPGA Speedup:    " << cpu_duration.count() / fpga_duration.count() << " x" << std::endl;
	}

	// Release allocated memory
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	clReleaseProgram(program);
	clReleaseContext(context);	
	clReleaseDevice(device);	

	return (diff?1:0);
}


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
