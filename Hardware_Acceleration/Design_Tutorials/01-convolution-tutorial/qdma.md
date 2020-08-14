<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ Application Acceleration Development Flow Tutorials</h1>
   <a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">See 2019.2 Vitis Application Acceleration Development Flow Tutorials</a>
   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# 7. Using QDMA Streaming with Multiple Compute Units

 In this section, you will modify the design to use multiple CUs with streaming functionality based on the output from the previous lab.

>**TIP:** The completed kernel source file is provided in the `reference-files/qdma` folder if needed.

>**IMPORTANT:** Before diving into the code transformation, some of the header files: `types.h` and `kernel.h`, `xcl2.hpp` and `xcl2.cpp` and `kernel_types.h` (new file) are added/changed from the previous step to separate the host and kernel code function/struct definitions.

In this step, you must make both the code and kernel code transformations.

Open the `convolve.cpp` host code file from `src/qdma`, and make the following modifications.

In the previous step, you developed kernel code to divide the image into a partition, so each CU can work on an individual portion in parallel. In this step, each CU will be associated with a stream interface, so you need to partition the image on the host, so the data can be sent accordingly to the device.

## Initialize the Device

In this step, you must set and create the stream classes. As you work through this section, refer to step 1 in the host code. This typically involves the following steps:

- Declare the kernel streaming class and the streaming APIs. 
- Create kernel objects for each CU.
- Initialize the streaming classes.
- Create streams for the CUs.

1. Declare the classes for `mem_ext_ptr` and the custom stream APIs that binds to Xilinx Streaming APIs.

    ```
    #include "CL/cl_ext_xilinx.h"

    decltype(&clCreateStream) xcl::Stream::createStream = nullptr;
    decltype(&clReleaseStream) xcl::Stream::releaseStream = nullptr;
    decltype(&clReadStream) xcl::Stream::readStream = nullptr;
    decltype(&clWriteStream) xcl::Stream::writeStream = nullptr;
    decltype(&clPollStreams) xcl::Stream::pollStreams = nullptr;
    ```

2. Associate each streaming interface  with a unique kernel `cl_kernel` object for each CU.

    ```
    std::string krnl_name = args.kernel_name;
    std::vector<cl::Kernel> convolve_kernel(NCU);

    for (int i = 0; i < NCU; i++)
    {
                cu_id = std::to_string(i + 1);
            auto krnl_name_full = krnl_name +":{" + "convolve_fpga_" + cu_id +"}" ;
                printf("Creating a kernel [%s] for CU(%d)\n",
                        krnl_name_full.c_str(),
                        i);
                convolve_kernel[i] = cl::Kernel(
                                program, krnl_name_full.c_str(), &err);
    }
    ```

3. Before using the streaming interface on the host, the streaming class needs to be initialized with the platform. The device info is available from `device.getInfo`.

    ```
    auto platform_id = device.getInfo<CL_DEVICE_PLATFORM>(&err);

    //Initialization of streaming class is needed before using it.
    xcl::Stream::init(platform_id);
    ```

4. Looking at the kernel interface, the three function arguments that are mapped to AXI can be converted into streams. Create three streams for each CU. 

    Each of the streams should be directly attached to the OpenCL device object because it does not use any command queue. A stream itself is a command queue that only passes the data in a particular direction, either from host to kernel or from kernel to host.

    - The first step is creating the stream using the `clcreatestream` which take five arguments:
       - **`device_id`**: Device handle on which the stream will be created.
       - **`flags`**: An appropriate flag should be used to denote the stream as XCL_STREAM_READ_ONLY or XCL_STREAM_WRITE_ONLY from the perspective of the kernel program.
       - **`attributes`**: Attributes of the requested stream.
       - **`ext`**: Specify how the stream is connected to the device, a Xilinx extension pointer object (cl_mem_ext_ptr_t) is used to identify the kernel, and the kernel argument the stream is associated with. The `ext.flag` is used to communicate to the respective kernel argument
          - **`ext.param`**: Specify the kernel name.
          - **`ext.flags`**: Specify to which kernel argument the stream has to connected.
          - **`ext.obj`**: Set to null.
       - **`errcode_ret`**: Return value (for example, CL_SUCCESS).

        ```
        // Streams
        std::vector<cl_stream> write_stream_a(NCU);
        std::vector<cl_stream> write_stream_b(NCU);
        std::vector<cl_stream> read_stream(NCU);
        cl_int ret;

        for (int i = 0; i < NCU; i++) {
                // Device Connection specification of the stream through extension pointer
                cl_mem_ext_ptr_t ext;
                ext.param = convolve_kernel[i].get();
                ext.obj = NULL;
                // The .flag should be used to denote the kernel argument
                // Create write stream for argument 0 and 1 of kernel
                ext.flags = 0;
                    write_stream_a[i] = xcl::Stream::createStream(
                        device.get(), XCL_STREAM_READ_ONLY, CL_STREAM, &ext, &ret);
                ext.flags = 2;
                    write_stream_b[i] = xcl::Stream::createStream(
                        device.get(), XCL_STREAM_READ_ONLY, CL_STREAM, &ext, &ret);

                //Create read stream for argument 2 of kernel
                ext.flags = 1;
                    read_stream[i] = xcl::Stream::createStream(
                        device.get(), XCL_STREAM_WRITE_ONLY, CL_STREAM, &ext, &ret);
            }
        ```

