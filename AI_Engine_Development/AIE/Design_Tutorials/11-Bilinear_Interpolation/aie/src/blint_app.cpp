//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Richard Buz

#include "bilinear_graph.h"
#include "config.h"

bilinear_graph blint;

int main(void)
{
	blint.init();
	blint.run(NRUN);
	blint.end();

  return 0;
}
