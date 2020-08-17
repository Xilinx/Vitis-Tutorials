<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ Application Acceleration Development Flow Tutorials</h1>
   <a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">See 2019.2 Vitis Application Acceleration Development Flow Tutorials</a>
   </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# Experiencing Acceleration Performance

In this lab, you will experience the acceleration potential by running the application first as a software-only version and then as an optimized FPGA-accelerated version using a precompiled FPGA accelerator.

1. Run the following command to set up the application.

   ```bash
   # Source the Vitis runtime environment
   export LAB_WORK_DIR=<Downloaded Github repository>/docs/bloom
   ```

2. Next, build the C application:
   1. Navigate to the `cpu_src` directory.
   2. Use the following command to run the original application with the number of documents as the argument, and generate the golden output file for comparison.

      ``` bash
      cd $LAB_WORK_DIR/cpu_src/
      make run
      ```

      The generated output compute scores are stored in the host code in the `cpu_profile_score` array that represents the outputs for the total number of specified documents.

      ```
      ./host 100000
      Initializing data
      Creating documents - total size : 1398.903 MBytes (349725824 words)
      Creating profile weights

      Total execution time of CPU          |  2949.3867 ms
      Compute Hash processing time         |  2569.3266 ms
      Compute Score processing time        |   380.0601 ms
      --------------------------------------------------------------------
      Execution COMPLETE
      ```
  
3. Run the application on the FPGA.
     For the purposes of this lab, the FPGA accelerator is implemented with an 8x parallelization factor.

   *  Eight input words are processed in parallel, producing eight output flags in parallel during each clock cycle. 

      To run the optimized application on the FPGA, run the following `make` command.

         ``` bash
         make run_fpga SOLUTION=1
         ```

         The following output displays.

         ```
         Processing 1398.905 MBytes of data
         Splitting data in 8 sub-buffers of 174.863 MBytes for FPGA processing
         --------------------------------------------------------------------
         Executed FPGA accelerated version  |   427.1341 ms   ( FPGA 230.345 ms )
         Executed Software-Only version     |   3057.6307 ms
         --------------------------------------------------------------------
         Verification: PASS
         ```

         The computed throughput is:

         Throughput = Total data/Total time = 1.39 GB/427.1341ms = 3.25 GB/s

         By efficiently leveraging FPGA acceleration, the throughput of the application increases by a factor of 7. 

## Next Steps

In this step, you observed the acceleration that can be achieved using an FPGA. Next, you will [architect the application for the application](./3_architect-the-application.md) and dive into what functions can be accelerated by profiling the original applications. You will also define the interface boundaries and performance constraints to achieve the desired acceleration.

<hr/>
<p align="center" class="sphinxhide"><b><a href="/docs/vitis-getting-started/README.md">Return to Getting Started Pathway</a> — <a href="docs/README.md">Return to Start of Tutorial</a></b></p>

<p align="center" class="sphnixhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
