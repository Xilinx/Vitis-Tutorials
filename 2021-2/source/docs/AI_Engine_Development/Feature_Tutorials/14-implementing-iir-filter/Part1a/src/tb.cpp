/****************************************************************
# Support
GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

# License
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

<p align="center"><sup>XD0xx | &copy; Copyright 2021 Xilinx, Inc.</sup></p>
****************************************************************/

#include "kernel.hpp"
#include "graph.hpp"
#include "C1.h"

using namespace std;

using namespace adf;

// the testbench requires a virtual platform for simulation
simulation::platform<1, 1> platform("data/input.dat", "output.dat");

// specify the DFG
the_graph my_graph;

// define connections between virtual platform and the DFG
connect<> net0 (platform.src[0], my_graph.in);
connect<> net1 (my_graph.out, platform.sink[0]);

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
	//my_graph.run(2);
	my_graph.run(num_iterations);	// run the DFG for the specified number of iterations

	my_graph.end();					// housekeeping

	cout << endl;
	cout << "*******************" << endl;
	cout << "* It is finished! *" << endl;
	cout << "*******************" << endl;
	cout << endl;

	return (0);

} // end main()
