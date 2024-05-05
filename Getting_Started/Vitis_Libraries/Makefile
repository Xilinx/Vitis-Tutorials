prepare:
	mkdir -p ref_files
	cp $(DSPLIB_ROOT)/L1/examples/1Dfix_impulse/src/* ref_files/.
	cp -r $(DSPLIB_ROOT)/L1/include/hw/vitis_fft/fixed ref_files/.

build: 

	vitis -s project.py

.PHONY: run
run: prepare build
	@echo "Finished build and run"

check:
	@echo "Finished check"

all: run


# -----------------------------------------------------------------------------
#
clean:
	rm -rf tmp
#
