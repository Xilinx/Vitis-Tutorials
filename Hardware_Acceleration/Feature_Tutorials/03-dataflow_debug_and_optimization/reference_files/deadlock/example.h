/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11 
*/
#include "hls_stream.h"

void example(hls::stream<int>& A, hls::stream<int>& B);
void proc_1(hls::stream<int>& A, hls::stream<int>& B, hls::stream<int>& C);
void proc_1_1(hls::stream<int>& A, hls::stream<int>& B, hls::stream<int>& C);
void proc_1_2(hls::stream<int>& A, hls::stream<int>& B, hls::stream<int>& C, hls::stream<int>& D);
void proc_2(hls::stream<int>& A, hls::stream<int>& B, hls::stream<int>& C);
void proc_2_1(hls::stream<int>& A, hls::stream<int>& B, hls::stream<int>& C, hls::stream<int>& D);
void proc_2_2(hls::stream<int>& A, hls::stream<int>& B, hls::stream<int>& C);
