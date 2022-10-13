
base_mk_trgts="$1"
trgt="$2"
gemmpt="$3"
en_trace="$4"
iters="$5"
freq="$6"

################################################
# 1 Enable Trace Profiling Flag
# -> 0 = trace profiling is disabled.
# -> 1 = trace profiling is enabled.
# -> default = 1
# -> For all designs targeted to hw_emu,
#    profiling is by default disabled.
################################################
if [ -z "$en_trace" ]
then
   en_trace="1"
fi

#################################################
## Design Types
## -> 0 = functional dsn without trace profiling.
## -> 1 = functional dsn with trace profiling.
## -> 2 = no transpose dsn with trace profiling.
## -> 3 = no ddr dsn with trace profiling.
## -> default = 1
## -> For all designs targeted to hw_emu,
##    profiling is by default disabled.
#################################################
#if [[ $dsnType == "" ]]
#then
#   dsnType="1"
#fi
#
##Adding bsub append name for design types...
#bsub_apnd=("funcNoTrace" "func" "noTrans" "noDDR")

################################################
# Target...
# -> hw or hw_emu.
# -> default = hw_emu
################################################
if [ -z "$trgt" ]
then
   trgt=1
   trgt_loopStart=0
else
   if [[ $trgt == "hw" ]]
   then
      trgt=0
   
   else
      trgt=1
   fi
   
   trgt_loopStart=$trgt
fi

trgt_lst=("hw" "hw_emu")

################################################
# GEMM Size Designs...
# -> 0 = 32x32x32.
# -> 1 = 64x64x64. 
# -> 2 = 128x128x128.
# -> 3 = 256x256x256.
# -> 4 = 512x512x512.
# -> 5 = 1024x1024x1024
# -> default = 1 (32x32x32).
################################################
if [ -z "$gemmpt" ]
then
   gemmpt=5
   gemmpt_loopStart=0

elif [[ $gemmpt == "a" ]]
then
   gemmpt=5
   gemmpt_loopStart=0

else
   gemmpt_loopStart=$gemmpt

fi

gemm_pt=("32" "64" "128" "256" "512" "1024")


################################################
# Iteration Count...
# -> default = 16
################################################
if [ -z $iters ]
then
   iters="16"
fi

################################################
# Frequency...
# -> default = 312.5Mhz
################################################
if [ -z "$freq" ]
then
   freq="312.5"
fi

################################################
# Updating make targets based on
# above options...
################################################
if [ -z "$base_mk_trgts" ]
then
   base_mk_trgts="clean_tgt all"
fi

################################################
# Defining different mem sizes for the bsubs...
################################################

#bsub memory sizes: For increasing no. of instances...
mem=($(( 25 * 1024 )) $(( 50 * 1024 )) $(( 100 * 1024 )))
################################################
# Execute...
################################################

for ((i=$gemmpt_loopStart;i<=$gemmpt;i++))
do
   for ((k=$trgt_loopStart;k<=$trgt;k++))
   do
      if [[ ${trgt_lst[$k]} == "hw" ]]
      then
         mk_targets="$base_mk_trgts EN_TRACE=$en_trace TARGET=${trgt_lst[$k]} GEMM_SIZE=${gemm_pt[$i]} ITER_CNT=$iters PL_FREQ=$freq"
      else
         mk_targets="$base_mk_trgts TARGET=${trgt_lst[$k]} GEMM_SIZE=${gemm_pt[$i]} ITER_CNT=$iters PL_FREQ=$freq LAUNCH_HW_EMU_EXEC=1"
      fi
   
      echo "##########################################################################################################" | tee -a gemm_build.log
      echo "GEMM SIZE: ${gemm_pt[$i]}x${gemm_pt[$i]}x${gemm_pt[$i]}, Target: ${trgt_lst[$k]}, Iterations: $iters and Frequency: $freq"Mhz"" | tee -a gemm_build.log
      echo "Make args: $mk_targets" | tee -a gemm_build.log
      echo "" | tee -a gemm_build.log
         
      logName="gemm_${gemm_pt[$i]}x${gemm_pt[$i]}x${gemm_pt[$i]}"_"${trgt_lst[$k]}_aie"
      echo $logName | tee -a gemm_build.log
      echo "" | tee -a gemm_build.log
         
      bsub -I -q sil_vivado -sp 1000 -R "rusage[mem=${mem[0]}]" -J "$logName"_`date | awk '{split($4,a,":"); print a[2]"_"a[3]}'` -o "$logName".log "make $mk_targets" &
      #bsub -I -q medium -sp 1000 -R "rusage[mem=${mem[$j]}]" -J "$logName"_aie -o "$logName".log "make $mk_targets" &
       echo ${mem[$j]} | tee -a gemm_build.log
       echo "" | tee -a gemm_build.log
   done
done