## Executing the Kernel

Now that you have set up the stream classes for the hardware, this step lets you send the data to the kernel on the device. As you work through this section, refer to step 2 in the host code.

- Set up the non-streaming kernel arguments 
- Execute the kernel.
- Set up the offsets to divide the image for each CU.
- Initiate and send the read and write stream transfer per CU.
- Pool the streams.

1. Set the remaining non-streaming interface arguments and enqueue the kernel using the following standard APIs `- setargs` and `enqueueTask`. 

    ```
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
    ```

2. Before initiating the read and write transfers, you must partition the image with the correct offsets to access the image.

    * The `line_offset` and `offset` variables are used to calculate the offsets from the beginning of the image to the first pixel that the CU will read.
    * The `padding` variable, on the other hand, is the number of pixels to read including the regions around the convolution window.
    * The `num_lines` variable is the number of lines per CU.
    * The `elements` is the total number of bytes to be computed per CU.

        ```
        for(int cu = 0; cu < compute_units; cu++) {
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
        ```

3. Initiate the read and write stream transfer per CU using the `clReadStream` and `clWriteStream` commands. The read and write streams takes five arguments.
    - **`stream`**: The first argument takes the stream it needs to read/write to the device.
    - **`ptr`**: The ptr argument contains the place for input/output argument. 
    - **`size`**: The number of bytes to write.
    - **`req_type`**: The usage of attribute CL_STREAM_XFER_REQ associated with read and write request. The req_type should set the following arguments:
       - **`flag`**: The flag is used to denote transfer mechanism:
            - **`CL_STREAM_NONBLOCKING`**: By default, the Read and Write transfers are blocking. For non-blocking transfer, CL_STREAM_NONBLOCKING has to be set.
	      - **`.priv_data`**: The `.priv_data` is used to specify a string (as a name for tagging purposes) associated with the transfer. This will help identify specific transfer completion when polling the stream completion. It is required when using the non-blocking version of the API.
    - **`errcode_ret`**: Return value (for example, CL_SUCCESS).

```
int vector_size_bytes = sizeof(RGBPixel)* (elements + padding); 
       int coeff_size = sizeof(float)* COEFFICIENT_SIZE*COEFFICIENT_SIZE ;
        cl_stream_xfer_req rd_req{0};
        cl_stream_xfer_req wr_req{0};

        rd_req.flags = CL_STREAM_EOT |CL_STREAM_NONBLOCKING;
        wr_req.flags = CL_STREAM_EOT |CL_STREAM_NONBLOCKING;
        
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
                                          (elements)*sizeof(int),
                                          &rd_req,
                                          &ret);
```

4. Poll all the streams for completion.

    * For the non-blocking transfer, a polling API is provided to ensure the read/write transfers are completed. 
    * For the blocking version of the API, polling is not required. The polling results are stored in the cl_streams_poll_req_completions array, which can be used in verifying and checking the stream events result.

    The `clPollStreams` is a blocking API. It returns the execution to the host code as soon as it receives the notification that all stream requests have been completed, or until you specify the timeout.

    ```
    int num_compl = 3 * NCU;

        // Checking the request completions
        cl_streams_poll_req_completions *poll_req;
        poll_req = (cl_streams_poll_req_completions *)malloc(
            sizeof(cl_streams_poll_req_completions) * num_compl);
        memset(poll_req, 0, sizeof(cl_streams_poll_req_completions) * num_compl);
        print(
            "\n clPollStreams for (%d) events (CU: %d, axis_in: 2, axis_out: 1)\n",
            num_compl,
            NCU);
    xcl::Stream::pollStreams(device.get(),
                                        poll_req,
                                        num_compl,
                                        num_compl,
                                        &num_compl,
                                        50000,
                                        &ret);
    ```

## Release the Streams

After the successful poll request is completed, the streams need to be released using the `releaseStream`.

```
for (int i = 0; i < NCU; i++) {
    xcl::Stream::releaseStream(write_stream_a[i]);
    xcl::Stream::releaseStream(write_stream_b[i]);
    xcl::Stream::releaseStream(read_stream[i]);
  }
```

