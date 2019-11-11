<table>
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2019.2 Vitis™ Application Acceleration Development Flow Tutorials</h1>
   <a href="https://github.com/Xilinx/SDAccel-Tutorials/branches/all">See SDAccel™ Development Environment 2019.1 Tutorials</a>
   </td>
 </tr>
 <tr>
 <td align="center"><h1>Optimizing Accelerated FPGA Applications: Bloom Filter Example
 </td>
 </tr>
</table>

# 6. Optimizing with Multiple DDR Banks

In the previous lab, multiple compute units (CUs) are used to achieve performance improvement. In this lab, you will use multiple DDR banks to further increase the performance.

Because the DDR memory has a maximum of 15 ports available for kernel, if you increase the number of CUs natively on a single DDR bank, it could create contention for the DDR memory ports. For instance, in this tutorial, you have four ports per CU and increasing the number of CUs to four requires 16 memory ports. You have four DDR banks on the Alveo Data Center U200 accelerator card, which utilize these DDR banks to allow room for more performance. The computation of the individual documents are independent of each other; therefore, the usage of multiple DDR with multiple CUs on each document should further increase the performance.

In this lab, you will increase the number of CUs to four with two DDR banks so that each DDR bank has two CUs.

## Host Code Updates to Support Multiple DDR Banks

Change number of CUs from `2` to `4`, so that each DDR bank will have two CUs.

```
 unsigned int num_compute_units = 2;
```

To:

```
unsigned int num_compute_units = 4;
```

## Makefile Updates to Support Multiple DDR Banks

1. You have added the `sp` flag to specify the CUs that are associated with a specific DDR bank in a config file. For the bloom filter `bloom_filter`, use PLRAM.

   PLRAM is a memory on the accelerator board with lower latency than DDR. Its total size on the Alveo Data Center U200 accelerator card is 128 KB. The size of the bloom filter array is 64 KB and two compute units consume a size of 128 KB. You will use two PLRAMs for the design, and there are a total of four PLRAMs available in the Alveo Data Center U200 accelerator card.

2. Change the `connectivity.cfg` file as follows.

   ```
   [connectivity]
   nk=runOnfpga:2:runOnfpga_1.runOnfpga_2
   ```

   To:

   ```
   [connectivity]
   nk=runOnfpga:4:runOnfpga_1.runOnfpga_2:runOnfpga_3:runOnfpga_4
   sp=runOnfpga_1.fpga_profile_score:DDR[0]
   sp=runOnfpga_2.fpga_profile_score:DDR[0]
   sp=runOnfpga_3.fpga_profile_score:DDR[0]
   sp=runOnfpga_4.fpga_profile_score:DDR[0]
   sp=runOnfpga_1.doc_sizes:DDR[0]
   sp=runOnfpga_2.doc_sizes:DDR[0]
   sp=runOnfpga_3.doc_sizes:DDR[0]
   sp=runOnfpga_4.doc_sizes:DDR[0]
   sp=runOnfpga_1.input_words:DDR[0]
   sp=runOnfpga_2.input_words:DDR[0]
   sp=runOnfpga_3.input_words:DDR[1]
   sp=runOnfpga_1.profile_weights:DDR[0]
   sp=runOnfpga_2.profile_weights:DDR[0]
   sp=runOnfpga_3.profile_weights:DDR[0]
   sp=runOnfpga_4.profile_weights:DDR[0]
   sp=runOnfpga_1.bloom_filter:PLRAM[0]
   sp=runOnfpga_2.bloom_filter:PLRAM[0]
   sp=runOnfpga_3.bloom_filter:PLRAM[1]  
   sp=runOnfpga_4.bloom_filter:PLRAM[1]
   ```

## Run Hardware Emulation

Go to the `makefile` directory, and use the following command to run hardware emulation.

   ```
   make run TARGET=hw_emu STEP=multiddr SOLUTION=1 NUM_DOCS=100
   ```

## Generate Reports for Hardware Emulation

Use the following command to generate the Profile Summary and Timeline Trace reports.

```
make view_report TARGET=hw_emu STEP=multiddr
```

## Profile Summary for Hardware Emulation

1. Click the Profile Summary report from Vitis analyzer, which displays as follows.

   ![](./images/profile_multiddr.PNG)

2. Capture the performance data from Kernel Detail Trace, and add it to the following table.

   | Step                            | Number of Documents   | Average Document Size(kB) | Time (Hardware) (ms) | Throughput (MBps) |
   | :-----------------------        | :----------- | ------------: | ------------------: | ----------------: |
   | CPU                       |     100 |           16 |              11.23 |   124.67        |
   | baseline                   |     100 |           16 |             38|  36.84          |
   | localbuf                  | 100 | 16| 1.67 | 838.32
   | dataflow | 100 | 16 | 1.567 | 893.42
   | multicu | 100 | 16 | 0.822 |1703.16 |
   | multiddr | 100 | 16 | 0.55 | 2909.09
   ---------------------------------------

## Timeline Trace for Hardware Emulation

The Timeline Trace report in Vitis analyzer displays as follows.

![](./images/new_tutorial_bloom_multiddr_timeline_hw_emu.PNG)

* There is overlap of execution between the four compute units.
 * There is overlap between data transfer and compute unit for larger data set.

## Next Step

In the next section, you will [run all previously mentioned optimizations on hardware](./runningonhardware.md) to see the performance improvements.
</br>
<hr/>
<p align="center"><b><a href="/docs/vitis-getting-started/">Return to Getting Started Pathway</a> — <a href="./README.md">Return to Start of Tutorial</a></b></p>

<p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>
