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
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

// libv4l2
#include <linux/videodev2.h>

#include <glog/logging.h>

#include "hlsV4l2Device.hpp"

std::string fourcc(unsigned int format)
{
	char formatArray[] = { (char)(format&0xff), (char)((format>>8)&0xff), (char)((format>>16)&0xff), (char)((format>>24)&0xff), 0 };
	return std::string(formatArray, strlen(formatArray));
}

// -----------------------------------------
//    V4L2Device
// -----------------------------------------
V4l2Device::V4l2Device(const V4L2DeviceParameters&  params, v4l2_buf_type deviceType) : m_params(params), m_fd(-1), m_deviceType(deviceType), m_bufferSize(0), m_format(0)
{
}

V4l2Device::~V4l2Device() 
{
	this->close();
}

void V4l2Device::close() 
{
	if (m_fd != -1) 		
		::close(m_fd);
	
	m_fd = -1;
}

// query current format
void V4l2Device::queryFormat()
{
	struct v4l2_format     fmt;
	memset(&fmt,0,sizeof(fmt));
	fmt.type  = m_deviceType;
	if (0 == ioctl(m_fd,VIDIOC_G_FMT,&fmt)) 
	{
		m_format     = fmt.fmt.pix.pixelformat;
		m_width      = fmt.fmt.pix.width;
		m_height     = fmt.fmt.pix.height;
		m_bufferSize = fmt.fmt.pix.sizeimage;

		LOG(INFO) << m_params.m_devName << ":" << fourcc(m_format) << " size:" << m_width << "x" << m_height << " bufferSize:" << m_bufferSize;
	}
}