This completes the required host code modifications.

## Kernel Code Modifications

Next, modify the kernel code. The kernel code changes can be divided into four steps:

- Class used for host to kernel streaming.
- Limitations of QDMA data type.
- Change pointers to hls::stream.
- Modify the data movers.

### Host to Kernel Streaming 

Vitis HLS provides a C++ template class hls::stream<> for modeling streaming data structures. The streams implemented with the hls::stream<> class have the following attributes.

 - Must use `qdma_axis<D,0,0,0>` data type. The `qdma_axis` data is available in the `ap_axi_sdata.h` header file.

 - The `qdma_axis` data type contains three variables, which should be used inside the kernel code:
    * **`data`**: Internally, the qdma_axis data type contains an ap_int <D> that should be accessed by the .get_data() and .set_data() method. The D must be 8, 16, 32, 64, 128, 256, or 512 bits wide.
    * **`last`**: The last variable is used to indicate the last value of an incoming and outgoing stream. When reading from the input stream, `last` is used to detect the end of the stream. Similarly, when the kernel writes to an output stream transferred to the host, the `last` variable must be set to indicate the end of stream.
    - **`get_last/set_last`**: Accesses and sets the last variable used to denote the end of the stream.
	- **`keep`**: In some special situations, the `keep` signal can be used to truncate the last data to the fewer number of bytes. However, the keep should not be used to any data other than the last data from the stream. Therefore, in most of the cases, you should set keep to -1 for all of the outgoing data from the kernel.
	- **`get_keep/set_keep`**: Accesses/sets the `keep` variable. For all the data before the last data, `keep` must be set to -1 to denote all bytes of the data are valid. For the last data, the kernel has the flexibility to send fewer bytes. For example, for the four bytes of data transfer, the kernel can truncate the last data by sending one byte, two bytes, or three bytes using the following set_keep() function.
        - If the last data is one byte ≥ `.set_keep(1)`
        - If the last data is two bytes ≥ `.set_keep(3)`
        - If the last data is three bytes ≥ `.set_keep(7)`
        - If the last data is s all four bytes (similar to all non-last data) ≥ `set_keep(-1)`.

### QDMA Data Type Limitations

Currently, the qdma class only supports `ap_uint` and cannot be overloaded. This design uses `floats` and `RGBPixel` as the data types. You will use a struct function to/from to separate and concatenate the `ap_int` into the `RGB` type.

```
#ifdef KERNEL_HEADERS
#include "ap_axi_sdata.h"
#include "ap_int.h"
#include "hls_stream.h"
#define DWIDTH 32

typedef qdma_axis<DWIDTH, 0, 0, 0> pkt;
#endif


struct RGBPixel
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
//    unsigned char a;
#ifdef KERNEL_HEADERS

void init(ap_int<32> d )
{
#pragma HLS INLINE
    r = d.range(7,0);
    g = d.range(15,8);
    b = d.range(23,16);

}
ap_int<32> func1()
{
#pragma HLS INLINE 
  ap_int<32> c = (ap_int<8>(b) , ap_int<8>(g), ap_int<8>(r));
    return c; 
}

#endif

}__attribute__((aligned(4)));
```

The same limitation is also applied to the `coeff` argument, so you will use a union to convert an `int` to a `float`. Place this in the `convolve_fpga.cpp`.

```
union test{
int x;
float y;
};
```

### Modify the Kernel Interface

Change the following kernel interface from memory mapped to `hls::stream`.  

```
void convolve_fpga( hls::stream<pkt>& inFrame, hls::stream<pkt>& outFrame,
		hls::stream<pkt>& coefficient,
		int coefficient_size, int img_width, int img_heightint line_offset, int num_lines) 
```

### Modify the Data Movers

In the previous step, the kernel code is divided into three steps: read, compute, and write functions. The read/write function are the data movers from/to the DDR. This step only needs to change these data movers which originally use array pointers to `hls::stream` objects. This `hls::stream` uses the `ap_axiu` objects as mentioned above. This function uses the methods mentioned above to read the data until the last packet is received using the `do-while` loop.

```
 void read_dataflow(hls::stream<RGBPixel>& read_stream, hls::stream<pkt>& in,
                   int img_width, int elements, int half,
                   int top_padding, int bottom_padding) {
     while(top_padding--) {
         read_stream << zero;
     }
    RGBPixel a_temp;
     int pixel = 0;
     ap_int<1> last;
     bool eos = false;
     do{

    pkt a_t = in.read();
    ap_int<32> in1 = a_t.get_data();
    last = a_t.get_last();
    if(last)
    {
        eos = true;
    }
      a_temp.init(in1);
       RGBPixel a;
       a = a_temp;
         read_stream << a_temp;

     }while(eos ==false);

        while(bottom_padding--) {
         read_stream << zero;
     }
 }


 void write_dataflow(hls::stream<pkt>& outFrame, hls::stream<RGBPixel>& write_stream,
                     int elements)
 {
     int pixel = 0;
     pkt t_out;
     ap_int<32> a_out;
     RGBPixel tmpout;
     int i=0;
     while(elements--) {

        write_stream >> tmpout;
        a_out = tmpout.func1();
       t_out.set_data(a_out);
        i++;
        if(elements ==0)
        {
            t_out.set_last(1);
        }
        else
        {
            t_out.set_last(0);
           t_out.set_keep(-1);
             outFrame.write(t_out);
        }
     }
  }
```

