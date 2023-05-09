/*
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
*/

#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>

#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY 1
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl2.hpp>
#include <CL/cl_ext_xilinx.h>

template <typename T>
struct aligned_allocator
{
  using value_type = T;
  T* allocate(std::size_t num)
  {
    void* ptr = nullptr;
    if (posix_memalign(&ptr,4096,num*sizeof(T)))
      throw std::bad_alloc();
    return reinterpret_cast<T*>(ptr);
  }
  void deallocate(T* p, std::size_t num)
  {
    free(p);
  }
};

class BitmapInterface
{
 private:
  char* core ;
  char* dib ;
  const char* filename ;
  int* image ;

  // Core header information
  unsigned short magicNumber ;
  unsigned int fileSize ;
  unsigned int offsetOfImage ;

  // DIB information
  int sizeOfDIB ;
  int sizeOfImage ;
  int height ;
  int width ;

 public:
  BitmapInterface(const char* f) ;
  ~BitmapInterface() ;

  bool readBitmapFile() ;
  bool writeBitmapFile(int* otherImage = NULL); 

  inline int* bitmap() { return image ; } 
  unsigned int numPixels() { return sizeOfImage/3 ; }

  inline int getHeight() { return height ; }
  inline int getWidth() { return width ; }

} ;

BitmapInterface::BitmapInterface(const char* f) : filename(f) 
{
  core = NULL ;
  dib = NULL ;
  image = NULL ;

  magicNumber = 0 ;
  fileSize = 0 ;
  offsetOfImage = 0 ;

  sizeOfDIB = 0 ;
  sizeOfImage = 0 ;

  height = -1 ;
  width = -1 ;
}

BitmapInterface::~BitmapInterface()
{
  if (core != NULL)
    delete [] core ;
  if (dib != NULL)
    delete [] dib ;
  if (image != NULL)
    delete [] image ;
}

bool BitmapInterface::readBitmapFile()
{
  // First, open the bitmap file
  int fd ;
  unsigned int fileSize ;

  fd = open(filename, O_RDONLY) ;
  if (fd < 0)
  {
    std::cerr << "Cannot read image file " << filename << std::endl ;
    return false ;
  }

  core = new char[14] ;
  read(fd, core, 14) ;
  magicNumber = (*(unsigned short*)(&(core[0]))) ;
  fileSize = (*(unsigned int*)(&(core[2]))) ;
  offsetOfImage = (*(unsigned int*)(&(core[10]))) ;

  // Just read in the DIB, but don't process it
  sizeOfDIB = offsetOfImage - 14 ;
  dib = new char[sizeOfDIB] ;
  read(fd, dib, sizeOfDIB) ;

  width = (*(int*)(&(dib[4]))) ;
  height = (*(int*)(&(dib[8]))) ;
  
  sizeOfImage = fileSize - 14 - sizeOfDIB ;
  int numPixels = sizeOfImage / 3 ; // RGB

  image = new int[numPixels] ;

  for (int i = 0 ; i < numPixels ; ++i)
  {
    // Use an integer for every pixel even though we might not need that
    //  much space (padding 0 bits in the rest of the integer)
    image[i] = 0 ; 
    read(fd, &(image[i]), 3) ;
  }

  return true ;
}
            
bool BitmapInterface::writeBitmapFile(int* otherImage)
{
  int fd ;
  fd = open("output.bmp", O_WRONLY | O_CREAT, 0644) ;

  if (fd < 0)
  {
    std::cerr << "Cannot open output.bmp for writing!" << std::endl ;
    return false ;
  }

  write(fd, core, 14) ;
  write(fd, dib, sizeOfDIB) ;

  int numPixels = sizeOfImage / 3 ;

  int* outputImage = otherImage != NULL ? otherImage : image ;
  
  for (int i = 0 ; i < numPixels ; ++i)
  {
    write(fd, &(outputImage[i]), 3) ;
  }

  return true ;
}

#define OCL_CHECK(error,call)                                       \
    call;                                                           \
    if (error != CL_SUCCESS) {                                      \
      printf("%s:%d Error calling " #call ", error code is: %d\n",  \
              __FILE__,__LINE__, error);                            \
      exit(EXIT_FAILURE);                                           \
    }                                       

