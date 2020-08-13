
#include "common.h"
#include "constants.h"
#include "kernels.h"

#include <vector>
#include <cstdio>

#include "xcl2.hpp"

using std::vector;

bool operator==(const RGBPixel& lhs, const RGBPixel& rhs) {
    return lhs.r == rhs.r &&
           lhs.g == rhs.g &&
           lhs.b == rhs.b;
}

void
test(vector<RGBPixel>& in, vector<RGBPixel>& out,
     float* coefficients, int coefficient_size,
     int width, int height) {
    vector<RGBPixel> gold(out.size());
    convolve_cpu(in.data(), gold.data(), coefficients, coefficient_size, width, height);
    auto it = mismatch(begin(gold), end(gold), begin(out));
    if(it.first != end(gold)) {
        printf("Incorrect result: \n Expected: (%d %d %d)\nResult:  (%d %d %d)\n ",
               it.first->r, it.first->g, it.first->b, it.second->r, it.second->g, it.second->b);
    }
}

void convolve(FILE* streamIn, FILE* streamOut,
              float* coefficients, int coefficient_size,
              arguments args) {
    size_t frame_bytes = args.width * args.height * sizeof(RGBPixel);
    size_t gray_frame_bytes = args.width * args.height * sizeof(GrayPixel);
    vector<RGBPixel> inFrame(args.width * args.height);
    vector<RGBPixel> outFrame(args.width * args.height);
    vector<GrayPixel> grayFrame(args.width * args.height);

    size_t bytes_read = 0;
    size_t bytes_written = 0;


    size_t total_coefficient_size = coefficient_size * coefficient_size;
    vector<float, aligned_allocator<float>> filter_coeff(coefficients, coefficients + total_coefficient_size);
    size_t coefficient_size_bytes = sizeof(float) * total_coefficient_size;


    vector<cl::Device> devices = xcl::get_xil_devices();
    cl::Device device = devices[0];


    cl::Context context(device);
    cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE);


    cl::Program::Binaries bins = xcl::import_binary_file(args.binary_file);
    devices.resize(1);
    cl::Program program(context, devices, bins);
    cl::Kernel convolve_kernel(program, args.kernel_name);


    cl::Buffer buffer_input(context, CL_MEM_READ_ONLY, frame_bytes, NULL);
    cl::Buffer buffer_output(context, CL_MEM_WRITE_ONLY, frame_bytes, NULL);
    cl::Buffer buffer_coefficient(context, CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY, coefficient_size_bytes, filter_coeff.data());


    convolve_kernel.setArg(0, buffer_input);
    convolve_kernel.setArg(1, buffer_output);
    convolve_kernel.setArg(2, buffer_coefficient);
    convolve_kernel.setArg(3, coefficient_size);
    convolve_kernel.setArg(4, args.width);
    convolve_kernel.setArg(5, args.height);

    q.enqueueMigrateMemObjects({buffer_coefficient}, 0);


    auto fpga_begin = std::chrono::high_resolution_clock::now();

    for(int frame_count = 0; frame_count < args.nframes; frame_count++) {
        // Read frame
        bytes_read = fread(inFrame.data(), 1, frame_bytes, streamIn);
        if(bytes_read != frame_bytes) {
        	printf("\nError: partial frame.\nExpected %zu\nActual %zu\n", frame_bytes, bytes_read);
        	break;
        }

        /*
        convolve_cpu(inFrame.data(), outFrame.data(),
                     coefficients, coefficient_size,
                     args.width, args.height);
        */

        q.enqueueWriteBuffer(buffer_input, CL_FALSE, 0, frame_bytes, inFrame.data());

        q.enqueueTask(convolve_kernel);

        q.enqueueReadBuffer(buffer_output, CL_TRUE, 0, frame_bytes, outFrame.data());

        if(args.gray) {
          grayscale_cpu(outFrame.data(), grayFrame.data(), args.width, args.height);
          bytes_written = fwrite(outFrame.data(), 1, gray_frame_bytes, streamOut);
          fflush(streamOut);
          if (bytes_written != gray_frame_bytes) {
            printf("\nError: partial frame.\nExpected %zu\nActual %zu\n",
                   gray_frame_bytes, bytes_written);
            break;
          }
        } else {
          bytes_written = fwrite(outFrame.data(), 1, frame_bytes, streamOut);
          fflush(streamOut);
          if (bytes_written != frame_bytes) {
            printf("\nError: partial frame.\nExpected %zu\nActual %zu\n",
                   frame_bytes, bytes_written);
            break;
          }
          // test(inFrame, outFrame, coefficients, coefficient_size, width, height);
        }

        print_progress(frame_count, args.nframes);
    } 
    q.finish();

    auto fpga_end = std::chrono::high_resolution_clock::now();

    // Report performance (if not running in emulation mode)
    if (getenv("XCL_EMULATION_MODE") == NULL) {
        std::chrono::duration<double> fpga_duration = fpga_end - fpga_begin;
        std::cout << "                 " << std::endl;
        std::cout << "FPGA Time:       " << fpga_duration.count() << " s" << std::endl;
        std::cout << "FPGA Throughput: "
                  << (1920*1080*4*132) / fpga_duration.count() / (1024.0*1024.0)
                  << " MB/s" << std::endl;
     }
}
