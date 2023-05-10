/*
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
*/


 #include <adf.h>
#include "system_settings.h"

#include "graph.h"


TopGraph G;


int main()
{
	G.init();
	G.run(NFRAMES);
	G.end();

	return(0);
}
