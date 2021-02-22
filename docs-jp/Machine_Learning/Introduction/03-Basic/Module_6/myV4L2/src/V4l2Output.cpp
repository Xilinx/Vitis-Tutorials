/*

  Some source codes below are copied
https://github.com/xxradon/libv4l2_opencv_mat
  They are modified.

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

*/
#include <string.h>
#include <linux/videodev2.h>
#include <glog/logging.h>
#include "V4l2Output.hpp"
#include "V4l2MmapDevice.hpp"
#include "V4l2ReadWriteDevice.hpp"

// -----------------------------------------
//    create video output interface
// -----------------------------------------
V4l2Output* V4l2Output::create(const V4L2DeviceParameters & param, IoType iotype)
{
	V4l2Output* videoOutput = NULL;
	V4l2Device* videoDevice = NULL; 
	int caps = V4L2_CAP_VIDEO_OUTPUT;
	switch (iotype)
	{
		case IOTYPE_MMAP: 
			videoDevice = new V4l2MmapDevice(param, V4L2_BUF_TYPE_VIDEO_OUTPUT); 
			caps |= V4L2_CAP_STREAMING;
		break;
		case IOTYPE_READWRITE:
			videoDevice = new V4l2ReadWriteDevice(param, V4L2_BUF_TYPE_VIDEO_OUTPUT); 
			caps |= V4L2_CAP_READWRITE;
		break;
	}
	
	if (videoDevice &&  !videoDevice->init(caps))
	{
		delete videoDevice;
		videoDevice=NULL; 
	}
	
	if (videoDevice)
	{
		videoOutput = new V4l2Output(videoDevice);
	}	
	return videoOutput;
}

// -----------------------------------------
//    constructor
// -----------------------------------------
V4l2Output::V4l2Output(V4l2Device* device) : V4l2Access(device)
{
}

// -----------------------------------------
//    destructor
// -----------------------------------------
V4l2Output::~V4l2Output() 
{
}

// -----------------------------------------
//    check writability
// -----------------------------------------
int V4l2Output::isWritable(timeval* tv)
{
	int fd = m_device->getFd();
	fd_set fdset;
	FD_ZERO(&fdset);	
	FD_SET(fd, &fdset);
	return select(fd+1, NULL, &fdset, NULL, tv);
}

// -----------------------------------------
//    write to V4l2Device
// -----------------------------------------
size_t V4l2Output::write(char* buffer, size_t bufferSize)
{
	return m_device->writeInternal(buffer, bufferSize);
}


bool V4l2Output::startPartialWrite(void)
{
	return m_device->startPartialWrite();
}

size_t V4l2Output::writePartial(char* buffer, size_t bufferSize)
{
	return m_device->writePartialInternal(buffer, bufferSize);
}

bool V4l2Output::endPartialWrite(void)
{
	return m_device->endPartialWrite();
}

