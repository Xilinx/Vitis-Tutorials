<!--
Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT

Author: Faisal El-Shabani
-->

# Packet Sender (packet_sender)

## Overview

The `packet_sender` is a high-performance AMD Vitis HLS kernel designed for TDM (Time-Division Multiplexing) FIR packet switching systems. It formats continuous streams of complex samples into packetized AXI-Stream outputs suitable for AI Engine consumption.

This design operates at **128-bit data width @ 312.5 MHz** (3.2ns clock period), delivering a sustained throughput of 1.25 Giga-samples per second (Gsps) per stream (2.5 Gsps total across dual streams).

### Key Features

- **Dual-stream processing**: Processes two independent input streams (`sig_i_0`, `sig_i_1`) and generates two corresponding packetized output streams (`sig_o_0`, `sig_o_1`)
- **AI Engine packet format**: Generates AMD AI Engine packet headers with programmable routing IDs
- **High throughput**: 128-bit data path achieves 1.25 Gsps per stream (4 × cint16 samples per clock cycle @ 312.5 MHz)
- **Resource efficient**: Zero BRAM utilization (LUTRAM-based buffering), minimal logic footprint (2.5% LUT, <1% FF)
- **Dataflow architecture**: Consumer-producer pipeline with stream-of-blocks synchronization
- **Verified timing closure**: Achieved timing at 312.5 MHz with 43% positive slack (estimated Fmax 548.85 MHz)
- **Robust TKEEP handling**: Explicit TKEEP signaling (0xFFFF for valid data) prevents data loss in width converter chains

## Architecture

The `packet_sender` implements a **dataflow architecture** with two primary functional blocks:

1. **Consumer**: Reads continuous sample streams, organizes data into stream-of-blocks buffers
2. **Producer**: Reads buffered samples, formats packets with headers, writes packetized AXI-Stream outputs

```
Input Streams (128-bit AXI-Stream)
    sig_i_0 ──────┐
    sig_i_1 ──────┤
                  │
              ┌───▼────┐
              │Consumer│  (II=16, Latency=528 cycles)
              └───┬────┘
                  │
         Stream-of-Blocks Buffers
         (ss0, ss1: 512 × 128-bit LUTRAM)
                  │
              ┌───▼────┐
              │Producer│  (II=33, Latency=532 cycles)
              └───┬────┘
                  │
Output Streams (128-bit AXI-Stream with TKEEP/TLAST)
    sig_o_0 ──────┤
    sig_o_1 ──────┘
```

### Stream-of-Blocks Design

The design uses **stream-of-blocks** (`hls::stream_of_blocks`) for efficient data exchange between consumer and producer:

- **Storage**: Two buffers (`ss0`, `ss1`), each 512 words deep × 128 bits wide
- **Implementation**: LUTRAM (distributed memory), zero BRAM utilization
- **Access pattern**: Consumer writes 512 words (2048 samples), Producer reads same in packet format
- **Synchronization**: Producer blocks on acquisition until Consumer completes write

### DATAFLOW Operation

The `packet_sender_wrapper` top-level function uses `#pragma HLS DATAFLOW` to enable concurrent execution:

- **Consumer**: Processes input streams with II=16 (one iteration every 16 clock cycles)
- **Producer**: Generates output packets with II=33 (one packet every 33 clock cycles)
- **Pipeline depth**: 1061 cycles total latency, 533 cycles throughput interval
- **Overlap**: Consumer and Producer execute in parallel after pipeline fill

### Packet Ordering (Stride-8 Pattern)

The producer implements a **stride-8 access pattern** to optimize packet routing to AI Engine tiles. Packets are read from the stream-of-blocks buffers and transmitted in the following order:

**Output sequence**: 0, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6, 14, 7, 15

This pattern alternates between the lower half (packets 0-7) and upper half (packets 8-15) of the packet stream, which helps reduce latency delta between TDM AI Engine output ports by distributing packets more evenly across the processing timeline.

The stride pattern is implemented using the formula:
```cpp
unsigned int pkt_idx = (ii / 2) + ((ii % 2) * 8);
```

