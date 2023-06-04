#include <stdio.h>
#include "graph.h"

extern graph_t* build_first_topo();
extern void dump_graph(graph_t *graph);

int main(int argc, char **argv) {
    graph_t *topo = build_first_topo();
    size_graph(topo);
    dump_graph(topo);
    return 0;
}