// intialize the V4L2 connection
bool V4l2Device::init(unsigned int mandatoryCapabilities )
{
        struct stat sb;
        if ( (stat(m_params.m_devName.c_str(), &sb)==0) && ((sb.st_mode & S_IFMT) == S_IFCHR) )
        {
        if (initdevice(m_params.m_devName.c_str(), mandatoryCapabilities) == -1 )
		{
			LOG(ERROR) << "Cannot init device:" << m_params.m_devName;
		}
	}
	else
	{
                // open a normal file
                m_fd = open(m_params.m_devName.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
	}
	return (m_fd!=-1);
}

// intialize the V4L2 device
int V4l2Device::initdevice(const char *dev_name,unsigned int mandatoryCapabilities  )
{
	m_fd = open(dev_name,  m_params.m_openFlags);
/*	if (m_fd < 0) 
	{
		LOG(ERROR) << "Cannot open device:" << m_params.m_devName << " " << strerror(errno);
		this->close();
		return -1;
	}
*/
    CHECK_GE(m_fd, 0)<<"Cannot open device:"<< m_params.m_devName<<" "<<strerror(errno);
    /*
     * check the number of input devices
     */
    struct v4l2_input input;

    struct v4l2_standard standard;

    memset (&input, 0, sizeof (input));

    //get the device index
    input.index = m_params.m_inputIndex;
    /*
    if (-1 == ioctl(m_fd, VIDIOC_S_INPUT, &input)){
         LOG(ERROR) << "can not set " << m_params.m_inputIndex <<" input index";
         return -1;
    }
    */
    CHECK_NE(ioctl(m_fd, VIDIOC_S_INPUT, &input), -1)<<"can not set " << m_params.m_inputIndex <<" input index";
    /*
    if (-1 == ioctl (m_fd, VIDIOC_G_INPUT, &input.index)) {

        LOG(ERROR) << "get VIDIOC_G_INPUT error ";
        return -1;
    }
    */
    CHECK_NE(ioctl (m_fd, VIDIOC_G_INPUT, &input.index), -1)<< "get VIDIOC_G_INPUT error ";

    //get the enumerate info of the specific device number
    /*
    if (-1 == ioctl (m_fd, VIDIOC_ENUMINPUT, &input)) {
     LOG(ERROR) << "Get ”VIDIOC_ENUM_INPUT” error";
    return -1;
    }*/

    CHECK_NE(ioctl (m_fd, VIDIOC_ENUMINPUT, &input), -1)<< "Get ”VIDIOC_ENUM_INPUT” error";
    //LOG(NOTICE) << "Current input " <<input.name<<" supports:";

    LOG(INFO)<<"Current input " <<input.name<<" supports:";
    memset (&standard, 0, sizeof (standard)); standard.index = 0;
    

  //List all supported standards

    while (0 == ioctl (m_fd, VIDIOC_ENUMSTD, &standard)) {

        if (standard.id & input.std)
           // LOG(NOTICE) << standard.name;
            LOG(INFO)<<standard.name;
            standard.index++;
    }
   

    /* EINVAL indicates the end of the enumeration, which cannot be empty unless this device falls under the USB exception.*/
    /*
    if (errno != EINVAL || standard.index == 0) {
     LOG(NOTICE) << "Get ”VIDIOC_ENUMSTD” error";
    }*/

    LOG_IF(INFO, (errno != EINVAL || standard.index) == 0)<<"Get [VIDIOC_ENUMSTD] error";


	if (checkCapabilities(m_fd,mandatoryCapabilities) !=0)
	{
		this->close();
		return -1;
	}	
	if (configureFormat(m_fd) !=0) 
	{
		this->close();
		return -1;
	}
	if (configureParam(m_fd) !=0)
	{
		this->close();
		return -1;
	}
	
	return m_fd;
}
static std::string frmtype2s(unsigned type)
{
    static const char *types[] = {
        "Unknown",
        "Discrete",
        "Continuous",
        "Stepwise"
    };

    if (type > 3)
        type = 0;
    return types[type];
}
static std::string fract2sec(const struct v4l2_fract &f)
{
    char buf[100];

    sprintf(buf, "%.3f s", (1.0 * f.numerator) / f.denominator);
    return buf;
}

static std::string fract2fps(const struct v4l2_fract &f)
{
    char buf[100];

    sprintf(buf, "%.3f fps", (1.0 * f.denominator) / f.numerator);
    return buf;
}

static void print_frmival(const struct v4l2_frmivalenum &frmival)
{
//    printf("%s\tInterval: %s ", prefix, frmtype2s(frmival.type).c_str());

    if (frmival.type == V4L2_FRMIVAL_TYPE_DISCRETE) {
        LOG(INFO) << "Interval: "<<frmtype2s(frmival.type)<< " " <<fract2sec(frmival.discrete) << " ->"<< fract2fps(frmival.discrete);
    } else if (frmival.type == V4L2_FRMIVAL_TYPE_STEPWISE) {
        LOG(INFO) << "Interval: "<<frmtype2s(frmival.type)<<fract2sec(frmival.stepwise.min)<< " - " << fract2sec(frmival.stepwise.max)
                     <<" with step "<<fract2sec(frmival.stepwise.step);
        LOG(INFO) << fract2sec(frmival.stepwise.min)<< " - " << fract2sec(frmival.stepwise.max)
                     <<" with step "<<fract2sec(frmival.stepwise.step);
    }
}

// check needed V4L2 capabilities,get the device the basic imformation
int V4l2Device::checkCapabilities(int fd, unsigned int mandatoryCapabilities)
{
	struct v4l2_capability cap;
    /*
        struct v4l2_capability

        {

        u8 driver[16]; // driver name
        u8 card[32]; // device name
        u8 bus_info[32]; // device bus info
        u32 version;//driver version
        u32 capabilities;// suppoted operation
        u32 reserved[4]; // reserve 
        };
    */
	memset(&(cap), 0, sizeof(cap));
	/*if (-1 == ioctl(fd, VIDIOC_QUERYCAP, &cap)) 
	{
		LOG(ERROR) << "Cannot get capabilities for device:" << m_params.m_devName << " " << strerror(errno);
		return -1;
	}*/
    LOG_IF(FATAL, -1 == ioctl(fd, VIDIOC_QUERYCAP, &cap))<<"Cannot get capabilities for device:" 
    << m_params.m_devName << " " << strerror(errno);

	LOG(INFO) << "driver:" << cap.driver << " capabilities:" << std::hex << cap.capabilities <<  " mandatory:" << mandatoryCapabilities << std::dec;
    LOG(INFO) << "card:" << cap.card <<	" Bus info: " << cap.bus_info;
    memcpy(bus_info, cap.bus_info, sizeof(cap.bus_info));
    /// 检查设备的能力
	if ((cap.capabilities & V4L2_CAP_VIDEO_OUTPUT))  LOG(INFO) << m_params.m_devName << " support output";
    if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        LOG(INFO) << m_params.m_devName << " support capture";
 
        struct v4l2_fmtdesc fmtdesc;
        fmtdesc.index=0;
        fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
        LOG(INFO) <<"capture Support format:";
//        while(ioctl(m_fd, VIDIOC_ENUM_FMT, &fmtdesc) != -1)
//        {
////          printf("\t%d.%s\n",fmtdesc.index+1,fmtdesc.description);
//          LOG(NOTICE) <<(fmtdesc.index+1) <<"."<<fmtdesc.description<<",pixelformat:"<< fourcc(fmtdesc.pixelformat) ;

//          fmtdesc.index++;
//        }


        {
            //collect the parameters supported by the camera

            enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            struct v4l2_fmtdesc fmtdesc;
            struct v4l2_frmsizeenum frmsize;
            struct v4l2_frmivalenum frmival;

            fmtdesc.index = 0;
            fmtdesc.type = type;
            while (ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc) >= 0) {
              LOG(INFO) <<(fmtdesc.index+1) <<"."<<fmtdesc.description<<",supoort resolution:";
              frmsize.pixel_format = fmtdesc.pixelformat;
              frmsize.index = 0;
              while (ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frmsize) >= 0){

                if(frmsize.type == V4L2_FRMSIZE_TYPE_DISCRETE){
                    LOG(INFO) << "width:"<<frmsize.discrete.width << ",height:"<< frmsize.discrete.height;
                    frmival.index = 0;
                    frmival.pixel_format = fmtdesc.pixelformat;
                    frmival.width = frmsize.discrete.width;
                    frmival.height = frmsize.discrete.height;
                    while (ioctl(fd, VIDIOC_ENUM_FRAMEINTERVALS, &frmival) >= 0) {
                        print_frmival(frmival);
                        frmival.index++;
                    }
                }else if(frmsize.type == V4L2_FRMSIZE_TYPE_STEPWISE){
                    //LOG(NOTICE) << "width:"<<frmsize.discrete.width << ",height:"<< frmsize.discrete.height;
                    LOG(INFO)<<"width:"<<frmsize.discrete.width << ",height:"<< frmsize.discrete.height;
                }

                frmsize.index++;
                      }

              fmtdesc.index++;
            }

          }



    }

	if ((cap.capabilities & V4L2_CAP_READWRITE))     LOG(INFO) << m_params.m_devName << " support read/write";
	if ((cap.capabilities & V4L2_CAP_STREAMING))     LOG(INFO) << m_params.m_devName << " support streaming";
    if ((cap.capabilities & V4L2_CAP_TIMEPERFRAME))  LOG(INFO) << m_params.m_devName << " support timeperframe"; 
	
	/*if ( (cap.capabilities & mandatoryCapabilities) != mandatoryCapabilities )
	{
		LOG(ERROR) << "Mandatory capability not available for device:" << m_params.m_devName;
		return -1;
	}*/
    LOG_IF(FATAL, (cap.capabilities & mandatoryCapabilities) != mandatoryCapabilities)<<
     "Mandatory capability not available for device:" << m_params.m_devName;
	
	return 0;
}

