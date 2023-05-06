# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT

SHELL := /bin/bash

PHONY = clean all help
PHONY += hls_kernels hls_clean hls_all
PHONY += vadd_zcu012 mmult_zcu012 mmult_vck190
PHONY += vadd_clean mmult_zclean mmult_vclean build_mult
.PHONY : $(PHONY)


ECHO = @echo
help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate everything for this design (both 9 Vitis HLS and 3 Vitis designs)"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the above generated files."
	$(ECHO) ""
	$(ECHO) "  make hls_all"
	$(ECHO) "      Command to run Vitis HLS only on the 9 examples."
	$(ECHO) ""
	$(ECHO) "  make hls_clean"
	$(ECHO) "      Command to remove all the HLS generated files on the 9 examples."
	$(ECHO) ""
	$(ECHO) "  make vadd_zcu102"
	$(ECHO) "      Command to build example8 VADD with Vitis flow for ZCU102."
	$(ECHO) ""
	$(ECHO) "  make vadd_clean"
	$(ECHO) "      Command to clean the Vitis project of VADD."
	$(ECHO) ""
	$(ECHO) "  make mmult_zcu102"
	$(ECHO) "      Command to build example9 MATRIX MULT with Vitis flow for ZCU102."
	$(ECHO) ""
	$(ECHO) "  make mmult_zclean"
	$(ECHO) "      Command to clean the Vitis project of MATRIX MULT for ZCU102."
	$(ECHO) ""
	$(ECHO) "  make mmult_vck190"
	$(ECHO) "      Command to build example9 MATRIX MULT with Vitis flow for VCK190."
	$(ECHO) ""
	$(ECHO) "  make mmult_vclean"
	$(ECHO) "      Command to clean the Vitis project of MATRIX MULT for VCK190."
	$(ECHO) ""

vck190_clean:	mmult_vclean
zcu102_clean:	vadd_zclean mmult_zclean
clean:		hls_clean zcu102_clean
#all:		mmult_zcu102 hls_kernels vadd_zcu102
all:		hls_kernels2
#vadd_zcu102

hls_clean:
	$(MAKE) -C ./1_fix_fir_filter    clean
	$(MAKE) -C ./2_float_fir_filter  clean
	$(MAKE) -C ./3_img_median_filter clean
	$(MAKE) -C ./4_dependency        clean
	$(MAKE) -C ./5_img_histEq        clean
	$(MAKE) -C ./6_sqrt              clean
	$(MAKE) -C ./7_atan2             clean
	$(MAKE) -C ./8_vect_add          clean
	$(MAKE) -C ./9_matrix_mult_z     clean

hls_kernels:
	$(MAKE) -C ./1_fix_fir_filter    kernels
	$(MAKE) -C ./2_float_fir_filter  kernels
	$(MAKE) -C ./3_img_median_filter kernels
	$(MAKE) -C ./4_dependency        kernels
	$(MAKE) -C ./5_img_histEq        kernels
	$(MAKE) -C ./6_sqrt              kernels
	$(MAKE) -C ./7_atan2             kernels
	$(MAKE) -C ./8_vect_add          kernels
	$(MAKE) -C ./9_matrix_mult_z     kernels

hls_kernels2:
		$(MAKE) -C ./8_vect_add          hls_kernels
		$(MAKE) -C ./9_matrix_mult_z     hls_kernels

vadd_zclean:
		$(MAKE) -C ./8_vect_add  zcu102_clean

vadd_zcu102:
		$(MAKE) -C ./8_vect_add  zcu102_all

mmult_zclean:
		$(MAKE) -C ./9_matrix_mult_z       zcu102_clean

mmult_zcu102:
		$(MAKE) -C ./9_matrix_mult_z       zcu102_all

show_dir_zcu102:
		$(MAKE) -C ./9_matrix_mult_z       show_dir

# REMEMBER THAT VCK190 NEEDS DIFFERENT ENV FROM ZCU102
mmult_vclean:
		$(MAKE) -C ./9_matrix_mult_v       vck190_clean

mmult_vck190:
		$(MAKE) -C ./9_matrix_mult_v       vck190_all

show_dir_vck190:
		$(MAKE) -C ./9_matrix_mult_v       show_dir
