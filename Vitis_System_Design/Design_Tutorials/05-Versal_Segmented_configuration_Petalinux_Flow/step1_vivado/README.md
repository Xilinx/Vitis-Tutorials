# Step 1 Tutorial: Create Base Platform with Segmented Configuration

## Step Overview

In this step, you will create an **extensible hardware platform** for the VCK190 board using Vivado. This platform serves as the foundation for your acceleration application and implements **segmented configuration**, which divides the device configuration into two segments.

### What You Will Learn

- How to use Versal CED (Common Embedded Platform) template
- How to configure a platform for segmented configuration
- Understanding base platform components (PS, NoC, DDR)
- How to export extensible XSA files

### What This Step Does

This step creates the **first segment** of the segmented configuration:

1. **Creates Vivado Project**: Uses Versal CED template to create a base platform
2. **Exports XSA**: Generates extensible XSA file for hardware

## Design Flow

### What is an Extensible Platform?

An **extensible platform** is a hardware platform that:
- Contains base platform components (PS, NoC, DDR)
- Provides extensible interfaces for future kernel connections
- Supports segmented configuration
- Can be extended with custom kernels in later steps



### Segmented Configuration - PS Part

This step creates the **PS part (Processing System)** which:
- Is loaded during system boot via BOOT.BIN
- Contains essential platform components (PS, NoC, DDR)
- Enables DDR memory initialization
- Provides stable base for multiple applications
- The PL part (created in Step 2) will be loaded by the application during runtime

## Step-by-Step Instructions

### Step 1.1: Navigate to Step 1 Directory

```bash
cd step1_vivado
```

### Step 1.2: Review the Vivado Script

The platform creation is automated using `run.tcl`. Let's understand what it does:

**Key Actions in run.tcl**:
1. Creates Vivado project using Versal CED template
2. Sets VCK190 board part
3. Instantiates CED example design with:
   - Design type: Extensible
   - AI Engine: Enabled
4. Generates block design
5. Exports extensible XSA files

### Step 1.3: Build the Platform

Run the Makefile to create the platform:

```bash
make all
```

Or from the root directory:

```bash
cd ..
make step1
```

### Step 1.4: Verify Output

After build completes, verify the output files:

```bash
ls -lh build/vivado/*.xsa
```

You should see:
- `vck190_hw.xsa` - Extensible XSA for hardware

## Detailed Analysis

### CED Template Configuration

The `run.tcl` script uses the Versal CED template with these key settings:

```tcl
instantiate_example_design -template xilinx.com:design:versal_comn_platform:2.0 \
  -design versal_comn_platform \
  -options { Design_type.VALUE Extensible Include_AIE.VALUE true}
```

**Key Options**:
- `Design_type.VALUE Extensible`: Creates extensible platform (not fixed)
- `Include_AIE.VALUE true`: Enables AI Engine array

### Segmented Configuration Setup

The CED template automatically configures segmented configuration when:
- Design type is set to "Extensible"
- Platform is configured for Versal ACAP

This means:
- Base platform components are in first segment
- Extensible interfaces are ready for second segment
- Platform supports dynamic PL loading

<p class="sphinxhide" align="center"><sub>Copyright © 2026 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
