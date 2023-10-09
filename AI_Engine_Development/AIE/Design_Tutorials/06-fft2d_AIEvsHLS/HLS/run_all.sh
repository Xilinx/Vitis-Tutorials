#!/bin/bash

#echo "Inp Args: $@..." | tee -a fft2d_build.log

#################################################
#Argument List and Default Values...
#Associative Array referenced by strings
declare -A arg_lst
arg_lst[mk]="all"
arg_lst[tgt]="all"
arg_lst[dt]="all"
arg_lst[pt]="all"
arg_lst[insts]="all"
arg_lst[freq]="250"
arg_lst[en_trace]="1"
arg_lst[iters]="16"

#echo ${#arg_lst[@]}

for arg in "$@"
do
   opt=`echo $arg | awk -F = '{print $1}'`
   opt_val=`echo $arg | awk -F = '{print $2}'`
   opt_found=false
   
   #echo $opt
   #echo $opt_val
   
   #Iterate over all the indexes of the associative array(using !)...
   for i in "${!arg_lst[@]}"
   do
      #echo $i
      if [[ $opt == $i ]]
      then
         arg_lst[$opt]=$opt_val
         opt_found=true
      
      fi
   done
   
   if [[ $opt_found == false ]]
   then
      echo "Option Invalid..."
      echo "Pick from mk, tgt, dt, pt, insts, freq, en_trace and iters"
      echo "Script argument format:"
      echo "arg=arg_value (all without space)"
      
      #Exit out of script with invalid arg exit code (128)...
      exit 128
   fi
done

# For every key in the associative array arg_lst...
echo "Print out all arg values"

for KEY in "${!arg_lst[@]}"; do
  # Print the KEY value
  echo "Key: $KEY"
  
  # Print the VALUE attached to that KEY
  echo "Value: ${arg_lst[$KEY]}"
  echo ""
done

################################################
# Updating make targets based on
# above options...
################################################
clean_all_dsns="False"

# Incase make cleanall is used...
if [[ ${arg_lst[mk]} == "cleanall" ]]
then
   clean_all_dsns="True"
   arg_lst[mk]="clean_tgt"
fi

################################################
# Target...
# -> hw or hw_emu.
# -> default = hw_emu
################################################
if [[ ${arg_lst[tgt]} == "all" ]]
then
   tgt_lst=("hw" "hw_emu")

else
   tgt_lst=${arg_lst[tgt]}

fi

#echo ${tgt_lst[*]}

################################################
# Datatype...
# -> cint16 or cfloat.
# -> default = cint16
################################################
if [[ ${arg_lst[dt]} == "all" ]]
then
   dt_lst=("cint16" "cfloat")

else
   dt_lst=${arg_lst[dt]}

fi

#echo ${dt_lst[*]}

################################################
# FFT Point Designs...
# ->   64pt = 32 x 64.
# ->  128pt = 64 x 128.
# ->  256pt = 128 x 256.
# ->  512pt = 256 x 512.
# -> 2048pt = 1024 x 2048.
# -> default = 2048pt (1024 x 2048).
################################################
if [[ ${arg_lst[pt]} == "all" ]]
then
   pt_lst=("64" "128" "256" "512" "2048")

else
   pt_lst=${arg_lst[pt]}

fi

#echo ${pt_lst[*]}

################################################
# FFT Insts Options...
# ->  1 = x1.
# ->  5 = x5.
# -> 10 = x10.
# -> default = 1 (x1).
################################################
if [[ ${arg_lst[insts]} == "all" ]]
then
   insts_lst=("1" "5" "10")

else
   insts_lst=${arg_lst[insts]}

fi

#echo ${insts_lst[*]}

################################################
# Frequency...
# -> default = 250Mhz
################################################

################################################
# 1 Enable Trace Profiling Flag
# -> 0 = trace profiling is disabled.
# -> 1 = trace profiling is enabled.
# -> default = 1
# -> For all designs targeted to hw_emu,
#    profiling is by default disabled.
################################################

################################################
# Iteration Count...
# -> default = 16
################################################

################################################
# Defining different mem sizes for the bsubs...
################################################

#bsub memory sizes: For increasing no. of instances...
#mem=($(( 32 * 1024 )) $(( 64 * 1024 )) $(( 128 * 1024 )))
#mem=($(( 128 * 1024 )) $(( 160 * 1024 )) $(( 200 * 1024 )))

#echo ${mem[*]}

#Associative Array referenced by strings
declare -A mem
mem["1"]="30000"
mem["5"]="60000"
mem["10"]="120000"

################################################
# Execute...
################################################

for dt in ${dt_lst[*]}
do
   #echo $dt
   if [[ $dt == "cint16" ]]
   then
       dt_index=0
   else
      dt_index=1
   fi
   for pt in ${pt_lst[*]}
   do
      #echo $pt
      for insts in ${insts_lst[*]}
      do
         #echo $insts
         for tgt in ${tgt_lst[*]}
         do
            #echo $tgt
            mk_targets="${arg_lst[mk]} EN_TRACE=${arg_lst[en_trace]} TARGET=$tgt FFT_2D_PT=$pt ITER_CNT=${arg_lst[iters]} PL_FREQ=${arg_lst[freq]} FFT_2D_INSTS=$insts FFT_2D_DT=$dt_index"
            #echo $mk_targets
            
            echo "##########################################################################################################" | tee -a fft2d_build.log
            echo "FFT Points: "$pt"pt, Target: $tgt, Iterations: ${arg_lst[iters]} and Frequency: ${arg_lst[freq]}"Mhz"" | tee -a fft2d_build.log
            echo "Make args: $mk_targets" | tee -a fft2d_build.log
            echo "" | tee -a fft2d_build.log
            
            logName="fft2d_$dt"_$pt"pt"_"$insts"x_$tgt""
            echo "Logging build under "$logName".log" | tee -a fft2d_build.log
            echo "" | tee -a fft2d_build.log
            
            echo "Building design for $insts instance, find the log in "$logName".log..." | tee -a fft2d_build.log
            #bsub -q sil_vivado -sp 1000 -R "rusage[mem=${mem[$insts]}]" -J "$logName"_hls_`date | awk '{split($4,a,":"); print a[2]"_"a[3]}'` -oo "$logName".log "make $mk_targets" &
            bsub -q sil_vivado -R "rusage[mem=${mem[$insts]}]" -J "$logName"_hls_`date | awk '{split($4,a,":"); print a[2]"_"a[3]}'` -oo "$logName".log "make $mk_targets" &
            echo ${mem[$insts]} | tee -a fft2d_build.log
            echo "" | tee -a fft2d_build.log
            #sleep 1.5s
         done
      done
   done
done

# If make cleanall is called...
if [[ $clean_all_dsns == "True" ]]
then
   while true
   do
      blst=""
      blst=`bjobs -J "fft2d*hls*"`
      #echo $blst
      
      if [[ -z "$blst" ]]
      then
         make cleanall
         
         break
      fi
   done
fi
