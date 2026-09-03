#!/usr/bin/env python3
#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani
#
# Extract AIE resource utilization metrics from compilation results
# Usage: python extract_aie_resources.py <example_dir>

import json
import csv
import sys
import os
import re
import glob

def extract_compute_and_total_tiles(active_cores_path):
    """
    Extract number of compute tiles and total tiles from active_cores.json
    
    Returns:
        tuple: (num_compute, num_total)
            num_compute: Number of compute tiles (from ActiveCores field)
            num_total: Total number of AIE tiles (from ActiveMemory field)
    """
    num_compute = 0
    num_total = 0
    
    try:
        with open(active_cores_path, 'r') as f:
            data = json.load(f)
        
        # Extract compute tiles from ActiveCores field
        # Can be either a list of dictionaries or a simple list
        if 'ActiveCores' in data and data['ActiveCores']:
            if isinstance(data['ActiveCores'], list):
                if isinstance(data['ActiveCores'][0], dict):
                    # Format: [{"24_0": "/path/..."}, {"25_0": "/path/..."}]
                    num_compute = len(data['ActiveCores'])
                else:
                    # Format: [24, 25, 26]
                    num_compute = len(data['ActiveCores'])
        
        # Extract total tiles from ActiveMemory field
        # Each entry is in format "column_row"
        if 'ActiveMemory' in data and data['ActiveMemory']:
            unique_tiles = set()
            
            for mem_tile in data['ActiveMemory']:
                # Extract column and row
                parts = mem_tile.split('_')
                if len(parts) == 2:
                    tile_key = f"{parts[0]}_{parts[1]}"
                    unique_tiles.add(tile_key)
            
            num_total = len(unique_tiles)
    except Exception as e:
        print(f"Warning: Could not parse active_cores.json: {e}")
    
    return num_compute, num_total


def extract_plio_counts(mapping_report_path):
    """
    Extract number of input and output PLIOs from graph_mapping_analysis_report.txt
    
    Returns:
        tuple: (num_input_plios, num_output_plios)
    """
    num_input = 0
    num_output = 0
    
    try:
        with open(mapping_report_path, 'r') as f:
            text = f.read()
        
        # Extract the Port Mapping Report section
        port_start = text.find('Port Mapping Report:')
        mem_bank_start = text.find('Memory Bank Report:')
        
        if port_start != -1 and mem_bank_start != -1:
            port_section = text[port_start:mem_bank_start]
            
            # Count individual PLIO instances based on Dir and stream type
            # Dir=out with stream = input PLIO (data flows out from PLIO to graph)
            # Dir=in with stream = output PLIO (data flows in from graph to PLIO)
            input_plios = set()
            output_plios = set()
            
            import re
            for line in port_section.split('\n'):
                # Skip header and empty lines
                if not line.strip() or 'PortName' in line or '=' in line:
                    continue
                
                # Look for stream ports only
                if 'stream' in line:
                    # Split line into columns and look for Dir field
                    parts = line.split()
                    if len(parts) < 10:
                        continue
                    
                    # Typically: PortName Dir PrtType BufType Buffer ... GraphName
                    # Find Dir column (should be second column, but look for 'in' or 'out')
                    direction = None
                    for i, part in enumerate(parts):
                        if part == 'in' or part == 'out':
                            # Make sure it's actually the Dir column (comes after PortName)
                            if i > 0 and parts[i+1] == 'stream':
                                direction = part
                                break
                    
                    if direction is None:
                        continue
                    
                    # Extract PLIO name from graph name column (has .out[ or .in[ pattern)
                    if direction == 'out':
                        # Input PLIO: look for pattern like "in_a[0].out[0]"
                        match = re.search(r'([\w\[\]]+)\.out\[', line)
                        if match:
                            input_plios.add(match.group(1))
                    elif direction == 'in':
                        # Output PLIO: look for pattern like "out_q[0].in[0]"
                        match = re.search(r'([\w\[\]]+)\.in\[', line)
                        if match:
                            output_plios.add(match.group(1))
            
            num_input = len(input_plios)
            num_output = len(output_plios)
    except Exception as e:
        print(f"Warning: Could not extract PLIO counts: {e}")
    
    return num_input, num_output


def main():
    # Get the example directory from command line argument (single_tile or multi_tile)
    if len(sys.argv) < 2:
        print("ERROR: Please specify example directory (single_tile or multi_tile)")
        print("Usage: python extract_aie_resources.py <example_dir>")
        sys.exit(1)
    
    example_dir = sys.argv[1]
    
    # Define file paths relative to utility_scripts directory
    # Search for mapping analysis report (graph name may vary)
    reports_dir = f"../{example_dir}/Work/reports"
    mapping_report_pattern = f"{reports_dir}/*_mapping_analysis_report.txt"
    mapping_report_matches = glob.glob(mapping_report_pattern)
    
    if not mapping_report_matches:
        print(f"ERROR: No mapping analysis report found matching pattern: {mapping_report_pattern}")
        print("Please run 'make compile' first to generate compilation artifacts.")
        sys.exit(1)
    
    if len(mapping_report_matches) > 1:
        print(f"WARNING: Multiple mapping reports found: {mapping_report_matches}")
        print(f"Using first match: {mapping_report_matches[0]}")
    
    mapping_report = mapping_report_matches[0]
    active_cores_file = f"../{example_dir}/Work/aie/active_cores.json"
    output_csv = f"../{example_dir}/Work/aie_resources.csv"
    
    # Check if required files exist
    if not os.path.exists(active_cores_file):
        print(f"ERROR: Active cores file not found: {active_cores_file}")
        print("Please run 'make compile' first to generate compilation artifacts.")
        sys.exit(1)
    
    # Extract resource metrics
    num_compute, num_total = extract_compute_and_total_tiles(active_cores_file)
    num_input_plios, num_output_plios = extract_plio_counts(mapping_report)
    
    # Write results to CSV
    try:
        with open(output_csv, 'w', newline='') as f:
            writer = csv.writer(f)
            writer.writerow(['Metric', 'Value'])
            writer.writerow(['Compute Tiles', num_compute])
            writer.writerow(['Total AIE Tiles', num_total])
            writer.writerow(['Input PLIOs', num_input_plios])
            writer.writerow(['Output PLIOs', num_output_plios])
        
        print(f"SUCCESS: AIE resource metrics written to {output_csv}")
        print(f"  Compute tiles: {num_compute}")
        print(f"  Total AIE tiles: {num_total}")
        print(f"  Input PLIOs: {num_input_plios}")
        print(f"  Output PLIOs: {num_output_plios}")
    except Exception as e:
        print(f"ERROR: Failed to write CSV file: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
