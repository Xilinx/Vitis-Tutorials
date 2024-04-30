//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "platform.h"
#include "xparameters.h"
#include "xil_io.h"
#include "xil_cache.h"

#include "test_cfg.h"

#define NUM_AXI 64
#define PL_FREQ (3125/6.0)

#include "driver_fft.h"


//--------------------------------------------------------
// Post process the results of one AXI bus
//--------------------------------------------------------
void post_results(int num_axi, int num_tests, uint64_t *r, uint64_t sums[5]){

	uint64_t totalout=0;
	uint64_t totalmis=0;
	uint64_t totalidl=0;
	uint64_t latmax = 0;
	uint64_t latmin = -1;
	uint64_t latency;


	for(int testid=0; testid<num_tests; testid++){

		totalout+= r[4*num_axi*testid];
		totalmis+= r[4*num_axi*testid+1];
		totalidl+= r[4*num_axi*testid+2];
		latency  = r[4*num_axi*testid+3];

		latmax = (latmax>latency)?  latmax:latency;
		latmin = (latmin<latency)?  latmin:latency;

	}

	sums[0] = totalout;
	sums[1] = totalmis;
	sums[2] = totalidl;
	sums[3] = latmax;
	sums[4] = latmin;

}

//--------------------------------------------------------
// Print Summary on Screen
//--------------------------------------------------------
int print_summary(int num_axi, int num_tests, uint64_t *r, int samples_per_cycle, double plfreq, double &min_throughput){

	int errcnt=0;

	min_throughput = 10000;

	for(int axiid=0; axiid<num_axi; axiid++){

		uint64_t sums[5];
		post_results(num_axi, num_tests, &r[axiid*4], sums);

		uint64_t totalout=sums[0];
		uint64_t totalmis=sums[1];
		uint64_t totalidl=sums[2];
		double latmax = sums[3];
		double latmin = sums[4];

		double throughput = ( samples_per_cycle * totalout * plfreq)/(totalidl+totalout);
		double latmax_us = latmax/plfreq;
		double latmin_us = latmin/plfreq;

		printf("%2d   %6.3f-%6.3f %6.1fMsps %13llu %11llu   ",  axiid, latmin_us, latmax_us, throughput, totalout, totalmis);

		if (totalmis==0) printf("PASS\n");
		else{
			printf("FAIL\n"); errcnt++;
		}

		if (min_throughput>throughput)	min_throughput = throughput;

	}
	return(errcnt);
}


//---------------------------------------
// Main Program Starts Here
//---------------------------------------
int main()
{

	volatile uint32_t v=0;
	uint32_t counter=0;
	uint32_t min_itecnt;
	uint64_t results[NUM_TST*NUM_AXI*4];
	uint32_t errcnt = 0;

	init_platform();
	Xil_DCacheDisable();
	sleep(1);

	//--------------------------
	// Start the test
	//--------------------------
	printf("- \n");
	printf("Beginning test\n");


	min_itecnt = NUM_ITE_MIN;

	for(int testid=0; testid<NUM_TST; testid++){

		//--------------------------------------------------
		// Start the test
		//--------------------------------------------------
		drv_onoff(1);
		printf("-------------------------------------------\n");
		printf("Test %d has started\n", testid);
		printf("Min IteCnt = %d\n", min_itecnt);


		//--------------------------------------------------
		// Wait until min number of iterations is reached
		//--------------------------------------------------
		counter=0;
		while(1){

			counter++;
			// read number of iterations
			v = drv_itecnt();

			if (v >= min_itecnt) {
				printf("IteCnt = %d. Stopping the test after %d Read.\n", v, counter);
				min_itecnt+=NUM_ITE_STP;
				drv_onoff(0);
				break;
			}

		}

		//--------------------------------------------------
		// Wait until the monitor is stopped
		//--------------------------------------------------
		counter=0;
		while(1){
			counter++;
			// wait for the monitor done flag
			if (drv_isdone()){
				printf("Monitor has stopped. (Count = %d)\n", counter);
				break;
			}else if (counter>4000){
				printf("Time out. (Count = %d)\n", counter);
				drv_show_reg();
				break;
			}
		}

		//-------------------------
		// Collect Test Results
		//-------------------------
		drv_disp(&results[testid*NUM_AXI*4]);

	}

	//----------------------------
	// Print out a short summary
	//-------------------------
	double min_thruput = 0;

	printf("\n------------------------------------------------------------------\n");
	printf("--             1M-point FP32 FFT TEST SUMMARY                   --\n");
	printf("------------------------------------------------------------------\n");
	printf("AXI-S  Latency(us)  Throughput   No.Outputs    Mismatch   Result\n");
	printf("------------------------------------------------------------------\n");
	errcnt += print_summary(NUM_AXI, NUM_TST, results, 1, PL_FREQ, min_thruput);
	printf("------------------------------------------------------------------\n\n");

	printf(" Measured Throughput = %.1f Gsps\n\n", min_thruput*NUM_AXI/1000);

	if(errcnt==0) printf("PASS!\n\n");
	else printf("FAIL! ErrCnt = %d\n\n", errcnt);

	printf("Finished!\n");

	if(IS_COSIM) printf("\n(Press CTRL-SHIFT-A and then X to exit.)\n");
	else  printf("\n(Press CTRL-\\ to exit.)\n");

	cleanup_platform();

	return 0;
}
