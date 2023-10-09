#! /bin/bash


bsub -q sil_vivado -sp 1000 -R "rusage[mem=40000]" -J Nbody_sim_x100_aie_hw -oo Nbody_sim_x100_aie_hw.log "make all" &
bsub -q sil_vivado -sp 1000 -R "rusage[mem=40000]" -J Nbody_sim_x1_aie_hw -oo Nbody_sim_x1_aie_hw.log "make x1_design" &
bsub -q sil_vivado -sp 1000 -R "rusage[mem=40000]" -J Nbody_sim_x10_aie_hw -oo Nbody_sim_x10_aie_hw.log "make x10_design" &

bsub -q sil_vivado -sp 1000 -R "rusage[mem=40000]" -J Nbody_sim_x100_aie_hw_emu -oo Nbody_sim_x100_aie_hw_emu.log "make all TARGET=hw_emu" &
bsub -q sil_vivado -sp 1000 -R "rusage[mem=40000]" -J Nbody_sim_x1_aie_hw_emu -oo Nbody_sim_x1_aie_hw_emu.log "make x1_design TARGET=hw_emu" &
bsub -q sil_vivado -sp 1000 -R "rusage[mem=40000]" -J Nbody_sim_x10_aie_hw_emu -oo Nbody_sim_x10_aie_hw_emu.log "make x10_design TARGET=hw_emu" &

#bsub -q sil_vivado -sp 1000 -R "rusage[mem=50000]" -J Nbody_sim_x1_aie_hw -oo Nbody_sim_x1_aie_hw.log "make $mk TARGET=hw_emu " &
