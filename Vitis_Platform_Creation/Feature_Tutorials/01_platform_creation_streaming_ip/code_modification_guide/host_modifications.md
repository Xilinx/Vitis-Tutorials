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
OCL_CHECK(err, cl::Buffer buffer_a(context, CL_MEM_READ_ONLY, size_in_bytes, NULL, &err));
OCL_CHECK(err, cl::Buffer buffer_b(context, CL_MEM_READ_ONLY, size_in_bytes, NULL, &err));
OCL_CHECK(err, cl::Buffer buffer_result(context, CL_MEM_WRITE_ONLY, size_in_bytes, NULL, &err));
```

***NEW***
```
OCL_CHECK(err, cl::Buffer buffer_a(context, CL_MEM_READ_ONLY, size_in_bytes, NULL, &err));
OCL_CHECK(err, cl::Buffer buffer_b(context, CL_MEM_READ_ONLY, size_in_bytes, NULL, &err));
OCL_CHECK(err, cl::Buffer buffer_result(context, CL_MEM_WRITE_ONLY, size_in_bytes, NULL, &err));

cl::Buffer buffer_waveout(context, CL_MEM_WRITE_ONLY, 1024*sizeof(int));
```

#### 3. Bond the buffer with new arguments of the kernel

***OLD***
```
OCL_CHECK(err, err = krnl_vector_add.setArg(narg++, buffer_a));
OCL_CHECK(err, err = krnl_vector_add.setArg(narg++, buffer_b));
OCL_CHECK(err, err = krnl_vector_add.setArg(narg++, buffer_result));
OCL_CHECK(err, err = krnl_vector_add.setArg(narg++, DATA_SIZE));
```

***NEW***
```
OCL_CHECK(err, err = krnl_vector_add.setArg(narg++, buffer_a));
OCL_CHECK(err, err = krnl_vector_add.setArg(narg++, buffer_b));
OCL_CHECK(err, err = krnl_vector_add.setArg(narg++, buffer_result));
OCL_CHECK(err, err = krnl_vector_add.setArg(narg++, DATA_SIZE));

krnl_vector_add.setArg(narg++,buffer_waveout);
```

#### 4. Map the new buffer to a pointer

***OLD***
```
OCL_CHECK(err,
          ptr_a = (int*)q.enqueueMapBuffer(buffer_a, CL_TRUE, CL_MAP_WRITE, 0, size_in_bytes, NULL, NULL, &err));
OCL_CHECK(err,
          ptr_b = (int*)q.enqueueMapBuffer(buffer_b, CL_TRUE, CL_MAP_WRITE, 0, size_in_bytes, NULL, NULL, &err));
OCL_CHECK(err, ptr_result = (int*)q.enqueueMapBuffer(buffer_result, CL_TRUE, CL_MAP_READ, 0, size_in_bytes, NULL,
                                                     NULL, &err));
```

***NEW***
```
OCL_CHECK(err,
          ptr_a = (int*)q.enqueueMapBuffer(buffer_a, CL_TRUE, CL_MAP_WRITE, 0, size_in_bytes, NULL, NULL, &err));
OCL_CHECK(err,
          ptr_b = (int*)q.enqueueMapBuffer(buffer_b, CL_TRUE, CL_MAP_WRITE, 0, size_in_bytes, NULL, NULL, &err));
OCL_CHECK(err, ptr_result = (int*)q.enqueueMapBuffer(buffer_result, CL_TRUE, CL_MAP_READ, 0, size_in_bytes, NULL,
                                                     NULL, &err));

int *ptr_waveout = (int *) q.enqueueMapBuffer (buffer_waveout , CL_TRUE , CL_MAP_READ , 0, 1024*sizeof(int));
```

#### 5. Read data from DDR

***OLD***
```
OCL_CHECK(err, q.enqueueMigrateMemObjects({buffer_result}, CL_MIGRATE_MEM_OBJECT_HOST));
```

***NEW***
```
OCL_CHECK(err, q.enqueueMigrateMemObjects({buffer_result}, CL_MIGRATE_MEM_OBJECT_HOST));
q.enqueueMigrateMemObjects({buffer_waveout},CL_MIGRATE_MEM_OBJECT_HOST);
```

#### 6. Save the data into a file

This can be added right after the piece of result verification that already exists.

***OLD***
```
int match = 0;
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
int match = 0;
 for (int i = 0; i < DATA_SIZE; i++) {
     int host_result = ptr_a[i] + ptr_b[i];
     if (ptr_result[i] != host_result) {
         printf(error_message.c_str(), i, host_result, ptr_result[i]);
         match = 1;
         break;
     }
 }

 FILE *fp_dout;
 fp_dout=fopen("wave_out.txt","w");

 for (int i = 0; i < 1024; i++) {
   fprintf(fp_dout,"%d\n",ptr_waveout[i]<<17);
 }

 fclose(fp_dout);
```

#### 7. Unmap the buffer object

***OLD***
```
OCL_CHECK(err, err = q.enqueueUnmapMemObject(buffer_a, ptr_a));
OCL_CHECK(err, err = q.enqueueUnmapMemObject(buffer_b, ptr_b));
OCL_CHECK(err, err = q.enqueueUnmapMemObject(buffer_result, ptr_result));
```

***NEW***
```
OCL_CHECK(err, err = q.enqueueUnmapMemObject(buffer_a, ptr_a));
OCL_CHECK(err, err = q.enqueueUnmapMemObject(buffer_b, ptr_b));
OCL_CHECK(err, err = q.enqueueUnmapMemObject(buffer_result, ptr_result));

q.enqueueUnmapMemObject(buffer_waveout , ptr_waveout);
```

# License

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.

You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )



Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
