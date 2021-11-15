# Profiling the Application

Vitis generates various system and kernel resource performance reports during compilation, XRT also collects profiling data during application execution in both software and hardware emulation and system run mode. A run summary report linked lots of reports from compilation and execution is automatically generated after running the active build, and can be viewed in *Vitis Analyzer*. 

Profiling reports and data can be used to isolate performance bottlenecks in the application, identify problems in the system, and optimize the design to improve performance. To obtain profiling reports, make sure associated switches are turned on at the *xrt.ini* file, which should be placed at the same directory as the host executable file. We already delivered a working *xrt.ini* with this tutorial, you can check it under *./sw/build/* directory. For more details, refer to [xrt.ini File](https://www.xilinx.com/html_docs/xilinx2020_1/vitis_doc/obl1532064985142.html) from Vitis online documentation.

```
[Debug]
profile=true
timeline_trace=true
data_transfer_trace=fine
```

Let's take the Hardware Run result of *rtc_alpha_tb* on U200 card as an example. There are reports of 6 categories listed with run summary, waveform will not be available because this is a run summary from system run. 

```
vitis_analyzer rtc_alpha_hw.xclbin.run_summary
```

## Run Guidance

Guidance includes message for reported violations, a brief suggested resolution, and a detailed resolution provided as a web link. The rules are generic rules based on an extensive set of reference designs, thus might not be applicable for every design. You need to understand the specific guidance rules and take appropriate action based on your specific algorithm and requirements.

<div align="center">
<img src="./images/hw_guidance.PNG" alt="Hardware Run Guidance" >
</div>

## Platform and System Diagrams

The Platform and System Diagrams display a representation of the platform resources and the kernel code integrated onto the platform. The System Diagram shows memory banks or PLRAMs used by the XCLBIN, how the function arguments of CUs are connected to AXI4 interfaces. It includes profile data from the run, and the resource information from the bottom table can also be displayed in a box next to each kernel or CU in the System Diagram. 

Please use upper right **Settings** button to display or hide Unused Memory, Interface Ports, Profile Info, and Resource info.

<div align="center">
<img src="./images/hw_sys_diagram.PNG" alt="Hardware System Diagram" >
</div>

## Profile Summary

Enabling profile data capturing for traffic between the kernels and host consumes additional resources and may impact performance, so we cleared those elements out of the sources as we delivered the pre-built XCLBIN files with this tutorial. 

You could add `--profile_kernel` option during the Vitis compiler linking process to enable data profiling for kernels by adding Acceleration Monitors and AXI Performance Monitors to kernels. See below modification on *./hw/Makefile* as an example, detailed description can be found in the [Vitis Compiler Command](https://www.xilinx.com/html_docs/xilinx2020_1/vitis_doc/vitiscommandcompiler.html#wrj1504034328013) from Vitis online documentation. 

```
rtc_alpha_$(TARGET).xclbin: $(XOS_RTC_ALPHA)
	v++ -l $(XOCCFLAGS) $(XOCCLFLAGS) --config xclbin_rtc_alpha.ini --profile_kernel data:all:all:all  -o $@ $(XOS_RTC_ALPHA)

rtc_gen_test_$(TARGET).xclbin: $(XOS_RTC_GEN_TEST)
	v++ -l $(XOCCFLAGS) $(XOCCLFLAGS) --config xclbin_rtc_gen_test.ini --profile_kernel data:all:all:all  -o $@ $(XOS_RTC_GEN_TEST)
```

<div align="center">
<img src="./images/hw_profile_summary.PNG" alt="Profile Summary" >
</div>

Even without `--profile_kernel` option added while generating XCLBIN files, most of the profiling reports are still available, only few sections like the Kernel Data Transfers will show no data.

<div align="center">
<img src="./images/no_kernel_data.PNG" alt="No Kernel Data" >
</div>

##  Application Timeline

Application Timeline collects and displays host and kernel events on a common timeline, it helps to understand and visualize the overall health and performance of the systems. The graphical display makes it easy to discover issues regarding kernel synchronization and efficient concurrent execution. 

<div align="center">
<img src="./images/hw_timeline.PNG" alt="Application Timeline" >
</div>

If you are looking for more details of Profiling and how to use *Vitis Analyzer*, check on [Profiling the Application](https://www.xilinx.com/html_docs/xilinx2020_1/vitis_doc/profilingapplication.html) from Vitis online documentation. 

<p class="sphinxhide" align="center"><sup>Copyright&copy; 2020-2021 Xilinx</sup></p>
