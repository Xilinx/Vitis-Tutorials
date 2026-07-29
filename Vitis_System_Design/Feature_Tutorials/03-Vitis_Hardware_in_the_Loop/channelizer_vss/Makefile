#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Richard Buz
#
# ============================================================
# Makefile: Channelizer VSS
#
# Builds the M=16, SSR=8, P/Q=8/7 channelizer subsystem:
#   - 3 HLS PL kernels  (permute_i, permute_o, cyclic_shift)
#   - 2 AIE sub-graphs  (polyphase filterbank + DFT)
#
# Target platform: xcvc1902-vsva2197-2MP-e-S (VCK190)
#
# Usage:
#   make all     # HLS + AIE compile then VSS link
#   make hls     # HLS kernels only
#   make aie     # AIE graph only
#   make link    # link step (requires hls + aie first)
#   make clean
#
# Required environment:
#   source <Vitis_install>/settings64.sh
# ============================================================

# Propagate v++ / vivado exit codes through "| tee" log pipes
SHELL        := bash
.SHELLFLAGS  := -o pipefail -c

# ---------- Tools ----------
VPP    := v++

# ---------- Source paths ----------
HLS_SRC := ../channelizer_src/hls
AIE_SRC := ../channelizer_src/aie
AIE_TOP := channelizer_aie_top.cpp

# ---------- Output artefacts ----------
# v++ -c --mode hls places the .xo inside the work directory
XO_PERMUTE_I    := m16_ssr8_permute_fb_i_wrapper/m16_ssr8_permute_fb_i_wrapper.xo
XO_PERMUTE_O    := m16_ssr8_permute_fb_o_wrapper/m16_ssr8_permute_fb_o_wrapper.xo
XO_CYCLIC_SHIFT := m16_ssr8_cyclic_shift_wrapper/m16_ssr8_cyclic_shift_wrapper.xo
LIBADF          := libadf.a
OUTPUT_DIR      := vss_out

# ---------- VSS params config ----------
VSS_CFG := system.cfg

# ===========================================================
.PHONY: all hls aie link clean

all: link

# -----------------------------------------------------------
# HLS kernel compilation
#
# v++ -c --mode hls reads the [hls] config block.
# syn.output.format=xo in the config causes the compiler to
# emit a Vitis Kernel Object (.xo) directly — no extra packaging step.
#
# Port naming: hls::stream arrays disaggregate as <param>_<index>
#   e.g.  sig_i[7] -> sig_i_0 .. sig_i_6
#         sig_o[8] -> sig_o_0 .. sig_o_7
# -----------------------------------------------------------

hls: $(XO_PERMUTE_I) $(XO_PERMUTE_O) $(XO_CYCLIC_SHIFT)

$(XO_PERMUTE_I): $(HLS_SRC)/m16_ssr8_permute_fb_i/m16_ssr8_permute_fb_i.cpp \
                 $(HLS_SRC)/m16_ssr8_permute_fb_i/m16_ssr8_permute_fb_i.h \
                 hls_permute_fb_i.cfg
	$(VPP) -c --mode hls \
	    --config hls_permute_fb_i.cfg \
	    --work_dir m16_ssr8_permute_fb_i_wrapper \
	    2>&1 | tee permute_fb_i_hls.log

$(XO_PERMUTE_O): $(HLS_SRC)/m16_ssr8_permute_fb_o/m16_ssr8_permute_fb_o.cpp \
                 $(HLS_SRC)/m16_ssr8_permute_fb_o/m16_ssr8_permute_fb_o.h \
                 hls_permute_fb_o.cfg
	$(VPP) -c --mode hls \
	    --config hls_permute_fb_o.cfg \
	    --work_dir m16_ssr8_permute_fb_o_wrapper \
	    2>&1 | tee permute_fb_o_hls.log

$(XO_CYCLIC_SHIFT): $(HLS_SRC)/m16_ssr8_cyclic_shift/m16_ssr8_cyclic_shift.cpp \
                    $(HLS_SRC)/m16_ssr8_cyclic_shift/m16_ssr8_cyclic_shift.h \
                    hls_cyclic_shift.cfg
	$(VPP) -c --mode hls \
	    --config hls_cyclic_shift.cfg \
	    --work_dir m16_ssr8_cyclic_shift_wrapper \
	    2>&1 | tee cyclic_shift_hls.log

# -----------------------------------------------------------
# AIE graph compilation
#
# channelizer_aie_top.cpp instantiates both sub-graphs:
#   - m16_ssr8_filterbank_graph  (PLIO: filterbank_in_1..8, filterbank_out_1..8)
#   - m16_ssr8_dft_graph         (PLIO: dft_in_1..8, dft_out_1..8)
#
# pl-freq must match the HLS clock: 200 MHz
# -----------------------------------------------------------

aie: $(LIBADF)

$(LIBADF): $(AIE_TOP) \
           $(AIE_SRC)/m16_ssr8_filterbank/m16_ssr8_filterbank_graph.h \
           $(AIE_SRC)/m16_ssr8_filterbank/m16_ssr8_filterbank_taps_init.h \
           $(AIE_SRC)/m16_ssr8_filterbank/polyphase_fir_graph.h \
           $(AIE_SRC)/m16_ssr8_filterbank/polyphase_fir.h \
           $(AIE_SRC)/m16_ssr8_filterbank/polyphase_fir.cpp \
           $(AIE_SRC)/m16_ssr8_dft/m16_ssr8_dft_graph.h \
           $(AIE_SRC)/m16_ssr8_dft/twiddle_m16_ssr8_dft.h \
           $(AIE_SRC)/m16_ssr8_dft/dft_1xN_widget_graph.h \
           $(AIE_SRC)/m16_ssr8_dft/dft_1xN_widget.h \
           $(AIE_SRC)/m16_ssr8_dft/dft_1xN_widget.cpp
	$(VPP) --compile --mode aie \
	    --part xcvc1902-vsva2197-2MP-e-S \
	    --include=$(AIE_SRC)/m16_ssr8_filterbank \
	    --include=$(AIE_SRC)/m16_ssr8_dft \
	    --aie.workdir=./Work \
	    --aie.pl-freq=200 \
	    -o $@ $(AIE_TOP) \
	    2>&1 | tee aie_compile.log

# -----------------------------------------------------------
# VSS link
# Combines the .xo kernels and libadf.a into a Vitis Subsystem IP.
# Output goes to $(OUTPUT_DIR)/; not an xclbin.
# -----------------------------------------------------------

link: $(XO_PERMUTE_I) $(XO_PERMUTE_O) $(XO_CYCLIC_SHIFT) \
      $(LIBADF) $(VSS_CFG)
	$(VPP) --link --mode vss \
	    $(LIBADF) \
	    $(XO_PERMUTE_I) $(XO_PERMUTE_O) $(XO_CYCLIC_SHIFT) \
	    --config $(VSS_CFG) \
	    --out_dir $(OUTPUT_DIR) \
	    2>&1 | tee vpp_link.log

# -----------------------------------------------------------
# Clean
# -----------------------------------------------------------

clean:
	rm -rf *.xo *.a *.ltx *.log *.jou
	rm -rf *.csv *.db
	rm -rf Work _x .Xil vitis-comp.json .crashReporter*
	rm -rf $(OUTPUT_DIR)
	rm -rf m16_ssr8_permute_fb_i_wrapper \
	       m16_ssr8_permute_fb_o_wrapper \
	       m16_ssr8_cyclic_shift_wrapper
	rm -rf analyzer_input v++.link_summary .ipcache
