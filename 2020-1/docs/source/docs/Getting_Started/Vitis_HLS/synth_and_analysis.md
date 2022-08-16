﻿<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://www.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.1 Vitis™ Application Acceleration Tutorials</h1>
   <a href="https://github.com/Xilinx/Vitis-Tutorials/branches/all">See 2019.2 Vitis Application Acceleration Development Flow Tutorials</a>
  </td>
 </tr>
 <tr>
 <td>
 </td>
 </tr>
</table>

# 2. Running High-Level Synthesis and Analyzing Results

## Run the C Simulation

With the source code and testbench added to the project, you can now run the C simulation.
>TIP: For more information about the features of a well-written test bench, refer to [Writing a Test Bench](https://www.xilinx.com/cgi-bin/docs/rdoc?v=2020.1;t=vitis+doc;d=verifyingcodecsimulation.html;a=sav1584759936384) in the Vitis HLS Flow of the Vitis Unified Software Platform Documentation (UG1416).

1. From the main toolbar, select **Run C Simulation**. 

   The C Simulation Dialog is displayed.

    ![Add Sources](./images/run_c_simulation.png)

2. Select the **Enable Pre-synthesis Control Flow Viewer** check box to produce a control flow view of your design after C simulation.

3. Click **OK**.

   After the simulation completes, the tool displays the `dct_sim.log` file with the results of simulation, and also displays the Pre-synthesis Control Flow graph. The log file does not display information other that the test passed. That is always a good sign.

   The Control Flow graph, as shown in the following figure, offers some insight into the control structure of your code. Take a look at to see how Vitis HLS understands your code.
   >**NOTE**: Control structure here refers to the branches in your code caused by various kinds of loops and conditional statements like `if` and `switch` statements.

     ![Presynthesis Control FLow](./images/presynth-ctrl-flow-graph.png)
4. Zoom in on the graph and examine the structure of the DCT function. It includes the following elements:

   * **read_data**: Reads the data from the DDR, as an array, and reformats it into a matrix.
   * **dct_2d with a nested function dct_1d**: Processes the matrix of values, applying the cosine transform to the matrix values.
   * **write_data**: Writes the results back to DDR, converting the matrix back into an array.

5. Look at the Loops view below the graph. 

   This view shows the loops found in the code, and displays some basic statistics related to the loop iterations, and timing. Vitis HLS automatically applies names to unlabeled loops, and associates these names with the loop. These names can then be used in the Tcl Console view to refer to the loop. If you select a loop in the view, the loop is also cross-selected in the control flow graph and the source code.

   Some of the key highlights of the Control Flow graph are as follows:

   * Shows the critical path in your C/C++ code (as indicated by the red arrows and is an artifact of code profiling). 

       * The percentages on the red arrows indicate the percentage of time that particular branch is taken versus the other branch. This gives you an idea of where the time is spent inside the design and the areas to focus on.
         
      >**NOTE**: Because this path is derived by running the testbench, this path is specific to your test.

   * Allows you to see where the memory reads/writes are in the control flow graph. This allows you to get a sense for memory port contention issues. 
       * If memory accesses are on different branches, then these accesses would be consider mutually exclusive (and therefore no conflict can occur). 
      * If however, the memory accesses are on the same branch, then loop II violations can occur because of multiple memory accesses in the same sequential portion of the code. 

6. To run synthesis, simply click the **C Synthesis** toolbar button. The synthesis command runs, and writes a transcript in the Console view. Examine the transcript to see what steps the tool takes during synthesis. The following list describes some of the steps listed:
   * Project and solution initialization loads source and constraints files, and configures the active solution for synthesis.
   * Start compilation reads source files into memory. 
   * Interface detection and setup reviews and generates port and block interfaces for the function. 
   * Burst read and write analysis for ports/interfaces.
   * Compiler transforms code to operations.
   * Performs Synthesizeability checks.
   * Automatic pipelining of loops at tripcount threshold.
   * Unrolling loops, both automatic and user-directed.
   * Balance expressions using associative and commutative properties. 
   * Loop flattening to reduce loop hierarchy.
   * Partial write detection (writing part of a memory word)
   * Finish architecture synthesis, start scheduling.
   * End scheduling, generate RTL code.
   * Report FMax and loop constraint status.

   The Vitis HLS tool also automatically inlines small functions, dissolving the logic into the higher-level calling functions, and pipelines small loops with limited iterations. These features are configurable by user directives or pragmas.

7. After synthesis completes, the Synthesis Summary report displays as shown in the following figure. Review the results.

   ![Simplified Synthesis Report](./images/dct_synthesis_report.png)

   Notice that the various sub-functions from the Pre-synthesis control flow diagram are no longer reported in the synthesis results. This is because the tool has inlined these functions automatically. You can disable the inlining of specific functions by adding the INLINE OFF pragma or directive for the function, or by adding the DATAFLOW optimization to the design, which you will be doing later in this tutorial.

   The Vitis HLS tool also automatically pipelined loops that have fewer than a specified number of iterations. Pipelining loops with fewer than 64 iterations is the default setting. When pipelining, the tool tries to achieve an II of 1. The II is the number of clock cycles before the next iteration of the loop is processed.

   When pipelining the loop with `II=1`, you want the next iteration to start at the next clock cycle. However, in this design the tool has failed to achieve that result, as it reports two II violations (in two loops).

## Analyze the Results

The Synthesis Summary report displays the results. Now you can analyze the design to try to determine what is causing the II violations.

1. At the bottom of the IDE, next to the Console view, select the **DRCs** view. This view reports a little more information related to the two II violations as shown in the following figure.

   >**TIP**: If the DRCs view is not displayed, select **Window** > **Show View** > **DRCs**.

    ![Synthesis Results DRC](./images/synth-results-drc.png)

   You can see from the Details in the report, there is a load operation related to the array `col_inbuf` in the loop `Col_DCT_Loop_DCT_Outer_Loop`. The buffer transaction is taking too long to complete in one clock cycle, thus preventing II=1.

2. In the upper right hand corner of the Vitis HLS IDE, click the **Analysis** perspective to change to the Analysis perspective, and open the Schedule Viewer.

      The Analysis perspective provides a view of different elements of your project, to evaluate the results of synthesis and the performance of your current solution. The Analysis perspective opens the Schedule Viewer by default.

   The Schedule Viewer shows each operation (in chronological order) in the synthesized function, and the clock cycle in which the operation is scheduled in. It is presented as a timeline starting at clock cycle 1, and running through to completion. You can select operations to view the connections between them.

   The default view shows all of the operations, though the drop-down menu at the top of the view lets you select specific elements of the design that are of interest. In this case, you can view the II violations, as shown in the following figure.

   ![Schedule Viewer](./images/schedule-viewer-ii-violation.png)

3. Select the `col_inbuf_load_4` operation. You can see that the length of the operation is greater than a single clock cycle. This extends from clock cycle 86 into clock cycle 87. 

   In the Schedule Viewer, the dotted line in each clock cycle represents the clock uncertainty region. If an operation extends into this dotted region, or into the next clock cycle, then it has exceeded the user requested budget of a single clock cycle.

   Because the `col_inbuf_load_4` operation takes more than one clock cycle (effectively two), the scheduler determines that it either needs more memory ports to achieve an II of 1 or it needs to relax the II to 4 in order to achieve a possible schedule. Because the relax_latency attribute is set to TRUE, the scheduler relaxes the II to 4 to achieve a successful schedule.  

## Next Step

Next, learn about [using optimization techniques](./optimization_techniques.md) to achieve your desired performance.
</br>
<hr/>
<p align="center" class="sphinxhide"><b><a href="/README.md">Return to Main Page</a> — <a href="./README.md">Return to Start of Tutorial</a></b></p>

<p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
