#include "graph.h"
#include <memory.h>
#include <stdio.h>

static unsigned int hash_code(void *ptr, unsigned int size) {
    unsigned int value = 0, i = 0; 
    char *str = (char *)ptr; 
    while(i < size) {
        value += *str; 
        value *= 97; 
        str++;  
        i++;
    }
    return value;
}

void interface_assign_mac_address(interface_t *interface) {
    node_t *node = interface->att_node; 
    if(!node) 
        return; 
    unsigned int hash_code_val = 0; 
    hash_code_val = hash_code(node->node_name, NODE_NAME_SIZE); 
    hash_code_val *= hash_code(interface->if_name, IF_NAME_SIZE); 
    memset(IF_MAC(interface), 0, sizeof(IF_MAC(interface))); 
    memcpy(IF_MAC(interface), (char *)&hash_code_val, sizeof(unsigned int));
}

bool node_set_loopback_address(node_t *node, char *ip_addr) {
    assert(ip_addr);
    node->node_nw_prop.is_lb_addr_config = true;
    strncpy(NODE_LO_ADDR(node), ip_addr, 16); 
    NODE_LO_ADDR(node)[15] = '\0';  

    return true; 
}

bool node_set_intf_ip_address(node_t *node, char *local_if, char *ip_addr, char mask) {
    interface_t *interface = get_node_if_by_name(node, local_if); 
    
    if(!interface) assert(0); 
    
    strncpy(IF_IP(interface), ip_addr, 16); 
    IF_IP(interface)[15] = '\0'; 
    interface->intf_nw_props.mask = mask;
    interface->intf_nw_props.is_ip_addr_config = true; 

    return true; 
}

bool node_unset_intf_ip_address(node_t *node, char *local_if) {
    return true;    
}

void dump_nw_graph(graph_t *graph) {

    node_t *node;
    glthread_t *curr;
    
    printf("Topology Name = %s\n", graph->topology_name);

    ITERATE_GLTHREAD_BEGIN(&graph->node_list, curr){
        node = graph_glue_to_node(curr);
        dump_node_nw_props(node);    
    } ITERATE_GLTHREAD_END(&graph->node_list, curr);
}

void dump_node_nw_props(node_t *node) {
    printf("Node name: %s\n", node->node_name); 
    printf("Is loopback address configured? %s\n", (node->node_nw_prop.is_lb_addr_config) ? "YES" : "NO"); 
    if(node->node_nw_prop.is_lb_addr_config) {
        printf("Loopback address: %s\n", NODE_LO_ADDR(node));
    }
}