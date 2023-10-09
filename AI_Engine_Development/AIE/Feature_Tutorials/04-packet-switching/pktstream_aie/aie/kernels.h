/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#ifndef __KERNELS_H__
#define __KERNELS_H__
void aie_core1(input_pktstream *in,output_pktstream *out);
void aie_core2(input_pktstream *in,output_pktstream *out);
void aie_core3(input_pktstream *in,output_pktstream *out);
void aie_core4(input_pktstream *in,output_pktstream *out);
#endif
