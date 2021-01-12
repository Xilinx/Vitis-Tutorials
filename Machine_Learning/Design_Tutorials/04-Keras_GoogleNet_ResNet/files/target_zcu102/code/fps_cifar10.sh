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

# check LeNet fps
python3 ./code/src/classification.py --images ./cifar10_test/ --threads 1 --model ./cifar10/LeNet/model/LeNet.xmodel  --classes CIFAR10
python3 ./code/src/classification.py --images ./cifar10_test/ --threads 5 --model ./cifar10/LeNet/model/LeNet.xmodel  --classes CIFAR10
python3 ./code/src/classification.py --images ./cifar10_test/ --threads 6 --model ./cifar10/LeNet/model/LeNet.xmodel  --classes CIFAR10

# check miniVggNet fps
python3 ./code/src/classification.py --images ./cifar10_test/ --threads 1 --model ./cifar10/miniVggNet/model/miniVggNet.xmodel  --classes CIFAR10
python3 ./code/src/classification.py --images ./cifar10_test/ --threads 5 --model ./cifar10/miniVggNet/model/miniVggNet.xmodel  --classes CIFAR10
python3 ./code/src/classification.py --images ./cifar10_test/ --threads 6 --model ./cifar10/miniVggNet/model/miniVggNet.xmodel  --classes CIFAR10

# check miniGoogleNet fps
python3 ./code/src/classification.py --images ./cifar10_test/ --threads 1 --model ./cifar10/miniGoogleNet/model/miniGoogleNet.xmodel  --classes CIFAR10
python3 ./code/src/classification.py --images ./cifar10_test/ --threads 5 --model ./cifar10/miniGoogleNet/model/miniGoogleNet.xmodel  --classes CIFAR10
python3 ./code/src/classification.py --images ./cifar10_test/ --threads 6 --model ./cifar10/miniGoogleNet/model/miniGoogleNet.xmodel  --classes CIFAR10


# check miniResNet fps
python3 ./code/src/classification.py --images ./cifar10_test/ --threads 1 --model ./cifar10/miniResNet/model/miniResNet.xmodel  --classes CIFAR10
python3 ./code/src/classification.py --images ./cifar10_test/ --threads 5 --model ./cifar10/miniResNet/model/miniResNet.xmodel  --classes CIFAR10
python3 ./code/src/classification.py --images ./cifar10_test/ --threads 6 --model ./cifar10/miniResNet/model/miniResNet.xmodel  --classes CIFAR10
