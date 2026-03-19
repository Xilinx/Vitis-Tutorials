# Tutorial: VCK190 Segmented Configuration with Petalinux

**Version:**  
**Vitis: 2025.2**  
**Vivado: 2025.2**  
**Petalinux: 2025.2**

This tutorial guides you through creating a complete acceleration application for the **VCK190 board** using **segmented configuration desing** and **Petalinux common image**. You will learn how to build a hardware platform, integrate AI Engine and PL kernels, and deploy a complete system.

>Note: This tutorial uses a Makefile-based project and is only intended for the hardware flow. The Vitis GUI flow and hardware emulation are not supported or tested.

## Tutorial Overview

This tutorial demonstrates a complete design flow that includes:

- **Segmented Configuration**: Separates base platform (first segment) from PL design (second segment) for faster boot times and dynamic PL loading
- **AI Engine Integration**: Develops and integrates AI Engine graph with PL kernels
- **Petalinux Integration**: Uses pre-built Petalinux common image for rapid application development
- **Complete System Deployment**: Generates SD card contents ready for board execution

### What You Will Learn

By completing this tutorial, you will understand:

1. How to create an extensible hardware platform using Vivado CED template
2. How to develop and integrate AI Engine graphs with PL kernels
3. How to use segmented configuration for Versal ACAP devices
4. How to build host applications and deploy complete systems to VCK190 board using Petalinux common image


##  Tutorial Architecture

The tutorial is organized into three main steps, each building upon the previous:

```
┌─────────────────────────────────────────────────────────────┐
│  Step 1: Create Base Platform (Vivado)                      │
│  ───────────────────────────────────────────────────────    │
│  • Create extensible XSA using Versal CED template          │
│  • Export base platform                     │
│                                                              │
│  Output: vck190_hw.xsa                      │
└───────────────────┬─────────────────────────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────────────────────────┐
│  Step 2: Integrate Kernels (Vitis)                          │
│  ───────────────────────────────────────────────────────    │
│  • Compile AI Engine graph (libadf.a)                       │
│  • Compile PL kernels (mm2s, s2mm)                          │
│  • Link kernels with extensible XSA                         │
│  • Generate fixed XSA (contains PL design)                 │
│                                                              │
│  Output: sys_gm2aie_hw.xsa, libadf.a                       │
└───────────────────┬─────────────────────────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────────────────────────┐
│  Step 3: Build Application (Petalinux)                       │
│  ───────────────────────────────────────────────────────    │
│  • Create Vitis platform from fixed XSA                    │
│  • Build host application using Petalinux SDK               │
│  • Package XCLBIN and PDI files                            │
│  • Generate SD card contents                               │
│                                                              │
│  Output: application, gm2aie.xclbin, SD card image        │
└─────────────────────────────────────────────────────────────┘
```

## Tutorial Structure

```
seg_petalinux/
├── README.md                    # This tutorial overview
├── Makefile                     # Top-level Makefile (orchestrates all steps)
│
├── step1_vivado/                # Step 1: Base Platform Creation
│   ├── README.md                # Step 1 detailed tutorial
│   ├── Makefile                 # Step 1 build automation
│   └── run.tcl                  # Vivado script for platform creation
│
├── step2_vitis_integration/     # Step 2: Kernel Integration
│   ├── README.md                # Step 2 detailed tutorial
│   ├── Makefile                 # Step 2 build automation
│   ├── cfg/                     # Configuration files
│   │   └── system.cfg           # Kernel connectivity configuration
│   ├── src/                     # AI Engine sources
│   │   ├── graph.cpp            # AI Engine graph definition
│   │   ├── graph.h              # Graph class definition
│   │   ├── kernels.h             # Kernel function declarations
│   │   └── include.h            # Common definitions
│   └── pl_kernel/               # PL kernel sources
│       ├── mm2s.cpp             # Memory-to-Stream kernel
│       └── s2mm.cpp             # Stream-to-Memory kernel
│
└── step3_application/           # Step 3: Application Development
    ├── README.md                # Step 3 detailed tutorial
    ├── Makefile                 # Step 3 build automation
    ├── platform_creation.py     # Vitis platform/app creation script
    ├── host_src/                # Host application sources
    │   ├── host.cpp             # Main host application
    │   ├── input.h              # Input test data
    │   └── golden.h             # Expected output (golden reference)
    └── cfg/
        └── package.cfg          # Package configuration
```

