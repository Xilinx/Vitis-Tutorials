/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * dummy_ps_app.c: simple test application
 *
 * This application configures is a dummy application in order to run the PMC
 * to load the AI Engine application
 *
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"


int main()
{
    init_platform();

    print("Running an application on the PS to run PMC\n\r");
    print("This will load the AI Engine application");
    while(1);
    cleanup_platform();
    return 0;
}
