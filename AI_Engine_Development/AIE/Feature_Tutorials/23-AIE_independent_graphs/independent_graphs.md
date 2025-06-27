<table class="sphinxhide" width="100%">
 <tr width="100%">
    <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Development</h1>
    <a href="https://www.xilinx.com/products/design-tools/vitis.html">See Vitis™ Development Environment on xilinx.com</br></a>
    <a href="https://www.xilinx.com/products/design-tools/vitis/vitis-ai.html">See Vitis™ AI Development Environment on xilinx.com</a>
    </td>
 </tr>
</table>

# Compiling AI Engine Graphs for Independent Partitions

This tutorial covers the compilation and linking flows that are specific to AI Engine partition flow. The reference design (./independent_graphs) contains three AI Engine partitions that are located in folders `pr0_gmio`, `pr1_rtp`, and `pr2_perf` inside the reference design. PL kernels are in the `pl_kernels` folder. The host code to control multiple partitions is in the `sw` folder.

## Step 1: Compile and Verify Each Partition with AIE simulator


### Partition `pr0` in folder `pr0_gmio`

Change directory to `pr0_gmio`:

```
cd <TUTORIAL_PATH>/independent_graphs/pr0_gmio
```

Look at aie.cfg:

```
[aie]
enable-partition=6:1:pr0 
```

It defines the partition named  `pr0`. The partition starts from column `6` and totally `1` column.

When compiling AI Engine graph with above configuration file, it constrains all the resources to be within the constrained columns.

To compile the graph, run the following command:

```
make aie
```

To simulator the graph, run the following command:

```
make aiesim
```

### Partition `pr1` in folder `pr1_rtp` and partition `pr2` in folder `pr2_perf`

Similar to the previous section, the `aie.cfg` in these folders contain the partition configuration. You can use `make aie` and `make aiesim` to compile and simulate the designs:

```
cd <TUTORIAL_PATH>/independent_graphs/pr1_rtp
make aie
make aiesim
cd <TUTORIAL_PATH>/independent_graphs/pr2_perf
make aie
make aiesim
```

## Step 2: V++ linker to integrate the partitions

V++ linker can link multiple independent compilation results together. The `Makefile` contains the V++ linker command:

```
LIBADF   = pr0_gmio/libadf.a pr1_rtp/libadf.a pr2_perf/libadf.a
...
xsa: guard-PLATFORM_REPO_PATHS ${XSA}
${XSA}: ${LIBADF} ${VPP_SPEC} ${XOS}
	${VCC} -g -l --platform ${PLATFORM} ${XOS} ${LIBADF} -t ${TARGET} ${VPP_FLAGS} -o $@
```

>Note: Single partition flow uses multiple `libadf.a` for `v++ -l` command.

In the system configuration file `system.cfg`, the graph ports in the graphs are prefixed with the partition name. For example, the PLIO port `Dataout0` is named as `pr1_Dataout0` in the configuration file:

```
[connectivity]
stream_connect=ai_engine_0.pr1_Dataout0:s2mm_1.s
stream_connect=datagen.out:ai_engine_0.pr1_Datain0
```

Run the following command to link the design:

```
cd <TUTORIAL_PATH>/independent_graphs
make xsa
```

## Step 3: Compile host code

This reference design provides separate host applications to control different partitions. For example, host code in `independent_graphs/pr0_gmio/sw` can be used to test partition `pr0` only in hardware. However, the reference design also provides host code in `independent_graphs/sw` that controls multiple partitions in a single application. 

In this reference design, the merged XCLBIN is used. In this flow, the graph name used in the PS host code for XRT API is prefixed with the partition name, for example:

```
auto ghdl=xrt::graph(device,uuid,"pr0_gr");//partition name = "pr0", graph name = "gr"
```

To compile host code for partitions:

```
cd <TUTORIAL_PATH>/independent_graphs
make host
```

## Step 4: Package for hardware

To package the partitions for hardware, run the following command:

```
cd <TUTORIAL_PATH>/independent_graphs
make package
```

>Note: Multiple `libadf.a` are provided for the packager command, and multiple host applications are packaged into the SD card.

## Step 5: Run applications in HW

Boot with sd_card.img and change the working directory to `/run/media/mmcblk0p1`. 

Run applications to control the separate partitions (each exe controls one partition):

```
./host0.exe a.xclbin
./host1.exe a.xclbin
./host2.exe a.xclbin
```

Or run the application to control multiple partitions:

```
./host.exe a.xclbin
```

AI Engine graphs cannot be re-executed without rebooting on non-DFX platforms unless partition reloading is enabled in the XCLBIN. This means that, in order to rerun the application targeting either individual partitions or all three partitions, you will have to reboot the board. 

For details on enabling partition reloading, please refer to [Partition Reload](./partition_reload.md).

## Summary

After completing this tutorial, you learned:

* Specifying compiler configurations for AI Engine partitions.
* V++ linker and V++ packager commands for combining AI Engine partitions.
* Host code to control different partitions in hardware.

### Support

GitHub issues will be used for tracking requests and bugs. For questions go to [forums](http://forums.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2022–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>

