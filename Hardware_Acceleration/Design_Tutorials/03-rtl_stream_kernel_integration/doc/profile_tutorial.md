# Profiling the Application

Vitis generates various system and kernel resource performance reports during compilation, XRT also collects profiling data during application execution in both software and hardware emulation and system run mode. A run summary report linked lots of reports from compilation and execution is automatically generated after running the active build, and can be viewed in *Vitis Analyzer*. 

Profiling reports and data can be used to isolate performance bottlenecks in the application, identify problems in the system, and optimize the design to improve performance. To obtain profiling reports, make sure associated switches are turned on at the *xrt.ini* file, which should be placed at the same directory as the host executable file. We already delivered a working *xrt.ini* with this tutorial, you can check it under *./sw/build/* directory. For more details, refer to [xrt.ini File](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/xrt.ini-File) from Vitis online documentation.

```
[Emulation]
debug_mode=batch

[Debug]
opencl_trace=true
device_trace=fine
```

Let's take the Hardware Run result of *rtc_alpha_tb* on U200 card as an example. There are reports of 6 categories listed with run summary, waveform will not be available because this is a run summary from system run. 

```
vitis_analyzer xrt.run_summary
```

## Run Guidance

Guidance includes message for reported violations, a brief suggested resolution, and a detailed resolution provided as a web link. The rules are generic rules based on an extensive set of reference designs, thus might not be applicable for every design. You need to understand the specific guidance rules and take appropriate action based on your specific algorithm and requirements.

<div align="center">
<img src="./images/hw_guidance.PNG" alt="Hardware Run Guidance" >
</div>

## Profile Summary

Enabling profile data capturing for traffic between the kernels and host consumes additional resources and can impact performance, so the pre-built XCLBIN files do not include profiling. 

You can add the `--profile` option during the linking process to enable device data profiling by adding Acceleration Monitors and AXI Performance Monitors to the system design. See the following modification on *./hw/Makefile* as an example. A detailed description can be found in the [--profile Options](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/profile-Options) of the Vitis compiler command. 

```
rtc_alpha_$(TARGET).xclbin: $(XOS_RTC_ALPHA)
	v++ -l $(XOCCFLAGS) $(XOCCLFLAGS) --config xclbin_rtc_alpha.ini --profile.data:all:all:all  -o $@ $(XOS_RTC_ALPHA)

rtc_gen_test_$(TARGET).xclbin: $(XOS_RTC_GEN_TEST)
	v++ -l $(XOCCFLAGS) $(XOCCLFLAGS) --config xclbin_rtc_gen_test.ini --profile.data:all:all:all  -o $@ $(XOS_RTC_GEN_TEST)
```

<div align="center">
<img src="./images/hw_profile_summary.PNG" alt="Profile Summary" >
</div>

Even without `--profile` option added while generating XCLBIN files, most of the profiling reports are still available, only few sections like the Kernel Data Transfers will show no data.

##  Application Timeline

The Application Timeline collects and displays host and kernel events on a common timeline, it helps to understand and visualize the overall health and performance of the systems. The graphical display makes it easy to discover issues regarding kernel synchronization and efficient concurrent execution. 

<div align="center">
<img src="./images/hw_timeline.PNG" alt="Application Timeline" >
</div>

If you are looking for more details of Profiling and how to use *Vitis Analyzer*, check on [Profiling the Application](https://docs.xilinx.com/r/en-US/ug1393-vitis-application-acceleration/Profiling-the-Application). 

Return to [Top-Level of Tutorial](../README.md)

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>

