//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <complex>
#include "packet_ids_c.h"

// Configuration parameters
#define NUM_INPUT_PORTS 4       // 4 input ports (reduced from 8)
#define NUM_OUTPUT_PORTS 8      // 8 output ports (unchanged)
#define DATA_WIDTH 128          // 128-bit interface @ 312.5 MHz (equivalent bandwidth to 64-bit @ 625 MHz)
#define HEADER_WIDTH 32         // 32-bit header
#define SAMPLES_PER_PACKET 128  // packet size in samples per packet
#define PACKETS_PER_PORT 8      // 8 packets per input port per transform
#define PACKETS_PER_TRANSFORM (NUM_INPUT_PORTS * PACKETS_PER_PORT)  // 32 total packets per transform (4 inputs × 8 packets) 
#define PACKETS_PER_OUTPUT_PORT (PACKETS_PER_TRANSFORM / NUM_OUTPUT_PORTS)  // 4 packets per output port (32 total / 8 outputs)

// Data types
typedef ap_int<32> sample_int_t;
typedef ap_uint<HEADER_WIDTH> header_t;
typedef ap_uint<DATA_WIDTH> data_t;

// Packet header structure (per AMD AI Engine packet format)
// Only bits [4:0] are used for packet ID
typedef struct {
    ap_uint<5> pkt_id;      // Bits [4:0]: Packet ID
    ap_uint<27> reserved;   // Bits [31:5]: Reserved/other fields
} pkt_header_t;

// Complex int32 (cint32 = 32-bit real + 32-bit imag = 64 bits total)
typedef struct {
    sample_int_t real;
    sample_int_t imag;
} cint32_t;

// 64-bit data containing one cint32 sample
typedef union {
    ap_uint<64> raw;
    cint32_t sample;
    struct {
        sample_int_t real;
        sample_int_t imag;
    } parts;
} data_64b_t;

// 128-bit data containing two cint32 samples or header + samples
// Layout: [127:96]=imag1/real1_hi, [95:64]=real1/imag0, [63:32]=imag0/real0_hi, [31:0]=real0/header
typedef union {
    data_t raw;  // 128-bit raw data
    struct {
        cint32_t sample0;  // Lower 64 bits [63:0]
        cint32_t sample1;  // Upper 64 bits [127:64]
    } samples;
    struct {
        header_t header;           // [31:0]: Header
        sample_int_t first_real;   // [63:32]: Real part of first sample
        sample_int_t first_imag;   // [95:64]: Imag part of first sample
        sample_int_t second_real;  // [127:96]: Real part of second sample
    } header_word;
    struct {
        ap_uint<64> lower;  // [63:0]
        ap_uint<64> upper;  // [127:64]
    } halves;
} data_128b_t;

static const unsigned int packet_ids_0[PACKETS_PER_PORT]={PLIO_o_0_0,  PLIO_o_0_1,  PLIO_o_0_2,  PLIO_o_0_3,  PLIO_o_0_4,  PLIO_o_0_5,  PLIO_o_0_6,  PLIO_o_0_7}; //macro values are generated in packet_ids_c.h
static const unsigned int packet_ids_1[PACKETS_PER_PORT]={PLIO_o_1_0,  PLIO_o_1_1,  PLIO_o_1_2,  PLIO_o_1_3,  PLIO_o_1_4,  PLIO_o_1_5,  PLIO_o_1_6,  PLIO_o_1_7}; //macro values are generated in packet_ids_c.h
static const unsigned int packet_ids_2[PACKETS_PER_PORT]={PLIO_o_2_0,  PLIO_o_2_1,  PLIO_o_2_2,  PLIO_o_2_3,  PLIO_o_2_4,  PLIO_o_2_5,  PLIO_o_2_6,  PLIO_o_2_7}; //macro values are generated in packet_ids_c.h
static const unsigned int packet_ids_3[PACKETS_PER_PORT]={PLIO_o_3_0,  PLIO_o_3_1,  PLIO_o_3_2,  PLIO_o_3_3,  PLIO_o_3_4,  PLIO_o_3_5,  PLIO_o_3_6,  PLIO_o_3_7}; //macro values are generated in packet_ids_c.h

// Function prototype
void packet_receiver_wrapper(
    hls::stream<data_t> in_port[NUM_INPUT_PORTS],
    hls::stream<data_t> out_port[NUM_OUTPUT_PORTS]
);
