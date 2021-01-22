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


# run VART Python APIs with multi-threading on baseline CNN
echo " "
echo "BASELINE CNN WITH PYTHON APP AND MULTITHREADING"
echo " "
python3 ./code/src/classification.py --images ./test_images/ --threads 1 --model ./baseline/model/arm64_4096/alexnetBNnoLRN.xmodel
python3 ./code/src/classification.py --images ./test_images/ --threads 2 --model ./baseline/model/arm64_4096/alexnetBNnoLRN.xmodel
python3 ./code/src/classification.py --images ./test_images/ --threads 3 --model ./baseline/model/arm64_4096/alexnetBNnoLRN.xmodel
python3 ./code/src/classification.py --images ./test_images/ --threads 4 --model ./baseline/model/arm64_4096/alexnetBNnoLRN.xmodel
python3 ./code/src/classification.py --images ./test_images/ --threads 5 --model ./baseline/model/arm64_4096/alexnetBNnoLRN.xmodel
python3 ./code/src/classification.py --images ./test_images/ --threads 6 --model ./baseline/model/arm64_4096/alexnetBNnoLRN.xmodel
python3 ./code/src/classification.py --images ./test_images/ --threads 7 --model ./baseline/model/arm64_4096/alexnetBNnoLRN.xmodel


# run VART Python APIs with multi-threading on pruned CNN
echo " "
echo "PRUNED CNN WITH PYTHON APP AND MULTITHREADING"
echo " "
python3 ./code/src/classification.py --images ./test_images/ --threads 1 --model ./pruned/model/arm64_4096/alexnetBNnoLRN.xmodel
python3 ./code/src/classification.py --images ./test_images/ --threads 2 --model ./pruned/model/arm64_4096/alexnetBNnoLRN.xmodel
python3 ./code/src/classification.py --images ./test_images/ --threads 3 --model ./pruned/model/arm64_4096/alexnetBNnoLRN.xmodel
python3 ./code/src/classification.py --images ./test_images/ --threads 4 --model ./pruned/model/arm64_4096/alexnetBNnoLRN.xmodel
python3 ./code/src/classification.py --images ./test_images/ --threads 5 --model ./pruned/model/arm64_4096/alexnetBNnoLRN.xmodel
python3 ./code/src/classification.py --images ./test_images/ --threads 6 --model ./pruned/model/arm64_4096/alexnetBNnoLRN.xmodel
python3 ./code/src/classification.py --images ./test_images/ --threads 7 --model ./pruned/model/arm64_4096/alexnetBNnoLRN.xmodel
