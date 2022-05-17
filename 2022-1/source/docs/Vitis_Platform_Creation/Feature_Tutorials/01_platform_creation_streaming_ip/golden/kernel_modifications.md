Here it lists the changes required for the vadd kernel for this tutorial. Please mind that this is just one example and there are also other ways to implement the same functions.

#### 1. Include necessary file headers.

Add following 'include' commands at the beginning of the file, just before the 'define' command.

***OLD***:
```
#define DATA_SIZE 4096
```

***NEW***:
```
#include "ap_int.h"
#include "ap_axi_sdata.h"
#include "hls_stream.h"

#define DATA_SIZE 4096
```

#### 2. Define the AXI stream data type

Define a new 'pkt' type before the kernel function definition.

***OLD***
```
extern "C" {
void krnl_vadd(...
  ){
...
```

***NEW***
```
typedef ap_axis<15, 0, 0, 0> pkt;
extern "C" {
void krnl_vadd(...
  ){
...
```

#### 3. Add more ports to the kernel functions

Modify the kernel function to include more ports for interacting with the IP inside platform.

***OLD***
```
extern "C" {
void krnl_vadd(uint32_t* in1, uint32_t* in2, uint32_t* out, int size) {
```

***NEW***
```
void krnl_vadd(uint32_t* in1, uint32_t* in2, uint32_t* out, int size, int *wave_out, hls::stream<pkt> &dds_in) {
```

#### 4. Add a piece of logic in the kernel function body

This logic simply read the data into the kernel and then send it out through another port. You can add it into any place and here is an example of adding it at the end of the function body.

***OLD***
```
store_result(out, out_stream, size);
```

***NEW***
```
store_result(out, out_stream, size);

for (int i = 0; i < 1024; i++) {
#pragma HLS PIPELINE II = 1
   pkt value = dds_in.read();
   wave_out[i] = value.data;

}
```
# License

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.

You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )



Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
