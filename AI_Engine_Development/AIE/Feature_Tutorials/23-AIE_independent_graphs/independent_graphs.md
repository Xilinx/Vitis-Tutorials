<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ AI Engine Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">Refer to the Vitis™ Development Environment on amd.com</a>
        </br>
      <a href="https://www.amd.com/en/products/software/vitis-ai.html">Refer to the Vitis™ AI Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# Compiling AI Engine Graphs for Independent Partitions

This tutorial covers the compilation and linking flows that are specific to AI Engine partition flow. The reference design (`./independent_graphs`) contains three AI Engine partitions in folders `pr0_gmio`, `pr1_rtp`, and `pr2_perf` inside the reference design. PL kernels are in the `pl_kernels` folder. The host code to control multiple partitions is in the `sw` folder.

## Step 1: Compile and Verify Each Partition with AIE simulator

### Partition `pr0` in Folder `pr0_gmio`

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

When compiling AI Engine graph with preceding configuration file, it constrains all the resources to be within the constrained columns.

To compile the graph, run the following command:

```
make aie
```

To simulator the graph, run the following command:

```
make aiesim
```

### Partition `pr1` in Folder `pr1_rtp` and Partition `pr2` in Folder `pr2_perf`

Similar to the previous section, the `aie.cfg` in these folders contain the partition configuration. You can use `make aie` and `make aiesim` to compile and simulate the designs:

```
cd <TUTORIAL_PATH>/independent_graphs/pr1_rtp
make aie
make aiesim
cd <TUTORIAL_PATH>/independent_graphs/pr2_perf
make aie
make aiesim
```

## Step 2: V++ Linker to Integrate the Partitions

V++ linker can link multiple independent compilation results together. The `Makefile` contains the V++ linker command:

```
LIBADF   = pr0_gmio/libadf.a pr1_rtp/libadf.a pr2_perf/libadf.a
...
xsa: guard-PLATFORM_REPO_PATHS ${XSA}
${XSA}: ${LIBADF} ${VPP_SPEC} ${XOS}
  ${VCC} -g -l --platform ${PLATFORM} ${XOS} ${LIBADF} -t ${TARGET} ${VPP_FLAGS} -o $@
```

>Note: Single partition flow uses multiple `libadf.a` for `v++ -l` command.

In the system configuration file `system.cfg`, the partition name prefixes the graph ports in the graphs. For example, the PLIO port `Dataout0` is `pr1_Dataout0` in the configuration file:

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

## Step 3: Compile Host Code

This reference design provides separate host applications to control different partitions. For example, host code in `independent_graphs/pr0_gmio/sw` can be used to test partition `pr0` only in hardware. However, the reference design also provides host code in `independent_graphs/sw` that controls multiple partitions in a single application.

This reference design uses the merged XCLBIN. In this flow, the partition name prefixes the graph name used in the PS host code for XRT API, for example:

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

>Note: Multiple `libadf.a` are provided for the packager command, and the SD card contains multiple host applications.

## Step 5: Run Applications in HW

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

AI Engine graphs cannot be re-executed without rebooting on non-DFX platforms, unless you enable partition reloading in the XCLBIN. This means that to rerun the application targeting either individual partitions or all three partitions, you need to reboot the board. 

For details on enabling partition reloading, refer to [Partition Reload](./partition_reload.md).

## Summary

After completing this tutorial, you learned:

* Specifying compiler configurations for AI Engine partitions.
* V++ linker and V++ packager commands for combining AI Engine partitions.
* Host code to control different partitions in hardware.

### Support

GitHub issues are used to track requests and bugs. For questions go to [forums](http://forums.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2022–2025 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
