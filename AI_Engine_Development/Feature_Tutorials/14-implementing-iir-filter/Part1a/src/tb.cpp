/*********************************************************************
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
**********************************************************************/

#include "kernel.hpp"
#include "graph.hpp"
#include "C1.h"

using namespace std;

using namespace adf;

// specify the DFG
the_graph my_graph;

const unsigned num_pts = 256;								// number of sample points in "input.dat"
const unsigned num_iterations = num_pts/8;					// number of iterations to run

// main simulation program
int main() {

	cout << endl;
	cout << "***********************" << endl;
	cout << "* In the beginning... *" << endl;
	cout << "***********************" << endl;
	cout << endl;

	my_graph.init();				// load the DFG into the AI engine array, establish     connectivity, etc.

	my_graph.update(my_graph.cmtx1, C1, 96);
	
	my_graph.run(num_iterations);	// run the DFG for the specified number of iterations

	my_graph.end();					// housekeeping

	cout << endl;
	cout << "*******************" << endl;
	cout << "* It is finished! *" << endl;
	cout << "*******************" << endl;
	cout << endl;

	return (0);

} // end main()