## Run Hardware Emulation for Multiple CUs with Streaming Interfaces

1. Before running emulation, you need to set the number of CUs to 4. Open the `design.cfg` and modify the `nk` option as follows.

   ```
   nk=convolve_fpga:4
   ```

   The `nk` option is used to specify the number of kernel instances, or CUs, generated during the linking step of the build process.

2. Go to the `makefile` directory.
3. Use the following command to run hardware emulation.

   ```
   make run TARGET=hw_emu STEP=qdma SOLUTION=1 NUM_FRAMES=1
   ```

    The following code shows the results of this kernel running on four CUs.

    ```
    Data transfer on stream interfaces
    HOST-->convolve_fpga_1:coefficient           0.035 KB        
    HOST-->convolve_fpga_3:inFrame               24.012 KB       
    convolve_fpga_3:outFrame-->HOST              20.000 KB       
    HOST-->convolve_fpga_4:coefficient           0.035 KB        
    HOST-->convolve_fpga_4:inFrame               22.000 KB       
    convolve_fpga_4:outFrame-->HOST              20.000 KB       
    HOST-->convolve_fpga_1:inFrame               22.012 KB       
    convolve_fpga_1:outFrame-->HOST              20.000 KB       
    HOST-->convolve_fpga_2:coefficient           0.035 KB        
    HOST-->convolve_fpga_2:inFrame               24.012 KB       
    convolve_fpga_2:outFrame-->HOST              20.000 KB       
    HOST-->convolve_fpga_3:coefficient            0.035 KB        
    ```

    You can now perform four times more work in about the same amount of time. Because each CU needs to read the surrounding padded lines, more data is transferred from the global memory.

## View Profile Summary Report for Hardware Emulation

Use the following command to view the Profile Summary report.

```
make view_run_summary TARGET=hw_emu STEP=qdma
```

The kernel execution time for the four CUs is around 0.135061 ms each.

Here is the updated table.

| Step                    | Image Size | Time (HW-EM)(ms) | Reads (KB)      | Writes (KB)    | Avg. Read (KB) | Avg. Write (KB) | Bandwidth (MBps)  |
| :-------------------    | :--------- | ---------------: | --------------: | -------------: | -------------: | --------------: | ---------: |
| baseline                |     512x10 | 3.903            | 344             |        20.0    |          0.004 |           0.004 |    5.2     |
| localbuf                |     512x10 | 1.574 (2.48x)    | 21 (0.12x)      |        20.0    |          0.064 |           0.064 |    13      |
| fixed-type data         |     512x10 | 0.46 (3.4x)      | 21              |        20.0    |          0.064 |           0.064 |    44      |
| dataflow                |     512x10 | 0.059 (7.8x)     | 21              |        20.0    |          0.064 |           0.064 |    347     |
| multi-CU                |     512x40*| 0.358           | 92        |       80.0 (4x)|          0.064 |           0.064 |    1365*   |
| Stream-multi-CU	  |     512x40*| 0.130561  (~3x)  | 96.188 (4.3x)       |       80.0 |          22.540 |           0.036 |    1200   |

>**NOTE:**
>
>* The Stream-multi-CU version processed four times of the data compared to the previous versions. Even if the execution time for each CU does not change, four parallel CUs increase the system performance by almost four times.
>* This is calculated by 4x data/time. Here the data transfer time is not accounted for, and you assume that the four CUs are executing in parallel. This is not as accurate as the hardware run, but you will use it as a reference for optimizations effectiveness.

## Next Steps

In this step, you performed the host code and kernel code modifications to generate multiple streaming  CUs. In the next step, you have the application [run the accelerator in hardware](./RunOnHardware.md).

[hostopt_hwemu_profilesummary]: ./images/191_hostopt_hwemu_pfsummary_40_2.jpg "Host code optimization version hardware emulation profile summary"
</br>
<hr/>
<p align="center" class="sphinxhide"><b><a href="/docs/vitis-getting-started/README.md">Return to Getting Started Pathway</a> — <a href="/docs/convolution-tutorial/README.md">Return to Start of Tutorial</a></b></p>

<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
