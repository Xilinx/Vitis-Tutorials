/*

  Some source codes below are copied
https://github.com/xxradon/libv4l2_opencv_mat
https://github.com/mpromonet/libv4l2cpp

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
#ifndef V4L2_ACCESS
#define V4L2_ACCESS

#include "hlsV4l2Device.hpp"

class V4l2Access
{
	public:
		enum IoType
		{
			IOTYPE_READWRITE,
			IOTYPE_MMAP
		};
		
		V4l2Access(V4l2Device* device);
		virtual ~V4l2Access();
		
		int getFd()         { return m_device->getFd();         }
		unsigned int getBufferSize() { return m_device->getBufferSize(); }
		unsigned int getFormat()     { return m_device->getFormat();     }
		unsigned int getWidth()      { return m_device->getWidth();      }
		unsigned int getHeight()     { return m_device->getHeight();     }
		void queryFormat()  { m_device->queryFormat();          }

		int isReady()       { return m_device->isReady();       }
		int start()         { return m_device->start();         }
		int stop()          { return m_device->stop();          }

	private:
		V4l2Access(const V4l2Access&);
		V4l2Access & operator=(const V4l2Access&);
	
	protected:
		V4l2Device* m_device;		
};


#endif
