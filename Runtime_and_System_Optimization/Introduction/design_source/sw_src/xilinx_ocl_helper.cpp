#include "xilinx_ocl_helper.hpp"

#include <unistd.h>

namespace xilinx {
namespace example_utils {
std::vector<cl::Device> XilinxOclHelper::find_xilinx_devices()
{
    size_t i;
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    cl::Platform platform;
    for (i = 0; i < platforms.size(); i++) {
        platform                  = platforms[i];
        std::string platform_name = platform.getInfo<CL_PLATFORM_NAME>();
        if (platform_name == "Xilinx") {
            break;
        }
    }
    if (i == platforms.size()) {
        throw_lineexception("Unable to find Xilinx OpenCL devices");
    }

    // Get ACCELERATOR devices
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_ACCELERATOR, &devices);
    return devices;
}

void XilinxOclHelper::initialize(std::string xclbin_file_name)
{
    // Find Xilinx OpenCL devices
    std::vector<cl::Device> devices = find_xilinx_devices();

    bool programmed = false;

    // Load the XCLBIN
    if (access(xclbin_file_name.c_str(), R_OK) != 0) {
        throw_lineexception("Specified XCLBIN not found");
    }

    std::ifstream xclbin(xclbin_file_name.c_str(), std::ifstream::binary);
    xclbin.seekg(0, xclbin.end);
    unsigned int nb = xclbin.tellg();
    xclbin.seekg(0, xclbin.beg);
    char *buf = new char[nb];
    xclbin.read(buf, nb);
    cl::Program::Binaries bins;
    bins.push_back({buf, nb});

    cl_int err;
    // Initialize our OpenCL context
    for (unsigned int i = 0; i < devices.size(); i++) {
        device  = devices[i];
        context = cl::Context(device);

        // Attempt to program the devic
        try {
            program = cl::Program(context, {device}, bins, NULL, &err);
        }
        catch (cl::Error &e) {
            continue;
        }

        programmed     = true;
        is_initialized = true;
        break;
    }

    if (!programmed) {
        throw_lineexception("Provided XCLBIN is not compatible with any system device");
    }
}

cl::Kernel XilinxOclHelper::get_kernel(std::string kernel_name)
{
    if (!is_initialized) {
        throw_lineexception("Attempted to get kernel without initializing OCL");
    }

    cl::Kernel krnl(program, kernel_name.c_str());
    return krnl;
}

cl::CommandQueue XilinxOclHelper::get_command_queue(bool in_order, bool enable_profiling)
{
    if (!is_initialized) {
        throw_lineexception("Attempted to get command queue without initializing OCL");
    }

    cl_command_queue_properties props = 0;

    if (!in_order) {
        props |= CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
    }
    if (enable_profiling) {
        props |= CL_QUEUE_PROFILING_ENABLE;
    }

    cl::CommandQueue q(context, device, props);
    return q;
}

cl::Buffer XilinxOclHelper::create_buffer(size_t size, cl_mem_flags flags)
{
    if (!is_initialized) {
        throw_lineexception("Attempted to create buffer before initialization");
    }

    cl::Buffer buf(context, flags, size, NULL, NULL);
    return buf;
}

cl::Buffer XilinxOclHelper::create_buffer_in_bank(int bank, size_t size, cl_mem_flags flags)
{
    if (!is_initialized) {
        throw_lineexception("Attempted to create buffer before initialization");
    }

    cl_mem_ext_ptr_t bank_ext;
    bank_ext.flags = bank | XCL_MEM_TOPOLOGY;
    bank_ext.obj   = NULL;
    bank_ext.param = 0;

    cl::Buffer buf(context, flags | CL_MEM_EXT_PTR_XILINX, size, &bank_ext, NULL);
    return buf;
}

int XilinxOclHelper::get_fd_for_buffer(cl::Buffer buf)
{
    int fd;
    xclGetMemObjectFd(buf(), &fd);
    return fd;
}

cl::Buffer XilinxOclHelper::get_buffer_from_fd(int fd)
{
    cl::Buffer buffer;
    xclGetMemObjectFromFd(context(), device(), 0, fd, &buffer());
    return buffer;
}

const cl::Context &XilinxOclHelper::get_context()
{
    return context;
}

XilinxOclHelper::XilinxOclHelper()
{
}

XilinxOclHelper::~XilinxOclHelper()
{
}

} // namespace example_utils
} // namespace xilinx
