//
// Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Faisal El-Shabani

#include "packet_receiver.h"
#include <iostream>
#include <iomanip>

// Test configuration: 4 transforms of 4096 samples each
#define NUM_TRANSFORMS 4                                                    // Number of transforms to test
#define SAMPLES_PER_TRANSFORM SAMPLES_PER_PACKET*PACKETS_PER_PORT*NUM_INPUT_PORTS // Samples per transform
#define NUM_TEST_SAMPLES (SAMPLES_PER_TRANSFORM * NUM_TRANSFORMS)  // Total samples
#define NUM_PACKETS (NUM_TEST_SAMPLES / SAMPLES_PER_PACKET)  // Total packets

// Test mode: set to 1 for out-of-order packet testing, 0 for sequential
#define TEST_OUT_OF_ORDER 1

// Helper function to create cint32 sample
cint32_t make_cint32(sample_int_t real, sample_int_t imag) {
    cint32_t sample;
    sample.real = real;
    sample.imag = imag;
    return sample;
}

// ============================================================================
// 128-BIT PACKING HELPERS
// ============================================================================
// DWC pairs consecutive 64-bit words into 128-bit words
// 128-bit word format: [upper 64 bits | lower 64 bits]
//
// For header word (first word of packet):
//   Lower 64b: [header(31:0) | real0(63:32)]
//   Upper 64b: [imag0(31:0) | real1(63:32)]
//
// For middle words:
//   Lower 64b: [imag_n(31:0) | real_n+1(63:32)]
//   Upper 64b: [imag_n+1(31:0) | real_n+2(63:32)]
//
// For TLAST word (last word contains cross-packet boundary!):
//   Lower 64b: [imag127(31:0) | unused(63:32)]
//   Upper 64b: [next_header(31:0) | next_real0(63:32)]
// ============================================================================

// Helper: Pack header + first two samples into 128-bit word
data_t pack_header_128b(header_t header, sample_int_t real0, sample_int_t imag0, sample_int_t real1) {
    data_128b_t word = {0};
    // Lower 64 bits: [header | real0]
    word.raw(31, 0) = header;
    word.raw(63, 32) = real0;
    // Upper 64 bits: [imag0 | real1]
    word.raw(95, 64) = imag0;
    word.raw(127, 96) = real1;
    return word.raw;
}

// Helper: Pack two middle sample pairs into 128-bit word
data_t pack_samples_128b(sample_int_t imag_n, sample_int_t real_n1, sample_int_t imag_n1, sample_int_t real_n2) {
    data_128b_t word = {0};
    // Lower 64 bits: [imag_n | real_n+1]
    word.raw(31, 0) = imag_n;
    word.raw(63, 32) = real_n1;
    // Upper 64 bits: [imag_n+1 | real_n+2]
    word.raw(95, 64) = imag_n1;
    word.raw(127, 96) = real_n2;
    return word.raw;
}

// Helper: Pack TLAST word with cross-packet boundary
// Lower 64b: [imag127 | unused]
// Upper 64b: [next_header | next_real0]
data_t pack_tlast_128b(sample_int_t imag127, header_t next_header, sample_int_t next_real0) {
    data_128b_t word = {0};
    // Lower 64 bits: [imag127 | unused]
    word.raw(31, 0) = imag127;
    word.raw(63, 32) = 0;  // Unused
    // Upper 64 bits: [next_header | next_real0]
    word.raw(95, 64) = next_header;
    word.raw(127, 96) = next_real0;
    return word.raw;
}

