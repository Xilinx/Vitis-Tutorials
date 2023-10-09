/*********************************************************************
Copyright (C) 2023, Advanced Micro Devices, Inc. All rights reserved.
SPDX-License-Identifier: X11
**********************************************************************/

//#define RTP_SWITCH		// comment out to check impulse response

#include "kernel.hpp"
#include "graph.hpp"
#include "C1a.h"
#include "C2a.h"
#include "C3a.h"

#ifdef RTP_SWITCH
	#include "C1b.h"
	#include "C2b.h"
	#include "C3b.h"
#endif // RTP_SWITCH

using namespace std;

using namespace adf;

// specify the DFG
the_graph my_graph;

const unsigned num_pts = 256;								// number of sample points in "input.dat"
#ifndef RTP_SWITCH
	const unsigned num_iterations = num_pts/8;				// number of iterations to run
#else
	const unsigned num_iterations = num_pts/8/2;			// number of iterations to run
#endif // RTP_SWITCH

// main simulation program
int main() {

	cout << endl;
	cout << "***********************" << endl;
	cout << "* In the beginning... *" << endl;
	cout << "***********************" << endl;
	cout << endl;

	my_graph.init();				// load the DFG into the AI engine array, establish     connectivity, etc.

	my_graph.update(my_graph.cmtx1, C1a, 96);
	my_graph.update(my_graph.cmtx2, C2a, 96);
	my_graph.update(my_graph.cmtx3, C3a, 96);

	#ifndef RTP_SWITCH
		my_graph.run(num_iterations);	// run the DFG for the specified number of iterations
	#else
		// run with set "a" for 1st half
		my_graph.run(num_iterations);	// run the DFG for the specified number of iterations
		my_graph.wait();				// wait for all iterations to finish before loading new coefficients

		// load set "b"
		my_graph.update(my_graph.cmtx1, C1b, 96);
		my_graph.update(my_graph.cmtx2, C2b, 96);
		my_graph.update(my_graph.cmtx3, C3b, 96);

		my_graph.run(num_iterations);	// run the DFG for the specified number of iterations

	#endif // RTP_SWITCH

	my_graph.end();					// housekeeping

	cout << endl;
	cout << "*******************" << endl;
	cout << "* It is finished! *" << endl;
	cout << "*******************" << endl;
	cout << endl;

	return (0);

} // end main()
