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

# now run semantic segmentation with 3 CNNs using VART C++ APIs with multithreads
python3 ./code/src/app_mt.py  --model ./fcn8/model/fcn8.xmodel --thread 1 --images ./dataset1/img_test/ --miou 1 

python3 ./code/src/app_mt.py  --model ./fcn8/model/fcn8.xmodel --thread 2 --images ./dataset1/img_test/
python3 ./code/src/app_mt.py  --model ./fcn8/model/fcn8.xmodel --thread 4 --images ./dataset1/img_test/
python3 ./code/src/app_mt.py  --model ./fcn8/model/fcn8.xmodel --thread 6 --images ./dataset1/img_test/

python3 ./code/src/app_mt.py  --model ./fcn8ups/model/fcn8ups.xmodel --thread 1 --images ./dataset1/img_test/  --miou 1
python3 ./code/src/app_mt.py  --model ./fcn8ups/model/fcn8ups.xmodel --thread 2 --images ./dataset1/img_test/
python3 ./code/src/app_mt.py  --model ./fcn8ups/model/fcn8ups.xmodel --thread 4 --images ./dataset1/img_test/
python3 ./code/src/app_mt.py  --model ./fcn8ups/model/fcn8ups.xmodel --thread 6 --images ./dataset1/img_test/

python3 ./code/src/app_mt.py  --model ./unet/v2/model/unet2.xmodel   --thread 1 --images ./dataset1/img_test/  --miou 1
python3 ./code/src/app_mt.py  --model ./unet/v2/model/unet2.xmodel   --thread 2 --images ./dataset1/img_test/
python3 ./code/src/app_mt.py  --model ./unet/v2/model/unet2.xmodel   --thread 4 --images ./dataset1/img_test/
python3 ./code/src/app_mt.py  --model ./unet/v2/model/unet2.xmodel   --thread 6 --images ./dataset1/img_test/
