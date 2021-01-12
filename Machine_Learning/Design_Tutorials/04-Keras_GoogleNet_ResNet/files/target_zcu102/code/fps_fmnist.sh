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
python3 ./code/src/classification.py --images ./fmnist_test/ --threads 1 --model ./fmnist/LeNet/model/LeNet.xmodel  --classes "fmnist"
python3 ./code/src/classification.py --images ./fmnist_test/ --threads 5 --model ./fmnist/LeNet/model/LeNet.xmodel  --classes "fmnist"
python3 ./code/src/classification.py --images ./fmnist_test/ --threads 6 --model ./fmnist/LeNet/model/LeNet.xmodel  --classes "fmnist"

# check miniVggNet fps
python3 ./code/src/classification.py --images ./fmnist_test/ --threads 1 --model ./fmnist/miniVggNet/model/miniVggNet.xmodel  --classes "fmnist"
python3 ./code/src/classification.py --images ./fmnist_test/ --threads 5 --model ./fmnist/miniVggNet/model/miniVggNet.xmodel  --classes "fmnist"
python3 ./code/src/classification.py --images ./fmnist_test/ --threads 6 --model ./fmnist/miniVggNet/model/miniVggNet.xmodel  --classes "fmnist"

# check miniGoogleNet fps
python3 ./code/src/classification.py --images ./fmnist_test/ --threads 1 --model ./fmnist/miniGoogleNet/model/miniGoogleNet.xmodel  --classes "fmnist"
python3 ./code/src/classification.py --images ./fmnist_test/ --threads 5 --model ./fmnist/miniGoogleNet/model/miniGoogleNet.xmodel  --classes "fmnist"
python3 ./code/src/classification.py --images ./fmnist_test/ --threads 6 --model ./fmnist/miniGoogleNet/model/miniGoogleNet.xmodel  --classes "fmnist"


# check miniResNet fps
python3 ./code/src/classification.py --images ./fmnist_test/ --threads 1 --model ./fmnist/miniResNet/model/miniResNet.xmodel  --classes "fmnist"
python3 ./code/src/classification.py --images ./fmnist_test/ --threads 5 --model ./fmnist/miniResNet/model/miniResNet.xmodel  --classes "fmnist"
python3 ./code/src/classification.py --images ./fmnist_test/ --threads 6 --model ./fmnist/miniResNet/model/miniResNet.xmodel  --classes "fmnist"
