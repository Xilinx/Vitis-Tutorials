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
