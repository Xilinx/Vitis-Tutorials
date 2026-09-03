# Shared Utility Scripts

Python-based utilities for extracting performance metrics from AIE simulation results. These scripts are shared across all Cholesky and QRD designs (single-tile and multi-tile).

## Scripts

### extract_latency.py
Extracts latency metrics using Vitis Python API.

**Usage:**
```bash
vitis -s extract_latency.py <path_to_example_dir>
```

**Input:** `aiesimulator_output/default.aierun_summary`  
**Output:** `aiesimulator_output/latency_summary.csv`

**Columns:** Output Port, Input Port, First Latency (µs), Average Latency (µs), Last Latency (µs)

---

### extract_throughput.py
Calculates update rate and throughput from timestamped output files.

**Usage:**
```bash
python3 extract_throughput.py <path_to_example_dir> <datatype> <plio_width>
```

**Examples:**
- Cholesky: `extract_throughput.py . float 64`
- QRD: `extract_throughput.py . cfloat 64`

**Input:** `aiesimulator_output/data/*.txt` (TLAST timestamped output files)  
**Output:** `aiesimulator_output/throughput_summary.csv`

**Columns:** Output File, Samples per Iteration, Update Rate (KHz), Throughput (Msps)

**Supported datatypes:** `float` (32-bit), `cfloat` (64-bit), `int16` (16-bit), `cint16` (32-bit), `int32` (32-bit), `cint32` (64-bit)

---

### extract_aie_resources.py
Extracts AIE resource utilization from compilation reports.

**Usage:**
```bash
python3 extract_aie_resources.py <path_to_example_dir>
```

**Input:**
- `Work/aie/active_cores.json` (compute tiles and total tiles)
- `Work/reports/graph_mapping_analysis_report.txt` (PLIO counts)

**Output:** `Work/aie_resources.csv`

**Metrics:** Compute Tiles, Total AIE Tiles, Input PLIOs, Output PLIOs

**Note:** 
- Compute tiles extracted from `ActiveCores` field (kernels running computation)
- Total tiles extracted from `ActiveMemory` field (all tiles used)

---

## Integration

All utilities are called from Makefile targets and results are read by `verify_results.m`:

```makefile
extract-latency:
	vitis -s ../../utility_scripts/extract_latency.py .

extract-resources:
	python3 ../../utility_scripts/extract_aie_resources.py .

extract-throughput:
	python3 ../../utility_scripts/extract_throughput.py . <datatype> <plio_width>
```

## License

Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.  
SPDX-License-Identifier: MIT
