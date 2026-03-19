# Creating VMA for Vitis Export to Vivado

This step links the VSS with the platform. 

The platform provides interfaces with PFM properties. v++ link connects the platform interfaces with the VSS interfaces.

## Flow Overview

This step is the **third step** in the VSS→VMA→NPM integration flow. It links the platform-independent VSS with the custom platform and generates a VMA (Vitis Metadata Archive) for Vivado import.

**Position in the flow**: 
```
AIE (libadf.a) → VSS → Platform → VMA (this step) → Vivado Import → NPM
```

**What this step does**:
1. Takes the VSS file (`vss_ss.vss`) and platform XSA (`hw.xsa`) as inputs
2. Links the VSS subsystem to the platform hardware interfaces
3. Generates a VMA file containing all metadata needed for Vivado import

**Why this step is needed**: 
- **Bridges Vitis and Vivado**: VMA is the format that allows seamless transition from Vitis development to Vivado implementation
- **Complete design integration**: Links the platform-independent VSS with platform-specific hardware


**Output**: `post_link.vma` - A Vitis Metadata Archive that will be imported into Vivado in the next step

## Build

```Bash
make all
```

## Source and Configuration files

The `system.cfg` provides the connection descriptions. 

## Makefile Commands

### `make all` or `make hw_link`
**Command**: `v++ -l -t hw -s --platform <XSA> <VSS_FILE> --config system.cfg --export_archive -o post_link.vma`

**Key options**:
- `--export_archive`: Generates VMA (extensible) instead of XSA (fixed)
- `--platform <XSA>`: Platform XSA file
- `--config system.cfg`: Interface connections

**Output**: `post_link.vma`

## Key TCL/v++ Commands

**VMA generation**:
```bash
v++ -l -t hw -s --platform <XSA> <VSS_FILE> --config system.cfg --export_archive -o post_link.vma
```
- `--export_archive`: **Critical** - Generates extensible VMA (allows custom RTL) instead of fixed XSA
- VMA contains: BD files, IP configs, timing constraints, interface definitions

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>