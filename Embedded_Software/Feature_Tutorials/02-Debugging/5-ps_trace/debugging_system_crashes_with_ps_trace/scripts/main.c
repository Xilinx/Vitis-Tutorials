/** Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
******************************************************************************************/
#include "xil_printf.h" 
#include "xgpio.h"       
#include "sleep.h"       
#include <xparameters.h>

#define LED_ON   1
#define LED_OFF  0

// Pattern 1: LEDs 0 and 2 on (0x5)
#define PATTERN1 0x5
// Pattern 2: LEDs 1 and 3 on (0xA)
#define PATTERN2 0xA

int main(void)
{
    XGpio Gpio;  // GPIO instance
    int status;
    int i;

    xil_printf("\r\n--- VCK190 Bare-Metal GPIO LED Test ---\r\n");

    // Initialize AXI GPIO (from xparameters.h — auto-generated from your .xsa)
    status = XGpio_Initialize(NULL, XPAR_AXI_GPIO_1_BASEADDR);
    if (status != XST_SUCCESS) {
        xil_printf("GPIO init failed (status=%d)\r\n", status);
        return XST_FAILURE;
    }
    xil_printf("AXI GPIO initialized @ 0x%08X\r\n", XPAR_AXI_GPIO_1_BASEADDR);

    // Set direction to output (Channel 1, all bits output)
    XGpio_SetDataDirection(&Gpio, 1, 0x0);  // 0 = all outputs
    xil_printf("GPIO direction set to output\r\n");

    // Toggle patterns on LEDs (like the original loop for 2 "GPUs", but for 4 LEDs)
    for (i = 0; i < 10; i++) {  // Run 10 cycles (adjust as needed)
        // Turn on Pattern 1
        XGpio_DiscreteWrite(&Gpio, 1, PATTERN1);
        xil_printf("Pattern 1 (0x%X) - LEDs ON\r\n", PATTERN1);
        sleep(5);  // 5 seconds (like original)

        // Turn off
        XGpio_DiscreteWrite(&Gpio, 1, LED_OFF);
        xil_printf("LEDs OFF\r\n");
        sleep(1);  // 1 second pause

        // Turn on Pattern 2
        XGpio_DiscreteWrite(&Gpio, 1, PATTERN2);
        xil_printf("Pattern 2 (0x%A) - LEDs ON\r\n", PATTERN2);
        sleep(5);

        // Turn off
        XGpio_DiscreteWrite(&Gpio, 1, LED_OFF);
        xil_printf("LEDs OFF\r\n");
        sleep(1);
    }

    xil_printf("Test complete - LEDs off\r\n");
    XGpio_DiscreteWrite(&Gpio, 1, LED_OFF);

    return XST_SUCCESS; 
}