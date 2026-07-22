<table class="sphinxhide" style="width:100%;">
  <tr>
    <td align="center">
      <picture>
        <source media="(prefers-color-scheme: dark)" srcset="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/logo-white-text.png">
        <img alt="AMD logo" src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%">
      </picture>
      <h1>AMD Vitis™ AI Engine Tutorials</h1>
      <a href="https://www.amd.com/en/products/software/adaptive-socs-and-fpgas/vitis.html">Refer to Vitis™ Development Environment on amd.com</a>
        </br>
      <a href="https://www.amd.com/en/products/software/vitis-ai.html">Refer to Vitis™ AI Development Environment on amd.com</a>
    </td>
  </tr>
</table>

# Back-Projection for Synthetic Aperture Radar on AI Engines

## Back-Projection Engine

This section provides an overview of the Synthetic Aperture Radar (SAR) BP engine built using AI Engines. A single engine instance provides a complete implementation of the SAR BP algorithm. You can use multiple instances of the engine to increase throughput by partitioning a non-overlapping portion of the target image to each engine instance. The next section covers this.

### Design Approach

This tutorial implements the full SAR BP algorithm in AI Engine tiles—all compute workloads are partitioned to the AI Engine array. PL URAM stores the SAR image due to its 2 GB footprint. The design establishes a "back-and-forth" streaming data flow from PL to the AI Engine array and back again. The BP engine updates the input SAR image based on the current input radar pulse, producing a revised output SAR image that streams back to PL URAM. The design approach adopts the following concepts.

#### DDR Buffers and PL URAM Buffers

* The `pulse_i` buffer in DDR stores a set of 586 radar pulses of length 2048. These are the zero-padded radar signatures collected from the GOTCHA data set [[1]] over azimuth angles $38,39,40,41,$, and $42$ for "Pass 1" on the "HH" polarization. This data set is input one time to the engine for every radar pulse processed.
* The `coord_i` buffer in DDR stores a set of 586 tuples $(A_x,A_y,A_z)$ representing the 3D coordinates of the antenna platform for each radar pulse captured. This buffer acts as a real-time parameter (RTP) vector for the BP engine that transfers one time to the AI Engine array before the processing of any radar pulses.
* The `R0_range_i` buffer in DDR stores a set of 586 samples representing the range $R_0$ from antenna platform to SAR target image center. This buffer acts as a real-time parameter (RTP) vector for the BP engine that transfers one time to the AI Engine array before the processing of any radar pulses.

* A single buffer `image_buffer` located in PL URAM stores the full SAR target image. The AIE array streams the input image from PL, updates the image for the current radar pulse in the AIE array, then streams the updated resultant image back to the PL URAM which stores it in the same buffer.
* The AI Engine array can run for `NPULSE_USE` radar pulses. The BP engine graph processes a full radar pulse per graph iteration.
* Asynchronous DMA transfers send the `pulse_i` data from DDR to AI Engine array. A single DMA transfer initiates for all radar pulses. A single graph iteration processes each radar pulse.
* After all radar pulses process, you can upload the final target image from PL to DDR using a synchronous DMA transfer.

#### BP Engine Graph and Kernel Scheduling

