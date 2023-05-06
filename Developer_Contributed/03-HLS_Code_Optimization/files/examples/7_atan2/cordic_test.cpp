/*
# Copyright © 2023 Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
*/

#include <stdio.h>
#include <stdlib.h>
#include "read_file.h"

#include "cordic.h"

const int N = 1024*4; //110465;

coord_t input_data[N][2];
phase_t ref_results[N][1];

int main(void)
{
	coord_t inp_x, inp_y, out_x, out_y;
	phase_t inp_rad, out_rad;
	float res, ref_res;

	float diff, total_error = 0;

	int i;
	FILE *fp, *fm;

	//const float angle = M_PI_2/N;

	fp = fopen("./test_data/vector.dat", "w");
	if (fp == NULL) {
		fprintf(stderr, "unable to open output file\n");
	}
	fscanf(fp, "%*[^\n]\n"); //skip first  line
	fscanf(fp, "%*[^\n]\n"); //skip second line

	fm = fopen("./test_data/check_res.m", "w");
	if (fm == NULL) {
		fprintf(stderr, "unable to open MATLAB ile\n");
	}
	fprintf(fm, "clear all; close all; clc;\n");
	fprintf(fm, "%%%%\n");
	fprintf(fm, "dat = load('vector.dat');\n");
	fprintf(fm, "diff = abs(dat(:,3) - dat(:,4));\n");
	fprintf(fm, "diff2= abs(dat(:,4) - dat(:,5));\n");
	fprintf(fm, "figure; plot(diff);  title 'error CORDIC vs. ATAN2'; grid;\n");
	fprintf(fm, "figure; plot(diff2); title 'error2'; grid;\n");
	fprintf(fm, "max_diff = max(diff)\n");
	fprintf(fm, "sum_abs_err = sum(diff)\n");
	fprintf(fm, "max_diff2 = max(diff2)\n");
	fprintf(fm, "sum_abs_err2 = sum(diff2)\n");

	//read files
	read_file<N, 2, coord_t>("./test_data/input_data.txt", input_data);
	read_file<N, 1, phase_t>("./test_data/ref_results.txt", ref_results);

	// vectoring mode
	inp_rad = 0;
	for (i = 0; i < N; i++) {
		inp_y = (coord_t) input_data[i][0];
		inp_x = (coord_t) input_data[i][1];
		ref_res = (phase_t) ref_results[i][0];

		//void cordic (   bool mode,   coord_t x0,   coord_t y0,   phase_t z0,   coord_t *xn,   coord_t *yn,   phase_t *zn);
		//if ( (inp_y>0) & (inp_x>0) )
		{
			// REF DESIGN
			float c_atan2 = atan2f((float) inp_y, (float) inp_x);
			// DUT
			top_atan2(inp_y, inp_x, &out_rad);

			//check results
			diff = out_rad - ref_results[i][0];
			if (diff < 0)
				diff = 0 - diff; // take absolute value
			total_error += diff;
			fprintf(fp, "%20.10f %20.10f %20.10f %20.10f %20.10f\n",
					(float) inp_y, (float) inp_x, (float) out_rad,
					(float) ref_results[i][0], (float) c_atan2);
			//fprintf(stderr, "%20.10f %20.10f %20.10f %20.10f\n", (float) inp_y, (float) inp_x, (float) out_rad, (float) ref_results[i][0]);
		}
	}

	fprintf(stderr, "total    error = %f\n", total_error);
	fprintf(stderr, "relative error = %f\n", total_error / N);

	fclose(fp);
	fclose(fm);

	return 0;

}
