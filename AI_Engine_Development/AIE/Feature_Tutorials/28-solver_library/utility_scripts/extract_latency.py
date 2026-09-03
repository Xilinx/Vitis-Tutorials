#!/usr/bin/env python3
#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani
#
# Extract AIE simulation latency metrics using Vitis Python API
# Usage: vitis -s extract_latency.py <example_path>

import vitis
import sys
import os
import csv

# Get the example path from command line argument
if len(sys.argv) < 2:
    print("ERROR: Please specify path to example directory")
    print("Usage: vitis -s extract_latency.py <example_path>")
    sys.exit(1)

example_path = sys.argv[1]

# Get the aierun_summary file path
summary_file = os.path.join(example_path, "aiesimulator_output", "default.aierun_summary")

# Check if file exists
if not os.path.exists(summary_file):
    print(f"ERROR: Summary file not found: {summary_file}")
    print("Please run AIE simulation with profiling enabled first.")
    sys.exit(1)

# Create Vitis client
client = vitis.create_client()

# Initialize vitis_analyzer summary object
try:
    summary = client.get_vitis_analyzer(summary_file)
    
    # Export latency table to temporary CSV file
    import tempfile
    temp_csv = tempfile.NamedTemporaryFile(mode='w', delete=False, suffix='.csv')
    temp_csv.close()
    
    summary.export_aiesim_latency(temp_csv.name, overwrite=True)
    
    # Process the CSV to create a summary with key metrics
    summary_csv = os.path.join(example_path, "aiesimulator_output", "latency_summary.csv")
    
    # Read the CSV and filter for PLIO ports
    key_metrics = []
    with open(temp_csv.name, 'r') as f:
        # Read CSV with proper handling of whitespace
        content = f.read()
        # Remove spaces after commas in the header line
        lines = content.split('\n')
        if lines:
            lines[0] = lines[0].replace(', ', ',')
        content = '\n'.join(lines)
        
        # Parse the cleaned CSV
        import io
        reader = csv.DictReader(io.StringIO(content))
        for row in reader:
            output_port = row['Output']
            input_port = row['Input']
            first_latency_ps = float(row['First Latency (ps)'])
            last_latency_ps = float(row['Last Latency (ps)'])
            avg_latency_ps = float(row['Average Latency (ps)'])
            
            # Only keep PLIO or mat* ports (top-level ports)
            if 'PLIO' in output_port or 'mat' in output_port:
                # Convert ps to microseconds
                first_latency_us = first_latency_ps / 1e6
                avg_latency_us = avg_latency_ps / 1e6
                last_latency_us = last_latency_ps / 1e6
                key_metrics.append({
                    'Output Port': output_port,
                    'Input Port': input_port,
                    'First Latency (us)': f"{first_latency_us:.2f}",
                    'Average Latency (us)': f"{avg_latency_us:.2f}",
                    'Last Latency (us)': f"{last_latency_us:.2f}"
                })
    
    # Write summary CSV
    with open(summary_csv, 'w', newline='') as f:
        if key_metrics:
            fieldnames = ['Output Port', 'Input Port', 'First Latency (us)', 'Average Latency (us)', 'Last Latency (us)']
            writer = csv.DictWriter(f, fieldnames=fieldnames, quoting=csv.QUOTE_ALL)
            writer.writeheader()
            writer.writerows(key_metrics)
            print(f"SUCCESS: Latency summary written to {summary_csv}")
            print(f"  Found {len(key_metrics)} PLIO latency metrics")
        else:
            # Write empty file with header
            fieldnames = ['Output Port', 'Input Port', 'First Latency (us)', 'Average Latency (us)', 'Last Latency (us)']
            writer = csv.DictWriter(f, fieldnames=fieldnames, quoting=csv.QUOTE_ALL)
            writer.writeheader()
            print(f"WARNING: No PLIO latency metrics found")
    
    # Clean up temporary file
    os.unlink(temp_csv.name)
    
except Exception as e:
    print(f"ERROR: Failed to extract latency: {e}")
    sys.exit(1)
finally:
    # Close the client connection and terminate the vitis server
    vitis.dispose()