namespace xcl {
std::vector<cl::Device> get_devices(const std::string& vendor_name) {

    size_t i;
    cl_int err;
    std::vector<cl::Platform> platforms;
    OCL_CHECK(err, err = cl::Platform::get(&platforms));
    cl::Platform platform;
    for (i  = 0 ; i < platforms.size(); i++){
        platform = platforms[i];
        OCL_CHECK(err, std::string platformName = platform.getInfo<CL_PLATFORM_NAME>(&err));
        if (platformName == vendor_name){
            std::cout << "Found Platform" << std::endl;
            std::cout << "Platform Name: " << platformName.c_str() << std::endl;
            break;
        }
    }
    if (i == platforms.size()) {
        std::cout << "Error: Failed to find Xilinx platform" << std::endl;
        exit(EXIT_FAILURE);
    }
   
    //Getting ACCELERATOR Devices and selecting 1st such device 
    std::vector<cl::Device> devices;
    OCL_CHECK(err, err = platform.getDevices(CL_DEVICE_TYPE_ACCELERATOR, &devices));
    return devices;
}
   
std::vector<cl::Device> get_xil_devices() {
    return get_devices("Xilinx");
}

char* read_binary_file(const std::string &xclbin_file_name, unsigned &nb) 
{
    std::cout << "INFO: Reading " << xclbin_file_name << std::endl;

	if(access(xclbin_file_name.c_str(), R_OK) != 0) {
		printf("ERROR: %s xclbin not available please build\n", xclbin_file_name.c_str());
		exit(EXIT_FAILURE);
	}
    //Loading XCL Bin into char buffer 
    std::cout << "Loading: '" << xclbin_file_name.c_str() << "'\n";
    std::ifstream bin_file(xclbin_file_name.c_str(), std::ifstream::binary);
    bin_file.seekg (0, bin_file.end);
    nb = bin_file.tellg();
    bin_file.seekg (0, bin_file.beg);
    char *buf = new char [nb];
    bin_file.read(buf, nb);
    return buf;
}
};
int main(int argc, char* argv[])
{
    cl_int err;
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <xclbin> <input bitmap> <golden bitmap>" << std::endl;
        return EXIT_FAILURE ;
    }
    const char* bitmapFilename = argv[2];
    const char* goldenFilename;

    //Read the input bit map file into memory
    BitmapInterface image(bitmapFilename);
    bool result = image.readBitmapFile() ;
    if (!result)
    {
        std::cout << "ERROR:Unable to Read Input Bitmap File "<< bitmapFilename << std::endl;
        return EXIT_FAILURE ;
    }

    int width = image.getWidth() ;
    int height = image.getHeight();

    //Allocate Memory in Host Memory
    size_t image_size = image.numPixels();
    size_t image_size_bytes = image_size * sizeof(int);
    std::vector<int,aligned_allocator<int>> inputImage(image_size);
    std::vector<int,aligned_allocator<int>> outImage(image_size);

    // Copy image host buffer
    memcpy(inputImage.data(), image.bitmap(), image_size_bytes);

