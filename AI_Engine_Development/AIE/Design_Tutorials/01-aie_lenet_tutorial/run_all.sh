#! /bin/bash

mk="$1"

bsub -q sil_vivado -sp 1000 -R "rusage[mem=40000]" -J lenet_x1_aie_hw_emu -oo lenet_x1_aie_hw_emu.log "make $mk" &
bsub -q sil_vivado -sp 1000 -R "rusage[mem=50000]" -J lenet_x1_aie_hw -oo lenet_x1_aie_hw.log "make $mk TARGET=hw EN_TRACE=1" &
