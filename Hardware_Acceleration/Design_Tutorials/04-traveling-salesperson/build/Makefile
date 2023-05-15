build: opt1 opt2

opt1:
        vitis_hls hls.tcl
        @echo "********************"
        @echo "Finished opt1"
        @echo "********************"

opt2: 
        vitis_hls hls_opt.tcl
        @echo "********************"
        @echo "Finished opt2"
        @echo "********************"

.PHONY: run
run: build
        @echo "Finished build and run"

check:
        @echo "Finished check"

all: build


# -----------------------------------------------------------------------------
#
clean:
        rm -rf proj*
#
