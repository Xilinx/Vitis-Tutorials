/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/

#include "mgs_qrd_wbs.h"

int main() {
	float Qr_i[MAX_ROW*MAX_COL];
	float Qi_i[MAX_ROW*MAX_COL];
	int col = 32;
	int row = 32;
	float br_i[MAX_ROW];
	float bi_i[MAX_ROW];
	float xr_i[MAX_COL];
	float xi_i[MAX_COL];

	mgs_qrd( Qr_i, Qi_i, col, row, br_i, bi_i, xr_i,xi_i);

	return 0;
}
