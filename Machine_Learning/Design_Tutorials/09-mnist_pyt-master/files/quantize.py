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
Simple PyTorch MNIST example - quantization
'''

'''
Author: Mark Harvey, Xilinx inc
'''

import os
import sys
import argparse
import random
import torch
import torchvision
import torch.nn as nn
import torch.nn.functional as F
from pytorch_nndct.apis import torch_quantizer, dump_xmodel

from common import *


DIVIDER = '-----------------------------------------'




def quantize(dset_dir,float_model,quant_mode,batchsize,quant_model):

  # use GPU if available   
  if (torch.cuda.device_count() > 0):
    print('You have',torch.cuda.device_count(),'CUDA devices available')
    for i in range(torch.cuda.device_count()):
      print(' Device',str(i),': ',torch.cuda.get_device_name(i))
    print('Selecting device 0..')
    device = torch.device('cuda:0')
  else:
    print('No CUDA devices available..selecting CPU')
    device = torch.device('cpu')

  # load trained model
  model = CNN().to(device)
  model.load_state_dict(torch.load(os.path.join(float_model,'f_model.pth')))

  # force to merge BN with CONV for better quantization accuracy
  optimize = 1

  # override batchsize if in test mode
  if (quant_mode=='test'):
    batchsize = 1
  
  rand_in = torch.randn([batchsize, 1, 28, 28])
  quantizer = torch_quantizer(quant_mode, model, (rand_in), output_dir=quant_model) 
  quantized_model = quantizer.quant_model


  # data loader
  test_dataset = torchvision.datasets.MNIST(dset_dir,
                                            train=False, 
                                            download=True,
                                            transform=test_transform)

  test_loader = torch.utils.data.DataLoader(test_dataset,
                                            batch_size=batchsize, 
                                            shuffle=False)

  # evaluate 
  test(quantized_model, device, test_loader)


  # export config
  if quant_mode == 'calib':
    quantizer.export_quant_config()
  if quant_mode == 'test':
    quantizer.export_xmodel(deploy_check=False, output_dir=quant_model)
  
  return



def run_main():

  # construct the argument parser and parse the arguments
  ap = argparse.ArgumentParser()
  ap.add_argument('-d',  '--dset_dir',   type=str, default='dataset',    help='Path to test & train datasets. Default is dataset')
  ap.add_argument('-m',  '--float_model',type=str, default='float_model',help='Path to folder containing trained model. Default is float_model')
  ap.add_argument('-q',  '--quant_mode', type=str, default='calib',      choices=['calib','test'], help='Quantization mode (calib or test). Default is calib')
  ap.add_argument('-b',  '--batchsize',  type=int, default=100,          help='Testing batchsize - must be an integer. Default is 100')
  ap.add_argument('-o',  '--quant_model', type=str, default='quant_model', help='Path to quantize results folder. Default is quant_model')
  args = ap.parse_args()

  print('\n'+DIVIDER)
  print('PyTorch version : ',torch.__version__)
  print(sys.version)
  print(DIVIDER)
  print(' Command line options:')
  print ('--dset_dir     : ',args.dset_dir)
  print ('--float_model  : ',args.float_model)
  print ('--quant_mode   : ',args.quant_mode)
  print ('--batchsize    : ',args.batchsize)
  print ('--quant_model  : ',args.quant_model)
  print(DIVIDER)

  quantize(args.dset_dir,args.float_model,args.quant_mode,args.batchsize,args.quant_model)

  return



if __name__ == '__main__':
    run_main()

