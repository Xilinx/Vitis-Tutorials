#!/bin/bash

## © Copyright (C) 2016-2020 Xilinx, Inc
##
## Licensed under the Apache License, Version 2.0 (the "License"). You may
## not use this file except in compliance with the License. A copy of the
## License is located at
##
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
## WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
## License for the specific language governing permissions and limitations
## under the License.

# unpack test images
echo " "
echo "PREPARE TEST IMAGES"
echo " "
tar -xvf test_images.tar
mv ./test ./test_images
rm ./test_images/*.txt

# compile CNN application
echo " "
echo "COMPILE CNN C++ APPLICATION"
echo " "
cd code
bash -x ./build_app.sh
mv code ../run_cnn # change name of the application
cd ..

##################### BASELINE CNN

# now run the baseline CNN using VART C++ APIs
echo " "
echo "RUN BASELINE CNN C++ APP"
echo " "
./run_cnn ./baseline/model/arm64_4096/alexnetBNnoLRN.xmodel   | tee ./rpt/logfile_target_baseline.txt

# check DPU prediction accuracy of C++ application
echo " "
echo "CHECK DPU PREDICTION ACCURACY OF C++ APP with BASELINE CNN"
echo " "
python3 ./code/src/check_dpu_runtime_accuracy.py -i ./rpt/logfile_target_baseline.txt | tee ./rpt/tmp.txt
tail -n 6 ./rpt/tmp.txt > ./rpt/summary_baseline_prediction_accuracy_on_dpu.txt
rm ./rpt/tmp.txt ./rpt/logfile_target_baseline.txt

# check DPU prediction accuracy of Python application
echo " "
echo "CHECK DPU PREDICTION ACCURACY OF PYTHON APP with BASELINE CNN"
echo " "
python3 ./code/src/classification.py --images ./test_images --threads 1 --model ./baseline/model/arm64_4096/alexnetBNnoLRN.xmodel --postpr 1

##################### PRUNED CNN

# now run the pruned CNN using VART C++ APIs
echo " "
echo "RUN PRUNED CNN C++ APP"
echo " "
./run_cnn ./pruned/model/arm64_4096/alexnetBNnoLRN.xmodel   | tee ./rpt/logfile_target_pruned.txt

# check DPU prediction accuracy of C++ application
echo " "
echo "CHECK DPU PREDICTION ACCURACY OF C++ APP with PRUNED CNN"
echo " "
python3 ./code/src/check_dpu_runtime_accuracy.py -i ./rpt/logfile_target_pruned.txt | tee ./rpt/tmp.txt
tail -n 6 ./rpt/tmp.txt > ./rpt/summary_pruned_prediction_accuracy_on_dpu.txt
rm ./rpt/tmp.txt ./rpt/logfile_target_pruned.txt

# check DPU prediction accuracy of Python application (UNCOMMENT POSTPROCESSING LINES of "classification.py")
echo " "
echo "CHECK DPU PREDICTION ACCURACY OF PYTHON APP with PRUNED CNN"
echo " "
python3 ./code/src/classification.py --images ./test_images --threads 1 --model ./pruned/model/arm64_4096/alexnetBNnoLRN.xmodel  --postpr 1

###############################################################################

# run fps measurements
echo " "
echo "MEASURE THROUGHPUT (FPS) OF PYTHON APP"
echo " "
bash ./code/run_alexnet_fps.sh



#remove test images
echo " "
echo "REMOVE TEST IMAGES"
echo " "
rm -r test_images
