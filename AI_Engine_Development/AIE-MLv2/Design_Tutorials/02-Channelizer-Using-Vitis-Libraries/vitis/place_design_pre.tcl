#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani

create_pblock pblock_1
resize_pblock pblock_1 -add {SLICE_X192Y332:SLICE_X215Y375 BLI_A_GRP0_X117Y1:BLI_A_GRP0_X134Y1 BLI_A_GRP1_X117Y1:BLI_A_GRP1_X134Y1 BLI_A_GRP2_X117Y1:BLI_A_GRP2_X134Y1 BLI_B_GRP0_X117Y1:BLI_B_GRP0_X134Y1 BLI_B_GRP1_X117Y1:BLI_B_GRP1_X134Y1 BLI_B_GRP2_X117Y1:BLI_B_GRP2_X134Y1 BLI_C_GRP0_X117Y1:BLI_C_GRP0_X134Y1 BLI_C_GRP1_X117Y1:BLI_C_GRP1_X134Y1 BLI_C_GRP2_X117Y1:BLI_C_GRP2_X134Y1 BLI_D_GRP4_X117Y1:BLI_D_GRP4_X134Y1 BLI_D_GRP5_X117Y1:BLI_D_GRP5_X134Y1 BLI_D_GRP6_X117Y1:BLI_D_GRP6_X134Y1 BLI_D_GRP7_X117Y1:BLI_D_GRP7_X134Y1 DSP58_CPLX_X6Y166:DSP58_CPLX_X6Y187 DSP_X12Y166:DSP_X13Y187 IRI_QUAD_X133Y1356:IRI_QUAD_X150Y1559 RAMB18_X14Y166:RAMB18_X17Y187 RAMB36_X14Y83:RAMB36_X17Y93}
set cells1 [get_cells {vitis_design_i/ai_engine_0/inst/ai_pl_ch_5 vitis_design_i/ai_engine_0/inst/ai_pl_ch_7 vitis_design_i/ai_engine_0/inst/ai_pl_ch_9 vitis_design_i/ai_engine_0/inst/ai_pl_ch_11 vitis_design_i/ai_engine_0/inst/ai_pl_ch_13 vitis_design_i/ai_engine_0/inst/ai_pl_ch_15 vitis_design_i/ai_engine_0/inst/ai_pl_ch_17 vitis_design_i/ai_engine_0/inst/ai_pl_ch_19 vitis_design_i/ai_engine_0/inst/pl_ai_ch_3 vitis_design_i/ai_engine_0/inst/pl_ai_ch_5 vitis_design_i/ai_engine_0/inst/pl_ai_ch_7 vitis_design_i/ai_engine_0/inst/pl_ai_ch_9 vitis_design_i/ai_engine_0/inst/pl_ai_ch_11 vitis_design_i/ai_engine_0/inst/pl_ai_ch_13 vitis_design_i/ai_engine_0/inst/pl_ai_ch_15 vitis_design_i/ai_engine_0/inst/pl_ai_ch_17}]
foreach c1 $cells1 {
   add_cells_to_pblock pblock_1 [get_cells -hierarchical -filter "PRIMITIVE_TYPE !~ ADVANCED.AIE.* && NAME =~ $c1/*"]
}
set_property IS_SOFT 0 [get_pblocks pblock_1]


create_pblock pblock_2
resize_pblock pblock_2 -add {SLICE_X216Y332:SLICE_X243Y375 BLI_A_GRP0_X135Y1:BLI_A_GRP0_X149Y1 BLI_A_GRP1_X135Y1:BLI_A_GRP1_X149Y1 BLI_A_GRP2_X135Y1:BLI_A_GRP2_X149Y1 BLI_B_GRP0_X135Y1:BLI_B_GRP0_X149Y1 BLI_B_GRP1_X135Y1:BLI_B_GRP1_X149Y1 BLI_B_GRP2_X135Y1:BLI_B_GRP2_X149Y1 BLI_C_GRP0_X135Y1:BLI_C_GRP0_X149Y1 BLI_C_GRP1_X135Y1:BLI_C_GRP1_X149Y1 BLI_C_GRP2_X135Y1:BLI_C_GRP2_X149Y1 BLI_D_GRP4_X135Y1:BLI_D_GRP4_X149Y1 BLI_D_GRP5_X135Y1:BLI_D_GRP5_X149Y1 BLI_D_GRP6_X135Y1:BLI_D_GRP6_X149Y1 BLI_D_GRP7_X135Y1:BLI_D_GRP7_X149Y1 IRI_QUAD_X151Y1356:IRI_QUAD_X168Y1559 PCIE50_X1Y3:PCIE50_X1Y3 URAM288_X1Y83:URAM288_X1Y93}
set cells2 [get_cells {vitis_design_i/ai_engine_0/inst/ai_pl_ch_0 vitis_design_i/ai_engine_0/inst/ai_pl_ch_1 vitis_design_i/ai_engine_0/inst/ai_pl_ch_2 vitis_design_i/ai_engine_0/inst/ai_pl_ch_3 vitis_design_i/ai_engine_0/inst/ai_pl_ch_4 vitis_design_i/ai_engine_0/inst/ai_pl_ch_6 vitis_design_i/ai_engine_0/inst/ai_pl_ch_8 vitis_design_i/ai_engine_0/inst/ai_pl_ch_10 vitis_design_i/ai_engine_0/inst/ai_pl_ch_12 vitis_design_i/ai_engine_0/inst/ai_pl_ch_14 vitis_design_i/ai_engine_0/inst/ai_pl_ch_16 vitis_design_i/ai_engine_0/inst/ai_pl_ch_18 vitis_design_i/ai_engine_0/inst/pl_ai_ch_0 vitis_design_i/ai_engine_0/inst/pl_ai_ch_1 vitis_design_i/ai_engine_0/inst/pl_ai_ch_2 vitis_design_i/ai_engine_0/inst/pl_ai_ch_4 vitis_design_i/ai_engine_0/inst/pl_ai_ch_6 vitis_design_i/ai_engine_0/inst/pl_ai_ch_8 vitis_design_i/ai_engine_0/inst/pl_ai_ch_10 vitis_design_i/ai_engine_0/inst/pl_ai_ch_12 vitis_design_i/ai_engine_0/inst/pl_ai_ch_14 vitis_design_i/ai_engine_0/inst/pl_ai_ch_16}]
foreach c2 $cells2 {
   add_cells_to_pblock pblock_2 [get_cells -hierarchical -filter "PRIMITIVE_TYPE !~ ADVANCED.AIE.* && NAME =~ $c2/*"]
}
set_property IS_SOFT 0 [get_pblocks pblock_2]