* The BP engine uses multi-rate scheduling to control its system level operation. The scheme is hard-coded yet configurable for processing any number of radar pulses.
* The design employs a single top-level graph.
* A single graph iteration corresponds to a full update of the SAR target image for one radar pulse. Using multi-rate scheduling, each AI Engine kernel executes as many times as required to perform its workload for that pulse.
* The `ifft2k_async()` graph contains two kernels. The `ifft()` kernel performs the inverse transform function. The `lut()` kernel computes the slope and offset LUTs required by the downstream  `interp1()` graph. Each kernel must be performed once per radar pulse. Consequently, the `ifft()` and `lut()` kernels both use a setting of `repetition_count=1`.
* The `range_gen()` kernel generates the $(x,y,z)$ coordinates of the target image in a 'just-in-time' fashion, as this data is already known and is easily computed. This saves considerable storage for the implementation.
* All other graphs in the BP engine perform computations related to updating the SAR target image. Because the memory footprint for this image exceeds the local tile memory, partial SAR image data is streamed through the design using double-buffering. The design adopts a size of 1024 samples for these I/O kernel buffers. For a $512\times 512$ image, it follows that 256 kernel invocations are required per graph iteration in order to process the full target image; all remaining kernels use `repetition_count=256`.
* Both `interp1()` kernels (for servicing the real and imaginary components of the phase correction) must re-use its slope and offset LUTS over multiple kernel invocations to process the full target image. Because these LUTs are computed only one time per graph iteration, the design must employ asynchronous buffering of these LUTs. Otherwise, the default multi-rate scheduling insists on a 256-fold replication of these buffers. This is infeasible. For this reason, the `interp1()` kernel must be hand-coded to manage this asynchronous buffering. The current DSP library `func_approx()` IP (which otherwise could perform the required linear interpolation functionality) only supports synchronous buffering.
* The memory footprint of the `ifft()` and `lut()` kernels is heavy and is only performed one time per graph iteration. These outputs are held constant over 256 invocations of all remaining kernels. Consequently, the design elects to use `single_buffer()` designations on the I/O buffers of `ifft()` and `lut()`. This has minimal impact on the overall system throughput, because most of the compute workload of the current radar pulse can hide DDR input transfers of the next radar pulse, as noted earlier.

### Graph View

The following figure shows the graph view for the SAR BP engine.

* The graph has two inputs, `ifft_i`, `image_i`, and one output `image_o`. GMIO drives the `ifft_i`, whereas the other two are both driven by PLIO.
* The `ifft()` and `lut()` kernels execute one time per graph iteration to compute the iFFT and populate the slope & offset LUTs required for linear interpolation.
* The `range_gen()` kernel also executes one time per graph iteration to compute the $(x,y,z)$ coordinates of the target image.
* All remaining kernels execute 256 times per graph iteration, processing 1024 pixels per kernel invocation.
* Two separate kernels `interp1_re()` and `interp1_im()` implement the `interp1()` graph. Each kernel uses asynchronous input buffers to manage its input LUTs.
* Two separate kernels `cos()` and `sin()` implement the `expjx()` graph. These are instantiations of `func_approx()` from the Vitis DSP Library.

![figure](../images/bp-engine-graph-view.png)

### Floorplan View

The floorplan view of the SAR BP engine design is shown in the following figure. Mapping and placement are achieved using a "bounding-box" constraint with a $4\times 8$ rectangle of tiles. AI Engine compilation uses `Xmapper=BufferOptLevel9` to ensure there are no memory bank conflicts that could stall the pipeline.

![figure](../images/bp-engine-floorplan-view.png)

### Resource Utilization

The following figure shows the AI Engine resources for the SAR BP engine. The design uses 14 tiles for compute and 26 tiles overall for compute and buffering. Recall the earlier system partitioning analysis estimated a total of 31 tiles, indicating this initial provisioning was not overly aggressive. The engine requires a total of one GMIO and two PLIO interface ports. The memory footprint of the design is large but the mapper/router can automatically find a contention-free solution with a simple area group constraint.

![figure](../images/bp-engine-aie-resources.png)

### Throughput and Latency

This section investigates the throughput and latency of the SAR BP engine using both hardware emulation and hardware.

#### Hardware Emulation

