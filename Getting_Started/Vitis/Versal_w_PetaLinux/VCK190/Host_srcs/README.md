<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ Getting Started Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">See Vitis™ Development Environment on amd.com</a>
    </td>
  </tr>
</table>


# Host Application Compilation

To write an application code for A72 targeting the Linux OS, we can make use of AMD Runtime Library (XRT). 
As the XRT manages the hardware access, use of XRT APIs help the application development. 

Before we deep dive into the host code development, let's understand the goal of the application code:
Our goal is to create input data for the AI Engine and PL kernel, pass the data to the kernels for execution and then read the output from the DDR.


Let's get started with the host code application development. 

1. Load the device
```
	unsigned int dev_index = 0;
	char* xclbinFilename = argv[1];
	auto device = xrt::device(dev_index);
	auto xclbin_uuid = device.load_xclbin(xclbinFilename);
```

2. Input prepration
```
      	int sizePLIn0  = DATA_SIZE_IN0;
	int* DataInput0 = new int[sizePLIn0];
	for(int i=0;i<sizePLIn0;i++){
	DataInput0[i]= input_data[i];
		printf("DataInput0[%d]=%d\n", i,DataInput0[i]);
	}

	int sizePLIn1  = DATA_SIZE_IN1;
	int* DataInput1 = new int[sizePLIn1];
	for(int i=0;i<sizePLIn1;i++){
	DataInput1[i]= input1_data[i];
		printf("DataInput1[%d]=%d\n", i,DataInput1[i]);
	}

	int sizePLOut0 = DATA_SIZE_OUT0;
	int* goldenPL0     = new int[sizePLOut0];

	for(int i=0;i<sizePLOut0;i++){
        goldenPL0[i]=golden_data[i] ;
		printf("goldenPL0[%d]=%d\n", i,goldenPL0[i]);
	}
```

3. Buffer allocation and mapping
```
	auto in_bohdl0 = xrt::bo(device, sizePLIn0*sizeof(int), 0, 0);
	auto in_bomapped0 = in_bohdl0.map<uint32_t*>();
	memcpy(in_bomapped0, DataInput0, sizePLIn0 * sizeof(int));
	printf("Input memory virtual addr 0x%px\n", in_bomapped0);
	
	auto in_bohdl1 = xrt::bo(device, sizePLIn1*sizeof(int), 0, 0);
	auto in_bomapped1 = in_bohdl1.map<uint32_t*>();
	memcpy(in_bomapped1, DataInput1, sizePLIn1 * sizeof(int));
	printf("Input memory virtual addr 0x%px\n", in_bomapped1);
	
	auto out_bohdl0 = xrt::bo(device, sizePLOut0*sizeof(int), 0, 0);
	auto out_bomapped0 = out_bohdl0.map<uint32_t*>();
	memset(out_bomapped0, 0xABCDEF00, sizePLOut0 * sizeof(int));
	printf("Output memory virtual addr 0x%px\n", out_bomapped0);
	
	in_bohdl0.sync(XCL_BO_SYNC_BO_TO_DEVICE, sizePLIn0 * sizeof(int), 0);
	in_bohdl1.sync(XCL_BO_SYNC_BO_TO_DEVICE, sizePLIn1 * sizeof(int), 0);
	out_bohdl0.sync(XCL_BO_SYNC_BO_FROM_DEVICE, sizePLOut0 * sizeof(int), 0);
```

4. Open the Kernels
```
	auto mm2s_1 = xrt::kernel(device, xclbin_uuid, "mm2s:{mm2s_1}");
	auto mm2s_1_rhdl = mm2s_1(in_bohdl0, nullptr, sizePLIn0 );
	auto mm2s_2 = xrt::kernel(device, xclbin_uuid, "mm2s:{mm2s_2}");
	auto mm2s_2_rhdl = mm2s_2(in_bohdl1, nullptr, sizePLIn1 );
	auto s2mm_1 = xrt::kernel(device, xclbin_uuid, "s2mm:{s2mm}");
	auto s2mm_1_rhdl = s2mm_1(out_bohdl0, nullptr, sizePLOut0 );
	auto graph = xrt::graph(device, xclbin_uuid, "mygraph");
```

5.  Run the Kernels
```
	graph.run(N_ITER);
	mm2s_1_rhdl.wait();
	mm2s_2_rhdl.wait();
	s2mm_1_rhdl.wait();
```

6. Check the output with golden data
```
	int errorCount = 0;
	for (int i = 0; i < sizePLOut0; i++) {
		if ((signed)out_bomapped0[i] != (signed)goldenPL0[i]) {
		printf("Error found @ %d, %ld != %ld\n", i, (long int)out_bomapped0[i], (long int)goldenPL0[i]);
		errorCount++;
		}
```

7. Closing the run and printing the results
```
	delete[] DataInput0;
	delete[] DataInput1;
	delete[] goldenPL0;
	graph.end();
	if (errorCount)
            printf("Test failed with %d errors\n", errorCount);
  	else
     	    printf("TEST PASSED\n");
	return errorCount;
```

## Compilation of Host Code:

Once the application code is ready and verified, we will now compile the host code using the "**aarch64-linux-gnu-g++**". Please refer to the [Host_srcs/Makefile](./Makefile) to access the full command.
```
aarch64-linux-gnu-g++ host.cpp -I<required_include> <gcc_flags> <sysroot> -o host.exe
```
The above command will generate an executable host.exe that we will use in the Vitis packager to generate the necessary files to boot the device and run the above host code application.



Next Chapter: [Use Vitis Linker and Packager to build the design](../vitis_dir/README.md)


<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
