#Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
#SPDX-License-Identifier: X11



foreach BLI_tile [get_tiles -filter TILE_TYPE=~BLI_?RAM_?OCF_T?_TILE] {
    foreach BLI_bel [get_bels -of $BLI_tile] {
        if {[get_site_pins -quiet -of [get_nodes -quiet -uphill -of [get_nodes -quiet -uphill -of [get_nodes -quiet -uphill -of [get_nodes -quiet -uphill -of [get_site_pins -quiet -of [get_bel_pins -quiet */D -of $BLI_bel]]]]]] -filter NAME=~*/?_T*] != ""} {
            set_property PROHIBIT TRUE $BLI_bel
        }
    }   
}
