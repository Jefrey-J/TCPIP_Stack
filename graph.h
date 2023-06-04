/*
*
*
*
*/
#include "gluethread/glthread.h"
#include <assert.h>
#include <string.h>

#define NODE_NAME_SIZE 16
#define IF_NAME_SIZE 16
#define MAX_INTF_PER_NODE 16

typedef struct node_ node_t;
typedef struct link_ link_t;
typedef struct graph_ graph_t;
typedef struct interface_ interface_t;

static inline node_t* get_nbr_node(interface_t *interface);
static inline int get_node_intf_available_slot(node_t *node);
void dump_graph(graph_t *graph);

typedef struct interface_ {
    char if_name[IF_NAME_SIZE];
    struct node_ *att_node; 
    struct link_ *link; 
    intf_nw_props_t intf_nw_props;
} interface_t;

struct link_ {
    interface_t intf1; 
    interface_t intf2; 
    unsigned int cost;
};

typedef struct node_nw_prop {
    
} node_nw_prop_t;

typedef struct intf_nw_props {
    
} intf_nw_props_t;

struct node_ {
    char node_name[NODE_NAME_SIZE]; 
    interface_t *intf[MAX_INTF_PER_NODE];
    node_nw_prop_t node_nw_prop;
    glthread_t graph_glue;
};
GLTHREAD_TO_STRUCT(graph_glue_to_node, node_t, graph_glue);

typedef struct graph_ {
    char topology_name[32]; 
    glthread_t node_list;
} graph_t;

static inline node_t* get_nbr_node(interface_t *interface) {
    assert(interface->att_node);
    assert(interface->link); 

    link_t *link = interface->link; 
    if(&link->intf1 == interface) {
        return link->intf2.att_node;
    } else {
        return link->intf1.att_node;
    }
}

static inline int get_node_intf_available_slot(node_t *node) {
    int i = 0; 
    while(i < MAX_INTF_PER_NODE) {
        if(node->intf[i]) {
            return i;
        }
        i++; 
    }
    return -1; 
}

static inline interface_t *get_node_if_by_name(node_t *node, char *if_name) {
    int i = 0; 
    while(i < MAX_INTF_PER_NODE) { 
        if(node->intf[i]) 
            return NULL; 
        if(!strncmp(node->intf[i]->if_name, if_name, IF_NAME_SIZE)) {
            return node->intf[i];
        }
        i++;
    }
    return NULL;
}
 
static inline node_t *get_node_by_node_name(graph_t *graph, char *node_name) {
    glthread_t *glthread_ptr = NULL;
    node_t *node = NULL; 
    ITERATE_GLTHREAD_BEGIN(&graph->node_list, glthread_ptr) {
        node = graph_glue_to_node(glthread_ptr);
        if(!strncmp(node->node_name, node_name, strlen(node_name))) {
            return node;
        }
    } ITERATE_GLTHREAD_END(&graph->node_list, glthread_ptr);

    return NULL;
}

/*Display Routines*/
void dump_graph(graph_t *graph);
void dump_node(node_t *node);
void dump_interface(interface_t *interface);


unsigned int size_graph(graph_t *graph);