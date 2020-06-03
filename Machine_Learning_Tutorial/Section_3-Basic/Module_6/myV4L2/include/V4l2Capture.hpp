
#ifndef V4L2_CAPTURE
#define V4L2_CAPTURE

#include "V4l2Access.hpp"
#include "opencv2/core/core.hpp"
#include <vector>


// ---------------------------------
// V4L2 Capture
// ---------------------------------
class V4l2Capture : public V4l2Access
{		
	protected:	
		V4l2Capture(V4l2Device* device);
	
	public:
        /**
         * @brief create Capture
         * @param param

         * @param iotype
         * @return
         */
		static V4l2Capture* create(const V4L2DeviceParameters & param, IoType iotype = V4l2Access::IOTYPE_MMAP);
		virtual ~V4l2Capture();

        size_t read(char* buffer, size_t bufferSize);


        /*
        *   output the vector of images
        *
        */
       int read_images(std::vector<cv::Mat>& readImage);


        /**
         * @brief read  read images from device
         * @param readImage put the image into readImage
         * @return 
         */
        int read(cv::Mat &readImage);
        /**
         * @brief isReadable 
         * @param tv waiting time
         *      timeval tv;
                tv.tv_sec=1;
                tv.tv_usec=0;
                int ret = this->isReadable(&tv);
         *
         * @return -1 fail      0  timeout        1 succes
         */
        int isReadable(timeval* tv);
        /**
         * @brief getBusInfo get the bus address. obtain the address of the bus. 
         * When multiple cameras are input to the device, it is approiate to determine which camera we are collecting.
         * @return
         */
        const char * getBusInfo();
        
};


#endif
