/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/

#define BUFFER_SIZE 256

kernel __attribute__((reqd_work_group_size(1, 1, 1)))

void vadd_mm(global const int* a,
                   global const int* b,
                   global int* c,
                   const int n_elements)
{
    int arrayA[BUFFER_SIZE];
    for (int i = 0 ; i < n_elements ; i += BUFFER_SIZE)
    {
        int size = BUFFER_SIZE;
        //boundary check
        if (i + size > n_elements) size = n_elements - i;

        //Burst reading A
        readA: for (int j = 0 ; j < size ; j++)
            arrayA[j] = a[i+j];

        //Burst reading B and calculating C and Burst writing
        // to  Global memory
        vadd_wrteC: for (int j = 0 ; j < size ; j++)
            c[i+j] = arrayA[j] + b[i+j];
    }
}

