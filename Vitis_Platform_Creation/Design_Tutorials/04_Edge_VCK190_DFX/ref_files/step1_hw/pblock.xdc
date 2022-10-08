# Copyright 2021 Xilinx Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

########################################
#
#This file works only with our platform. There will be need to review this xdc as per your resource requirements in static region.
# Creating pblock for Vitis Region

create_pblock pblock_VitisRegion
add_cells_to_pblock [get_pblocks pblock_VitisRegion] [get_cells -quiet [list design_1_i/VitisRegion]]

#Resizing the pblock as per clock regions
resize_pblock [get_pblocks pblock_VitisRegion] -add {CLOCKREGION_X0Y3:CLOCKREGION_X11Y5 CLOCKREGION_X3Y2:CLOCKREGION_X11Y2 CLOCKREGION_X4Y1:CLOCKREGION_X11Y1 CLOCKREGION_X7Y0:CLOCKREGION_X8Y0}

# Resizing the pblock and adding maximum resources to Vitis Region that is not required in static region
resize_pblock [get_pblocks pblock_VitisRegion] -add {SLICE_X116Y0:SLICE_X147Y91 SLICE_X76Y0:SLICE_X115Y187 SLICE_X60Y140:SLICE_X75Y185 SLICE_X60Y0:SLICE_X75Y138 SLICE_X50Y0:SLICE_X59Y187 SLICE_X0Y140:SLICE_X49Y187}
resize_pblock [get_pblocks pblock_VitisRegion] -add {BUFG_GT_X0Y24:BUFG_GT_X0Y47}
resize_pblock [get_pblocks pblock_VitisRegion] -add {BUFG_GT_SYNC_X0Y41:BUFG_GT_SYNC_X0Y81}
resize_pblock [get_pblocks pblock_VitisRegion] -add {DSP58_CPLX_X0Y46:DSP58_CPLX_X0Y93 DSP58_CPLX_X0Y0:DSP58_CPLX_X1Y45}
resize_pblock [get_pblocks pblock_VitisRegion] -add {DSP_X0Y46:DSP_X1Y93 DSP_X0Y0:DSP_X3Y45}
resize_pblock [get_pblocks pblock_VitisRegion] -add {GTY_QUAD_X0Y3:GTY_QUAD_X0Y3}
resize_pblock [get_pblocks pblock_VitisRegion] -add {IRI_QUAD_X47Y772:IRI_QUAD_X70Y779 IRI_QUAD_X0Y772:IRI_QUAD_X36Y779 IRI_QUAD_X0Y588:IRI_QUAD_X70Y771 IRI_QUAD_X47Y584:IRI_QUAD_X70Y587 IRI_QUAD_X36Y584:IRI_QUAD_X36Y587 IRI_QUAD_X36Y396:IRI_QUAD_X70Y583 IRI_QUAD_X36Y16:IRI_QUAD_X92Y395 IRI_QUAD_X31Y4:IRI_QUAD_X91Y15 IRI_QUAD_X31Y0:IRI_QUAD_X92Y3}
resize_pblock [get_pblocks pblock_VitisRegion] -add {NOC_NMU512_X0Y2:NOC_NMU512_X0Y3 NOC_NMU512_X0Y0:NOC_NMU512_X1Y1}
resize_pblock [get_pblocks pblock_VitisRegion] -add {NOC_NSU512_X0Y2:NOC_NSU512_X0Y3 NOC_NSU512_X0Y0:NOC_NSU512_X1Y1}
resize_pblock [get_pblocks pblock_VitisRegion] -add {RAMB18_X3Y0:RAMB18_X4Y47 RAMB18_X2Y0:RAMB18_X2Y95 RAMB18_X1Y72:RAMB18_X1Y93 RAMB18_X1Y0:RAMB18_X1Y69 RAMB18_X0Y72:RAMB18_X0Y95}
resize_pblock [get_pblocks pblock_VitisRegion] -add {RAMB36_X3Y0:RAMB36_X4Y23 RAMB36_X2Y0:RAMB36_X2Y47 RAMB36_X1Y36:RAMB36_X1Y46 RAMB36_X1Y0:RAMB36_X1Y34 RAMB36_X0Y36:RAMB36_X0Y47}
resize_pblock [get_pblocks pblock_VitisRegion] -add {URAM288_X2Y0:URAM288_X2Y23 URAM288_X1Y36:URAM288_X1Y46 URAM288_X1Y0:URAM288_X1Y34 URAM288_X0Y36:URAM288_X0Y47}
resize_pblock [get_pblocks pblock_VitisRegion] -add {URAM_CAS_DLY_X1Y0:URAM_CAS_DLY_X2Y0 URAM_CAS_DLY_X0Y1:URAM_CAS_DLY_X0Y1}

# Removing some sites from Vitis Region as they are in above clock regions but required  by static region
resize_pblock pblock_VitisRegion -remove [get_sites -of [get_tiles DDRMC_RIU_CORE_X90Y0]]
resize_pblock pblock_VitisRegion -remove [get_sites -of_objects [get_tiles CLK_VNOC_PSS_CCA_TILE_X23Y47]]

set_property SNAPPING_MODE ON [get_pblocks pblock_VitisRegion]
set_property IS_SOFT FALSE [get_pblocks pblock_VitisRegion]


