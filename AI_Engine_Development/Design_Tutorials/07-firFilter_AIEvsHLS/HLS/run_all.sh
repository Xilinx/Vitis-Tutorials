
base_mk_trgts="$1"
en_trace="$2"
trgt="$3"
fir_taps="$4"
noOf_firs="$5"
iters="$6"
freq="$7"

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
# FIR TAPs Designs...
# -> 0 = 15 Taps.
# -> 1 = 64 Taps.
# -> 2 = 129 Taps.
# -> 3 = 240 Taps.
# -> default = 0 (15 Taps).
################################################
if [ -z "$fir_taps" ]
then
   fir_taps=3
   firTaps_loopStart=0

elif [[ $fir_taps == "a" ]]
then
   fir_taps=3
   firTaps_loopStart=0

else
   firTaps_loopStart=$fir_taps

fi

fir_taps_lst=("15" "64" "129" "240")

################################################
# FIR Filters Options...
# -> 0 = x1.
# -> 1 = x10. //x2.
# -> 2 = x3.
# -> 3 = x4.
# -> 4 = x5.
# -> 5 = x6.
# -> 6 = x7.
# -> 7 = x8.
# -> 8 = x9.
# -> 9 = x10.
# -> default = 0 (x1).
################################################
max_insts=1
if [ -z "$noOf_firs" ]
then
   noOf_firs=$max_insts
   firFilters_loopStart=0

elif [[ $noOf_firs == "a" ]]
then
   noOf_firs=$max_insts
   firFilters_loopStart=0

else
   firFilters_loopStart=$noOf_firs

fi

fir_filters_lst=("1" "10")
#"3" "4" "5" "6" "7" "8" "9" "10")

################################################
# Iteration Count...
# -> default = 8
################################################
if [ -z $iters ]
then
   iters="8"
fi

################################################
# Frequency...
# -> default = 300Mhz
################################################
if [ -z "$freq" ]
then
   freq="300"
fi

################################################
# Updating make targets based on
# above options...
################################################
if [ -z "$base_mk_trgts" ]
then
   base_mk_trgts="all"
fi

################################################
# Defining different mem sizes for the bsubs...
################################################

#bsub memory sizes: For increasing no. of fir filters...
mem=($(( 256 * 1024 )) $(( 256 * 1024 )) $(( 25 * 1024 )) $(( 50 * 1024 )) $(( 50 * 1024 )) $(( 50 * 1024 )) $(( 50 * 1024 )) $(( 100 * 1024 )) $(( 100 * 1024 )) $(( 200 * 1024 )))
################################################
# Execute...
################################################

for ((i=$firTaps_loopStart;i<=$fir_taps;i++))
do
   for ((j=$firFilters_loopStart;j<=$noOf_firs;j++))
   do
      for ((k=$trgt_loopStart;k<=$trgt;k++))
      do
         mk_targets="$base_mk_trgts EN_TRACE=$en_trace TARGET=${trgt_lst[$k]} N_FIR_TAPS=${fir_taps_lst[$i]} ITER_CNT=$iters PL_FREQ=$freq N_FIR_FILTERS=${fir_filters_lst[$j]}"
         echo "##########################################################################################################" | tee -a fir_build.log
         echo "FIR TAPS: ${fir_taps_lst[$i]}, FIR FILTERS: ${fir_filters_lst[$j]}, Target: ${trgt_lst[$k]}, Iterations: $iters and Frequency: $freq"Mhz"" | tee -a fir_build.log
         echo "Make args: $mk_targets" | tee -a fir_build.log
         echo "" | tee -a fir_build.log
      
         logName=fir_${fir_taps_lst[$i]}_taps_${trgt_lst[$k]}_${fir_filters_lst[$j]}firs
         echo $logName | tee -a fir_build.log
         echo "" | tee -a fir_build.log
      
         echo "Building design for ${fir_filters_lst[$j]} FIR Filters, find the log in "$logName".log..." | tee -a fir_build.log
         bsub -I -q sil_vivado -sp 1000 -R "rusage[mem=${mem[$j]}]" -J "$logName"_hls_`date | awk '{split($4,a,":"); print a[2]"_"a[3]}'` -o "$logName".log "make $mk_targets" &
         #bsub -I -q medium -sp 1000 -R "rusage[mem=${mem[$j]}]" -J "$logName"_hls -o "$logName".log "make $mk_targets" &
         echo ${mem[$j]} | tee -a fir_build.log
         echo "" | tee -a fir_build.log
      done
   done
done
