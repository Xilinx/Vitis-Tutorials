#!/usr/bin/env python3
#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani
#
# Extract throughput metrics from AIE simulation output files
# Usage: python3 extract_throughput.py <example_dir> <datatype> <plio_width>
#   datatype: float, cfloat, cint16, etc.
#   plio_width: 32, 64, 128 (PLIO width in bits)

import sys
import os
import csv
import re
from pathlib import Path

def get_samples_per_line(datatype, plio_width):
    """
    Calculate how many samples are represented per line in PLIO output.
    
    Args:
        datatype: Data type (float, cfloat, cint16, etc.)
        plio_width: PLIO width in bits (32, 64, 128)
    
    Returns:
        Number of samples per line
    """
    # Define bit widths for different data types
    datatype_bits = {
        'float': 32,
        'cfloat': 64,  # complex float = 2 x 32-bit
        'int32': 32,
        'cint32': 64,  # complex int32 = 2 x 32-bit
        'int16': 16,
        'cint16': 32,  # complex int16 = 2 x 16-bit
    }
    
    if datatype not in datatype_bits:
        print(f"WARNING: Unknown datatype '{datatype}', assuming 32 bits")
        bits_per_sample = 32
    else:
        bits_per_sample = datatype_bits[datatype]
    
    samples_per_line = plio_width // bits_per_sample
    
    if samples_per_line < 1:
        samples_per_line = 1
        print(f"WARNING: PLIO width ({plio_width}) < sample width ({bits_per_sample}), assuming 1 sample per line")
    
    return samples_per_line

def parse_output_file(filepath, samples_per_line):
    """
    Parse AIE output file to extract TLAST timestamps and sample counts.
    
    Args:
        filepath: Path to output file
        samples_per_line: Number of samples per data line (based on datatype and PLIO width)
    
    Returns:
        dict with keys: 'tlast_timestamps', 'samples_between_tlast', 'first_timestamp'
    """
    tlast_timestamps = []
    first_timestamp = None
    samples_between = []
    current_samples = 0
    
    with open(filepath, 'r') as f:
        lines = f.readlines()
    
    i = 0
    while i < len(lines):
        line = lines[i].strip()
        
        # Check for data lines (non-timestamp, non-TLAST lines)
        if line and line[0] != 'T' and 'TLAST' not in line:
            # This is a data line, count samples
            current_samples += samples_per_line
        
        # Check for timestamp lines (format: "T 13678400 ps" or "T 13680 ns")
        elif line and line[0] == 'T':
            tokens = line.split()
            if len(tokens) >= 3:
                time_value = float(tokens[1])
                time_unit = tokens[2]
                
                # Convert to picoseconds
                if time_unit == 'ns':
                    time_ps = time_value * 1000
                elif time_unit == 'ps':
                    time_ps = time_value
                elif time_unit == 'us':
                    time_ps = time_value * 1e6
                else:
                    time_ps = time_value  # Assume ps
                
                # Record first timestamp
                if first_timestamp is None:
                    first_timestamp = time_ps
                
                # Check if next line contains TLAST
                if i + 1 < len(lines):
                    next_line = lines[i + 1].strip()
                    if 'TLAST' in next_line:
                        tlast_timestamps.append(time_ps)
                        if len(tlast_timestamps) > 1:
                            samples_between.append(current_samples)
                        current_samples = 0
        
        i += 1
    
    return {
        'tlast_timestamps': tlast_timestamps,
        'samples_between_tlast': samples_between,
        'first_timestamp': first_timestamp
    }

