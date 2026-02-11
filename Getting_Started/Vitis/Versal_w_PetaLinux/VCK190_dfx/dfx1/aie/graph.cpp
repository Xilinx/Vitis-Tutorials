#include "graph.h"



clipped clipgraph;



#if defined(__AIESIM__) || defined(__X86SIM__)
int main(int argc, char ** argv) {
	clipgraph.init();
    clipgraph.run(4);	// Note: The number of iterations can be changed if needed based on the input
    clipgraph.end();
    return 0;
}
#endif