where `ii` is the iteration index (0-15). This produces deterministic packet ordering while maintaining the same latency and throughput as other stride configurations.

## Data Format

### Input Interface

- **Type**: AXI-Stream (AXIS) without sideband signals
- **Width**: 128 bits per beat
- **Data organization**: 4 × cint16 samples per beat (real and imaginary 16-bit components)
- **Sample packing**: `[Sample3_Im | Sample3_Re | Sample2_Im | Sample2_Re | Sample1_Im | Sample1_Re | Sample0_Im | Sample0_Re]`
- **Ordering**: Little-endian (LSBs contain first sample in time)

### Output Interface

- **Type**: AXI-Stream (AXIS) with TKEEP, TSTRB, TLAST
- **Width**: 128 bits per beat (TDATA), 16 bits per beat (TKEEP, TSTRB)
- **Sideband signals**:
  - `TKEEP`: Byte-valid indicator (0xFFFF for full words, 0x000F for partial last word)
  - `TSTRB`: Position indicator (mirrors TKEEP)
  - `TLAST`: Packet boundary marker (asserted on last beat of each packet)

### Packet Structure

Each packet consists of **33 beats** (128-bit words):

1. **Header word** (1 beat):
   - Bits [31:0]: AMD AI Engine packet header
     - Packet ID (programmable routing identifier)
     - Packet type and control fields
   - Bits [127:32]: First 3 × cint16 samples

2. **Middle data words** (31 beats):
   - Beats 2-32: Continuous cint16 samples (4 samples per beat)
   - TKEEP = 0xFFFF (all 16 bytes valid)
   - TLAST = 0

3. **Last data word** (1 beat):
   - Beat 33: Final sample(s) with padding
   - TKEEP = 0x000F (lower 4 bytes valid, upper 12 bytes padding)
   - TLAST = 1 (marks packet boundary)

**Total samples per packet**: 128 cint16 samples (1 packet + 3 samples, 31 × 4 samples, 1 sample)

**Total packets per stream**: 16 packets (configured via `PACKET_NUM` parameter)

**Total output beats per stream**: 528 words (16 packets × 33 words/packet)

## Performance

### Latency and Throughput

| Metric | Cycles | Time (μs) | Notes |
|--------|--------|-----------|-------|
| **Latency (min)** | 1061 | 3.395 | Pipeline fill time |
| **Latency (max)** | 1061 | 3.395 | Pipeline fill time |
| **Throughput Interval (min)** | 533 | 1.706 | Steady-state output rate |
| **Throughput Interval (max)** | 533 | 1.706 | Steady-state output rate |
| **Samples per Stream** | 2048 | - | Total input samples per stream |
| **Packets per Stream** | 16 | - | Total output packets per stream |
| **Sample Throughput per Stream** | 1250.0 Msps | 1.25 Gsps | 4 samples/cycle @ 312.5 MHz |
| **Total Sample Throughput** | 2500.0 Msps | 2.5 Gsps | Dual streams (2 inputs or 2 outputs) |

### Timing

| Metric | Value | Unit |
|--------|-------|------|
| **Target Clock Period** | 3.20 | ns |
| **Estimated Clock Period** | 1.822 | ns |
| **Clock Uncertainty** | 0.86 | ns |
| **Worst Negative Slack (WNS)** | +1.378 | ns |
| **Target Frequency** | 312.5 | MHz |
| **Estimated Fmax** | 548.85 | MHz |
| **Timing Margin** | +43.2% | - |

### Resource Utilization

*Target Device: AMD Versal xcve2802-vsvh1760-2MP-e-S*

| Resource | Used | Available | Utilization (%) |
|----------|------|-----------|-----------------|
| **FF** | 9,995 | 1,041,408 | <1% |
| **LUT** | 13,061 | 520,704 | 2.5% |
| **BRAM_18K** | 0 | 1,200 | 0% |
| **URAM** | 0 | 264 | 0% |
| **DSP** | 0 | 1,312 | 0% |

