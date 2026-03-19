#!/usr/bin/env python3
"""
Convert step2 data files (input.txt, golden.txt) to C header files
Format: each line contains "real imag" (two integers)
Output: cint16 format (real[15:0], imag[31:16])
"""

import os
import sys

def convert_txt_to_h(txt_file, h_file, array_name, max_samples=None):
    """Convert txt file to C header file"""
    input_path = os.path.join('..', '..', 'step2_vitis_integration', 'src', 'data', txt_file)
    output_path = h_file
    
    if not os.path.exists(input_path):
        print(f"Error: {input_path} not found")
        return False
    
    data = []
    with open(input_path, 'r') as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            parts = line.split()
            if len(parts) >= 2:
                real = int(parts[0])
                imag = int(parts[1])
                # Convert to cint16 format: real[15:0], imag[31:16]
                cint16_value = (imag << 16) | (real & 0xFFFF)
                data.append(cint16_value)
                if max_samples and len(data) >= max_samples:
                    break
    
    # Write header file
    with open(output_path, 'w') as f:
        f.write(f"/*\n")
        f.write(f" * {h_file} - Generated from {txt_file}\n")
        f.write(f" * Contains {len(data)} samples of cint16 data\n")
        f.write(f" * Each sample is 32-bit: real[15:0], imag[31:16]\n")
        f.write(f" */\n\n")
        f.write(f"#ifndef {array_name.upper()}_H\n")
        f.write(f"#define {array_name.upper()}_H\n\n")
        f.write(f"#include <cstdint>\n\n")
        f.write(f"#define {array_name.upper()}_SIZE {len(data)}\n\n")
        f.write(f"static const int32_t {array_name}[{array_name.upper()}_SIZE] = {{\n")
        
        # Write data in rows of 8
        for i in range(0, len(data), 8):
            row = data[i:i+8]
            values = ', '.join([f"0x{v:08X}" for v in row])
            if i + 8 < len(data):
                f.write(f"    {values},\n")
            else:
                f.write(f"    {values}\n")
        
        f.write(f"}};\n\n")
        f.write(f"#endif // {array_name.upper()}_H\n")
    
    print(f"Converted {txt_file} -> {h_file} ({len(data)} samples)")
    return True

if __name__ == '__main__':
    # Convert input.txt to input.h (use first 128 samples for 4 iterations)
    convert_txt_to_h('input.txt', 'input.h', 'input_data', max_samples=128)
    
    # Convert golden.txt to output.h (all 128 samples)
    convert_txt_to_h('golden.txt', 'output.h', 'golden', max_samples=128)
