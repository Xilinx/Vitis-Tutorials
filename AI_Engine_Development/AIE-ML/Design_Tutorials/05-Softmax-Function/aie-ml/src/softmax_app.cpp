//
// Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
// SPDX-License-Identifier: MIT
//
// Author: Richard Buz

#include "softmax_graph.h"
#include "config.h"

softmax_graph smax;

int main(void)
{
	smax.init();
	smax.run(NRUN);
	smax.end();

  return 0;
}
