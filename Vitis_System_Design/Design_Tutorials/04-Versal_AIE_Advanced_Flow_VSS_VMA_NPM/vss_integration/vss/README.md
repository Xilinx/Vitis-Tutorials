# Creating Vitis Subsystem (VSS)

**VSS is an independent, platform-agnostic module** that can be used as a standalone IP. This example demonstrates how to create a VSS with only one AI Engine graph (libadf.a). No PL kernels are added, and **no platform dependency is required** for VSS build.

## Flow Overview

This step is the **first step** in the VSS→VMA→NPM integration flow. It packages the AIE design into a reusable Vitis Subsystem.

**Position in the flow**: 
```
AIE (libadf.a) → VSS (this step) → Platform → VMA → Vivado Import → NPM
```

**What this step does**:
1. Take the compiled AIE library (`libadf.a`) as input
2. Use v++ linker in VSS mode to create a standalone subsystem
3. Package the AIE graph into a reusable VSS IP that encapsulates the AIE design and can be used across different platforms.

**Why this step is needed**: 
- **Platform independence**: VSS allows AIE development without platform dependency. You can develop and test AIE designs before the platform is ready.
- **Reusability**: The VSS file can be used as a standalone IP in multiple projects or platforms
- **Modularity**: Separates AIE design from platform-specific details, enabling parallel development


**Key characteristic**: VSS build does **not require a platform**. It only needs the AIE library (`libadf.a`), making it truly platform-agnostic.

**Output**: `vss/vss_ss.vss` - A standalone VSS file that will be used in the link_vma step

## Build

```Bash
make all
```

## Source and Configuration files

vss.cfg

```ini
[connectivity]
vss=amd.com:vitis:vss_ss:1.0:ai_engine_0
```

CFG syntax: refer to UG1701.

## Expected outputs

```
vss
├── ip_repo
├── libadf.a
├── system.aieprj
├── vss_ss
```

## Makefile Commands

### `make all` or `make vss`
**Command**: `v++ --link --mode vss --part xcve2802-vsvh1760-2MP-e-S --config vss.cfg libadf.a`

**Key options**:
- `--mode vss`: Creates platform-independent VSS subsystem
- `--config vss.cfg`: VSS connectivity configuration

**Output**: `vss/vss_ss.vss`

## Key TCL/v++ Commands

**VSS build**:
```bash
v++ --link --mode vss --part <PART> --config vss.cfg libadf.a
```
- `--mode vss`: Creates reusable subsystem IP (platform-independent)
- `vss.cfg`: Defines VSS structure with AI Engine

<p class="sphinxhide" align="center"><sub>Copyright © 2020–2026 Advanced Micro Devices, Inc.</sub></p>

<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>