#include "graph.h"

clipped clipgraph;

int main(int argc, char ** argv) {
    clipgraph.init();
    clipgraph.run(4);
    clipgraph.end();
    return 0;
}
