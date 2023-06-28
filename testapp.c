#include <stdio.h>
#include "graph.h"
#include "CommandParser/libcli.h"
#include "comm.h"

extern graph_t *build_first_topo();
extern graph_t *linear_topo(); 
graph_t *topo = NULL; 
//extern void dump_graph(graph_t *graph);

int main(int argc, char **argv) {
    
    nw_init_cli();
    topo = linear_topo();
    start_shell();
    return 0;
}