// configure capture format 
int V4l2Device::configureFormat(int fd)
{
	// get current configuration
	this->queryFormat();		

	unsigned int width = m_width;
	unsigned int height = m_height;
	if (m_params.m_width != 0)  {
		width= m_params.m_width;
	}
	if (m_params.m_height != 0)  {
		height= m_params.m_height;
	}	
	if  ( (m_params.m_formatList.size()==0) && (m_format != 0) )  {
		m_params.m_formatList.push_back(m_format);
	}

	// try to set format, widht, height
	std::list<unsigned int>::iterator it;
	for (it = m_params.m_formatList.begin(); it != m_params.m_formatList.end(); ++it) {
		unsigned int format = *it;
		if (this->configureFormat(fd, format, width, height)==0) {
			// format has been set
			// get the format again because calling SET-FMT return a bad buffersize using v4l2loopback
			this->queryFormat();		
			return 0;
		}
	}
	return -1;
}

// configure capture format 
int V4l2Device::configureFormat(int fd, unsigned int format, unsigned int width, unsigned int height)
{
	struct v4l2_format   fmt;			
	memset(&(fmt), 0, sizeof(fmt));
	fmt.type                = m_deviceType;
	fmt.fmt.pix.width       = width;
	fmt.fmt.pix.height      = height;
	fmt.fmt.pix.pixelformat = format;
	fmt.fmt.pix.field       = V4L2_FIELD_ANY;
	/*
	if (ioctl(fd, VIDIOC_S_FMT, &fmt) == -1)
	{
		LOG(ERROR) << "Cannot set format:" << fourcc(format) << " for device:" << m_params.m_devName << " " << strerror(errno);
		return -1;
	}
    */
    LOG_IF(ERROR, ioctl(fd, VIDIOC_S_FMT, &fmt) == -1 )	<< fourcc(format) << " for device:" << m_params.m_devName << " " << strerror(errno);	
	/*if (fmt.fmt.pix.pixelformat != format) 
	{
		LOG(ERROR) << "Cannot set pixelformat to:" << fourcc(format) << " format is:" << fourcc(fmt.fmt.pix.pixelformat);
		return -1;
	}*/
    LOG_IF(ERROR, fmt.fmt.pix.pixelformat != format)<<"Cannot set pixelformat to:" << fourcc(format) << " format is:" << fourcc(fmt.fmt.pix.pixelformat);
    /*
	if ((fmt.fmt.pix.width != width) || (fmt.fmt.pix.height != height))
	{
		LOG(WARN) << "Cannot set size to:" << width << "x" << height << " size is:"  << fmt.fmt.pix.width << "x" << fmt.fmt.pix.height;
	}
    */
	LOG_IF(INFO, (fmt.fmt.pix.width != width) || (fmt.fmt.pix.height != height))<<"Cannot set size to:"
     << width << "x" << height << " size is:"  << fmt.fmt.pix.width << "x" << fmt.fmt.pix.height;

	m_format     = fmt.fmt.pix.pixelformat;
	m_width      = fmt.fmt.pix.width;
	m_height     = fmt.fmt.pix.height;		
	m_bufferSize = fmt.fmt.pix.sizeimage;
	
   // LOG(NOTICE) <<"Setting "<< m_params.m_devName << ":" << fourcc(m_format) << " size:" << m_width << "x" << m_height << " bufferSize:" << m_bufferSize;
	LOG(INFO)<<"Setting "<< m_params.m_devName << ":" << fourcc(m_format) << " size:" 
    << m_width << "x" << m_height << " bufferSize:" << m_bufferSize;
	return 0;
}