*Note: Zero BRAM utilization achieved through LUTRAM-based stream-of-blocks buffers (2 × 512 words × 128 bits)*

## Integration Notes

This design shall run at 312.5 MHz with 128-bit interfaces. It shall be connected to AI Engine PLIOs running at 625 with 64-bit interfaces. This results in a need for DWC & CDC.

### Width Converter (DWC) Compatibility

**CRITICAL**: When integrating with `axis_dwidth_converter` or similar width conversion IP, always set **TKEEP = 0xFFFF** for valid data words.

The AMD `axis_dwidth_converter` IP includes null detection logic:
```verilog
is_null = ~(|S_AXIS_TKEEP)  // True when TKEEP is all zeros
```

Setting TKEEP=0 (even when TLAST=0 per AXI-Stream spec) can cause the DWC to:
- Drop entire beats (interpret as null/invalid)
- Lose upper 64 bits when downsizing 128→64
- Corrupt packet boundaries

**Validated TKEEP encoding**:
- **Full words**: TKEEP = 0xFFFF (all 16 bytes valid)
- **Partial last word**: TKEEP = 0x000F (lower 4 bytes valid, matches actual data)

### AI Engine Integration

The packet header format is compatible with AMD AI Engine array routing:

- **Packet IDs**: Defined in `packet_ids_c.h` (included via hls.cfg `-I` directive)
- **Header generation**: `generateHeader()` function produces 32-bit AMD packet headers
- **Arrays**: `packet_sender_packet_ids_0[]` and `packet_sender_packet_ids_1[]` contain programmable routing IDs

Ensure packet IDs match the AI Engine graph configuration for correct data routing to compute tiles.

## Build Instructions

### Prerequisites

- AMD Vitis HLS 2025.2 or later
- Valid `packet_ids_c.h` file in `../../aie/Work/temp/` (generated by AI Engine compiler)

### Synthesis

```bash
make all
```

or manually:

```bash
v++ --config hls.cfg --compile --mode hls --work_dir packet_sender_wrapper
```

### C Simulation

```bash
v++ --config hls.cfg --run --mode hls --csim --work_dir packet_sender_wrapper
```

### Test Vector Generation

Generate input/output golden vectors using MATLAB:

```bash
matlab -batch "gen_vectors"
```

This creates:
- `data/sig_i_0.txt`: Input stream 0 (even 2k samples)
- `data/sig_i_1.txt`: Input stream 1 (odd 2k samples)
- `data/sig_o_0.txt`: Golden output stream 0 (528 words with TKEEP/TLAST)
- `data/sig_o_1.txt`: Golden output stream 1 (528 words with TKEEP/TLAST)

## Parameters

| Parameter | Value | Description |
|-----------|-------|-------------|
| `NBITS` | 128 | AXI-Stream interface width (bits) |
| `NSAMPLES` | 4096 | Total samples per stream per frame |
| `PACKET_NUM` | 16 | Number of packets per output stream |
| `DEPTH` | 512 | Stream-of-blocks buffer depth (words) |
| `TT_SAMPLE` | 32 | Single cint16 sample type width (bits) |
| `TT_DATA` | 128 | AXI-Stream data type (4 × TT_SAMPLE) |

All parameters defined in `packet_sender.h`.

## Verification Status

- ✅ **C Simulation (CSIM)**: Passed - Outputs match golden vectors bit-exact
- ✅ **C Synthesis**: Passed - Timing closure with 43% margin
- ✅ **Width Converter Integration**: Validated - Correct TKEEP prevents data loss
- ✅ **Resource Check**: Passed - Zero BRAM, minimal LUT/FF usage

## References

- AMD Vitis HLS Documentation: https://docs.amd.com/r/en-US/ug1399-vitis-hls
- AMD Versal Adaptive SoC AIE-ML Architecture Manual: https://docs.amd.com/r/en-US/am020-versal-aie-ml
- AXI-Stream Protocol Specification (ARM IHI 0051A)

## License

<p class="sphinxhide" align="center"><sub>Copyright © 2023-2026 Advanced Micro Devices, Inc.</sub></p>
<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>
