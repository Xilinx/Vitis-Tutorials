
#include "common.h"
#include "constants.h"
#include "kernels.h"

#include <vector>
#include <cstdio>

#include "xcl2.hpp"

#include <string>
// This extension file is required for stream APIs
#include "CL/cl_ext_xilinx.h"




// Declaration of custom stream APIs that binds to Xilinx Streaming APIs.
decltype(&clCreateStream) xcl::Stream::createStream = nullptr;
decltype(&clReleaseStream) xcl::Stream::releaseStream = nullptr;
decltype(&clReadStream) xcl::Stream::readStream = nullptr;
decltype(&clWriteStream) xcl::Stream::writeStream = nullptr;
decltype(&clPollStreams) xcl::Stream::pollStreams = nullptr;

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
//    vector<h_RGBPixel> h_outFrame(args.width * args.height);
    vector<GrayPixel> grayFrame(args.width * args.height);

    size_t bytes_read = 0;
    size_t bytes_written = 0;

int COEFFICIENT_SIZE= 3;
int half = COEFFICIENT_SIZE/2;
  int compute_units = 4;
  int NCU = compute_units;

///////////////
//
//defining variables 
//
//
////////////////
cl_int err;
    std::string cu_id;
    std::string krnl_name = args.kernel_name;
    std::vector<cl::Kernel> convolve_kernel(NCU); //krnls(NCU);

       RGBPixel a1[5120*4];
      

  /////////

    size_t total_coefficient_size = coefficient_size * coefficient_size;
    vector<float, aligned_allocator<float>> filter_coeff(coefficients, coefficients + total_coefficient_size);
    size_t coefficient_size_bytes = sizeof(float) * total_coefficient_size;


    vector<cl::Device> devices = xcl::get_xil_devices();
    cl::Device device = devices[0];


    cl::Context context(device);
    cl::CommandQueue q(context, device,
    cl::QueueProperties::Profiling | cl::QueueProperties::OutOfOrder);

    cl::Program::Binaries bins = xcl::import_binary_file(args.binary_file);
    devices.resize(1);
    cl::Program program(context, devices, bins);

// creating multiple CU's for multiple streams

    for (int i = 0; i < NCU; i++) 
    {
                cu_id = std::to_string(i + 1);
            auto krnl_name_full = krnl_name +":{" + "convolve_fpga_" + cu_id +"}" ;
                printf("Creating a kernel [%s] for CU(%d)\n",
                       krnl_name_full.c_str(),
                       i);
                convolve_kernel[i] = cl::Kernel(
                              program, krnl_name_full.c_str(), &err);
      //  std::cout<<"err is - NCU loop " << err<<std::endl;
    }


//    cl::Kernel convolve_kernel(program, args.kernel_name);

    auto platform_id = device.getInfo<CL_DEVICE_PLATFORM>(&err);

    //Initialization of streaming class is needed before using it.
    xcl::Stream::init(platform_id);

    // Streams
    std::vector<cl_stream> write_stream_a(NCU);
    std::vector<cl_stream> write_stream_b(NCU);
    std::vector<cl_stream> read_stream(NCU);
    cl_int ret;


for (int i = 0; i < NCU; i++) {
       cl_mem_ext_ptr_t ext;
        ext.param = convolve_kernel[i].get();
        ext.obj = NULL;
       ext.flags = 0;
            write_stream_a[i] = xcl::Stream::createStream(
                device.get(),XCL_STREAM_READ_ONLY, CL_STREAM, &ext, &ret);
        ext.flags = 2;
            write_stream_b[i] = xcl::Stream::createStream(
                device.get(), XCL_STREAM_READ_ONLY, CL_STREAM, &ext, &ret);
       ext.flags = 1;
            read_stream[i] = xcl::Stream::createStream(
                device.get(), XCL_STREAM_WRITE_ONLY, CL_STREAM, &ext, &ret);
    }

  int lines_per_compute_unit = args.height / compute_units;
