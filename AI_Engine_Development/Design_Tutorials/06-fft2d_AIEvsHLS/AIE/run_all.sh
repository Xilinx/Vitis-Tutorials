
base_mk_trgts="$1"
fft_2d_dt="$2"
en_trace="$3"
trgt="$4"
fftpt="$5"
noOf_fftinsts="$6"
iters="$7"
freq="$8"

#echo "Inp Args: $@..." | tee -a fft2d_build.log
################################################
# Datatype...
# -> cint16 or cfloat.
# -> default = cint16
################################################
if [ -z "$fft_2d_dt" ]
then
   fft_2d_dt=1
   fft_2d_dt_loopStart=0

else
   fft_2d_dt_loopStart=$fft_2d_dt
fi

fft_2d_dt_lst=("cint16" "cfloat")


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
   trgt=0
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
# FFT Point Designs...
# -> 0 = 32 x 64.
# -> 1 = 64 x 128.
# -> 2 = 128 x 256.
# -> 3 = 256 x 512.
# -> 4 = 1024 x 2048.
# -> default = 4 (1024 x 2048).
################################################
if [ -z "$fftpt" ]
then
   fftpt=4
   fftpt_loopStart=4

elif [[ $fftpt == "a" ]]
then
   fftpt=4
   fftpt_loopStart=4

else
   fftpt_loopStart=$fftpt

fi

fft2d_pt=("64" "128" "256" "512" "2048")
row=("32" "64" "128" "256" "1024")
col=("64" "128" "256" "512" "2048")

################################################
# FFT Insts Options...
# -> 0 = x1.
# -> 1 = x5.
# -> 2 = x10.
# -> 3 = x15.
# -> 4 = x20.
# -> 5 = x25.
# -> 6 = x30.
# -> 7 = x36.
# -> 8 = x40.
# -> 9 = x50.
# -> 10 = x60.
# -> default = 0 (x1).
################################################
max_insts=2
if [ -z "$noOf_fftinsts" ]
then
   noOf_fftinsts=$max_insts
   fftinsts_loopStart=0

elif [[ $noOf_fftinsts == "a" ]]
then
   noOf_fftinsts=$max_insts
   fftinsts_loopStart=0

else
   fftinsts_loopStart=$noOf_fftinsts

fi

fftinsts=("1" "5" "10" "15" "20" "25" "30" "36" "40" "50" "60")

################################################
# Iteration Count...
# -> default = 8
################################################
if [ -z $iters ]
then
   iters="1"
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
   base_mk_trgts="clean_target all"
fi

################################################
# Defining different mem sizes for the bsubs...
################################################

#bsub memory sizes: For increasing no. of instances...
mem=($(( 25 * 1024 )) $(( 50 * 1024 )) $(( 100 * 1024 )))
################################################
# Execute...
################################################

for ((l=$fft_2d_dt_loopStart;l<=$fft_2d_dt;l++))
do
   for ((i=$fftpt_loopStart;i<=$fftpt;i++))
   do
      for ((j=$fftinsts_loopStart;j<=$noOf_fftinsts;j++))
      do
         for ((k=$trgt_loopStart;k<=$trgt;k++))
         do
            if [[ ${trgt_lst[$k]} == "hw" ]]
            then
               mk_targets="$base_mk_trgts EN_TRACE=$en_trace TARGET=${trgt_lst[$k]} FFT_2D_PT=${fft2d_pt[$i]} ITER_CNT=$iters PL_FREQ=$freq FFT_2D_INSTS=${fftinsts[$j]} FFT_2D_DT=$l"
               #mk_targets="$base_mk_trgts LAUNCH_HW_EMU_EXEC=0 EN_TRACE=$en_trace TARGET=${trgt_lst[$k]} FFT_2D_PT=${fft2d_pt[$i]} ITER_CNT=$iters PL_FREQ=$freq FFT_2D_INSTS=${fftinsts[$j]} FFT_2D_DT=$l"
            else
               mk_targets="$base_mk_trgts TARGET=${trgt_lst[$k]} FFT_2D_PT=${fft2d_pt[$i]} ITER_CNT=$iters PL_FREQ=$freq FFT_2D_INSTS=${fftinsts[$j]} FFT_2D_DT=$l"
               #mk_targets="$base_mk_trgts TARGET=${trgt_lst[$k]} LAUNCH_HW_EMU_EXEC=0 FFT_2D_PT=${fft2d_pt[$i]} ITER_CNT=$iters PL_FREQ=$freq FFT_2D_INSTS=${fftinsts[$j]} FFT_2D_DT=$l"
            fi
         
            echo "##########################################################################################################" | tee -a fft2d_build.log
            echo "FFT Points: ${row[$i]}"x"${col[$i]}, Target: ${trgt_lst[$k]}, Iterations: $iters and Frequency: $freq"Mhz"" | tee -a fft2d_build.log
            echo "Make args: $mk_targets" | tee -a fft2d_build.log
            echo "" | tee -a fft2d_build.log
         
            logName="fft2d_${fft_2d_dt_lst[$l]}"_"${row[$i]}"x"${col[$i]}"_"${fftinsts[$j]}"x"_"${trgt_lst[$k]}""
            echo $logName | tee -a fft2d_build.log
            echo "" | tee -a fft2d_build.log
         
            echo "Building design for ${fftinsts[$j]} instance, find the log in "$logName".log..." | tee -a fft2d_build.log
            bsub -I -q sil_vivado -sp 1000 -R "rusage[mem=${mem[$j]}]" -J "$logName"_aie_`date | awk '{split($4,a,":"); print a[2]"_"a[3]}'` -o "$logName".log "make $mk_targets" &
            #bsub -I -q medium -sp 1000 -R "rusage[mem=${mem[$j]}]" -J "$logName"_aie_`date | awk '{split($4,a,":"); print a[2]"_"a[3]}'` -o "$logName".log "make $mk_targets" &
            echo ${mem[$j]} | tee -a fft2d_build.log
            echo "" | tee -a fft2d_build.log
         done
      done
   done
done
