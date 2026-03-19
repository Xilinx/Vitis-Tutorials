# Step 2 Tutorial: Integrate AI Engine and PL Kernels

## Step Overview

In this step, you will develop and integrate **AI Engine graph** and **PL (Programmable Logic) kernels** with the extensible platform from Step 1. This creates a complete hardware design that includes both the base platform (first segment) and the PL design with kernels (second segment).

### What You Will Learn

- How to develop AI Engine graphs
- How to develop PL kernels using HLS
- How to integrate kernels with extensible platform
- Understanding system connectivity and data flow
- How to generate fixed XSA containing both segments

### What This Step Does

This step creates the **PL part (Programmable Logic)** of the segmented configuration:

1. **Compiles AI Engine Graph**: Builds AI Engine graph with simple processing kernels into `libadf.a`
2. **Compiles PL Kernels**: Compiles HLS kernels (mm2s, s2mm) into `.xo` files
3. **Links System**: Links all kernels with extensible XSA
4. **Generates Fixed XSA**: Creates fixed XSA containing the PL design that will be loaded at runtime by the application

## Design Flow

### Design Overview

This step integrates kernels into a complete PL design:

```
┌─────────────┐      ┌──────────────────────────────┐      ┌─────────────┐
│   mm2s      │────▶│ AI Engine Graph              │────▶ │    s2mm     │
│  (PL)       │      │  first → second              │      │    (PL)     │
└─────────────┘      │  (simple kernels)            │      └─────────────┘
                     └──────────────────────────────┘
```

### Kernel Functions

1. **mm2s** (Memory-to-Stream):
   - Reads data from DDR memory
   - Streams data to AI Engine graph
   - PL kernel implemented in HLS
   - Processes 32-bit integer data

2. **AI Engine Graph**:
   - **simpleGraph**: Contains two kernels (`first` and `second`)
   - Both kernels use the `simple` function to process complex data (cint16)
   - Each kernel processes 32 samples per iteration
   - Graph runs for 4 iterations (128 total samples)
   - Implemented using AI Engine API

3. **s2mm** (Stream-to-Memory):
   - Receives streaming data from AI Engine graph
   - Writes results to DDR memory
   - PL kernel implemented in HLS
   - Processes 32-bit integer data

### Data Flow

```
DDR Memory → mm2s → AI Engine Graph (first → second) → s2mm → DDR Memory
```


## Step-by-Step Instructions

### Step 2.1: Navigate to Step 2 Directory

```bash
cd step2_vitis_integration
```

### Step 2.2: Build Fixed XSA

Build for hardware:

```bash
make build TARGET=hw
```

Or from the root directory:

```bash
cd ..
make step2_hw
```

**What Happens**:
1. Makefile checks all source files exist
2. Compiles AI Engine graph using `aiecompiler`
3. Compiles each PL kernel using `v++`
4. Links all kernels with extensible XSA
5. Generates fixed XSA file

### Step 2.3: Verify Output

After build completes, verify outputs:

```bash
ls -lh sys_gm2aie_*.xsa libadf.a *.xo
```

You should see:
- `sys_gm2aie_hw.xsa` - Fixed XSA for hardware
- `libadf.a` - AI Engine graph archive
- `mm2s.xo`, `s2mm.xo` - PL kernel objects

## Detailed Analysis



### Connectivity Configuration

The `system.cfg` file defines:

```ini
[connectivity]
nk=mm2s:1:mm2s_1
nk=s2mm:1:s2mm_1
stream_connect=mm2s_1.s:ai_engine_0.mygraph_in
stream_connect=ai_engine_0.mygraph_out:s2mm_1.s
```

**Key Connections**:
- `mm2s_1` output → AI Engine graph input (`mygraph_in`)
- AI Engine graph output (`mygraph_out`) → `s2mm_1` input

### PL Kernel Interfaces

Each PL kernel uses HLS interface pragmas:

**mm2s**:
- `m_axi`: Memory interface (DDR access)
- `axis`: Stream interface (to AI Engine)
- `s_axilite`: Control interface

**s2mm**:
- `m_axi`: Memory interface (DDR access)
- `axis`: Stream interface (from AI Engine)
- `s_axilite`: Control interface

<p class="sphinxhide" align="center"><sub>Copyright © 2026 Advanced Micro Devices, Inc</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
