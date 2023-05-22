<table class="sphinxhide" width="100%">
 <tr>
   <td align="center"><img src="https://raw.githubusercontent.com/Xilinx/Image-Collateral/main/xilinx-logo.png" width="30%"/><h1>AI Engine Debug Walkthrough Tutorial - From Simulation to Hardware</h1>
   </td>
 </tr>
  <tr>
 <td align="center"><h2>AI Engine/PL Kernel Debug with XRT Utilities on HW</h1>
 </td>
 </tr>
</table>

This section is useful when running the design in hardware, and there is a stall/hang. The first step is to examine the errors from XRT which in turn reflects the errors from the AIE driver. The following steps show you how to obtain this information running the tutorial design in hardware.

# xbutil

The utility, `xbutil`, is supported for PL/AI Engine debug. It can be used to:

* [Report the AI Engine Running Status](#report-the-ai-engine-status).
* [Report XRT Rrrors](#report-xrt-errors).

## Report the AI Engine Status

### Step 0: Prepare the Boot Image and Boot the Board

After the design is built correctly without error, you are ready to run on the hardware board.

* Flash the SD card with the built `sd_card.img`.
* Plug the flashed SD card into the SD card slot of the VCK190 board.
* Connect the USB type C cable to the board and computer that supports the serial port connection.
* Set the serial port configuration with Speed=115200, Data=8 bit, Parity=none, Stop bits=1 bit, and flow control=none.
* Power up the VCK190 board to see boot messages from the serial connection.

### Step 1.1: Generate AI Engine Status Data

Create an `xrt.ini` file on the SD card using the following lines.

```bash
[Debug]
aie_status=true
aie_status_interval_us=1000
```

### Step 1.2: Run the Application After Petalinux Boots Up on the Board

```bash
cd /run/media/mmcblk0p1
./host.exe a.xclbin
```

### Step 1.3: Collect the AI Engine Status Files

After the design run completes on the hardware, the generated profiling files and `run_summary` files need to be collected and are ready to be examined.

#### Step 1.3.1

Make sure the `aie_status_edge.json`, `aieshim_status_edge.json`, `summary.csv`, and `xrt.run_summary` files are generated on the SD card.

#### Step 1.3.2

Power off the board, take out the SD card from the board, and plug it into the host SD card reader.

#### Step 1.3.3

Copy the `aie_status_edge.json`, `aieshim_status_edge.json`, `summary.csv`, and `xrt.run_summary` files from the SD card back to where the design is.

* `xrt.run_summary`: Run summary that contains a list of the files information that can be used by `vitis analyzer`.
* `aie_status_edge.json`: Status of the AI Engine and AI Engine memory.
* `aieshim_status_edge.json`: AI Engine interface tiles status.
* `summary.csv`: Always created.

### Step 1.4: Launch Vitis Analyzer to Examine the Status Files

```bash
vitis_analyzer xrt.run_summary
```

After above command issued, expect to see the `vitis_analyzer` GUI launched. Click `Set AI Engine compile summary` in the summary view to set the AI Engine compile summary.
![alt text](images/bc_va.png)

## AI Engine Status Explanation

Click the `Graph` view then `Tile Status`.

* `Tile Status`: Shows all the tiles completed execution and at `Disabled, Done` state.
![alt text](images/bc_va1.png)

* `PC`: Shows the current PC (program counter) value. It can be cross-probed with the source code by clicking it.
![alt text](images/bc_va3.png)

* `Error Event`: Shows the error events that occurred before the status was generated.
* `Buffers`: Shows the buffer objects in the design with the current status.

Click `DMA Status`. The DMA status shows each tile's DMA input/output channel status.
![alt text](images/bc_va2.png)

Refer to the [UG1076, Analyzing AI Engine Status in Vitis Analyzer](https://docs.xilinx.com/r/en-US/ug1076-ai-engine-environment/Analyzing-AI-Engine-Status-in-Vitis-Analyzer) section for details.

## Report XRT Errors

Refer to the [xrt-error-api](https://xilinx.github.io/XRT/2022.1/html/xrt_native_apis.html#xrt-error-api) for detail information.

### Report Mechanism with XRT APIs

Asynchronous errors are cached in the driver subsystems and can be accessed by user application through APIs. Asynchronous errors retrieved from the driver are shared between the user space and kernel space.

Example using XRT APIs:

```bash
    dut.run(100);
    try {
        dut.wait();
    }
    catch (const std::system_error& ex) {
        if (ex.code().value() == ETIME) {
            xrt::error error(device, XRT_ERROR_CLASS_AIE);

            auto errCode = error.get_error_code();
            auto timestamp = error.get_timestamp();
            auto err_str = error.to_string();

            /* code to deal with this specific error */
            std::cout << timestamp << " error code:" << errCode << " Error:" << err_str << std::endl;
         } else {
         /* Something else */
         }
     }

```

### Step 2.1: Replace `host.cpp` with `host.cpp.error_code` that is Available from this Tutorial

```bash
cp ${PROJECT_PATH}/sw/host.cpp.error_code ${PROJECT_PATH}/sw/host.cpp
cd ${PROJECT_PATH}
```

### Step 2.2: Compile the `host.cpp` Code and Package to `sd_card.img` by Issuing the Following Commands

```bash
make host.exe
make package
```

### Step 2.3: Follow Step 0 to Prepare the Boot Image and Boot the Board

[Step 0: Prepare the Boot Image and Boot the board](#step-0-prepare-the-boot-image-and-boot-the-board)

### Step 2.4 : After the VCK190 is Powered Up and Booted Up Complete, Run the Binaries

```bash
cd /run/media/mmcblk0p1
./host.exe a.xclbin
```

### Step 2.5: Expected Result

![alt text](images/bc_err0.png)

### Step 2.6: Report Mechanism with xbutil

Issue the command `xbutil examine -d 0000:00:00.0 -r error` to the VCK190 console to get output from the console.
![alt text](images/bc_err.png)

# Support

GitHub issues will be used for tracking requests and bugs. For questions, go to [support.xilinx.com](https://support.xilinx.com/).

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2023 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