// configure capture FPS 
int V4l2Device::configureParam(int fd)
{
	if (m_params.m_fps!=0)
	{
		struct v4l2_streamparm   param;			
		memset(&(param), 0, sizeof(param));
		param.type = m_deviceType;
		param.parm.capture.timeperframe.numerator = 1;
		param.parm.capture.timeperframe.denominator = m_params.m_fps;
        /*
		if (ioctl(fd, VIDIOC_S_PARM, &param) == -1)
		{
			LOG(WARN) << "Cannot set param for device:" << m_params.m_devName << " " << strerror(errno);
		}
        */
        LOG_IF(INFO, ioctl(fd, VIDIOC_S_PARM, &param)==-1)<< "Cannot set param for device:" 
        << m_params.m_devName << " " << strerror(errno);
        //LOG(NOTICE) <<"Setting "<< "fps:" << param.parm.capture.timeperframe.numerator << "/" << param.parm.capture.timeperframe.denominator;
        LOG(INFO)<<"Setting "<< "fps:" << param.parm.capture.timeperframe.numerator 
        << "/" << param.parm.capture.timeperframe.denominator;
        //LOG(NOTICE) <<"Setting "<< "nbBuffer:" << param.parm.capture.readbuffers;
        LOG(INFO)<<"Setting "<< "nbBuffer:" << param.parm.capture.readbuffers;
	}
	
	return 0;
}


