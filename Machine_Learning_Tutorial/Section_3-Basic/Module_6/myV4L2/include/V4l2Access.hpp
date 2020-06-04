#ifndef V4L2_ACCESS
#define V4L2_ACCESS

#include "V4l2Device.hpp"

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

