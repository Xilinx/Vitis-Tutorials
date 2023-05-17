/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#ifndef __TASK_H__
#define __TASK_H__

#include <iostream>
#include <vector>
#include <ap_int.h>

#include "AlignedAllocator.h"
#include "ApiHandle.h"

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include "CL/opencl.h"

class Task {
  std::vector< ap_int<512>, AlignedAllocator< ap_int<512> >> m_in;
  std::vector< ap_int<512>, AlignedAllocator< ap_int<512> >> m_out;
  unsigned int      m_bufferSize;
  unsigned int      m_processDelay;

  cl_event          m_inEv;
  cl_event          m_outEv;
  cl_event          m_doneEv;

  cl_mem            m_inBuffer[1];
  cl_mem            m_outBuffer[1];

  bool              m_hasRun;
  
public:
  cl_event* getDoneEv()  { return &m_doneEv;  }

  Task(unsigned int bufferSize, unsigned int processDelay):
    m_in(bufferSize, 0),
    m_out(bufferSize),
    m_bufferSize(bufferSize),
    m_processDelay(processDelay),
    m_hasRun(false)
  {
  }
  Task(const Task &t):
    m_in(t.m_bufferSize, 0),
    m_out(t.m_bufferSize),
    m_bufferSize(t.m_bufferSize),
    m_processDelay(t.m_processDelay),
    m_hasRun(false)
  {
  }
  ~Task() {
    if(m_hasRun) {
      clReleaseMemObject(m_inBuffer[0]);
      clReleaseMemObject(m_outBuffer[0]);

      clReleaseEvent(m_inEv);
      clReleaseEvent(m_outEv);
      clReleaseEvent(m_doneEv);
    }
  }
  void run(ApiHandle &api, cl_event *prevEvent = nullptr) {
    int err;
    m_inBuffer[0] = clCreateBuffer(api.getContext(),
				   CL_MEM_USE_HOST_PTR |
				   CL_MEM_READ_ONLY,
				   m_bufferSize*sizeof(ap_int<512>), 
				   m_in.data(),
				   &err);
    m_outBuffer[0] = clCreateBuffer(api.getContext(),
				    CL_MEM_USE_HOST_PTR |
				    CL_MEM_WRITE_ONLY,
				    m_bufferSize*sizeof(ap_int<512>), 
				    m_out.data(),
				    &err);
    clSetKernelArg(api.getKernel(), 0, sizeof(cl_mem), &m_inBuffer[0]);
    clSetKernelArg(api.getKernel(), 1, sizeof(cl_mem), &m_outBuffer[0]);
    clSetKernelArg(api.getKernel(), 2, sizeof(unsigned int), &m_bufferSize);
    clSetKernelArg(api.getKernel(), 3, sizeof(unsigned int), &m_processDelay);

    if(prevEvent != nullptr) {
      clEnqueueMigrateMemObjects(api.getQueue(), 1, &m_inBuffer[0],
				 0, 1, prevEvent, &m_inEv);
    } else {
      clEnqueueMigrateMemObjects(api.getQueue(), 1, &m_inBuffer[0],
				 0, 0, nullptr, &m_inEv);
    }

    clSetKernelArg(api.getKernel(), 0, sizeof(cl_mem), &m_inBuffer[0]);
    clSetKernelArg(api.getKernel(), 1, sizeof(cl_mem), &m_outBuffer[0]);
    clSetKernelArg(api.getKernel(), 2, sizeof(unsigned int), &m_bufferSize);
    clSetKernelArg(api.getKernel(), 3, sizeof(unsigned int), &m_processDelay);

    clEnqueueTask(api.getQueue(), api.getKernel(), 1, &m_inEv, &m_outEv);
    
    clEnqueueMigrateMemObjects(api.getQueue(), 1, &m_outBuffer[0],
			       CL_MIGRATE_MEM_OBJECT_HOST,
			       1, &m_outEv, &m_doneEv);
    m_hasRun = true;
  }
  bool outputOk() {
    for(unsigned int i=0; i < m_bufferSize; i++) {
      if(m_out[i] != m_processDelay) {
	std::cout << "Output Error" << std::endl;
	return false;
      }
    }
    return true;
  }
 
};

#endif
