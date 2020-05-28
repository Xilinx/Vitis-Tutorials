# Section_3-Advanced
```
 * Copyright 2019 Xilinx Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ```
 ---
## Important:
-  ./image_input_refinedet
-  ./usb_input_multi_threads_refinedet_drm
-  ./usb_input_multi_threas_refinedet
**model name** :    refinedet_pruned_0_8
---
## 1 compile

execute the following command:

        sh build.sh

## 2 run the executable file.
```    
    drm mode :  /etc/init.d/weston stop; ./usb_input_multi_threads_refinedet_drm refinedet_pruned_0_8 0 -t 3
    local mode: ./usb_input_multi_threads_refinedet refinedet_pruned_0_8 0 -t 3
   
    sample : ./image_input_refinedet /usr/share/vitis_ai_library/models/refinedet_pruned_0_8/
    output : 
WARNING: Logging before InitGoogleLogging() is written to STDERR
I0517 21:39:49.318879 27477 image_input_refinedet.cpp:150] RESULT2:     115     94      480     357     0.980669
I0517 21:39:49.319383 27477 image_input_refinedet.cpp:150] RESULT2:     10      119     206     278     0.961984
I0517 21:39:49.319458 27477 image_input_refinedet.cpp:150] RESULT2:     452     77      478     219     0.915951
I0517 21:39:49.319499 27477 image_input_refinedet.cpp:150] RESULT2:     0       110     53      208     0.915951
```
   
