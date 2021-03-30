Here it lists the changes required for the vadd host application for this tutorial. Please mind that this is just one example and there are also other ways to implement the same functions.

#### 1. Include necessary file headers.

Add the new 'include' commands at the beginning of the file around the same place of other 'include' commands that already exists.

***OLD***
```
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include "vadd.h"
```

***NEW***
```
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include "vadd.h"

#include "ap_int.h"
#include <stdio.h>
```

#### 2. Create a new cl_mem buffer

Create a new buffer to communicate with the data from the newly added port of kernel.

***OLD***
```
cl::Buffer buffer_a(context, CL_MEM_READ_ONLY, size_in_bytes);
cl::Buffer buffer_b(context, CL_MEM_READ_ONLY, size_in_bytes);
cl::Buffer buffer_result(context, CL_MEM_WRITE_ONLY, size_in_bytes);
```

***NEW***
```
cl::Buffer buffer_a(context, CL_MEM_READ_ONLY, size_in_bytes);
cl::Buffer buffer_b(context, CL_MEM_READ_ONLY, size_in_bytes);
cl::Buffer buffer_result(context, CL_MEM_WRITE_ONLY, size_in_bytes);
cl::Buffer buffer_waveout(context, CL_MEM_WRITE_ONLY, 1024*sizeof(int));
```

#### 3. Bond the buffer with new arguments of the kernel

***OLD***
```
krnl_vector_add.setArg(narg++,buffer_a);
krnl_vector_add.setArg(narg++,buffer_b);
krnl_vector_add.setArg(narg++,buffer_result);
krnl_vector_add.setArg(narg++,DATA_SIZE);
```

***NEW***
```
krnl_vector_add.setArg(narg++,buffer_a);
krnl_vector_add.setArg(narg++,buffer_b);
krnl_vector_add.setArg(narg++,buffer_result);
krnl_vector_add.setArg(narg++,DATA_SIZE);
krnl_vector_add.setArg(narg++,buffer_waveout);
```

#### 4. Map the new buffer to a pointer

***OLD***
```
int *ptr_a = (int *) q.enqueueMapBuffer (buffer_a , CL_TRUE , CL_MAP_WRITE , 0, size_in_bytes);
int *ptr_b = (int *) q.enqueueMapBuffer (buffer_b , CL_TRUE , CL_MAP_WRITE , 0, size_in_bytes);
int *ptr_result = (int *) q.enqueueMapBuffer (buffer_result , CL_TRUE , CL_MAP_READ , 0, size_in_bytes);
```

***NEW***
```
int *ptr_a = (int *) q.enqueueMapBuffer (buffer_a , CL_TRUE , CL_MAP_WRITE , 0, size_in_bytes);
int *ptr_b = (int *) q.enqueueMapBuffer (buffer_b , CL_TRUE , CL_MAP_WRITE , 0, size_in_bytes);
int *ptr_result = (int *) q.enqueueMapBuffer (buffer_result , CL_TRUE , CL_MAP_READ , 0, size_in_bytes);
int *ptr_waveout = (int *) q.enqueueMapBuffer (buffer_waveout , CL_TRUE , CL_MAP_READ , 0, 1024*sizeof(int));
```

#### 5. Read data from DDR

***OLD***
```
q.enqueueMigrateMemObjects({buffer_result},CL_MIGRATE_MEM_OBJECT_HOST);
```

***NEW***
```
q.enqueueMigrateMemObjects({buffer_result},CL_MIGRATE_MEM_OBJECT_HOST);
q.enqueueMigrateMemObjects({buffer_waveout},CL_MIGRATE_MEM_OBJECT_HOST);
```

#### 6. Save the data into a file

This can be added right after the piece of result verification that already exists.

***OLD***
```
for (int i = 0; i < DATA_SIZE; i++) {
    int host_result = ptr_a[i] + ptr_b[i];
    if (ptr_result[i] != host_result) {
        printf(error_message.c_str(), i, host_result, ptr_result[i]);
        match = 1;
        break;
    }
}
```

***NEW***
```
for (int i = 0; i < DATA_SIZE; i++) {
    int host_result = ptr_a[i] + ptr_b[i];
    if (ptr_result[i] != host_result) {
        printf(error_message.c_str(), i, host_result, ptr_result[i]);
        match = 1;
        break;
    }
}

FILE *fp;
fp=fopen("wave_out.txt","w");

for (int i = 0; i < 1024; i++) {
  fprintf(fp,"%d\n",ptr_waveout[i]<<17);
}

fclose(fp);
```

#### 7. Unmap the buffer object

***OLD***
```
q.enqueueUnmapMemObject(buffer_a , ptr_a);
q.enqueueUnmapMemObject(buffer_b , ptr_b);
q.enqueueUnmapMemObject(buffer_result , ptr_result);
```

***NEW***
```
q.enqueueUnmapMemObject(buffer_a , ptr_a);
q.enqueueUnmapMemObject(buffer_b , ptr_b);
q.enqueueUnmapMemObject(buffer_result , ptr_result);
q.enqueueUnmapMemObject(buffer_waveout , ptr_waveout);
```

# License

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.

You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )



Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
