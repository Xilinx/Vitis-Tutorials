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
#include "V4l2Device.hpp"
#include "V4l2Capture.hpp"
#include <glog/logging.h>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

int main(){
    int verbose=0;
    int stop=0;
    const char * in_devname = "/dev/video0";/* V4L2_PIX_FMT_UYVY V4L2_PIX_FMT_MJPEG*/
    /*
     * V4L2_PIX_FMT_MJPEG ï¼ˆMJPEGï¼‰
     */
    /* V4L2DeviceParameters param(in_devname, V4L2_PIX_FMT_MJPEG , 1920, 1080, 30, 0,verbose);
    V4l2Capture* videoCapture = V4l2Capture::create(param, V4l2Access::IOTYPE_MMAP);
    if (videoCapture == NULL)
    {
        LOG(WARN) << "Cannot create V4L2 capture interface for device:" << "/dev/video0";
        return -1;
    }
    timeval tv;
    LOG(NOTICE) << "USB bus:" <<videoCapture->getBusInfo();
    LOG(NOTICE) << "Start Uncompressing " << in_devname ;

    while (!stop)
    {
        tv.tv_sec=1;
        tv.tv_usec=0;
        int ret = videoCapture->isReadable(&tv);
        if (ret == 1)
        {
            cv::Mat v4l2Mat(1080,1920,CV_8UC3);
            ret =  videoCapture->read(v4l2Mat);

            if (ret != 0)
            {
                LOG(NOTICE) << "stop " ;
                stop=1;
            }
            else
            {
               cv::imwrite("test.jpg",v4l2Mat);
            }
        }
        else if (ret == -1) //
        {
            LOG(NOTICE) << "stop " << strerror(errno);
            stop=1;
        }else if( ret == 0) //
        {

        }
        break;
    }
    delete videoCapture;
    */
    V4L2DeviceParameters mparam(in_devname, V4L2_PIX_FMT_UYVY , 1920, 1080, 30, 0,verbose);
    V4l2Capture* videoCapture = V4l2Capture::create(mparam, V4l2Access::IOTYPE_MMAP);
     timeval tv;
    while (!stop)
    {
        tv.tv_sec=1;
        tv.tv_usec=0;
        int ret = videoCapture->isReadable(&tv);
        if (ret == 1)
        {
            std::vector<cv::Mat> v4l2Mat;
            ret =  videoCapture->read_images(v4l2Mat);

            if (ret != 0)
            {
                LOG(INFO) << "stop " ;
                stop=1;
            }
            else
            {
              
               cv::imwrite("test0.jpg", v4l2Mat[0]);
               cv::imwrite("test1.jpg", v4l2Mat[1]);

            }
        }
        else if (ret == -1) //fail
        {
            LOG(INFO) << "stop " << strerror(errno);
            stop=1;
        }else if( ret == 0) // timeout
        {

        }
    }
}
