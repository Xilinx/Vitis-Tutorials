/*
* Copyright (C) 2020 - 2021 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software
* is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
* KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
* EVENT SHALL XILINX BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
* OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE. Except as contained in this notice, the name of the Xilinx shall
* not be used in advertising or otherwise to promote the sale, use or other
* dealings in this Software without prior written authorization from Xilinx.
*/

#include <ivas/ivas_kernel.h>
#include <stdio.h>
#include <unistd.h>

typedef struct _kern_priv
{
    int use_mean;
    int mean;
} ResizeKernelPriv;

int32_t
xlnx_kernel_start(IVASKernel *handle, int start, IVASFrame *input[MAX_NUM_OBJECT], IVASFrame *output[MAX_NUM_OBJECT]);
int32_t xlnx_kernel_done(IVASKernel *handle);
int32_t xlnx_kernel_init(IVASKernel *handle);
uint32_t xlnx_kernel_deinit(IVASKernel *handle);

uint32_t xlnx_kernel_deinit(IVASKernel *handle)
{
    ResizeKernelPriv *kernel_priv;
    kernel_priv = (ResizeKernelPriv *)handle->kernel_priv;
    free(kernel_priv);
    return 0;
}

int32_t xlnx_kernel_init(IVASKernel *handle)
{
    json_t *jconfig = handle->kernel_config;
    json_t *val; /* kernel config from app */
    ResizeKernelPriv *kernel_priv;

    handle->is_multiprocess = 0;

    kernel_priv = (ResizeKernelPriv *)calloc(1, sizeof(ResizeKernelPriv));
    if (!kernel_priv) {
        printf("Error: Unable to allocate resize kernel memory\n");
    }

    /* parse config */
    val = json_object_get(jconfig, "use_mean");
    if (!val || !json_is_number(val))
        kernel_priv->use_mean = 0;
    else {
        kernel_priv->use_mean = json_number_value(val);
        if (kernel_priv->use_mean != 0)
            kernel_priv->use_mean = 1;
    }
    printf("Resize: use mean=%d\n", kernel_priv->use_mean);

    /* parse config */
    val = json_object_get(jconfig, "mean_value");
    if (!val || !json_is_number(val))
        kernel_priv->mean = 0;
    else {
        kernel_priv->mean = json_number_value(val);
    }
    printf("Resize: mean=%d\n", kernel_priv->mean);

    handle->kernel_priv = (void *)kernel_priv;

    return 0;
}

int32_t xlnx_kernel_start(IVASKernel *handle, int start, IVASFrame *input[MAX_NUM_OBJECT], IVASFrame *output[MAX_NUM_OBJECT])
{
    ResizeKernelPriv *kernel_priv;
    kernel_priv = (ResizeKernelPriv *)handle->kernel_priv;

    ivas_register_write(handle, &(input[0]->props.width), sizeof(uint32_t), 0x34);   /* In width */
    ivas_register_write(handle, &(input[0]->props.height), sizeof(uint32_t), 0x3C);  /* In height */
    ivas_register_write(handle, &(input[0]->props.stride), sizeof(uint32_t), 0x44);  /* In height */
    ivas_register_write(handle, &(output[0]->props.height), sizeof(uint32_t), 0x54); /* Out height */
    ivas_register_write(handle, &(output[0]->props.width), sizeof(uint32_t), 0x4C);  /* Out width */
    ivas_register_write(handle, &(kernel_priv->use_mean), sizeof(uint32_t), 0x5C);   /* Use mean */
    ivas_register_write(handle, &(kernel_priv->mean), sizeof(uint32_t), 0x64);       /* Mean */
    ivas_register_write(handle, &(input[0]->paddr[0]), sizeof(uint32_t), 0x10);      /* Y input */
    ivas_register_write(handle, &(input[0]->paddr[1]), sizeof(uint32_t), 0x1C);      /* UV input */
    ivas_register_write(handle, &(output[0]->paddr[0]), sizeof(uint32_t), 0x28);     /* Output buffer */
    ivas_register_write(handle, &start, sizeof(uint32_t), 0x0);                      /* start */
    return 0;
}

int32_t xlnx_kernel_done(IVASKernel *handle)
{
    uint32_t val = 0, count = 0;
    do {
        ivas_register_read(handle, &val, sizeof(uint32_t), 0x0); /* start */
        count++;
        if (count > 1000000) {
            printf("ERROR: kernel done wait TIME OUT !!\n");
            return 0;
        }
    } while (!(0x4 & val));
    return 1;
}
