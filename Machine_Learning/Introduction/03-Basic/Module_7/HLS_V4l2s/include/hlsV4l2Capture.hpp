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
#ifndef HLSV4L2_CAPTURE
#define HLSV4L2_CAPTURE

#include "hlsV4l2Access.hpp"
#include "opencv2/core/core.hpp"
#include <vector>
#include "xcl2.hpp"
#define IN_HEIGHT 1296
#define IN_WIDTH 2304
#define OUT_HEIGHT 1080
#define OUT_WIDTH 1920
#define OUT_RESIZE_HEIGHT 360
#define OUT_RESIZE_WIDTH 480
// ---------------------------------
// V4L2 Capture
// ---------------------------------
class V4l2Capture : public V4l2Access
{
protected:
        V4l2Capture(V4l2Device *device);
        xcl::XilinxOcl xocl;

public:
        /**
         * @brief create Capture
         * @param param

         * @param iotype
         * @return
         */
        static V4l2Capture *create(const V4L2DeviceParameters &param, IoType iotype = V4l2Access::IOTYPE_MMAP);
        virtual ~V4l2Capture();

        size_t read(char *buffer, size_t bufferSize);

        /*
        *   output the vector of images
        *
        */
        int read_images(std::vector<cv::Mat> &readImage);
        /**
         * @brief read images with HLS kernel doing decode and resize
         * 
         * @param readImage 
         * @return int 
         */

        /**
         * @brief Using HLS kernel to decode the yuv data and do resize
         * 
         * @param readImage output parameters
         * @return int 
         */
        int read_images_with_kernel(std::vector<cv::Mat> &readImage);
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
        int isReadable(timeval *tv);
        /**
         * @brief getBusInfo get the bus address. obtain the address of the bus. 
         * When multiple cameras are input to the device, it is approiate to determine which camera we are collecting.
         * @return
         */
        const char *getBusInfo();

private:
        static uint8_t *out_buf_0;
        static uint8_t *out_buf_1;
        static uint8_t *out_buf_back_0;
        static uint8_t *out_buf_back_1;
        static unsigned int resize_size;
        static unsigned int full_size;
        static bool xocl_initialized;
        cl::Buffer imgToDevice;
        cl::Buffer resizeFromDevice;
        cl::Buffer fullFromDevice;
        cl::Kernel krnl;
        cl::Event event_sp;
        cl::CommandQueue q;
};

#endif
