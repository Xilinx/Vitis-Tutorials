//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Peifang Zhou, Bachir Berkane, Mark Rollins
//

#include "music_graph.h"

static_assert(ROW > COL,         "ROW (M) is not greater than COL (N)");
static_assert(ROW % 8 == 0,      "ROW is not a multiple of 8");
static_assert(COL % 4 == 0,      "COL is not a multiple of 4");
static_assert(COL <= 8,          "COL is greater than 8");
static_assert(ROW * COL <= 4096, "ROW * COL is greater than 4096");  // local memory size 32KB

//
// App Level Graph
//
class dut_graph : public adf::graph {
public:
  music_graph                   music;
  std::array<adf::input_plio,2> sig_i;
  adf::output_plio              sig_o;

  dut_graph( void )
  {
    // I/O:
    sig_i[0] =  adf::input_plio::create("PLIO_i_0", adf::plio_64_bits, "data/sig_i_0.txt");
    sig_i[1] =  adf::input_plio::create("PLIO_i_1", adf::plio_64_bits, "data/sig_i_1.txt");
    sig_o    = adf::output_plio::create("PLIO_o",   adf::plio_64_bits, "data/sig_o.txt");

    // Connections to block-level graph:
    adf::connect<>( sig_i[0].out[0], music.sig_i[0] );
    adf::connect<>( sig_i[1].out[0], music.sig_i[1]);
    adf::connect<>( music.sig_o,     sig_o.in[0] );

    // Placement:
    // IO Adapter kernel location constraint: ROW_0, column 11, for a total of 1 tile
    static constexpr unsigned IO_ADAPTER_COL_START = 11;
    adf::location<adf::kernel>(music.io_adapter.io_adapter_kernel) = adf::tile(IO_ADAPTER_COL_START, ROW_0);

    // QRD kernel location constraints: ROW_0, column: 12, 13, ..., 47, for a total of 36 tiles
    static constexpr unsigned COL_START = IO_ADAPTER_COL_START + 1;
    for (unsigned i = 0, col = COL_START; i < qrd_graph::NUM_QRD_KERNELS; ++i, ++col)
      adf::location<adf::kernel>(music.qrd.qrd_kernel[i]) = adf::tile(col, ROW_0);

    // SVD kernel location constraints: ROW_1, column: 47, 46, ..., 10, for a total of 38 tiles
    for (unsigned i = 0, col = COL_START + qrd_graph::NUM_QRD_KERNELS - 1; i < svd_graph::NUM_SVD_KERNELS; ++i, --col)
      adf::location<adf::kernel>(music.svd.svd_kernel[i]) = adf::tile(col, ROW_1);

    // DOA kernel location constraints: ROW_2, column: 10, 11, ..., 48, for a sub-total of 39 tiles
    static constexpr unsigned NUM_DOA_KERNELS_ON_ROW2 = 39;
    for (unsigned i = 0, col = COL_START - 2; i < NUM_DOA_KERNELS_ON_ROW2; ++i, ++col)
      adf::location<adf::kernel>(music.doa.doa_kernel[i]) = adf::tile(col, ROW_2);

    // DOA kernel location constraints: ROW_3, column: 48, 47, ..., 24, for a sub-total of 25 tiles, 39 + 25 = 64 DOA tiles
    for (unsigned i = NUM_DOA_KERNELS_ON_ROW2, col = 48; i < NUM_DOA_KERNELS; ++i, --col)
      adf::location<adf::kernel>(music.doa.doa_kernel[i]) = adf::tile(col, ROW_3);

    // Scanner kernel location constraints: ROW_3, column: 23, 22
    adf::location<adf::kernel>(music.scanner.scanner_kernel[0]) = adf::tile(23, ROW_3);
    adf::location<adf::kernel>(music.scanner.scanner_kernel[1]) = adf::tile(22, ROW_3);

    // Finder kernel location constraints:  ROW_3, column: 21, 20, ..., 9, for a sub-total of 13 tiles
    for (unsigned i = 0, col = 21; i < 13; ++i, --col)
      adf::location<adf::kernel>(music.finder.finder_kernel[i]) = adf::tile(col, ROW_3);
    // finder kernel location constraints: column 9, ROW_2, ROW_1, and down to the bottom ROW_0
    adf::location<adf::kernel>(music.finder.finder_kernel[13]) = adf::tile(9, ROW_2);
    adf::location<adf::kernel>(music.finder.finder_kernel[14]) = adf::tile(9, ROW_1);
    adf::location<adf::kernel>(music.finder.finder_kernel[15]) = adf::tile(9, ROW_0);
  }
};

// Instantiate an AIE graph:
dut_graph aie_dut;

int main(void) {
  aie_dut.init();
  aie_dut.run(4);
  aie_dut.end();

  return 0;
}
