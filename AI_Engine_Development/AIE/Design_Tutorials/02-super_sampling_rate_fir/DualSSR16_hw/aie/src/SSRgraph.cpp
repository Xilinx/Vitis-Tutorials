/*
Copyright (C) Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: MIT
*/


#include <adf.h>
#include "system_settings.h"

#include "SSRgraph.h"


TopGraph<15> G1;
TopGraph<25> G2;

#if defined(__AIESIM__) || defined(__X86SIM__)

int main()
{
	G1.init();
	G2.init();
	G1.run(NFRAMES);
	G2.run(NFRAMES);
	G1.end();
	G2.end();

	return(0);
}

#endif
