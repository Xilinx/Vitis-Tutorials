
#ifndef V4L2_MMAP_DEVICE
#define V4L2_MMAP_DEVICE
 
#include "V4l2Device.h"

#define V4L2MMAP_NBBUFFER 10

class V4l2MmapDevice : public V4l2Device
{	
	protected:	
		size_t writeInternal(char* buffer, size_t bufferSize);
		bool startPartialWrite(void);
		size_t writePartialInternal(char*, size_t);
		bool endPartialWrite(void);
		size_t readInternal(char* buffer, size_t bufferSize);
			
	public:
		V4l2MmapDevice(const V4L2DeviceParameters & params, v4l2_buf_type deviceType);		
		virtual ~V4l2MmapDevice();

		virtual bool init(unsigned int mandatoryiCapabilities);
		virtual bool isReady() { return  ((m_fd != -1)&& (n_buffers != 0)); }
		virtual bool start();
		virtual bool stop();
	
	protected:
		unsigned int  n_buffers;
	
		struct buffer 
		{
			void *                  start;
			size_t                  length;
		};
		buffer m_buffer[V4L2MMAP_NBBUFFER];
};

#endif

