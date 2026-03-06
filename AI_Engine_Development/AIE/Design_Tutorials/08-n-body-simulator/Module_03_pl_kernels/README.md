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

# Building the Design

*Estimated time: 11 minutes*

```
make all
```

or, follow steps 1-3 as follows:

### Step 1: Set the Vitis Utility Library Path

```
XFLIB_DIR_REL_PATH :=$(DSPLIB_VITIS)/utils
XFLIB_DIR := $(shell readlink -f $(XFLIB_DIR_REL_PATH))

```
This path contains the folder `utils` along with other libraries. It contains the L2 Data-Mover generator tool.

### Step 2: Generate mm2s_mp.cpp and s2mm_mp.cpp Datamover Kernels

```
make -f ./ksrc.mk GENKERNEL=$(XFLIB_DIR)/L2/scripts/generate_kernels SPEC=./kernel/spec.json TOOLDIR=./_krnlgen
```

Here you use the L2 Data-Mover generator tool ($(XFLIB_DIR)/L2/scripts/generate_kernels). This tool uses the `kernel/spec.json` specification to write `kernel/mm2s_mp.cpp` and `kernel/s2mm_mp.cpp` HLS kernel source files.

### Step 3: Compile HLS PL Kernels

Following is an example of how the `mm2s_mp` kernel compiles.

```
v++ -c                                                                 \
    -t hw                                                              \
    --platform xilinx_vck190_base_202420_1                             \
    --save-temps --optimize 2                                          \
    --hls.jobs 8 -I$(XFLIB_DIR)/L1/include                            \
    -I$(XFLIB_DIR)/L1/include/hw                                      \
    -I./kernel                                                         \
    -k mm2s_mp                                                          \
    --hls.clock 150000000:mm2s_mp                                       \
    --temp_dir ./build/_x_temp.hw.xilinx_vck190_base_202420_1          \
    --report_dir ./build/reports/_x.hw_emu.xilinx_vck190_base_202420_1 \
    -o './build/_x_temp.hw_emu.xilinx_vck190_base_202420_1/mm2s_mp.xo'  \
    ./kernel/mm2s_mp.cpp                                                 
```

The same compilation options are used to compile the `s2mm_mp`, `packet_sender`, and `packet_receiver` kernels.

## HLS PL Kernels

After coming up with 400 tile AI Engine design, the next step is to move data from DDR and send it to the AI Engine. Use the AMD Vitis™ core development kit to create kernel code in C++ for FPGA acceleration. The Vitis Compiler (`v++ -c`) compiles the kernel code into kernel objects (XO). The following table describes each HLS PL kernel.

|Kernel Name| Description| Fmax|
|---|---| ---|
|`mm2s_mp`|Dual-channel data-mover that moves data from DDR to AXI4-Stream.|411 MHz|
|`packet_sender`|Packet switching kernel that packetizes AXI4-Stream data by generating a header packet and appropriately asserting `TLAST`|580 MHz|
|`packet_receiver`|Packet switching kernel that evaluates packet headers from incoming streams and reroutes data to one of 4 AXI4-Streams|499.5 MHz|
|`s2mm_mp`|Quad-channel data-mover that moves data from AXI4-Stream to DDR.|411 MHz|

Using Vivado timing closure techniques, you can increase the FMax if needed. To showcase the example, integrate using the 300 MHz clock. There is also a 400 MHz timing-closed design in the [beamforming tutorial](../../03-beamforming).

![alt text](images/pl_kernels_highlighted.PNG)

### mm2s_mp

The `kernel/spec.json` specification generates the `mm2s_mp`. Review this file. Notice the `mm2s_mp` kernel implementation is set to `LoadDdrToStream`, meaning this kernel moves data from the DDR (AXI-MM) to AXI-Stream. It is specified to have two channels. The first channel moves data from a DDR `buffer` called `ibuff` to an AXI-stream called `s0`. This channel moves the `i` data out of DDR to AXI-Stream. The second channel moves `j` data from DDR buffer `jbuff` to an AXI-Stream `s1` and streams the data directly into the AI Engine's `input_j` port.  

### Packet_Sender

After the `mm2s_mp` kernel loads `i` data onto an AXI-Stream, the `s0` is the input to the `packet_sender` kernel. The `packet_sender` kernel takes raw `i` data and packetizes it for the AI Engine. Review the `kernel/packet_sender.cpp` definition. The `packet_sender` does the following:

* generates a header AXI-Stream packet
* reads the `rx` stream
* writes 224 AXI-Stream data packets to one of the 100 `tx` streams
* asserts `TLAST` appropriately on the last data packet

It repeats these actions so all 100 `tx` streams have a packet header and 224 data packets written to it. This is 1 iteration of data the AI Engine is expecting. The 100 `tx` streams connect to the 100 `input_i` ports on the AI Engine.

### Packet_Receiver

The AI Engine's 100 N-Body Subsystems crunch the N-Body equations on the `input_i` and `input_j` data. It then outputs four data packets on each of the 100 `output_i` ports. Each output data packet can have a header of 0, 1, 2, or 3. This indicates that it comes from `nbody_kernel[0]`,  `nbody_kernel[1]`, `nbody_kernel[2]`, or `nbody_kernel[3]` in each of the nbody_subsystems. The 100 `output_i` ports connect to the 100 `rx` streams on the `packet_receiver` kernel. The `packet_receiver` kernel receives four packets from each of the 100 `rx` streams, and depending on the packet header, writes the data to `tx0`, `tx1`, `tx2`, or `tx3` streams.

### s2mm_mp

The `kernel/spec.json` specification generatees the `s2mm_mp` kernel. Review this file again. Notice that the `s2mm_mp` kernel has an implementation `StoreStreamToMaster` which moves data from AXI-Streams to DDR. The `s2mm_mp` kernel has four channels: `k0`,`k1`,`k2`, and `k3`. Each stream writes the data coming from the `tx0`-`tx3` streams to a DDR buffer.  

## References

* [Vitis Libraries Github Repo](https://github.com/Xilinx/Vitis_Libraries)

* [Vitis Utilities Library Documentation](https://docs.amd.com/r/en-US/Vitis_Libraries/utils/index.html)

* [Vitis Compiler Command](https://docs.amd.com/r/en-US/ug1399-vitis-hls/vitis-v-and-vitis-run-Commands)
  
## Next Steps

After compiling the PL datamover kernels, you are ready to link the entire hardware design together in the next module, [Module 04 - Full System Design](../Module_04_full_system_design).

### Support

GitHub issues are used to track requests and bugs. For questions go to [support.xilinx.com](http://support.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
