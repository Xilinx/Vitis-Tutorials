build: dataflow deadlock

dataflow: dataflow/vitis_hls.log

dataflow/vitis_hls.log:
	cd dataflow ; vitis_hls script.tcl

deadlock: deadlock/vitis_hls.log

deadlock/vitis_hls.log: 
	cd deadlock ; vitis_hls script.tcl

.PHONY: run
run: build
	@echo "Finished build and run"

check:
	@echo "Finished check"

all: build


# -----------------------------------------------------------------------------
#
clean:
	rm -rf dataflow/*~ deadlock/*~ dataflow/*.log deadlock/*.log dataflow/proj deadlock/proj
#
