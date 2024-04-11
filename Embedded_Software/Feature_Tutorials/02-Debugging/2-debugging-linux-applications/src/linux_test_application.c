# Copyright 2020 Xilinx Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#define _ERROR3_
//#define _ERROR5_
//#define _ERROR6_

#ifdef _ERROR3_
#define BRAM_PHY_ADDR 0xA0000000
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#ifdef _ERROR5_
#include <pthread.h>
#include <string.h>
#endif

#ifdef _ERROR5_
	pthread_t tid[2];
	int counter;
	pthread_mutex_t lock;
#endif

#ifdef _ERROR5_
void* doSomeThing(void *arg)
{
  //  pthread_mutex_lock(&lock);

    unsigned long i = 0;
    counter += 1;
    printf("\n Job %d started\n", counter);

    for(i=0; i<(0xFFFFFFFF);i++);

    printf("\n Job %d finished\n", counter);

    //pthread_mutex_unlock(&lock);

    return NULL;
}
#endif

int main(void) {
#ifdef _ERROR3_
	int fd;
	void *ptr;
	unsigned addr, page_addr, page_offset;
	unsigned page_size=sysconf(_SC_PAGESIZE);
	unsigned *reg;
#endif

#ifdef _ERROR5_
	int i = 0;
	int err;
#endif

#ifdef _ERROR6_
	int* ptr;
	int n, i;
	n = 5;
#endif
#ifdef _ERROR3_

	fd=open("/dev/mem",O_RDONLY);
	if(fd<1) {
		printf("/dev/mem Open Failed\r\n");
		exit(-1);
	}

	addr=BRAM_PHY_ADDR;
	page_addr=(addr & ~(page_size-1));
	page_offset=addr-page_addr;

	ptr=mmap(NULL,page_size,PROT_READ,MAP_SHARED,fd,(addr & ~(page_size-1)));
	if((int)ptr==-1) {
		printf("Failed to allocate Virtual Address!\r\n");
		exit(-1);
	}

    reg = (unsigned *)ptr;
    printf("0x%08x\n",*((unsigned *)(ptr+page_offset)));
    printf("0x%08x\n", reg[0]);
    printf("0x%08x\n", reg[1]);
    printf("0x%08x\n", reg[2]);
    printf("0x%08x\n", reg[3]);

    return 0;
#endif

#ifdef _ERROR5_

   /* if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
*/
    while(i < 2) {
        err = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
//    pthread_mutex_destroy(&lock);

    return 0;
#endif

#ifdef _ERROR6_

    ptr = (int*)malloc(n * sizeof(int));

    if (ptr == NULL) {
    	printf("Memory not allocated.\n");
        exit(0);
    } else {
    	printf("Memory successfully allocated using malloc.\n");
    	for (i = 0; i < n; ++i) {
    		*ptr = i + 1;
    		ptr++;
    		//ptr[i] = i + 1;
    	}
    }
    printf("The elements of the array are: ");
    for (i = 0; i < n; ++i) {
    	printf("%d, ", ptr[i]);
    }
    return 0;
#endif
}