for (int i = 0; i < NCU; i++) {

    convolve_kernel[i].setArg(3, coefficient_size);
    convolve_kernel[i].setArg(4, args.width);
    convolve_kernel[i].setArg(5, args.height);
           convolve_kernel[i].setArg(6, i * lines_per_compute_unit);
            convolve_kernel[i].setArg(7, lines_per_compute_unit);
            q.enqueueTask(convolve_kernel[i]);
}
int img_width = args.width;
int img_height = args.height;
 
  
	auto fpga_begin = std::chrono::high_resolution_clock::now();

    for(int frame_count = 0; frame_count < args.nframes; frame_count++) {
        // Read frame
        bytes_read = fread(inFrame.data(), 1, frame_bytes, streamIn);
        if(bytes_read != frame_bytes) {
        	printf("\nError: partial frame.\nExpected %zu\nActual %zu\n", frame_bytes, bytes_read);
        	break;
        }
    for(int cu = 0; cu < compute_units; cu++) 
    {

            int line_offset = cu*lines_per_compute_unit;
     int num_lines = lines_per_compute_unit;
     int elements = img_width * num_lines;
     int offset = std::max(0, line_offset - half) * img_width;
      int top_padding = 0;
     int bottom_padding = 0;
     int padding = 0;
     if(line_offset == 0) {
         top_padding = half * img_width;
     } else {
         padding = img_width *  half;
     }
     if(line_offset + num_lines < img_height) {
         padding += img_width * half + COEFFICIENT_SIZE;
     }else {
         bottom_padding = img_width * (half) + COEFFICIENT_SIZE;
     }
       int vector_size_bytes = sizeof(RGBPixel)* (elements + padding); 
       int coeff_size = sizeof(float)* COEFFICIENT_SIZE*COEFFICIENT_SIZE ;
        cl_stream_xfer_req rd_req{0};
        cl_stream_xfer_req wr_req{0};

        rd_req.flags = CL_STREAM_EOT| CL_STREAM_NONBLOCKING;
        wr_req.flags = CL_STREAM_EOT | CL_STREAM_NONBLOCKING;

        auto write_tag_a = "write_a_" + std::to_string(cu);
        wr_req.priv_data = (void *)write_tag_a.c_str();
        RGBPixel *p = inFrame.data();
        p = p+ offset;
        std::cout << "\n Writing Stream write_stream_a[" << cu << "]";
                  xcl::Stream::writeStream(write_stream_a[cu],
                                           (p),
                                           vector_size_bytes,
                                           &wr_req,
                                           &ret);
        auto write_tag_b = "write_b_" + std::to_string(cu);
        wr_req.priv_data = (void *)write_tag_b.c_str();

        std::cout << "\n Writing Stream write_stream_b[" << cu << "]";
                   xcl::Stream::writeStream(write_stream_b[cu],
                                            (filter_coeff.data()),
                                           coeff_size,
                                           &wr_req,
                                           &ret);
       auto read_tag = "read_" + std::to_string(cu);
        rd_req.priv_data = (void *)read_tag.c_str();
        RGBPixel *out_base = outFrame.data();
       out_base = out_base + line_offset*img_width;
       std::cout << "\n Reading Stream read_stream[" << cu << "]";
                        xcl::Stream::readStream(read_stream[cu],
                                          (outFrame.data()+line_offset*img_width),
                                          (elements)*sizeof(RGBPixel),
                                          &rd_req,
                                          &ret);
    } 
   // Sync for the async streaming
   int num_compl = 3 * NCU;

    // Checking the request completions
    cl_streams_poll_req_completions *poll_req;
    poll_req = (cl_streams_poll_req_completions *)malloc(
        sizeof(cl_streams_poll_req_completions) * num_compl);
    memset(poll_req, 0, sizeof(cl_streams_poll_req_completions) * num_compl);
    printf(
        "\n clPollStreams for (%d) events (CU: %d, axis_in: 2, axis_out: 1)\n",
        num_compl,
        NCU);
    
   xcl::Stream::pollStreams(device.get(),
                                       poll_req,
                                       num_compl,
                                       num_compl,
                                       &num_compl,
                                       500,
                                       &ret);

    std::cout <<"completed pooling "<<std::endl;
    RGBPixel *a;
   a = outFrame.data();
   for(int k=0;k<4;k++)
    {
   for(int q=0;q<5120;q++)
   {
    std::cout <<"b " << (int)a->b << "g " << (int)a->g << "r "<< (int)a->r << std::endl;

   }
    std::cout << "----------------------------------------------"<<std::endl;
    std::cout << "----------------------------------------------"<<std::endl;
    std::cout << "----------------------------------------------"<<std::endl;
    std::cout << "----------------------------------------------"<<std::endl;
  
    }
        
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
       }

        print_progress(frame_count, args.nframes);
    }
//    q.finish();

    auto fpga_end = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NCU; i++) {
        xcl::Stream::releaseStream(read_stream[i]);
        xcl::Stream::releaseStream(write_stream_a[i]);
        xcl::Stream::releaseStream(write_stream_b[i]);
    }
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
