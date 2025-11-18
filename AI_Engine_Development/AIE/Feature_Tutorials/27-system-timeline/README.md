</table>
<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>



# System Timeline Tutorial

***Version: Vitis 2025.2***

This small tutorial is based on a basic design to test Vitis System Timeline feature.

## Introduction

In Vitis 2025.2 AMD introduced a new (early access) feature called **System Timeline**. This allows the user to trace all subsystems of the device (PL, PS and EI Engine array) and display them in Vitis Analyzer on the same graph with sychronized timeline. The goal is to have a deeper understanding of the way the various elements of the system work together and track system controller bugs: missing kernel start, wrong number of iterations...


>**IMPORTANT**: Before beginning the tutorial make sure you have installed the AMD Vitis™ 2025.2 software. The Vitis release includes all the embedded base platforms including the VCK190 base platform that is used in this tutorial. In addition, ensure you have downloaded the Common Images for Embedded Vitis Platforms from [this link](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html).

The ‘common image’ package contains a prebuilt Linux kernel and root file system that can be used with the AMD Versal™ board for embedded design development using the Vitis software platform.

Before starting this tutorial, run the following steps:

1. Go to the directory where you have unzipped the Versal Common Image package.
2. In a Bash shell, run the ``/Common Images Dir/xilinx-versal-common-v2025.2/environment-setup-cortexa72-cortexa53-amd-linux`` script. This script sets up the SDKTARGETSYSROOT and CXX variables. If the script is not present, you must run the ``/Common Images Dir/xilinx-versal-common-v2025.2/sdk.sh``.
3. Set up your ROOTFS and IMAGE to point to the ``rootfs.ext4`` and Image files located in the ``/Common Images Dir/xilinx-versal-common-v2025.2`` directory.
4. Set up your PLATFORM_REPO_PATHS environment variable to ``$XILINX_VITIS/base_platforms``.

This tutorial targets VCK190 production board for 2025.2 version.