The single engine version of the design may be evaluated for its throughput andd latency using hardware emulation. The simulation is run for only `NPULSE_USE=2` and `NFRAME=1` to make this practical (see [Line 13](../aie/sar_top_1engine/sar_top_1engine_cfg.h#L13) and [Line 18](../aie/sar_top_1engine/sar_top_1engine_cfg.h#L18) in `sar_top_1engine_cfg.h`). The simulation waveforms provide a means to investigate the behavior of the circuit. The following figure shows the waveforms. Some important observations include the following:

* The waveforms show the AXI-S stream traffic between the PL URAM buffer and the AI Engine array over the two streams `aie_to_buff` and `buff_to_aie`.
* The top-most waveform shows the initial iFFT computation occuring before any data returns to the PL URAM over `aie_to_buff` is $\approx 30 \mu s$.
* The middle waveform shows the time period for processing a single radar pulse is $\approx 662.6 \mu s$.
* The bottom waveform shows the time period for uploading the final target image is $\approx 720.9 \mu s$.
* Based on these values, you can compute an approximate frame rate for processing 586 pulses as $1/(30e-6+586*662.6e-6+720.9e-6) \approx 2.6$ frames per second. This is close to the frame rate predicted during system partitioning.
* The frame rate as measured by the time stamps (refer to the following figure for details) is 1.65 fps, a lower number because the image upload time is amortized over only two radar pulses in this case.

![figure](../images/bp-engine-throughput-emu.png)

Consider the latency of the design as the time required to process the full target image less the upload time. This assumes the image is produced as new iFFT radar pulses stream into the engine. In this case, the latency includes that of the iFFT processing plus the time required to process all the radar pulses. This amounts to approximately $390  ms$ for a full compliment of 586 radar pulses as shown in the following figure.

![figure](../images/bp-engine-latency-emu.png)

#### Hardware

The single engine version of the design may also be run in hardware on the VCK190 evaluation board. In this case, `NPULSE_USE=586` and `NFRAME=16` to run a full 16 frames with the full compliment of radar pulses for each. Throughput is measured using the `xrt::graph::get_timestamp()` function that returns a time stamp from the AIE array. The difference between two such timestamps provides the number of AI Engine clock cycles between them. Specifically, two time stamps are taken in [Line 178](../device1/host.cpp#L178) and [Line 208](../device1/host.cpp#L208) of `device1/host.cpp`. A screenshot of this run captured from the VCK190 board is shown below.

* Note the final throughput computed using `xrt::graph::get_timestamp()` is 2.6 frames per second. This matches well to the estimate computed from the 2-pulse simulation performed in hardware emulation.

![figure](../images/bp-engine-throughput-hw.png)

### Block Design: `ifft2k_async()`

The following figure shows details of the `ifft2k_async()` block.

* The `ifft2k_async()` graph contains two kernels, the `ifft()` and the `lut()`. Both use a `repetition_count=1` to perform their workloads once per graph iteration.
* The design uses single I/O buffering throughout to minimize its memory footprint without significant impact to throughput performance.
* The design implements the `ifft()` kernel using custom AIE API rather than using the DSP library because the kernel must perform a $1/N_{FFT}$ scaling and a `fftshift()` reordering of the front and back halves of the output data.
* The `lut()` kernel computes the slope and offset LUTs required by the downstream `interp1()` graph to perform linear interpolation of the `ifft()` outputs. Separate LUTs are produced for the real and imaginary parts of the `ifft()` output. The slope and offset entries of the last bin in the both output LUTs are set to zero for simplicity to align with the approach used for the `sin()` and `cos()` blocks.
* Two dummy output kernels validate the overall graph. This is because the output LUTs employ asynchronous single buffering.
* The throughput of ~115 Msps exceeds the worst-case requirement of 26 Msps identified during system partitioning when using eight engines to achieve a total aggregate throughput of > 20 fps.
* The latency of the design is ~32 µs.

![figure](../images/bp-engine-ifft2k-async.png)

### Block Design: `range_gen()`

The following figure shows details of the `range_gen()` block.

* The `range_gen()` contains a single kernel that generates the $(x,y,z)$ position across the target scene. This block uses a `repetition_count=1` to perform its workload once over each graph iteration.
* The $x$ and $y$ coordinates derive from the resolution computed from the radar system parameters. The $z$ coordinates are all set to zero in the absence of any targeting height information.
* The `range_gen()` generates the coordinates in the order expected by the `diff3dsq()` block to follow.

![figure](../images/bp-engine-range-gen.png)

### Block Design: `diff3dsq()`

The following figure shows details of the `diff3dsq()` block.

* The `diff3dsq()` graph computes the squared Euclidean distance between the antenna platform $(A_X,A_Y,A_Z)$ and every position $(x,y,z)$ in the target scene.
* The graph contains a single kernel that uses a `repetition_count=256` and a I/O buffer size of 1024 samples to perform the full compute workload of the target image in one graph iteration.
* The graph also scales its output by a fixed factor `sqrt_compress = 1.025640994e-08` to compress its dynamic range to the $1 \le x \lt 4$ range required for the downstream `sqrt()`. This constant was identified by examining the full dynamic range over the GOTCHA data set (but could be parameterized in principle).
* The graph uses a vector RTP of tuples $(A_X,A_Y,A_Z)$ to define the array platform coordinates for each radar pulse to process.
* The initiation interval $II=47$ is not the best pipelined solution here, but its throughput of 410 Msps is sufficient to meet the 400 Msps target of the design.

![figure](../images/bp-engine-diff3dsq.png)

### Block Design: `sqrt()`

The following figure shows details of the `sqrt()` block.

* The `sqrt()` graph is an instantiation of the `func_approx()` IP from the Vitis DSP Library configured with `TP_COARSE_BITS=10`. This yields an accuracy better than 1e-06 across the input range of $1 \le x \lt 4$.
* The graph is configured for `repetition_count=256` and uses I/O double buffers with 1024 samples each.
* The memory footprint for this block is large due to the lookup tables.
* The design throughput of 437 Msps exceeds the design target of 400 Msps.

![figure](../images/bp-engine-sqrt.png)

### Block Design: `dR_comp()`

The following figure shows details of the `dR_comp()` block.

* The `dR_comp()` graph applies an expansion gain to restore the output `sqrt()` dynamic range, computes the difference between that output and the $R_0$ range to target scene center, and then applies scaling gains for the downstream `fmod_floor()` and `interp1()` graphs to manage dynamic ranges of those functions.
* The graph uses a vector RTP of distances $R_0$ for each radar pulse to be processed.
* The downstream output for `fmod_floor()` is scaled by the radar parameters [[2]] $2F_{min}/c$ as required by the MATLAB system model. The factor of $/pi$ is omitted as it is incorporated into the LUT implementations of both `cos()` and `sin()` kernels.
* The downstream output for `interp1()` is offset and scaled by the inverse of the maximum target scene size $W_r$. This mapping computes $\hat{dR}=0.5+(1/W_r)dR$ such that $\hat{dR}$ runs over the range $(0,1)$ instead of $(-W_r/2,+W_r/2)$. This x-axis normalization simplifies the linear interpolation to follow.
* The design throughput of ~750 Msps exceeds the design target of 400 Msps.

![figure](../images/bp-engine-dRcomp.png)

### Block Design: `fmod_floor()`

The following figure shows details of the `fmod_floor()` block.

* The `fmod_floor()` graph reduces its output dynamic range to $(0,1)$ to match that expected by the downstream `expjx()` graph by computing $y=mod(x,1)=x-floor(x)$.
* The output is also clamped at `MAXVAL = 0.9990234375` to align with the highest bin in the LUT-based implementation of `expjx()`. This works together with those downstream graphs to approximate any input falling into this last bin as if it instead fell on the last bin lower edge to improve the accuracy of the approximate implementation.
* The design throughput of ~480 Msps exceeds the design target of 400 Msps.

![figure](../images/bp-engine-fmod-floor.png)

### Block Design: `expjx()`

The following figure shows details of the `expjx()` block.

* The `expjx()` graph computes the complex exponential of its input argument (assumed to be restricted to the $(0,1)$ range) using two underlying instances of the `func_approx()` IP from the Vitis DSP Library, one configured for `cos()` and another configured for `sin()`. Both use `TP_COARSE_BITS=10` to set the resolution of the linear interpolation.
* The initialization code of the `cos()` and `sin()` LUTs for slope and offset are not currently in the library. This tutorial provides these.
* The design throughput of ~435 Msps exceeds the design target of 400 Msps.

![figure](../images/bp-engine-expjx.png)

### Block Design: `interp1()`

The following figure shows details of the `interp1()` block.

* The `interp1()` graph performs linear interpolation on the real (or imaginary) part of the `ifft()` output using the slope and offset LUTs produced by that graph. The `dR_comp()` graph produces the x-axis inputs at which the interpolation is performed. The SAR BP engine requires two instances of this block.
* This block is hand-coded in AIE API but most of the code is from the Vitis DSP Library (`func_approx()` IP). Hand-coding means the input buffering of the slope and offset LUTs can be asynchronous. Currently, the library support is restricted to static LUT configurations, but the SAR algorithm requires new LUTs to be computed for each radar pulse.
* The design throughput of ~430 Msps exceeds the design target of 400 Msps.

![figure](../images/bp-engine-interp1.png)

### Block Design: `image_buffer()`

The `image_buffer()` block implements the image I/O buffer for the design. It uses PL URAM for image storage and is designed using Vitis HLS. The following figure shows details of the block.

* The HLS design consists of two phases: the `SEND_PULSES` phase transmits the image stored in the PL URAM to the AI engine over a single AXI-S stream. The image is updated by the BP SAR engine and returned to the PL URAM over another AXI-S stream. The process repeats for a number of radar pulses `NPULSE_USE` configured by the host software. When all radar pulses are processed, the `UPLOAD_IMAGE` phase uploads the final SAR target image to DDR over the NoC.
* The clock rate for the HLS design is 312.5 MHz. AXI-S I/O streams are 128-bit to align with the 1250 MHz @ 32-bit interface of the AI Engine array. Vitis tools automaacally instantiates the clock domain crossing and data width converter blocks.
* The main resource required is the 60 URAM blocks required for the image buffer.

![figure](../images/bp-engine-image-buffer.png)

Write the HLS code for `image_buffer()` so as to handle backpressure on the input and output AXI-S streams. The following figure shows the annotated HLS code. Note a few key aspects of the implementation:

* The `SEND_PULSES` section of the code on Line 12 captures a while loop that manages the read side and write side of the PL URAM image buffer. The write-side address leads the read side address by the latency of the AI Engine implementation. For this reason, we expect an offset between the write and read addresses.
* Backpressure can occur on both the write side or the read side of the buffer. Write-side backpressure happens every radar pulse when the graph stalls waiting for the iFFT to be complete before image processing commences. Read-side backpressure can occur in a similar manner when the final image pixels for a radar pulse transfer back to the PL URAM and the AI engine then stalls.
* You need the read and write sides to stall independently as they can stall at different times. This is achieved by each side validating the state of the AXI-S streams before performing a read or a write operation. The following figure shows the aspects of the code involved annotated in red.
* On the write side, a check is performed in Line 16 and writes occur only when the outgoing stream is not already full.
* On the read side, a check is performed in Line 26 and reads occur only when the incoming stream is not empty.
* The pragma on Line 7 ensures the PL URAM has a latency of only 1 cycle. This is needed in order to achieve II=1 pipelined operation of the `SEND_PULSES` task.
* The `UPLOAD_IMAGE` section of the code on Line 38 contains a for loop to transfer the final image back to the host using DDR over the NoC.

![figure](../images/bp-engine-image-buffer-hls.png)

### Final SAR BP Engine Performance

The following figure compares the performance of the final AI Engine implementation vs the ideal MATLAB baseline when run on the GOTCHA data set with a total of 586 radar pulses. This run shows the final performance with all the implementation artifacts. The SSIM metric has a value of 0.9965 and the PSNR has a value of 74.1 dB. These compare favorably to the early results computed during system modeling.

![figure](../images/matlab-versus-aie-final-image.png)

### References

[1]: <https://www.sdms.afrl.af.mil/index.php?collection=gotcha> "GOTCHA Volumetric SAR Data Set"
[[1]]: U.S. Air Force, "GOTCHA Volumetric SAR Data Set," U.S. Air Force Sensor Data Management System.

[2]:<https://www.spiedigitallibrary.org/conference-proceedings-of-spie/7699/1/SAR-image-formation-toolbox-for-MATLAB/10.1117/12.855375.short> "SAR image formation toolbox for MATLAB"
[[2]]: L.A. Gorham & L.J. Moore, "SAR Image Formation Toolbox for MATLAB", SPIE Defense, Security, and Sensing, Orlando, FL, 2010.

<p class="sphinxhide" align="center"><sub>Copyright © 2026 Advanced Micro Devices, Inc</sub></p>
<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/legal/copyright.html">Terms and Conditions</a></sup></p>