int main() {
    hls::stream<data_t> in_port[NUM_INPUT_PORTS];
    hls::stream<data_t> out_port[NUM_OUTPUT_PORTS];
    
    int errors = 0;
    
    std::cout << "========================================" << std::endl;
    std::cout << "Packet Receiver Testbench (128-bit @ 312.5 MHz, cint32)" << std::endl;
    std::cout << "Test Mode: " << (TEST_OUT_OF_ORDER ? "OUT-OF-ORDER" : "SEQUENTIAL") << " packet delivery" << std::endl;
    std::cout << "Multi-Transform Test: " << NUM_TRANSFORMS << " transforms" << std::endl;
    std::cout << "cint32 = 32-bit real + 32-bit imag = 64 bits total" << std::endl;
    std::cout << "128-bit interface: 2 samples per word (or header+sample)" << std::endl;
    std::cout << "Packet format (128-bit words):" << std::endl;
    std::cout << "  Word 0: [127:96]=real(1), [95:64]=imag(0), [63:32]=real(0), [31:0]=header" << std::endl;
    std::cout << "  Word 1: [127:96]=real(3), [95:64]=imag(2), [63:32]=real(2), [31:0]=imag(1)" << std::endl;
    std::cout << "  ..." << std::endl;
    std::cout << "  Word 64 (TLAST): [127:96]=next_real(0), [95:64]=next_header, [63:32]=unused, [31:0]=imag(127)" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // Test: Multiple transforms, each with 4096 samples
    // Total: 16,384 samples = 128 packets of 128 samples each
    // Distribution: 4 packets per port per transform (8 ports)
    // Each transform is processed by one DUT call
    std::cout << "\nTest: " << NUM_TRANSFORMS << " transforms of " << SAMPLES_PER_TRANSFORM 
              << " samples each = " << NUM_TEST_SAMPLES << " total samples" << std::endl;
    std::cout << "Total packets: " << NUM_PACKETS 
              << " (" << PACKETS_PER_TRANSFORM << " per transform, " 
              << PACKETS_PER_PORT << " packets per port per transform)" << std::endl;
    std::cout << "Each packet has " << SAMPLES_PER_PACKET << " samples" << std::endl;
    std::cout << "Data organization: COLUMN-MAJOR (32x128 matrix per transform)" << std::endl;
    std::cout << "Packets sent CONSECUTIVELY per port (0-3→port0, 4-7→port1, etc.)" << std::endl;
    
    // Process all transforms
    std::cout << "\nProcessing " << NUM_TRANSFORMS << " transforms..." << std::endl;
    
    // Out-of-order packet ID mapping: permutation of [0, 1, 2, 3, 4, 5, 6, 7]
    // In out-of-order mode, packets are sent with shuffled IDs
    // Example: ID sequence [2, 0, 3, 1, 6, 4, 7, 5] instead of [0, 1, 2, 3, 4, 5, 6, 7]
    int packet_id_order[PACKETS_PER_PORT];
    if (TEST_OUT_OF_ORDER) {
        // Out-of-order permutation: send packets in shuffled ID order
        packet_id_order[0] = 2;  // First packet sent has ID=2
        packet_id_order[1] = 0;  // Second packet sent has ID=0
        packet_id_order[2] = 3;  // Third packet sent has ID=3
        packet_id_order[3] = 1;  // Fourth packet sent has ID=1
        packet_id_order[4] = 6;  // Fifth packet sent has ID=6
        packet_id_order[5] = 4;  // Sixth packet sent has ID=4
        packet_id_order[6] = 7;  // Seventh packet sent has ID=7
        packet_id_order[7] = 5;  // Eighth packet sent has ID=5
        std::cout << "  Out-of-order packet ID sequence: [" 
                  << packet_id_order[0] << ", " 
                  << packet_id_order[1] << ", " 
                  << packet_id_order[2] << ", " 
                  << packet_id_order[3] << ", "
                  << packet_id_order[4] << ", "
                  << packet_id_order[5] << ", "
                  << packet_id_order[6] << ", "
                  << packet_id_order[7] << "]" << std::endl;
    } else {
        // Sequential order: [0, 1, 2, 3, 4, 5, 6, 7]
        for (int i = 0; i < PACKETS_PER_PORT; i++) {
            packet_id_order[i] = i;
        }
        std::cout << "  Sequential packet ID sequence: [0, 1, 2, 3, 4, 5, 6, 7]" << std::endl;
    }
    
    for (int transform = 0; transform < NUM_TRANSFORMS; transform++) {
        std::cout << "  Transform " << (transform + 1) << "/" << NUM_TRANSFORMS << "..." << std::endl;
        
        // Send packets for this transform distributed across all ports in round-robin fashion
        // Column-major ordering: Packet N contains samples N, N+32, N+64, ..., N+4064
        // Packet ID is LOCAL to each input port (0-3), encoded in bits [4:0] of the header
        int base_sample = transform * SAMPLES_PER_TRANSFORM;
        
        for (int pkt = 0; pkt < PACKETS_PER_TRANSFORM; pkt++) {
            int port = pkt / PACKETS_PER_PORT;  // Consecutive packets per port: 0-7→port0, 8-15→port1, etc.
            int send_order = pkt % PACKETS_PER_PORT;  // Order in which packet is sent (0-7)
            
            // Get the actual packet ID from the permutation array
            // In out-of-order mode, this differs from send_order
            int local_pkt_id = packet_id_order[send_order];
            
            // CRITICAL: Data content must match the packet ID, not the send order
            // We're sending packet with ID=local_pkt_id, which is the local_pkt_id-th packet
            // for this port in the column-major layout
            int actual_pkt_index = port * PACKETS_PER_PORT + local_pkt_id;
            
            // First sample in this packet (column-major: sample index = actual packet index)
            int first_sample_idx = base_sample + actual_pkt_index;
            
            // Print detailed input for all ports
            std::cout << "\n    [PORT " << port << " INPUT] Packet sent #" << send_order 
                      << " (ID=" << local_pkt_id << ", actual pkt_index=" << actual_pkt_index << "):" << std::endl;
            std::cout << "      Header: packet_id=" << local_pkt_id << std::endl;
            
            // ========================================================================
            // 128-BIT PACKET GENERATION
            // ========================================================================
            // Each packet has 128 samples = 65 128-bit words (including TLAST with cross-packet boundary)
            // Word 0: [header | real0 | imag0 | real1]
            // Word 1-63: [imag_n | real_n+1 | imag_n+1 | real_n+2]
            // Word 64 (TLAST): [imag127 | unused | next_header | next_real0]
            
            // First 128-bit word: header + first two samples
            data_t first_word;
            header_t pkt_header = local_pkt_id;  // Packet ID is LOCAL (0-7)
            sample_int_t real0 = first_sample_idx;
            sample_int_t imag0 = 0;  // Simplified: imag = 0
            sample_int_t real1 = base_sample + actual_pkt_index + PACKETS_PER_TRANSFORM;  // Column-major: +32
            first_word = pack_header_128b(pkt_header, real0, imag0, real1);
            in_port[port].write(first_word);
            
            std::cout << "      Word 0 [128b]: header=" << local_pkt_id 
                      << ", sample0=(" << real0 << ",0), sample1=(" << real1 << ",0)" << std::endl;
            
            // Middle words: pairs of samples (sample 2-126, which is 63 words)
            // Word i contains samples (2*i) and (2*i+1):
            //   [imag(2i-1) | real(2i) | imag(2i) | real(2i+1)]
            for (int i = 1; i < 64; i++) {  // Words 1-63
                int sample_n = i * 2;  // Even samples: 2, 4, 6, ..., 126
                sample_int_t imag_prev = 0;  // imag of previous sample (n-1)
                sample_int_t real_n = base_sample + actual_pkt_index + sample_n * PACKETS_PER_TRANSFORM;
                sample_int_t imag_n = 0;
                sample_int_t real_n1 = base_sample + actual_pkt_index + (sample_n + 1) * PACKETS_PER_TRANSFORM;
                
                data_t mid_word = pack_samples_128b(imag_prev, real_n, imag_n, real_n1);
                in_port[port].write(mid_word);
                
                // Print first two and last two words
                if (i < 2 || i >= 62) {
                    std::cout << "      Word " << i << " [128b]: sample" << sample_n 
                              << "=(" << real_n << ",0), sample" << (sample_n+1) 
                              << "=(" << real_n1 << ",0)" << std::endl;
                } else if (i == 2) {
                    std::cout << "      ... (words 2-61 omitted) ..." << std::endl;
                }
            }
            
            // TLAST word (word 64): [imag127 | unused | next_header | next_real0]
            // This word contains the end of the current packet AND the start of the next packet!
            sample_int_t imag127 = 0;  // Last sample's imag
            
            // Determine next packet's header and first sample
            header_t next_header;
            sample_int_t next_real0;
            
            if (send_order < PACKETS_PER_PORT - 1) {
                // Next packet exists in this transform
                int next_send_order = send_order + 1;
                int next_local_pkt_id = packet_id_order[next_send_order];
                int next_actual_pkt_index = port * PACKETS_PER_PORT + next_local_pkt_id;
                next_header = next_local_pkt_id;
                next_real0 = base_sample + next_actual_pkt_index;
            } else {
                // Last packet of this transform for this port
                // Next packet will be from next transform (or end of test)
                if (transform < NUM_TRANSFORMS - 1) {
                    // Next transform exists
                    int next_transform_base = (transform + 1) * SAMPLES_PER_TRANSFORM;
                    int next_local_pkt_id = packet_id_order[0];  // First packet of next transform
                    int next_actual_pkt_index = port * PACKETS_PER_PORT + next_local_pkt_id;
                    next_header = next_local_pkt_id;
                    next_real0 = next_transform_base + next_actual_pkt_index;
                } else {
                    // Very last packet - use dummy values
                    next_header = 0;
                    next_real0 = 0xDEADBEEF;  // Marker for debugging
                }
            }
            
            data_t tlast_word = pack_tlast_128b(imag127, next_header, next_real0);
            in_port[port].write(tlast_word);
            
            std::cout << "      Word 64 [TLAST]: imag127=0, next_header=" << next_header 
                      << ", next_real0=" << next_real0 << std::endl;
        }
        
        // Run the DUT once per transform
        packet_receiver_wrapper(in_port, out_port);
    }
    
    std::cout << "All transforms processed." << std::endl;
    
    // Check outputs - packets routed based on GLOBAL packet ID modulo 8
    // Output port N receives packets: N, N+8, N+16, N+24
    std::cout << "\nVerifying outputs..." << std::endl;
    std::cout << "Expected: Output port N receives packets N, N+8, N+16, N+24" << std::endl;
    
    int total_samples_out = 0;
    for (int port = 0; port < NUM_OUTPUT_PORTS; port++) {
        std::cout << "\n[PORT " << port << " OUTPUT] Detailed sample listing:" << std::endl;
        
        int sample_count = 0;
        int port_errors = 0;
        
        // Print first two and last two samples per packet
        std::cout << "  Samples (first 2 and last 2 per packet):" << std::endl;
        
        while (!out_port[port].empty()) {
            data_t out_data = out_port[port].read();
            data_128b_t word = {0};
            word.raw = out_data;
            
            // Extract TWO cint32 samples from 128-bit word
            // Word format: [sample1.imag | sample1.real | sample0.imag | sample0.real]
            cint32_t sample0, sample1;
            sample0.real = word.raw(31, 0);
            sample0.imag = word.raw(63, 32);
            sample1.real = word.raw(95, 64);
            sample1.imag = word.raw(127, 96);
            
            // Process both samples
            for (int s = 0; s < 2; s++) {
                cint32_t sample = (s == 0) ? sample0 : sample1;
                
                // Print first two and last two samples with packet boundaries
                int sample_in_packet = sample_count % SAMPLES_PER_PACKET;
                int packet_num = sample_count / SAMPLES_PER_PACKET;
                
                if (sample_in_packet == 0) {
                    std::cout << "    Packet " << packet_num << " (samples " << sample_count 
                              << "-" << (sample_count + SAMPLES_PER_PACKET - 1) << "):" << std::endl;
                }
                
                if (sample_in_packet < 2 || sample_in_packet >= SAMPLES_PER_PACKET - 2) {
                    std::cout << "      Sample " << sample_count << ": (" << sample.real << ", " << sample.imag << ")" << std::endl;
                } else if (sample_in_packet == 2) {
                    std::cout << "      ... (samples " << sample_count << "-" 
                              << (sample_count + SAMPLES_PER_PACKET - 5) << " omitted) ..." << std::endl;
                }
                
                // Verify packet routing based on GLOBAL PACKET ID modulo 8
                // Output port N receives packets: N, N+8, N+16, N+24
                // For multi-transform test:
                // - Total samples per port = NUM_TRANSFORMS * SAMPLES_PER_TRANSFORM / NUM_OUTPUT_PORTS
                // - Each transform contributes SAMPLES_PER_TRANSFORM / NUM_OUTPUT_PORTS samples per port
                
                int transform_idx = sample_count / (SAMPLES_PER_TRANSFORM / NUM_OUTPUT_PORTS);
                int sample_in_transform = sample_count % (SAMPLES_PER_TRANSFORM / NUM_OUTPUT_PORTS);
                
                if (transform_idx < NUM_TRANSFORMS) {
                    int base_sample = transform_idx * SAMPLES_PER_TRANSFORM;
                    
                    // NEW INTERLEAVED PATTERN:
                    // Output port N, sample i comes from block (N + (i%4)*8), sample index (i/4)
                    // i%4 cycles through: 0, 1, 2, 3, 0, 1, 2, 3, ... → blocks offset by 0, 8, 16, 24
                    // i/4 gives the sample index within each block: 0, 0, 0, 0, 1, 1, 1, 1, ...
                    int block_set = sample_in_transform % PACKETS_PER_OUTPUT_PORT;  // 0, 1, 2, 3
                    int sample_idx_in_block = sample_in_transform / PACKETS_PER_OUTPUT_PORT;  // Which sample in the block
                    int global_packet_id = port + block_set * NUM_OUTPUT_PORTS;  // Port N uses blocks N, N+8, N+16, N+24
                    
                    // Column-major: sample index = base + global_packet_id + sample_position * PACKETS_PER_TRANSFORM
                    int expected_real = base_sample + global_packet_id + sample_idx_in_block * PACKETS_PER_TRANSFORM;
                    int expected_imag = 0;  // Simplified: imag = 0
                    
                    if (sample.real != expected_real || sample.imag != expected_imag) {
                        if (port_errors < 5) {  // Only show first 5 errors per port
                            std::cout << "\n  [ERROR at sample " << sample_count 
                                      << " (transform " << transform_idx << ", local sample " << sample_in_transform << "): got (" 
                                      << sample.real << "," << sample.imag 
                                      << "), expected (" << expected_real << "," << expected_imag << ")] ";
                        }
                        port_errors++;
                        errors++;
                    }
                }
                
                sample_count++;
            }
        }
        
        std::cout << "  Total samples on port " << port << ": " << sample_count;
        int expected_count = NUM_TRANSFORMS * (SAMPLES_PER_TRANSFORM / NUM_OUTPUT_PORTS);  // 4 * 512 = 2048
        std::cout << " (Expected: " << expected_count << ")";
        if (port_errors > 0) {
            std::cout << " - " << port_errors << " ERRORS!";
        }
        std::cout << std::endl;
        
        total_samples_out += sample_count;
        
        // Verify expected count
        if (sample_count != expected_count) {
            std::cout << "  [ERROR: Sample count mismatch!]" << std::endl;
            errors++;
        }
    }
    
    std::cout << "\nTotal output samples: " << total_samples_out << " (Expected: " << NUM_TEST_SAMPLES << ")" << std::endl;
    

    
    // Summary
    std::cout << "\n========================================" << std::endl;
    if (errors == 0) {
        std::cout << "TEST PASSED" << std::endl;
    } else {
        std::cout << "TEST FAILED with " << errors << " errors" << std::endl;
    }
    std::cout << "========================================" << std::endl;
    
    return errors;
}
