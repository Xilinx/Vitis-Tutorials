#
# Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: X11
#

XSA							:= custom_platform_vck190.xsa
VITIS_PLATFORM				:= base_pfm_vck190
WORKSPACE					:= workspace

.PHONY: clean all

help::
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) "      Command to generate the full Vitis workspace"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Command to remove all the generated files."

pfm:../vivado/build/xsa_platform/${XSA}
	if [ ! -d "${WORKSPACE}" ]; then\
		mkdir ${WORKSPACE};\
	fi
	vitis -s vitis_pfm.py ${VITIS_PLATFORM} ${XSA}
	
aie_app:
	vitis -s aie_app.py ${VITIS_PLATFORM}
	
vpp_link:./workspace/simple_aie_application/src/project.cpp
	#sed -i '9 a\#if defined(__AIESIM__) || defined(__X86SIM__) || defined(__ADF_FRONTEND__)' ./workspace/simple_aie_application/src/project.cpp;
	#sed -i '16 a\#endif' ./workspace/simple_aie_application/src/project.cpp;
	#mkdir src
	echo "part=xcvc1902-vsva2197-2MP-e-S" > src/s2mm_hls_config.cfg;
	echo "[hls]" >> src/s2mm_hls_config.cfg;
	echo "flow_target=vitis" >> src/s2mm_hls_config.cfg;
	echo "package.output.format=xo" >> src/s2mm_hls_config.cfg;
	echo "package.output.syn=false" >> src/s2mm_hls_config.cfg;
	cat src/s2mm_hls_config.cfg > src/mm2s_hls_config.cfg
	echo "syn.file=${PWD}/../../src/s2mm.cpp" >> src/s2mm_hls_config.cfg;
	echo "syn.top=s2mm" >> src/s2mm_hls_config.cfg;
	echo "syn.file=${PWD}/../../src/mm2s.cpp" >> src/mm2s_hls_config.cfg;
	echo "syn.top=mm2s" >> src/mm2s_hls_config.cfg;
	echo "[advanced]" > src/hw_link_config.cfg;
	echo "param=compiler.addOutputTypes=hw_export" >> src/hw_link_config.cfg;
	echo "" >> src/hw_link_config.cfg;
	echo "[connectivity]" >> src/hw_link_config.cfg;
	echo "stream_connect=mm2s_1.s:ai_engine_0.mygraph_in" >> src/hw_link_config.cfg;
	echo "stream_connect=ai_engine_0.mygraph_out:s2mm_1.s" >> src/hw_link_config.cfg;
	vitis -s vpp_link.py ${VITIS_PLATFORM}
	
vitis: pfm aie_app vpp_link
	

clean:
	rm -rf ${WORKSPACE} .Xil