def calculate_throughput(parsed_data):
    """
    Calculate update rate and throughput from parsed data.
    
    Returns:
        dict with keys: 'update_rate_khz', 'throughput_msps', 'samples_per_iteration'
    """
    tlast_timestamps = parsed_data['tlast_timestamps']
    samples_between = parsed_data['samples_between_tlast']
    first_timestamp = parsed_data['first_timestamp']
    
    if not tlast_timestamps:
        return None
    
    # Determine samples per iteration
    if samples_between:
        # Use samples between last two TLAST (steady-state)
        samples_per_iteration = samples_between[-1]
    else:
        samples_per_iteration = 0
    
    # Calculate time difference
    if len(tlast_timestamps) >= 2:
        # Steady-state: time between last two TLAST
        time_diff_ps = tlast_timestamps[-1] - tlast_timestamps[-2]
    elif first_timestamp is not None:
        # Single iteration: time from first timestamp to TLAST
        time_diff_ps = tlast_timestamps[0] - first_timestamp
    else:
        return None
    
    if time_diff_ps <= 0:
        return None
    
    # Convert to seconds
    time_diff_s = time_diff_ps * 1e-12
    
    # Calculate update rate (iterations per second)
    update_rate_hz = 1.0 / time_diff_s
    update_rate_khz = update_rate_hz / 1e3
    
    # Calculate throughput (samples per second)
    if samples_per_iteration > 0:
        throughput_sps = update_rate_hz * samples_per_iteration
        throughput_msps = throughput_sps / 1e6
    else:
        throughput_msps = 0.0
    
    return {
        'update_rate_khz': update_rate_khz,
        'throughput_msps': throughput_msps,
        'samples_per_iteration': samples_per_iteration
    }

def main():
    if len(sys.argv) < 4:
        print("ERROR: Missing required arguments")
        print("Usage: python3 extract_throughput.py <example_dir> <datatype> <plio_width>")
        print("  example_dir: single_tile or multi_tile")
        print("  datatype: float, cfloat, cint16, etc.")
        print("  plio_width: 32, 64, 128 (PLIO width in bits)")
        sys.exit(1)
    
    example_dir = sys.argv[1]
    datatype = sys.argv[2]
    plio_width = int(sys.argv[3])
    
    # Calculate samples per line based on datatype and PLIO width
    samples_per_line = get_samples_per_line(datatype, plio_width)
    print(f"Configuration: datatype={datatype}, plio_width={plio_width} bits, samples_per_line={samples_per_line}")
    
    # Path to aiesimulator output data directory
    # Handle both relative path (when called from utility_scripts) and direct path
    if example_dir == '.':
        data_dir = Path("aiesimulator_output/data")
    else:
        data_dir = Path(f"{example_dir}/aiesimulator_output/data")
        if not data_dir.exists():
            data_dir = Path(f"../{example_dir}/aiesimulator_output/data")
    
    if not data_dir.exists():
        print(f"ERROR: Data directory not found: {data_dir}")
        sys.exit(1)
    
    # Find all .txt files in the data directory
    output_files = sorted(data_dir.glob("*.txt"))
    
    if not output_files:
        print(f"ERROR: No .txt files found in {data_dir}")
        sys.exit(1)
    
    # Process each output file
    results = []
    for output_file in output_files:
        print(f"Processing: {output_file.name}")
        
        parsed_data = parse_output_file(output_file, samples_per_line)
        metrics = calculate_throughput(parsed_data)
        
        if metrics:
            results.append({
                'Output File': output_file.name,
                'Samples per Iteration': metrics['samples_per_iteration'],
                'Update Rate (KHz)': f"{metrics['update_rate_khz']:.2f}",
                'Throughput (Msps)': f"{metrics['throughput_msps']:.2f}"
            })
            print(f"  Update Rate: {metrics['update_rate_khz']:.2f} KHz")
            print(f"  Throughput: {metrics['throughput_msps']:.2f} Msps")
        else:
            print(f"  WARNING: Could not calculate throughput")
    
    # Write to CSV
    if example_dir == '.':
        csv_file = Path("aiesimulator_output/throughput_summary.csv")
    else:
        csv_file = Path(f"{example_dir}/aiesimulator_output/throughput_summary.csv")
        if not csv_file.parent.exists():
            csv_file = Path(f"../{example_dir}/aiesimulator_output/throughput_summary.csv")
    
    if results:
        with open(csv_file, 'w', newline='') as f:
            fieldnames = ['Output File', 'Samples per Iteration', 'Update Rate (KHz)', 'Throughput (Msps)']
            writer = csv.DictWriter(f, fieldnames=fieldnames, quoting=csv.QUOTE_ALL)
            writer.writeheader()
            writer.writerows(results)
        
        print(f"\nSUCCESS: Throughput summary written to {csv_file}")
        print(f"  Processed {len(results)} output file(s)")
    else:
        print(f"\nERROR: No valid throughput data to write")
        sys.exit(1)

if __name__ == "__main__":
    main()
