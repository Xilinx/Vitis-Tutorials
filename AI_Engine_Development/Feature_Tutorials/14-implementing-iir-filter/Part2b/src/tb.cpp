/****************************************************************
# Support
GitHub issues will be used for tracking requests and bugs. For questions go to [forums.xilinx.com](http://forums.xilinx.com/).

# License
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at [http://www.apache.org/licenses/LICENSE-2.0]( http://www.apache.org/licenses/LICENSE-2.0 )

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

Copyright 2022 Xilinx, Inc.
****************************************************************/

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
