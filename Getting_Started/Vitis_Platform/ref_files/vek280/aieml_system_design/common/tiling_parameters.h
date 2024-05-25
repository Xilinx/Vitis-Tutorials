/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#pragma once

#include "adf.h"
#include "system_settings.h"

adf::tiling_parameters WriteAns_pattern = {
    .buffer_dimension={A_COLS,A_ROWS},
    .tiling_dimension={A_COLS,1},
    .offset={0,0},
    .tile_traversal={
        {.dimension=1, .stride=1, .wrap=A_ROWS}
    }
};

#if OPTIMIZED_SOURCE==0
adf::tiling_parameters ReadAns_pattern = {
    .buffer_dimension={A_COLS,A_ROWS},
    .tiling_dimension={ATILES_COLS,ATILES_ROWS},
    .offset={0,0},
    .tile_traversal={
        {.dimension=0, .stride=ATILES_COLS, .wrap=A_COLS/ATILES_COLS},
        {.dimension=1, .stride=ATILES_ROWS, .wrap=A_ROWS/ATILES_ROWS}
    }
};
#elif OPTIMIZED_SOURCE==1
adf::tiling_parameters ReadAns_pattern = {
    .buffer_dimension={A_COLS,A_ROWS},
    .tiling_dimension={ATILES_COLS,ATILES_ROWS*2},
    .offset={0,0},
    .tile_traversal={
        {.dimension=0, .stride=ATILES_COLS, .wrap=A_COLS/ATILES_COLS},
        {.dimension=1, .stride=ATILES_ROWS*2, .wrap=A_ROWS/ATILES_ROWS/2}
    }
};
#endif

adf::tiling_parameters WriteBns_pattern = {
    .buffer_dimension={B_COLS,B_ROWS},
    .tiling_dimension={B_COLS,1},
    .offset={0,0},
    .tile_traversal={
        {.dimension=1, .stride=1, .wrap=B_ROWS}
    }
};

adf::tiling_parameters ReadBns_pattern = {
    .buffer_dimension={B_COLS,B_ROWS},
    .tiling_dimension={BTILES_COLS,BTILES_ROWS},
    .offset={0,0},
    .tile_traversal={
        {.dimension=1, .stride=BTILES_ROWS, .wrap=B_ROWS/BTILES_ROWS},
        {.dimension=0, .stride=BTILES_COLS, .wrap=B_COLS/BTILES_COLS}
    }
};


#if OPTIMIZED_SOURCE==0
adf::tiling_parameters WriteCns_pattern = {
    .buffer_dimension={C_COLS,C_ROWS},
    .tiling_dimension={CTILES_COLS,CTILES_ROWS},
    .offset={0,0},
    .tile_traversal={
        {.dimension=0, .stride=CTILES_COLS, .wrap=C_COLS/CTILES_COLS},
        {.dimension=1, .stride=CTILES_ROWS, .wrap=C_ROWS/CTILES_ROWS}
    }
};
#elif OPTIMIZED_SOURCE==1

adf::tiling_parameters WriteCns_pattern = {
    .buffer_dimension={C_COLS,C_ROWS},
    .tiling_dimension={CTILES_COLS,CTILES_ROWS*2},
    .offset={0,0},
    .tile_traversal={
        {.dimension=0, .stride=CTILES_COLS, .wrap=C_COLS/CTILES_COLS},
        {.dimension=1, .stride=2*CTILES_ROWS, .wrap=C_ROWS/CTILES_ROWS/2}
    }
};
#endif

adf::tiling_parameters ReadCns_pattern = {
    .buffer_dimension={C_COLS,C_ROWS},
    .tiling_dimension={C_COLS,1},
    .offset={0,0},
    .tile_traversal={
        {.dimension=1, .stride=1, .wrap=C_ROWS}
    }
};
std::vector<uint32> DimAin = {
    ATILES_COLS*ATILES_ROWS, // Tile size
    A_ROWS*A_COLS/ATILES_COLS/ATILES_ROWS // Total number of Tiles
};

std::vector<uint32> DimBin = {
    BTILES_COLS*BTILES_ROWS,  // Tile size
    B_ROWS*B_COLS/BTILES_COLS/BTILES_ROWS // Total number of Tiles
};

std::vector<uint32> DimCout = {
    CTILES_COLS*CTILES_ROWS,  // Tile size
    C_ROWS*C_COLS/CTILES_COLS/CTILES_ROWS // Total number of Tiles
};


auto DummyAccessPattern = adf::buffer_descriptor(0, 0, {}, {});