// OPENCL HOST CODE AREA START
    // get_xil_devices() is a utility API which will find the xilinx
    // platforms and will return list of devices connected to Xilinx platform
    std::cout << "Creating Context..." << std::endl;
    std::vector<cl::Device> devices = xcl::get_xil_devices();
    cl::Device device = devices[0];

    OCL_CHECK(err, cl::Context context (device, NULL, NULL, NULL, &err));
    OCL_CHECK(err, cl::CommandQueue q (context, device, CL_QUEUE_PROFILING_ENABLE, &err));
    OCL_CHECK(err, std::string device_name = device.getInfo<CL_DEVICE_NAME>(&err));

    // find_binary_file() is a utility API which will search the xclbin file for
    // targeted mode (sw_emu/hw_emu/hw) and for targeted platforms.
    std::string binaryFile(argv[1]);

    // read_binary_file() is a utility API which will load the binaryFile
    // and will return Binaries.
    unsigned int fileBufSize;
    char* fileBuf = xcl::read_binary_file(binaryFile, fileBufSize);
    cl::Program::Binaries bins{{fileBuf, fileBufSize}};
    devices.resize(1);
    OCL_CHECK(err, cl::Program program (context, devices, bins, NULL, &err));
    OCL_CHECK(err, cl::Kernel apply_watermark(program, "apply_watermark", &err));
    cl_kernel krnl=apply_watermark.get();

    // For Allocating Buffer to specific Global Memory Bank, user has to use cl_mem_ext_ptr_t
    cl_mem_ext_ptr_t inExt, outExt;  // Declaring two extensions for both buffers
    inExt.flags  = 0; // argument index ( 0 means that this buffer will be passed to argument 0 of the kernel )
    outExt.flags = 1; // argument index ( 1 means that this buffer will be passed to argument 1 of the kernel )
    inExt.obj 	 = inputImage.data();
    outExt.obj   = outImage.data(); 
    // Setting kernel handle to Param
    inExt.param  = krnl ; outExt.param = krnl;

    // Allocate Buffer in Global Memory
    // Buffers are allocated using CL_MEM_USE_HOST_PTR for efficient memory and
    // Device-to-host communication
    std::cout << "Creating Buffers..." << std::endl;
    OCL_CHECK(err, cl::Buffer buffer_inImage(context, CL_MEM_READ_ONLY | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
    		image_size_bytes, &inExt, &err));
    OCL_CHECK(err, cl::Buffer buffer_outImage(context, CL_MEM_WRITE_ONLY | CL_MEM_EXT_PTR_XILINX | CL_MEM_USE_HOST_PTR,
    		image_size_bytes, &outExt, &err));

    // Copy input data to device global memory
    std::cout<< "Copying data..." << std::endl;
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_inImage}, 0));

    std::cout<< "Setting arguments..." <<std::endl;
    OCL_CHECK(err, err = apply_watermark.setArg(0, buffer_inImage));
    OCL_CHECK(err, err = apply_watermark.setArg(1, buffer_outImage));
    OCL_CHECK(err, err = apply_watermark.setArg(2, width));
    OCL_CHECK(err, err = apply_watermark.setArg(3, height));

    // Launch the Kernel
    // For HLS kernels global and local size is always (1,1,1). So, it is recommended
    // to always use enqueueTask() for invoking HLS kernel
    std::cout <<"Launching Kernel... " << std::endl;
    OCL_CHECK(err, err = q.enqueueTask(apply_watermark));

    // Copy Result from Device Global Memory to Host Local Memory
    std::cout << "Getting Results..." << std::endl;
    OCL_CHECK(err, err = q.enqueueMigrateMemObjects({buffer_outImage}, CL_MIGRATE_MEM_OBJECT_HOST));
    q.finish();
//OPENCL HOST CODE AREA ENDS

    bool match = true;
    if (argc > 3){
        goldenFilename = argv[3];
        //Read the golden bit map file into memory
        BitmapInterface goldenImage(goldenFilename);
        result = goldenImage.readBitmapFile() ;
        if (!result)
        {
            std::cout << "ERROR:Unable to Read Golden Bitmap File "<< goldenFilename << std::endl;
            return EXIT_FAILURE ;
        }
        //Compare Golden Image with Output image
        if ( image.getHeight() != goldenImage.getHeight() || image.getWidth() != goldenImage.getWidth()){
            match = false;
        }else{
            int* goldImgPtr = goldenImage.bitmap();
            for (unsigned int i = 0 ; i < image.numPixels(); i++){
                if (outImage[i] != goldImgPtr[i]){
                    match = false;
                    printf ("Pixel %d Mismatch Output %x and Expected %x \n", i, outImage[i], goldImgPtr[i]);
                    break;
                }
            }
        }
    }
    // Write the final image to disk
    image.writeBitmapFile(outImage.data());

    std::cout << (match ? "TEST PASSED" : "TEST FAILED") << std::endl;
    return (match ? EXIT_SUCCESS : EXIT_FAILURE) ;
}
