/*********************************************************************
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
**********************************************************************/

#include "kernel.hpp"
#include "graph.hpp"
#include "C1_e.h"
#include "C1_o.h"

using namespace std;

using namespace adf;

// specify the DFG
the_graph my_graph;

// main simulation program
int main() {

	cout << endl;
	cout << "***********************" << endl;
	cout << "* In the beginning... *" << endl;
	cout << "***********************" << endl;
	cout << endl;

	my_graph.init();				// load the DFG into the AI engine array, establish     connectivity, etc.

	my_graph.update(my_graph.cmtx_e, C1_e, 48);
	my_graph.update(my_graph.cmtx_o, C1_o, 48);

	my_graph.run(1);	// run the DFG for the specified number of iterations

	my_graph.end();					// housekeeping

	cout << endl;
	cout << "*******************" << endl;
	cout << "* It is finished! *" << endl;
	cout << "*******************" << endl;
	cout << endl;

	return (0);

} // end main()
