//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#define BA_DRV  XPAR_TST_DIN_0_BASEADDR
#define BA_MON  XPAR_TST_DOUT_0_BASEADDR

//----------------------------------------
// Register Map
//----------------------------------------
#define ADDR_ONOFF  0x20
#define ADDR_NITE   0x24
#define ADDR_ASEL   0x28
#define ADDR_RES0   0x2C
#define ADDR_RES1   0x30
#define ADDR_RES2   0x34
#define ADDR_RES3   0x38
#define ADDR_RES4   0x3C

//----------------------------------------
// Start/Stop Data Source
//----------------------------------------
void drv_onoff(int onoff){
	Xil_Out32(BA_DRV + ADDR_ONOFF, onoff);
}

//----------------------------------------
// Get Num of Iterations
//----------------------------------------
uint32_t drv_itecnt(void){
	volatile uint32_t v = Xil_In32(BA_DRV + ADDR_NITE);
	return(v);
}

//----------------------------------------
// Check Monitor Done or not
//----------------------------------------
bool drv_isdone(void){
	// check whether tx stimulus is disabled
	volatile uint32_t v = Xil_In32(BA_MON + ADDR_ONOFF);
	if((v&1)==1) return(false);
	// read the register again to check whether the monitor is done or not
	v = Xil_In32(BA_MON + ADDR_ONOFF);
	return((v>>16)&1);
}



void drv_show_reg(void){
	// check whether tx stimulus is disabled
	volatile uint32_t v = Xil_In32(BA_MON + ADDR_ONOFF);
	printf("[MON ONOFF] %08x\n", v);
}

//--------------------------------------------------------
// Print results on screen
//--------------------------------------------------------
void drv_disp(uint64_t r[NUM_AXI*4]){

	for(int axiid=0; axiid<NUM_AXI; axiid++){
		volatile uint32_t v;

		// select the axi bus
		Xil_Out32(BA_MON+ADDR_ASEL, axiid);

		// read results
		v = Xil_In32(BA_MON+ADDR_RES0);  r[(axiid<<2)|0] = ((uint64_t)v)<<16;
		v = Xil_In32(BA_MON+ADDR_RES1);  r[(axiid<<2)|1] = ((uint64_t)v)<<16;
		v = Xil_In32(BA_MON+ADDR_RES2);  r[(axiid<<2)|0]|=(v&0xffff); r[(axiid<<2)|1]|=(v>>16)&0xffff;
		v = Xil_In32(BA_MON+ADDR_RES3);  r[(axiid<<2)|2] = ((uint64_t)v)<<16;
		v = Xil_In32(BA_MON+ADDR_RES4);  r[(axiid<<2)|3] =(v&0xffff); r[(axiid<<2)|2]|=(v>>16)&0xffff;

		// Print Raw Numbers
		printf("[%d] Test Results: \n", axiid);
		printf("\tNum_of_Vld_Cycles  =%ld\n",         r[(axiid<<2)|0]);
		printf("\tNum_of_Mismatches  =%ld\n",         r[(axiid<<2)|1]);
		printf("\tNum_of_Idle_Cycles =%ld\n",         r[(axiid<<2)|2]);
		printf("\tInitial Latency    =%ld cycles\n",  r[(axiid<<2)|3]);

		if (r[(axiid<<2)|1]==0) printf("\t*********** PASS ***********\n\n");
		else printf("\t*********** FAIL ***********\n\n");
	}

}