Data generation for this tutorial requires [Python 3](https://www.python.org/downloads/). The following packages are required:

- os
- sys
- numpy

>**Note**: This tutorial assumes that you have a basic understanding of the Adaptive Data Flow (ADF) API and Xilinx® Runtime (XRT) API usage. For more information about ADF API and XRT usage, refer to AI Engine Runtime Parameter Reconfiguration Tutorial and the Versal Adaptive SoC AI Engine Programming Environment User Guide ([UG1076](https://docs.amd.com/access/sources/dita/map?isLatest=true&ft:locale=en-US&url=ug1076-ai-engine-environment)).

## Objectives

After completing this tutorial, you will be able to:

- Follow the complete flow to enable System Timeline for Hw debug.
 

## System Timeline complete flow

This tutorial is decomposed in 6 stages:

- AI Engine design and compilation
- PL kernels compilation
- Link stage to create the xsa with the AI Engine interface and the PL Kernels
- Host code compilation
- Packaging and create SD card image
- Run the design on the board and capture the trace
- Analyze the trace

The Makefile is built so that you can address each step alone or concatenate all steps in a single command:

```Makefile
build_hw:
   make TARGET=hw  clean data aie kernels xclbin host package
```


### AI Engine design and compilation

The design is based on the replication of the same processing chain:

1. Passthrough.
2. Filtering.
3. Gain.
4. Passthrough.

The filter and the gain kernels receive asynchronous RTP to set the coefficients and the gain value.

By default 4 of these chains are implemented in the design. It can be changed using the Makefile parameter `NAntenna`.

In `graph.cpp` the graph is instanciated as follows: `MyGraph<NAntenna,40> G("");`. The value `40` means that a utilization ratio of 40% is specified for the filter and the gain leading to a co-location for these 2 kernels. If you want them in different tiles, just replace by a value above 50.

Here is the subgraph of the 4th antenna:

![No graph image](images/AIEgraph.png)

What is important is that during the compilation, the user should declare that trace events should be extracted during runtime. Some specific flags have to be set depending on the way to extract these events: through GMIO or PLIO:

**GMIO based event extraction**

*aiecompiler_trace_gmio_options.cfg*

```shell
[aie]
event-trace=runtime
broadcast-enable-core=true
event-trace-port=gmio
xlopt=0
```

**PLIO based event extraction**

*aiecompiler_trace_plio_options.cfg*

```shell
[aie]
event-trace=runtime
broadcast-enable-core=true
num-trace-streams=16
event-trace-port=plio
trace-plio-width=128
xlopt=0
```

Here are some options definitions:
- `event-trace=runtime`: trace events will be specified at runtime. This is actually the only possible option for hardware tracing.
- `broadcast-enable-core=true`: ensures that the enable core signals are broadcasted so that all kernels start within a few clock cycles of each other.
- `event-trace-port=gmio/plio`: selects the port type used for event tracing. GMIO is generally used for designs with limited PL resources, while PLIO is preferred for designs with sufficient PL resources.
- `num-trace-streams=16`: sets the number of trace streams within the AI Engine array to be used for event tracing. The default is 4 streams, the maximum is 16. Increasing the number of streams can help reduce contention within the trace data path, especially in designs with a large number of active kernels. The drawback is that it would use more resource making it more difficult for the router to route the AI Engine design by itself.
- `trace-plio-width=128`: specifies the width of the PLIO trace interface.
- `xlopt=0`: disables extra optimizations that could interfere with event tracing. Typically the compiler will avoid inlining the kernels within the main function, allowing the user to see each kernel as a separate entity in the trace and have a clear view of all iterations.

### Link stage

PL Trace is enabled during system link stage. This is done adding the `--profile.data` flag to the `v++` command line. 

```shell
v++ -g -l --platform ${PLATFORM} ${XOS} ${LIBADF} -t hw --save-temps --verbose --config ${VPP_SPEC} --profile.data all:all:all -o XCLBIN_File
```

The --profile option can be used to profile many different activities. For more information, see [--profile Options in UG1702](https://docs.amd.com/r/en-US/ug1702-vitis-accelerated-reference/profile-Options).

.
`--profile.data all:all:all` will monitor data on all kernels and compute units.


### Run on hardware

The packaging step is creating a zip version of the sd card image that can be used in any usual SD Card flash software like Balena Etcher.

When the application is run on hardware, using XRT on Linux, the trace is captured using the following `xrt.ini` file specification:

```shell
# Debug group for the aie, ps and pl
[Debug]
aie_profile = false
aie_trace = true
device_trace=fine
continuous_trace = true
host_trace=true

# PL Trace buffer
trace_buffer_size = 32M
trace_buffer_offload_interval_ms = 5

# Subsection for AIE profile settings only if aie_profile is set to true
[AIE_profile_settings]
# Interval in between reading counters (in us)
interval_us = 1000

tile_based_aie_metrics = all:heat_map
tile_based_aie_memory_metrics = all:conflicts
tile_based_interface_tile_metrics = all:output_throughputs


# Subsection for AIE Trace only if aie_trace is set to true
[AIE_trace_settings]
# PLIO
reuse_buffer = true
periodic_offload = true
buffer_offload_interval_us = 50
buffer_size = 100M

tile_based_aie_tile_metrics = all:functions
enable_system_timeline = true

[Runtime]
verbosity = 10
```

The option `enable-system-timeline`is true by default. More information can be found at [*xrt.ini* file in UG1702](https://docs.amd.com/r/en-US/ug1702-vitis-accelerated-reference/xrt.ini-File).

Plug the sd card in your board, connect the right COM port and boot the board.

Login with username `petalinux` and set the password to whatever you want, let say `p`.

For the next steps you should be the superuser: `sudo su` and enter yout password.
Change `root` password: `passwd root`, let's use `r` as the password.

As you will have to copy back the trace files you must allow connection with `root` through ethernet: `vi /etc/ssh/sshd_config`. Change the option of `PermitRootLogin` into `yes`.

Now, go to the application directory: `cd /run/media/mmcblk0p1`.

In order to allow multiple runs with different options you can use the script `newdir` which will copy the necessary files into directory `ptest1`, `ptest2`, and so on.

in ptest1, check the content of `xrt.ini` and `embedded_exec.sh`.

Run the application: `./embedded_exec.sh`

All trace files should be generated in 2s. If you want to perform another test with other parameters, you can run `./newdir` from `/run/media/mmcblk0p1` and change the parameters as you wish in `ptest2/xrt.ini`. Type `reboot` to restart the board and re-run the application.

Once you have run the application with multiple sets of parameters, you can copy the various `ptest` directories back to your development machine using `scp` (use `ifconfig` to get the board inet address).

You can copy the whole `ptest*`directories to `ProfileData` on your development machine. The minimum set of files that you have to copy is: `*.csv, *.txt, *.bin, *summary`

![MissingImage](images/TraceFiles.png)

Now you can run `vitis_analyzer`on your development machine with the summary file: `vitis_analyzer xrt.run_summary`. to view the System Timeline you have to allow it on the tool: In the top bar menu click on *Vitis -> New Feature Preview*, and check *System Timeline*.

Click on the analysis tab on the left of Vitis Analyzer, and click on *Timeline Trace*.

The overall view covers the complete simulation time:

![Missing image](images/OverallTrace.png)

At the begining we can see when the processing system is opening the device and starts the PL kernels:

![Missing images](images/StartPLKernels.png)

Zooming in where the AI Engine graph starts, we can see the PL Kernels **gen2s** generating the data and the AI Engine kernels consuming these data. These trace are not perfectly aligned but still enough to understand the overall behaviour of the system.

![Missing image](images/StartGraph.png)

The polling interval is crucial in event alignment. Reducing the polling interval can help to better align events in the timeline at the expense of some timestamp file size increase. In order to show the effect of different polling intervals, you can modify the `buffer_offload_interval_us` parameter in `xrt.ini` file. The default value is 50µs, In the following example I increased it to 100µs:

![Missing image](images/StartGraphV2.png)

As you can see , the PL kernels are out of sync with the AI Engine array iterations.

### Other tests

You can play with all parameters:

- trace_buffer_offload_interval_ms
- tile_based_aie_tile_metrics
- Change or remove AI Engine Profiling metrics
- Change number of iterations in embedded_exec.sh

## Conclusion

In this tutorial, we explored how to set up and analyze system-level traces for AI Engine applications using system timeline feature. We covered the necessary configuration changes, the process of running the application on the target hardware, and the steps to collect and analyze the generated trace files. By leveraging the Vitis Analyzer tool, we can gain valuable insights into the performance and behavior of our AI Engine applications, enabling us to optimize and improve their efficiency.


## License

___

 The MIT License (MIT)

 Copyright (c) 2025 Advanced Micro Devices, Inc. All rights reserved.
 SPDX-License-Identifier: MIT

## Support

GitHub issues will be used for tracking requests and bugs. For questions, go to [support.xilinx.com](https://support.xilinx.com/).

<hr class="sphinxhide"></hr>

<p class="sphinxhide" align="center"><sub>Copyright © 2021–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
