'''
 Copyright 2020 Xilinx Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
'''

'''
Author: Mark Harvey, Xilinx Inc
'''

import os
import numpy as np


calib_batch_size = 10
dataset_dir = 'dataset'

# unpack dataset
npz_file = np.load(os.path.join(dataset_dir,'trainData.npz'))
x_train = npz_file['x']

def calib_input(iter):
  images = []
   
  # fetch a batch from dataset
  for i in range(calib_batch_size):
    image = x_train[(iter*calib_batch_size)+i]
    images.append(image)

  return {"images_in": images}
