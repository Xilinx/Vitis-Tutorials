#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
**

* © Copyright (C) 2016-2020 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/
'''

# ##################################################################################################
# USAGE
# python <WRK_DIR>/caffe/code/4_training.py -s <WRK_DIR>/caffe/models/miniVggNet/m3/solver_3_miniVggNet.prototxt -l <WRK_DIR>/caffe/models/miniVggNet/m3/logfile_3_miniVggNet.log

#by daniele.bagni@xilinx.com

# ##################################################################################################



import  os
import glob
from datetime import datetime

from config import cats_vs_dogs_config as config

import argparse

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-l", "--logfile", required=True, help="logfile")
ap.add_argument("-s", "--solver",  required=True, help="solver")

args = vars(ap.parse_args())
LOGFILE = args["logfile"]
SOLVER  = args["solver"]

CAFFE_TRAINING = "caffe train "

# ##################################################################################################

print("TRAINING WITH CAFFE")

caffe_solver  = SOLVER
caffe_logfile = LOGFILE

caffe_command = CAFFE_TRAINING + " --gpu all " + " --solver " + caffe_solver + " 2>&1 | tee " + caffe_logfile

startTime1 = datetime.now()
os.system(caffe_command)
endTime1 = datetime.now()
diff1 = endTime1 - startTime1
print("\n")
print("Elapsed time for Caffe training (s): ", diff1.total_seconds())
print("\n")
