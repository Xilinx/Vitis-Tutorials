
SRCDIR := .
NUM_DOCS := 100000

include common.mk

build: host

run: clean host
	./host $(NUM_DOCS)

run_gprof: clean host_gprof
	./host $(NUM_DOCS)

run_fpga:
	make --no-print-directory -C ../makefile run STEP=sw_overlap ITER=8 SOLUTION=1

help:
	@echo  " "
	@echo  " Makefile Usage:"
	@echo  " "
	@echo  "  Run Part 1 - Step 1 : make run "
