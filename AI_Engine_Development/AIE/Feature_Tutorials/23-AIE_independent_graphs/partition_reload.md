<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Partition Reloading

This tutorial covers the packaging requirements and host code programming guidance for partition reloading. The compilation and linking guidance can be found in [Compiling AI Engine Graphs for Independent Partitions](./independent_graphs.md).

## Generate AI Engine-only and PL-only XCLBIN

You must use the v++ packager to generate AI Engine-only and PL-only XCLBIN files that support runtime reloading.

* AI Engine-only XCLBIN: Contains AI Engine metadata and the AI Engine PDI (Programmable Device Image) for a single partition.

To generate an AI Engine-only XCLBIN for a specific partition, use the following command:

	${VCC} -p -s -t ${TARGET} -f ${XSA} \
		--package.defer_aie_run \
		--package.aie_overlay ./<PR>/libadf.a \
		--output <PR>.xclbin 

* PL-only XCLBIN: Contains PL metadata.

To generate PL-only XCLBIN, use the following command:

	${VCC} -p -s -t ${TARGET} -f ${XSA} \
		--config package_pl_only.cfg \
		--output pl.xclbin 

The content of `package_pl_only.cfg`:

	[advanced]
	param=package.generateFlatPlVersalXclbin=1

For more information on generating AI Engine-only and PL-only XCLBIN, please refer to [UG1076: Programming-the-PS-Host-Application](https://docs.amd.com/r/en-US/ug1076-ai-engine-environment/Programming-the-PS-Host-Application).

## Host code for controlling Graph and Partition Reloading

The host code for controlling graph and partition reloading usually contains following steps:

1. Open the device.

2. Load the PL-only XCLBIN and retrieve the PL UUID. Then, create a hardware context for the PL region.

3. Load the AI Engine-only XCLBIN and retrieve the AI Engine UUID. Create hardware contexts for AI Engine partitions as needed—multiple contexts can be created for different partitions.

4. Create buffer objects, PL kernel handles, and graph handles. Each object or handle is associated with a specific hardware context.

5. Pre-process the user input data.

6. Operate on the created objects or handles during execution.

7. Post-process the output data after execution.

Following is an example code for controlling graph and associated GMIO data transferring:

	auto xclbin_1 = xrt::xclbin(xclbinfile_gr1);
	auto uuid1 = device.register_xclbin(xclbin_1);
	std::cout<<"xclbin file name:"<<xclbinfile_gr1<<std::endl;
	xrt::hw_context *hwctx_1=new xrt::hw_context(device, uuid1);
	std::cout<<"Load XCLBIN successfully"<<std::endl;

	auto din_buffer = new xrt::aie::bo (*hwctx_1, BLOCK_SIZE_in_Bytes,xrt::bo::flags::normal, /*memory group*/0); //Only non-cacheable buffer is supported
	int* dinArray= din_buffer->map<int*>();
	auto dout_buffer = new xrt::aie::bo (*hwctx_1, BLOCK_SIZE_in_Bytes,xrt::bo::flags::normal, /*memory group*/0); //Only non-cacheable buffer is supported
	int* doutArray= dout_buffer->map<int*>();
    int* doutRef=(int*)malloc(BLOCK_SIZE_in_Bytes);
    std::cout<<"Allocate buffer completed"<<std::endl;

    int coeff[8]={1,2,3,4,5,6,7,8};
	
    for(int i=0;i<ITERATION*1024/4;i++){
		dinArray[i]=i;
    }

	auto ghdl=new xrt::graph(*hwctx_1,"gr");
	std::cout<<"Open partition 0 with graph1 successfully"<<std::endl;
	xrt::aie::buffer *bufIn=new xrt::aie::buffer(*hwctx_1, "gr.gmioIn");
	//memory group is 0, depending on the platform
	bufIn->async(*din_buffer, XCL_BO_SYNC_BO_GMIO_TO_AIE, BLOCK_SIZE_in_Bytes, 0); 
    ghdl->run(ITERATION);
	xrt::aie::buffer *bufOut=new xrt::aie::buffer(*hwctx_1, "gr.gmioOut");
	//memory group is 0, depending on the platform
	bufOut->async(*dout_buffer, XCL_BO_SYNC_BO_AIE_TO_GMIO, BLOCK_SIZE_in_Bytes, 0);
    //PS can do other tasks here when data is transferring
    std::cout<<"Waiting for graph to be completed"<<std::endl;
	bufOut->wait();

    ref_func(dinArray,coeff,doutRef,ITERATION*1024/4);
    for(int i=0;i<ITERATION*1024/4;i++){
		if(doutArray[i]!=doutRef[i]){
			std::cout<<"ERROR:dout["<<i<<"]="<<doutArray[i]<<",gold="<<doutRef[i]<<std::endl;
			error++;
		}
    }
    
    std::cout<<"GMIO transactions finished"<<std::endl;

	ghdl->end();
   	if(error==0){
		std::cout<<"TEST PASSED!"<<std::endl;
   	}else{
		std::cout<<"ERROR!"<<std::endl;
   	}
	//delete the objects that is associated to the hw context, and then delete the hw context.
	delete bufIn;
	delete bufOut;
	delete din_buffer;
	delete dout_buffer;
	delete ghdl;
	delete hwctx_1;

To reload the same partition multiple times, there can be multiple approaches:

1. Exit and restart the application. When the hardware context is created again in host code, the corresponding AI Engine PDI is reloaded into the partition.

2. Use C++ `new` and `delete` to explicitly allocate and destroy the hardware context (along with any objects created on it) before each reload, shown as above code. When the hardware context is created by the `new` method, the corresponding AI Engine PDI is reloaded into the partition.

3. Leverage C++ local scoping to ensure that the AI Engine hardware context and its associated operations are created and automatically destroyed between reloads.

For example (where the sub-function serves as the local scope between reloads):

	#include <stdlib.h>
	#include <fstream>
	#include <iostream>
	#include <unistd.h>
	#include <math.h>
	#include "xrt/xrt_kernel.h"
	#include "xrt/xrt_graph.h"
	
	const int ITERATION=4;
	const int ELEM_per_iter=256;
	int run(char* xclbinFilename_pl, char* xclbinFilename_aie, int rtp_type){
	
		size_t output_size_in_bytes = ELEM_per_iter*4*ITERATION;
		int ret;
		// Open xclbin
		auto dhdl = xrt::device(0); //device index=0
		auto xclbin_pl = xrt::xclbin(std::string(xclbinFilename_pl));
		auto uuid_pl = dhdl.register_xclbin(xclbin_pl);
		xrt::hw_context hwctx_pl{dhdl, uuid_pl};
		auto xclbin_aie = xrt::xclbin(std::string(xclbinFilename_aie));
		auto uuid_aie = dhdl.register_xclbin(xclbin_aie);
		xrt::hw_context hwctx_aie{dhdl, uuid_aie};
	
		// s2mm & datagen kernel handle
		auto s2mm = xrt::kernel(hwctx_pl, "s2mm:{s2mm_1}");
		auto datagen = xrt::kernel(hwctx_pl, "datagen");
	
		// output memory
		auto out_bo = xrt::bo(hwctx_pl, output_size_in_bytes,static_cast<xrt::bo::flags>(0),s2mm.group_id(0));
		auto host_out=out_bo.map<int*>();
	
		//kernel run
		auto s2mm_run = s2mm(out_bo, nullptr, ELEM_per_iter*ITERATION);//1st run for s2mm has started
		auto datagen_run = datagen(nullptr, ELEM_per_iter*ITERATION,0);
	
		auto ghdl=xrt::graph(hwctx_aie,"gr");
		ghdl.run(ITERATION);
		int value[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
		if(rtp_type==0){
			ghdl.update("gr.k.in[1]",10);
		}else if(rtp_type==1){
			ghdl.update("gr.k.in[1]",value);
		}
		ghdl.end();
	
		s2mm_run.wait();
		out_bo.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
	
		int match = 0;	
		int base=10;
		for (int i = 0; i < ITERATION; i++) {
			for(int j=0;j<ELEM_per_iter;j++){
				if(rtp_type==0){
					if(host_out[ELEM_per_iter*i+j]!=base+i*ELEM_per_iter+j){
						match=1;
					}
				}else if(rtp_type==1){
					if(host_out[ELEM_per_iter*i+j]!=value[j%16]+i*ELEM_per_iter+j){
						match=1;
					}
				}
			}
		}
	
		return match;
	}
	
	int main(int argc, char* argv[])
	{
		try {
			if(argc != 3 && argc != 4) {
				std::cout << "Usage: " << argv[0] <<" <pl.xclbin> <aie_only.xclbin> <aie_only_second.xclbin>" << std::endl;
				return EXIT_FAILURE;
			}
			char* xclbinFilename_pl = argv[1];
			char* xclbinFilename_aie = argv[2];
			auto match = run(xclbinFilename_pl, xclbinFilename_aie,0);
			std::cout << "TEST PR1 Graph0 " << (match ? "FAILED" : "PASSED") << "\n";
			if(argc==4){//Do second xclbin reload
				xclbinFilename_aie = argv[3];
				match = run(xclbinFilename_pl, xclbinFilename_aie,1);
				std::cout << "TEST PR1 SECOND GRAPH " << (match ? "FAILED" : "PASSED") << "\n";
			}
			return (match ? EXIT_FAILURE :  EXIT_SUCCESS);
		}	
			catch (std::exception const& e) {
			std::cout << "Exception: " << e.what() << "\n";
			std::cout << "FAILED TEST\n";
			return 1;
		}
	}

Note that all above concepts apply on the following reference designs:

## Reference Design 1: (./partition_reload_same_graph)

The design contains 3 partitions in directories `partition_reload_same_graph/pr0_gmio`, `partition_reload_same_graph/pr1_rtp` and `partition_reload_same_graph/pr2_perf`. The graph code can be found in `aie` directories in the partitions. The host code for running each partition seperately can be found in `sw` directories in the partitions. And the `partition_reload_same_graph/sw` directory contains host code for controlling multiple partitions. 

To build design ready for HW, run the following command:

```
cd <TUTORIAL_PATH>/partition_reload_same_graph
make package
```

Boot with sd_card.img and change the working directory to `/run/media/mmcblk0p1`. Run applications to control the separate partitions (each exe controls one partition):

```
./host0.exe pr0.xclbin
./host1.exe pl.xclbin pr1.xclbin
./host2.exe pl.xclbin pr2.xclbin
```

Following host application executes and reloads multiple partitions:

```
./host.exe pl.xclbin pr0.xclbin pr1.xclbin pr2.xclbin
```

## Reference Design 2: (./partition_reload_diff_graph)

The design contains 3 partitions in directories `partition_reload_diff_graph/pr0_gmio`, `partition_reload_diff_graph/pr1_rtp` and `partition_reload_diff_graph/pr2_perf`. The graph code can be found in `aie` directories in the partitions. Another different graph for the same partition can be found in `partition_reload_diff_graph/pr0_gmio/aie2` and `partition_reload_diff_graph/pr1_rtp/aie2`.

The host code for running each partition seperately can be found in `sw` directories in the partitions. And the `partition_reload_diff_graph/sw` directory contains host code for controlling multiple partitions. 

To build design ready for HW, run the following command:

```
cd <TUTORIAL_PATH>/partition_reload_diff_graph
make package
```

Boot with sd_card.img and change the working directory to `/run/media/mmcblk0p1`. Run applications to control the separate partitions:

```
./host0.exe pr0.xclbin pr0_2.xclbin 
./host1.exe pl.xclbin pr1.xclbin pr1_2.xclbin
./host2.exe pl.xclbin pr2.xclbin
```

Following host application executes and reloads multiple partitions with different graphs:

```
./host.exe pl.xclbin pr0.xclbin pr0_2.xclbin pr1.xclbin pr1_2.xclbin pr2.xclbin
```

## Reference Design 3: (./AIE_reload_whole_array)

The design contains 1 partition in directorie `AIE_reload_whole_array/pr1_rtp` that targets the whole AI Engine array. The graph code can be found in `aie` directory. The host code can be found in `AIE_reload_whole_array/pr1_rtp/sw` directory. 

To build design ready for HW, run the following command:

```
cd <TUTORIAL_PATH>/AIE_reload_whole_array
make package
```

Boot with sd_card.img and change the working directory to `/run/media/mmcblk0p1`. Run the application:

```
./host1.exe pl.xclbin pr1.xclbin 
```

Run above application multiple times to reload the whole AI Engine array. 

To reload different graphs in the same application, you may refer to the design `./partition_reload_diff_graph` and the host code `./partition_reload_diff_graph/pr1_rtp/sw/host.cpp`.

## Summary

After completing this tutorial, you learned:

* Hardware context in XRT API.
* Host code programming for partition reloading.

### Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums](http://forums.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2022–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>

