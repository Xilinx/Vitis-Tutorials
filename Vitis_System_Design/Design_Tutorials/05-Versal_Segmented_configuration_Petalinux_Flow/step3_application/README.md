# Step 3 Tutorial: Build Application and Deploy System

## Step Overview

In this final step, you will build the **host application** and generate **SD card contents** for board deployment. You'll use the fixed XSA from Step 2 to create a Vitis platform, build the host application using Petalinux SDK, and package everything for deployment.

### What You Will Learn

- How to create Vitis platform from fixed XSA
- How to develop host applications using XRT (Xilinx Runtime)
- Understanding system packaging (XCLBIN, PDI)
- How to generate SD card contents
- How to deploy and run applications on VCK190 board

### What This Step Does

This step completes the system by:

1. **Creates Vitis Platform**: Creates platform from fixed XSA for application development
2. **Builds Host Application**: Compiles host application using Petalinux SDK/sysroot
3. **Packages System**: Generates XCLBIN and PDI files from fixed XSA
4. **Generates SD Card Contents**: Creates SD card directory with all required files

## Design Flow

### System Architecture

The complete system includes:

```
┌─────────────────────────────────────────┐
│  Host Application (ARM CPU)             │
│  • Loads XCLBIN                         │
│  • Manages data transfer                │
│  • Controls kernels                     │
└──────────────┬──────────────────────────┘
               │ XRT API
               ▼
┌─────────────────────────────────────────┐
│  Hardware Platform                      │
│  • Base Platform (First Segment)        │
│  • PL Design (Second Segment)           │
│    - AI Engine Graph                    │
│    - PL Kernels                         │
└─────────────────────────────────────────┘
```



## Step-by-Step Instructions

### Step 3.1: Navigate to Step 3 Directory

```bash
cd step3_application
```

### Step 3.2: Review Host Application

Before building, understand the host application:

**Host Application** (`host_src/host.cpp`):
- Uses XRT API to interact with hardware
- Loads XCLBIN file
- Allocates buffers for input/output
- Configures and runs kernels
- Verifies results against golden reference


### Step 3.3: Build Application and SD Card

Build for hardware:

```bash
make sd_card TARGET=hw
```

Or from the root directory:

```bash
cd ..
make step3_hw TARGET=hw
```

**What Happens**:
1. Checks Step 2 build artifacts exist
2. Verifies Petalinux common image configuration
3. Generates XCLBIN and PDI from fixed XSA
4. Creates Vitis platform from fixed XSA
5. Builds host application using Petalinux SDK
6. Generates SD card contents

### Step 3.4: Verify Output

After build completes, verify outputs:

```bash
# Check application
ls -lh application/build/application

# Check XCLBIN
ls -lh gm2aie.xclbin

# Check SD card directory
ls -lh package.hw/sd_card/
```

## Detailed Analysis

### Host Application Structure

The host application performs these key operations:

**1. Initialize XRT**:
```cpp
auto dhdl = xrtDeviceOpen(0);
auto xclbin = load_xclbin(dhdl, xclbinFilename);
```

**2. Allocate Buffers**:
```cpp
xrtBufferHandle in_bohdl = xrtBOAlloc(dhdl, input_size_in_bytes, 0, 0);
xrtBufferHandle out_bohdl = xrtBOAlloc(dhdl, output_size_in_bytes, 0, 0);
```

**3. Configure and Start PL Kernels**:
```cpp
xrtKernelHandle mm2s_khdl = xrtPLKernelOpen(dhdl, uuid, "mm2s");
xrtRunHandle mm2s_rhdl = xrtRunOpen(mm2s_khdl);
xrtRunSetArg(mm2s_rhdl, 0, in_bohdl);
xrtRunSetArg(mm2s_rhdl, 2, size_in_samples);
xrtRunStart(mm2s_rhdl);

xrtKernelHandle s2mm_khdl = xrtPLKernelOpen(dhdl, uuid, "s2mm");
xrtRunHandle s2mm_rhdl = xrtRunOpen(s2mm_khdl);
xrtRunSetArg(s2mm_rhdl, 0, out_bohdl);
xrtRunSetArg(s2mm_rhdl, 2, size_out_samples);
xrtRunStart(s2mm_rhdl);
```

**4. Run AI Engine Graph**:
```cpp
auto ghdl = xrtGraphOpen(dhdl, uuid, "mygraph");
xrtGraphRun(ghdl, itr);  // itr = 4 iterations
```

**5. Wait for Completion**:
```cpp
auto state = xrtRunWait(mm2s_rhdl);
state = xrtRunWait(s2mm_rhdl);
xrtGraphEnd(ghdl, 0);
xrtGraphClose(ghdl);
```

<p class="sphinxhide" align="center"><sub>Copyright © 2026 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
