#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#

PLATFORMDIR  ?= platform
PF_STDNAME   ?= base_pfm_vck190
PLATFORMNAME ?= $(PLATFORMDIR)/$(PF_STDNAME)/export/$(PF_STDNAME)/$(PF_STDNAME).xpfm

INCLUDES           = --include=./ --include=./inc --include=./kernels
TEST_BENCH         = ./test_ssrfft_1m.cpp
OTHER_OPTS         = --aie.constraints=./shim_loc.json --aie.xlopt=1
ROUTER_OPTS        = --aie.Xmapper=disableFloorplanning --aie.Xrouter=disablepathbalancing \
	             --aie.Xrouter=disableSSFifoSharing --aie.disable-multirate
NUM_ITERATION      = 4
TEMP_FILE          = t.txt
LOG_FILE           = log.txt

SOURCES            = inc/*.h kernels/*.cpp kernels/*.h test_ssrfft_1m.h $(TEST_BENCH)

.PHONY:	all testvectors aie_compile aie_sim aie_check_op aie_get_tp x86_compile x86_sim x86_check_op clean cleanall

all:	libadf.a

# generate test vectors for AIE simulation
testvectors:
	cd ../matlab && matlab -nodisplay -nojvm -r "test_ssrfft_1mpt($(NUM_ITERATION),0,0);quit"

libadf.a:	$(SOURCES)
	echo "#define NUM_ITERATION (32 * $(NUM_ITERATION))" > test_cfg.h ;\
	awk -f gen_shim_constrain.awk ./shim_loc.cfg > ./shim_loc.json ;\
	v++ --compile --mode aie --target=hw --aie.verbose $(INCLUDES) $(TEST_BENCH) $(OTHER_OPTS) $(ROUTER_OPTS) --platform=../$(PLATFORMNAME) |& tee -a $(LOG_FILE)

x86_compile:
	@rm -rf Work *.a ;\
	echo "#define NUM_ITERATION (32 * $(NUM_ITERATION))" > test_cfg.h ;\
	awk -f gen_shim_constrain.awk ./shim_loc.cfg > ./shim_loc.json ;\
	v++ --compile --mode aie --target=x86sim --aie.verbose $(INCLUDES) $(TEST_BENCH) $(OTHER_OPTS) $(ROUTER_OPTS) --platform=../$(PLATFORMNAME) |& tee -a $(LOG_FILE)

aie_sim:
	@rm -rf aiesimulator_output ;\
	aiesimulator --pkg-dir=Work

x86_sim:
	@rm -rf x86simulator_output ;\
	x86simulator --pkg-dir=Work

x86_check_op:
	@for p in 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 ; do \
		for q in a b ; do \
			grep -ve '[XT]' ./x86simulator_output/data/fft_mid_out_$${p}$${q}.txt > ./data/fft_mid_out_$${p}$${q}_dut.txt ;\
			grep -ve '[XT]' ./x86simulator_output/data/fft_out_$${p}$${q}.txt > ./data/fft_gold_$${p}$${q}_dut.txt ;\
		done \
	done
	cd ../matlab && matlab -nodisplay -nojvm -r "diff_output_batch({'fft_mid_out', 'fft_gold'}, 0); quit"

aie_check_op:
	@for p in 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 ; do \
		for q in a b ; do \
			grep -ve '[XT]' ./aiesimulator_output/data/fft_mid_out_$${p}$${q}.txt > ./data/fft_mid_out_$${p}$${q}_dut.txt ;\
			grep -ve '[XT]' ./aiesimulator_output/data/fft_out_$${p}$${q}.txt > ./data/fft_gold_$${p}$${q}_dut.txt ;\
		done \
	done
	cd ../matlab && matlab -nodisplay -nojvm -r "diff_output_batch({'fft_mid_out', 'fft_gold'}, 0); quit"

aie_get_tp:
	@for p in 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 ; do \
		for q in a b ; do \
			grep -ve '[XT]' aiesimulator_output/data/fft_mid_out_$${p}$${q}.txt > $(TEMP_FILE);\
			x=`wc -l $(TEMP_FILE) | awk '{print $$1}'`;\
			y=`head -1 $(TEMP_FILE) | awk '{print NF}'`;\
			grep 'T ' aiesimulator_output/data/fft_mid_out_$${p}$${q}.txt > $(TEMP_FILE);\
			t0=`sed -ne '1p;$$p' $(TEMP_FILE) | awk '(NR==1){if($$3=="ns") print $$2; else if($$3=="us") print $$2*1000; else if ($$3=="ps") print $$2/1000}'`;\
			r=`sed -ne '1p;$$p' $(TEMP_FILE) | awk '(NR==2){if($$3=="ns") print $$2; else if($$3=="us") print $$2*1000; else if ($$3=="ps") print $$2/1000}' | awk -v a=$$x -v c=$$y -v b0=$$t0 '{printf a*c/2/($$1-b0)*1000}'`;\
			echo "1Mpt SSRFFT MID-OUT $$p$$q Throughput =  $$r (>400 MSPS)";\
		done \
	done
	@for p in 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 ; do \
		for q in a b ; do \
			grep -ve '[XT]' aiesimulator_output/data/fft_out_$${p}$${q}.txt > $(TEMP_FILE);\
			x=`wc -l $(TEMP_FILE) | awk '{print $$1}'`;\
			y=`head -1 $(TEMP_FILE) | awk '{print NF}'`;\
			grep 'T ' aiesimulator_output/data/fft_out_$${p}$${q}.txt > $(TEMP_FILE);\
			t0=`sed -ne '1p;$$p' $(TEMP_FILE) | awk '(NR==1){if($$3=="ns") print $$2; else if($$3=="us") print $$2*1000; else if ($$3=="ps") print $$2/1000}'`;\
			r=`sed -ne '1p;$$p' $(TEMP_FILE) | awk '(NR==2){if($$3=="ns") print $$2; else if($$3=="us") print $$2*1000; else if ($$3=="ps") print $$2/1000}' | awk -v a=$$x -v c=$$y -v b0=$$t0 '{printf a*c/2/($$1-b0)*1000}'`;\
			echo "1Mpt SSRFFT OUT $$p$$q Throughput =  $$r (>400 MSPS)";\
		done \
	done

clean:
	/bin/rm -rf Work* *.log *.xpe xnwOut .X* .A*
	/bin/rm -rf aiesimulator_output tmp pl_sample* *.a test_cfg.h
	/bin/rm -rf $(TEMP_FILE) log* $(LOG_FILE) ISS_RPC* Map_*.csv sol.db
	/bin/rm -rf *.jou *.debug .crashReporter temp shim_loc.json
	/bin/rm -rf hw x86simulator_output plio_throughput_info.json
	/bin/rm -rf ip* .nf*
	/bin/rm -rf xnwOut vivado_* vitis_*
	/bin/rm -rf *.a .Xil *.xo *.xpe *.xdc vivado*
	/bin/rm -rf *.log *.pdi  pmc* *.cdo ./host/*.elf ./host/*.o
	/bin/rm -rf host/test_cfg.h sol.db Map_Report.csv

cleanall:	clean
	/bin/rm -rf data