## Key Concepts

### Segmented Configuration

**Segmented configuration** separates the Processing System (PS) and Programmable Logic (PL) parts. The key principle is that the PL part is loaded by the application during runtime, not at boot time.

1. **PS Part (Processing System)**:
   - Contains the processor system, NoC to DDR initialization
   - Loaded during system boot via BOOT.BIN
   - Runs the Linux operating system and host application
   - Provides stable base for multiple applications
   - Created in **Step 1**

2. **PL Part (Programmable Logic)**:
   - Contains AI Engine graph and PL kernels
   - Compiled into XCLBIN and PDI files in **Step 2**
   - Loaded dynamically by the host application during runtime using XRT APIs
   - The application uses `xrtDeviceOpen()` and `load_xclbin()` to program the PL
   - Can be changed without full system reconfiguration or reboot
   - Created in **Step 2**, loaded in **Step 3**

**Key Benefit:**
The PL part is completely separated from the PS part. The host application running on the PS controls when and which PL design is loaded into the FPGA. This enables:
- Faster boot times (only PS components loaded during boot)
- Dynamic PL loading (load different designs without full reconfiguration)
- Multi-application support (different PL designs on same base platform)


### Design Flow

The tutorial follows a **bottom-up design flow**:

1. **Hardware Platform** (Step 1): Create base platform with extensible interfaces
2. **Kernel Integration** (Step 2): Develop and integrate kernels with platform
3. **Application Development** (Step 3): Build host application and deploy system

## Prerequisites

Before starting this tutorial, ensure you have:

### Required Software

1. **AMD Vivado 2025.2** with VCK190 board files installed
2. **AMD Vitis Unified IDE 2025.2** installed
3. **AMD Petalinux Common Image 2025.2** (for Step 3)
   - Download from [AMD Download Center](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/embedded-platforms.html)

### Required Hardware

- **VCK190 Evaluation Board** (for hardware testing)

### Required Knowledge

Basic understanding of:
- Vivado IP Integrator
- Vitis application acceleration development
- Versal segmented configuration concepts
- Linux command line
- Makefile basics

### Environment Setup

Source the required environments before starting:

```bash
# Vitis environment (includes Vivado)
source <vitis_install_dir>/settings64.sh

# Set Petalinux common image path (for Step 3)
export COMMON_IMAGE_VERSAL=/path/to/common_image_directory
```

## Quick Start

### Build Complete Flow for Hardware

To build the complete system for hardware deployment:

```bash
# From the tutorial root directory
make sd_card TARGET=hw
```

This single command will:
1. Generate extensible XSA (Step 1)
2. Build kernels and generate fixed XSA (Step 2)
3. Build application and generate SD card contents (Step 3)

## Step-by-Step Tutorial Guide

### Step 1: Create Base Platform

**Goal**: Create an extensible hardware platform with segmented configuration support.

**What You'll Do**:
- Use Vivado to create a Versal CED-based platform
- Configure for VCK190 board
- Enable segmented configuration
- Export extensible XSA files

**Key Learning Points**:
- Understanding extensible platforms
- Segmented configuration setup
- Base platform components (PS, NoC, DDR)

**See**: [Step 1 Tutorial Guide](step1_vivado/README.md)

### Step 2: Integrate Kernels

**Goal**: Develop and integrate AI Engine graph with PL kernels to create the PL design that will be loaded at runtime.

**What You'll Do**:
- Compile AI Engine graph with simple processing kernels
- Compile PL kernels (mm2s, s2mm)
- Link all kernels with the extensible platform
- Generate fixed XSA containing the PL design

**Key Learning Points**:
- AI Engine graph development
- PL kernel development with HLS
- System integration and connectivity
- Fixed XSA generation for runtime loading

**See**: [Step 2 Tutorial Guide](step2_vitis_integration/README.md)

### Step 3: Build Application

**Goal**: Build host application and generate SD card contents for board deployment.

