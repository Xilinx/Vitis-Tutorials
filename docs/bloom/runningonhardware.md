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

# 7. Running the Accelerator in Hardware

Until now, the results of all the previous labs were run in hardware emulation mode to give you an idea of how the optimization improves performance, while reducing the compilation time needed to build the system. In this section, you build and run each of the previous optimizations in hardware on an Alveo Data Center accelerator card.

After each run is finished, record the performance data from the Timeline Trace report, and fill in the table at the end of the section. Your numbers might vary.  
Note the following data:

* **Total Data**: Calculated by (**total number of documents x document size**).
* **Total Time**: Measured on the CPU with C++ timers.

   For a fair comparison, this will include the data transfer and kernel execution time.
* **Throughput**: Calculated by **Total Data Processed(MB)/Total Time(s)**

>**IMPORTANT**: Each of the steps in this lab compiles the hardware kernel and can take significant time to complete.

## Run the Baseline Application on Hardware

Use the following commands to run on hardware, generate, and view the Timeline Trace report.

```
make run TARGET=hw STEP=baseline NUM_DOCS=100000
```

## Run the Memory Transfer Lab on Hardware

Use the following commands to run on hardware, generate, and view the Timeline Trace report.

```
make run TARGET=hw STEP=localbuf  NUM_DOCS=100000
```

## Run Multiple Compute Units Lab on Hardware

Use the following commands to run on hardware, generate, and view the Timeline Trace.

```
make run TARGET=hw STEP=multicu  NUM_DOCS=100000
```

## Run Multiple DDR Banks Lab on Hardware

Use the following commands to run on hardware, generate, and view the Timeline Trace.

```
make run TARGET=hw STEP=multiddr  NUM_DOCS=100000
```

### Performance Table

The final performance benchmarking table displays as follows.

| Step                            | Number of Documents   | Average Document Size (kB) | Number of Compute Units | Time (Hardware) (s) | Throughput (MBps) |
| :-----------------------        | :----------- | ------------: | ----------------:| ------------------: | ----------------: |
| CPU                       |     100000 |           16 |     - |          11.45 |     139.73 (1xCPU)       |
| baseline_fpga                   |     100000 |           16 |   1 |            113.12 |     12.38 (0.09xCPU)          |
| localbuf                        |     100000|           16 |    1 |           2.40  |   666.67 (4.77xCPU)      |
| dataflow                       |     100000|           16 |      1 |          2.35  |   680.85 (4.87xCPU)      |
| multi-CU                        |     100000 |           16 |     2 |            0.95 |    1636.82.05xCPU)   |
| multi-DDR                        |     100000 |           16 |      4 |           0.62 |    2587.24(18.5xCPU)   |
---------------------------------------

>**NOTE**: Runtimes might vary from run to run.

It is important to understand why each step helped in improving performance:

* Copying the `bloom filter` array from the global to local memory helped to reduce latency of global memory accesses thereby improving performance.
* Using dataflow did not help much as the initiation interval (II) of loops because it is already 1, which is less improvement in performance.
* Replicating compute units helped because you are processing documents in parallel.
* Using multiple DDR banks with four CUs helped because you are scaling up the CUs and reducing contention on the DDR ports.

## Conclusion

Congratulations! You have successfully completed all the modules of this lab to convert a standard CPU-based application into an FPGA accelerated application, running with nearly 19x the throughput of CPU when running on the Alveo Data Center U200 accelerator card. You set performance objectives, and then you employed a series of optimizations to achieve your objectives.

1. You created a Vitis core development kit application from a basic C application.
2. You familiarized yourself with the reports generated during software and hardware emulation.
3. You explored various methods of optimizing your HLS kernel.
4. You learned how to set an OpenCL API command queue to execute out-of-order for improved performance.
5. You enabled your kernel to run on multiple CUs.
6. You enabled your kernel to run on multiple DDR banks
7. You ran the optimized application on the Alveo Data Center accelerator card to see the actual performance gains.
</br>
<hr/>
<p align= center><b><a href="/docs/vitis-getting-started/">Return to Getting Started Pathway</a> — <a href="./README.md">Return to Start of Tutorial</a></b></p>
</br>
<p align="center"><sup>Copyright&copy; 2019 Xilinx</sup></p>
