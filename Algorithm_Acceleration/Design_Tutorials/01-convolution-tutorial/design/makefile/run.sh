#! /bin/sh
#touch baseline_vitis.txt
#touch dataflow_vitis.txt
#touch fixedpoint_vitis.txt
#touch localbuff_vitis.txt
#touch multicu_vitis.txt
#touch qdma_vitis.txt


#make STEP=multicu clean
#make STEP=qdma clean
#make STEP=baseline clean
#make STEP=localbuf clean
#make STEP=fixedpoint clean
#make STEP=dataflow clean





#make run TARGET=hw STEP=baseline SOLUTION=1 NUM_FRAMES=1 | tee baselie_vitis.txt
#make run TARGET=hw STEP=localbuf SOLUTION=1 NUM_FRAMES=1 | tee localbuf_vitis.txt
#make run TARGET=hw STEP=fixedpoint SOLUTION=1 NUM_FRAMES=1 | tee fixedpoint_vitis.txt
#make run TARGET=hw STEP=dataflow SOLUTION=1 NUM_FRAMES=1| tee dataflow_vitis.txt
#make run TARGET=hw_emu STEP=multicu SOLUTION=1 | tee multicu_vitis.txt
#NUM_FRAMES=1
make run TARGET=hw_emu STEP=qdma SOLUTION=1 NUM_FRAMES=1 | tee qdma_vitis.txt
#vimdiff multicu_vitis.txt qdma_vitis.txt 
# NUM_FRAMES=1