**What You'll Do**:
- Create Vitis platform from fixed XSA
- Build host application using Petalinux SDK
- Package XCLBIN and PDI files
- Generate SD card contents

**Key Learning Points**:
- Vitis platform creation
- Host application development with XRT
- System packaging and deployment
- SD card preparation

**See**: [Step 3 Tutorial Guide](step3_application/README.md)



## Understanding the Design

### Application Overview

This tutorial implements a **simple AI Engine processing pipeline**:

1. **Input**: Data read from DDR memory via `mm2s` kernel
2. **Processing**: 
   - AI Engine graph with two simple kernels in a chain (first → second)
   - Each kernel processes 32 samples per iteration
   - Graph runs for 4 iterations
3. **Output**: Results written to DDR memory via `s2mm` kernel

### Data Flow

```
DDR Memory → mm2s → AI Engine Graph (first → second) → s2mm → DDR Memory
```

### Kernel Functions

- **mm2s**: Memory-to-Stream - Reads data from DDR and streams to AI Engine
- **simple**: AI Engine kernel - Processes complex data samples (cint16)
- **s2mm**: Stream-to-Memory - Writes results from AI Engine to DDR


## Expected Results

After completing all steps, you will have:

### Generated Files

- **Step 1**: `step1_vivado/build/vivado/vck190_hw.xsa`
- **Step 2**: `step2_vitis_integration/sys_gm2aie_hw.xsa`, `libadf.a`
- **Step 3**: `step3_application/application`, `gm2aie.xclbin`, SD card contents

### SD Card Contents

The final SD card will contain:
- `BOOT.BIN` - Boot image (base platform - first segment)
- `Image` - Linux kernel
- `gm2aie.xclbin` - XCLBIN file
- `vpl_gen_fixed_pld.pdi` - PDI file (PL design - second segment)
- `pl.dtbo` - Device tree overlay
- `application` - Host executable
- `run_app.sh` - Execution script
- `boot.scr`   - File to control the boot flow by uboot

## Testing on VCK190 Board

After building, you can test on VCK190 hardware:

1. **Prepare SD Card**: Copy all files from `step3_application/package.hw/sd_card/` to SD card
2. **Set Boot Mode**: Configure VCK190 to boot from SD card
3. **Boot Board**: Power on and boot into Linux
4. **Run Application**: Execute the application and verify results

See [Step 3 Tutorial](step3_application/README.md) for detailed board execution instructions.

## Troubleshooting

### Common Issues

1. **Environment Not Sourced**: Always source Vitis environment before running
2. **Missing Board Files**: Ensure VCK190 board files are installed
3. **Petalinux Image Not Found**: Download and set `COMMON_IMAGE_VERSAL` path
4. **Build Failures**: Check log files in each step's build directory


## Additional Resources

### AMD Documentation

- [UG1273: Segmented Configuration](https://docs.amd.com/r/en-US/ug1273-versal-acap-design/Segmented-Configuration)
- [UG1393: Vitis Application Acceleration](https://docs.amd.com/r/en-US/ug1393-vitis-application-acceleration)
- [UG1144: Petalinux Tools](https://docs.amd.com/r/en-US/ug1144-petalinux-tools-reference-guide)
- [VCK190 Board User Guide](https://www.xilinx.com/products/boards-and-kits/vck190.html)



## Cleanup

### Clean Intermediate Files

```bash
make clean
```

Removes build artifacts but keeps output files.

### Complete Cleanup

```bash
make ultraclean
```

Removes everything including output files. Use this to start fresh.

## Summary

This tutorial provides a complete hands-on experience with:

- Segmented configuration for Versal ACAP (PL loaded at runtime by application)
- AI Engine graph development
- PL kernel development and integration
- System integration and deployment
- Petalinux application development

By completing this tutorial, you will have the knowledge and experience to:
- Create extensible platforms for Versal devices
- Develop and integrate AI Engine and PL kernels
- Build complete systems with segmented configuration
- Deploy applications to VCK190 hardware

**Ready to start?** Begin with [Step 1: Base Platform Creation](step1_vivado/README.md)!

---

<p class="sphinxhide" align="center"><sub>Copyright © 2026 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
