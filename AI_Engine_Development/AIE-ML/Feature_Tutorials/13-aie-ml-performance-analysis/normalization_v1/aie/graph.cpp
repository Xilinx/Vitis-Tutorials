/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/
#include "graph.h"

SimpleGraph gr;

int main(int argc, char ** argv) {
	gr.init();
	gr.run(1);
	gr.wait();
	gr.end();
	return 0;
}
